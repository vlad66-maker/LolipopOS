#include <stdint.h>
#include "functions.h"

#define BUFFER_SIZE 256

char scancode_to_ascii[] = {
    [0x01] = 0x1B  , [0x02] = '1'   , [0x03] = '2' , [0x04] = '3'  , [0x05] = '4'     , [0x06] = '5',
    [0x07] = '6'   , [0x08] = '7'   , [0x09] = '8' , [0x0A] = '9'  , [0x0B] = '0'     , [0x0C] = '-', 
    [0x0D] = '='   , [0x0E] = '\b'  , [0x0F] = '\t', [0x10] = 'q'  , [0x11] = 'w'     , [0x12] = 'e',
    [0x13] = 'r'   , [0x14] = 't'   , [0x15] = 'y' , [0x16] = 'u'  , [0x17] = 'i'     , [0x18] = 'o', 
    [0x19] = 'p'   , [0x1A] = '['   , [0x1B] = ']' , [0x1C] = '\n' , [0x1E] = 'a'     , [0x1F] = 's', 
    [0x20] = 'd'   , [0x21] = 'f'   , [0x22] = 'g' , [0x23] = 'h'  , [0x24] = 'j'     , [0x25] = 'k', 
    [0x26] = 'l'   , [0x27] = ';'   , [0x29] = '`' , [0x2B] = '\\' , [0x2C] = 'z'     , [0x2D] = 'x', 
    [0x2E] = 'c'   , [0x2F] = 'v'   , [0x30] = 'b' , [0x31] = 'n'  , [0x32] = 'm'     , [0x33] = ',', 
    [0x34] = '.'   , [0x35] = '/'   , [0x39] = ' '  
};


static char buffer[BUFFER_SIZE];

int head = 0;   // куда пишем новый символ
int tail = 0;   // откуда читаем

// Добавить символ в буфер (вызывается из прерывания/опроса)
static void buffer_put(char c) {
    int next = (head + 1) % BUFFER_SIZE;
    if (next != tail) {
        buffer[head] = c;
        head = next;
    }
}

char getchar(void) {
    while (head == tail) {}  // ждём символ
    char c = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return c;
}

void keyboard_handler(void) {
    int8_t x = 0;
    int8_t y = 0;
    if (!(inb(0x64) & 1)) return;
    uint8_t sc = inb(0x60);
    if (sc >= 0x80) return;
    char ch = scancode_to_ascii[sc];
    if (ch) {
        buffer_put(ch);
    }
}
