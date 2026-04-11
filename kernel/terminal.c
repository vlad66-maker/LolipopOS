#include "functions.h"
#include "keyboard.h"
#include <stdint.h>

#define VIDEO ((char*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

char cmd[256];          // буфер для команды
int pos = 0;            // текущая позиция в буфере
int cursor_x = 0, cursor_y = 0;
extern int8_t head, tail;

// Вспомогательная функция для вывода приглашения
void print_prompt(void) {
    print_at("root@lolipop# ", 0, cursor_y, 0x1);
    cursor_x = 14;      // длина приглашения (подсчитано)
}

void scroll() {
    if (cursor_y < VGA_HEIGHT)
        return;

    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i++) {
        VIDEO[i] = VIDEO[i + VGA_WIDTH * 2];
    }

    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH * 2;
         i < VGA_HEIGHT * VGA_WIDTH * 2; i += 2) {
        VIDEO[i] = ' ';
        VIDEO[i + 1] = 0x07;
    }

    cursor_y = VGA_HEIGHT - 1;
}

void execute_command(const char *command) {
    
	if (strcmp(command, "help") == 0) {
        if (cursor_x <= 80) {
            cursor_x = 0;
            cursor_y++;
        }
        if (cursor_y >= 25) {
            scroll();
        } 
	char help_msg[] = {'A','v','a','i','l','a','b','l','e',' ','c','o','m','m','a','n','d','s',':',' ','h','e','l','p',',','c','l','e','a','r',',','c','a','l','c',',','r','e','b','o','o','t',',','s','h','u','t','d','o','w','n'};
        for (char i = 0; help_msg[i] != '\0'; i++) {
            hlt(20);
            put_char(help_msg[i], cursor_x, cursor_y, 0x0A);
            cursor_x++;
            
        }
    } else if (strcmp(command, "clear") == 0) {
        clear_screen(0x0F);
        cursor_x = 0;
        cursor_y = 0;
        cursor_y--;
    } else if (strcmp(command, "fastfetch") == 0) {
        if (cursor_x <= 80) {
            cursor_x = 0;
            cursor_y++;
        }
        if (cursor_y >= 25) {
            scroll();
        }  
        
        uint32_t ebx, ecx, edx;
        // Вызываем CPUID с EAX = 0
        // "n" в начале ассемблерной вставки - это заполнитель для вывода
        __asm__ volatile (
            "cpuid"
            : "=b"(ebx), "=c"(ecx), "=d"(edx)
            : "a"(0)
        );
        char vendor[13];
        // Копируем байты из регистров в строку
        *((uint32_t*)vendor) = ebx;
        *((uint32_t*)(vendor + 4)) = edx;
        *((uint32_t*)(vendor + 8)) = ecx;
        vendor[12] = '\0';

                // 1. Рисуем логотип (столбиком слева)
        hlt(20);
        print_at(" ;;;;;;;;;;;;;;;;;;;; ", 0, cursor_y, 0x0F);    
        hlt(20);
        print_at(" ;;;;;;;;;;;;;;;;;;;; ", 0, cursor_y + 1, 0x0F);
        hlt(20);
        print_at(" ;;;  ", 0, cursor_y + 2, 0x0F); 
        hlt(20);
        print_at("####", strlen(" ;;;"), cursor_y + 2, 0x3);
        hlt(20);
        print_at(";;;;;;;;;;;;;; ", strlen(";;;####"), cursor_y + 2, 0x0F);
        hlt(20);
        print_at(" ;;;", 0, cursor_y + 3, 0x0F);
        hlt(20);
        print_at("####", strlen(" ;;;"), cursor_y + 3, 0x3);
        hlt(20);
        print_at(";;;;;;;;;;;;;; ", strlen(";;;####"), cursor_y + 3, 0x0F);
        hlt(20);
        print_at(" ;;;", 0, cursor_y + 4, 0x0F);
        hlt(20);
        print_at("####", strlen(" ;;;"), cursor_y + 4, 0x3);
        hlt(20);
        print_at(";;;;;;;;;;;;;; ", strlen(";;;####"), cursor_y + 4, 0x0F);
        hlt(20);
        print_at(" ;;;", 0, cursor_y + 5, 0x0F);
        hlt(20);
        print_at("####", strlen(" ;;;"), cursor_y + 5, 0x3);
        hlt(20);
        print_at(";;;;;;;;;;;;;; ", strlen(";;;####"), cursor_y + 5, 0x0F);
        hlt(20);
        print_at(" ;;;", 0, cursor_y + 6, 0x0F);
        hlt(20);
        print_at("####", strlen(" ;;;"), cursor_y + 6, 0x3);
        hlt(20);
        print_at(";;;;;;;;;;;;;; ", strlen(";;;####"), cursor_y + 6, 0x0F);
        hlt(20);
        print_at(" ;;;", 0, cursor_y + 7, 0x0F);
        hlt(20);
        print_at("####", strlen(" ;;;"), cursor_y + 7, 0x3);
        hlt(20);
        print_at(";;;;;;;;;;;;;; ", strlen(";;;####"), cursor_y + 7, 0x0F);
        hlt(20);
        print_at(" ;;;", 0, cursor_y + 8, 0x0F);
        hlt(20);
        print_at("##############", strlen(" ;;;"), cursor_y+8, 0x3);
        hlt(20);
        print_at(";;;", strlen(" ;;;##############"), cursor_y + 8, 0x0F);
        hlt(20);
        print_at(" ;;;", 0, cursor_y + 9, 0x0F);
        hlt(20);
        print_at("##############", strlen(" ;;;"), cursor_y + 9, 0x3);
        hlt(20);
        print_at(";;;", strlen(" ;;;##############"), cursor_y+9, 0x0F);
        hlt(20);
        print_at(" ;;;;;;;;;;;;;;;;;;;; ", 0, cursor_y + 10, 0x0F);    
        hlt(20);
        print_at(" ;;;;;;;;;;;;;;;;;;;; ", 0, cursor_y + 10, 0x0F);

        // 2. Выводим инфу справа от логотипа (
        print_at(" LolipopOS", strlen(";;;;;;;;;;;;;;;;;;;;") + 2, cursor_y, 0x0C);        // Ярко-красный
        print_at(" -----------", strlen(";;;;;;;;;;;;;;;;;;;;") + 2, cursor_y + 1, 0x07);
        // Вывод вендора CPU 
        print_at("  CPU: ", strlen(";;;;;;;;;;;;;;;;;;;;") + 1, cursor_y + 2, 0x0A);       // Зеленый заголовок
        print_at(vendor, strlen(";;;;;;;;;;;;;;;;;;;;") + strlen("  CPU:  "), cursor_y + 2, 0x0F);        // Белый текст вендора
        // Выводим версию
        print_at("  Version: ", strlen(";;;####;;;;;;;;;;;;;") + 1, cursor_y+3, 0x0A);
        print_at("  0.0.2.1", strlen(";;;;;;;;;;;;;;;;;;;;   Version: "), cursor_y+3, 0x0F);

        // Смещаем курсор вниз, чтобы следующая команда не налезла на лого
        cursor_y += 10;
            
    } else if (strcmp(command, "shutdown") == 0) {
    	shutdown();
    } else if (strcmp(command, "reboot") == 0) {
    	reboot();
    } else if (
        strcmp(command, "help") != 0 &&
        strcmp(command, "clear") != 0 &&                                                                         
        strcmp(command, "fastfetch") != 0 &&
	strcmp(command, "shutdown") != 0 &&
	strcmp(command, "reboot") != 0)  {
        if (cursor_y >= 25) {
            scroll();
        } 
        print_at("Unknown command", 0, cursor_y, 0x0C);
    }
}

