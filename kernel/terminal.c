#include "functions.h"
#include "keyboard.h"
#include <stdint.h>

char cmd[256];          // буфер для команды
int pos = 0;            // текущая позиция в буфере
int cursor_x = 0, cursor_y = 0;
extern int8_t head, tail;

// Вспомогательная функция для вывода приглашения
void print_prompt(void) {
    print_at("root@lolipop# ", 0, cursor_y, 0x4);
    cursor_x = 14;      // длина приглашения (подсчитано)
}

void execute_command(const char *command) {
    if (strcmp(command, "help") == 0) {
        print_at("Available commands: help, clear, calc", 0, cursor_y+1, 0x2);
        cursor_y++;
    } else if (strcmp(command, "clear") == 0) {
        clear_screen(0x0F);
        cursor_x = 0;
        cursor_y = 0;
        cursor_y--;
    } else if (command != "help" || "clear")  {
        print_at("Unknown command", 0, cursor_y+1, 0x00);
        cursor_y++;
    }
}

void terminal_init(void) {
    // Один раз выводим приглашение
    print_prompt();

    while (1) {
        keyboard_handler();          // опрашиваем клавиатуру
        if (head != tail) {          // если есть символ в буфере
            char c = getchar();

            if (c == '\n') {         // Enter
                cmd[pos] = '\0';     // завершаем строку
                cursor_y++;           // переходим на новую строку для вывода результата
                execute_command(cmd);
                // после выполнения снова выводим приглашение
                cursor_y++;           // пропускаем строку
                print_prompt();
                pos = 0;              // сбрасываем позицию буфера
            }
            else if (c == '\b' || c == 0x08) {   // Backspace
                if (pos > 0) {
                    pos--;
                    if (cursor_x > 14) {   // если не в начале строки ввода
                        cursor_x--;
                        put_char(' ', cursor_x, cursor_y);
                    } else {
                        // если в начале строки, нужно перейти на предыдущую – пока не реализовано
                        // для простоты просто не даём стирать приглашение
                    }
                }
            }
            else {   // печатный символ
                if (pos < 255) {
                    cmd[pos++] = c;
                    put_char(c, cursor_x, cursor_y);
                    cursor_x++;
                    if (cursor_x >= 80) {   // достигли конца строки
                        cursor_x = 0;
                        cursor_y++;
                    }
                }
            }
        }
    }
}
