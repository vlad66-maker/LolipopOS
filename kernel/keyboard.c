#include <stdint.h>

char scancode_to_ascii[] = {
    [0x01] = 0x1B  , [0x02] = '1'   , [0x03] = '2' , [0x04] = '3'  , [0x05] = '4'     , [0x06] = '5',
    [0x07] = '6'   , [0x08] = '7'   , [0x09] = '8' , [0x0A] = '9'  , [0x0B] = '0'     , [0x0C] = '-', 
    [0x0D] = '='   , [0x0E] = '\b'  , [0x0F] = '\t', [0x10] = 'q'  , [0x11] = 'w'     , [0x12] = 'e',
    [0x13] = 'r'   , [0x14] = 't'   , [0x15] = 'y' , [0x16] = 'u'  , [0x17] = 'i'     , [0x18] = 'o', 
    [0x19] = 'p'   , [0x1A] = '['   , [0x1B] = ']' , [0x1C] = '\n' , [0x1D] = 0x0100  , [0x1E] = 'a',
    [0x1F] = 's'   , [0x20] = 'd'   , [0x21] = 'f' , [0x22] = 'g'  , [0x23] = 'h'     , [0x24] = 'j',
    [0x25] = 'k'   , [0x26] = 'l'   , [0x27] = ';' , [0x29] = '`'  , [0x2A] = 0x0200  , [0x2B] = '\\', 
    [0x2C] = 'z'   , [0x2D] = 'x'   , [0x2E] = 'c' , [0x2F] = 'v'  , [0x30] = 'b'     , [0x31] = 'n',
    [0x32] = 'm'   , [0x33] = ','   , [0x34] = '.' , [0x35] = '/'  , [0x39] = ' '  
};

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void put_char(char c, int x, int y) {
    char *video = (char*)0xB8000;
    int offset = (y * 80 + x) * 2;
    video[offset] = c;
    video[offset + 1] = 0x0F;
}

void keyboard_handler(void) {
    if (!(inb(0x64) & 1))
        return;

    uint8_t sc = inb(0x60);
    if (sc >= 0x80) return; // отпускания игнорируем

    char ch = scancode_to_ascii[sc];
    if (!ch) return;

    static int col = 0, row = 1;

    if (ch == '\n') {
        col = 0;
        row++;
    } else if (ch == '\b' || ch == '\x08') {
        if (col > 0) {
            col--;
            put_char(' ', col, row);
        } else if (row > 0) {
            row--;
            col = 79;
            put_char(' ', col, row);
        }
    } else {
        put_char(ch, col, row);
        col++;
        if (ch == 0x0200 && ch == 1) {
            ch = '!';
            put_char(ch, col, row);
        }
        if (col >= 80) {
            col = 0;
            row++;
        }
    }
}