void terminal_init(void) {
    // Один раз выводим приглашение
    print_prompt();

    while (1) {
        keyboard_handler();          // опрашиваем клавиатуру
        if (head != tail) {          // если есть символ в буфере
            char c = getchar();

            if (c == '\n') { 
                if (cursor_y >= 25) { scroll(); }

                cmd[pos] = '\0';

                cursor_x = 0;   
                cursor_y++;

                execute_command(cmd);

                if (cursor_y >= 25) { scroll(); }

                cursor_x = 0;   
                cursor_y++;

                if (cursor_y >= VGA_HEIGHT) {
                    scroll();
                }

                print_prompt();

                pos = 0;
            }
            else if (c == '\b' || c == 0x08) {   // Backspace
                if (pos > 0) {
                    pos--;
                    if (cursor_x > 14) {   // если не в начале строки ввода
                        cursor_x--;
                        put_char(' ', cursor_x, cursor_y, 0x0);
                    } else {
                        // если в начале строки, нужно перейти на предыдущую – пока не реализовано
                        // для простоты просто не даём стирать приглашение
                    }
                }
            }
            else {   // печатный символ
                if (pos < 255) {
                    cmd[pos++] = c;
                    put_char(c, cursor_x, cursor_y, 0x0F);
                    cursor_x++;
                    if (cursor_x >= 80) {   // достигли конца строки
                        cursor_x = 0;
                        cursor_y++;
                    } if (cursor_y >= VGA_HEIGHT) {
                        scroll();
                    }
                }
            }
        }
        
    }
}
