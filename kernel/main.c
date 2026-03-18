#include <stdint.h>
#include "keyboard.h"

#define VIDEO ((char*)0xB8000)

void print_at(const char *str, int x, int y, uint8_t color) {
    char *video = VIDEO;
    int offset = (y * 80 + x) * 2;
    for (int i = 0; str[i] != '\0'; i++) {
        video[offset + i*2] = str[i];
        video[offset + i*2 + 1] = color;
    }
}

void kernel_main(void) {
    print_at("LolipopOS", 0, 0, 0x0F);

    while (1) {
        keyboard_handler();      
    }
}
