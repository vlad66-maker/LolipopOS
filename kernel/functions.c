#include "functions.h"

#define VIDEO ((char*)0xB8000)


void print_at(const char *str, int x, int y, uint8_t color) {
    char *video = VIDEO;
    int offset = (y * 80 + x) * 2;
    for (int i = 0; str[i] != '\0'; i++) {
        video[offset + i*2] = str[i];
        video[offset + i*2 + 1] = color;
    }
}

void put_char(char c, int x, int y, int8_t color) {
    char *video = VIDEO;
    int offset = (y * 80 + x) * 2;
    video[offset] = c;
    video[offset + 1] = color;
}

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

void outw(uint16_t port, uint16_t data) {
	__asm__ volatile ("outw %0, %1" : : "a"(data), "Nd"(port));
}

uint32_t strlen(const char *str) {
    uint32_t len = 0;
    uint32_t i = 0;
    while (str[i] != '\0') {
        len++;
        i++;
    }
    return len;
}

void strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++) != '\0');
}

int8_t strcmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2) {
        str1++;
        str2++;
    }
    // Здесь либо один из символов ноль, либо они различаются
    return (unsigned char)*str1 - (unsigned char)*str2;
}

void clear_screen(uint8_t color) {
    for (int i = 0; i < 80*25; i++) {
        hlt(1);
        VIDEO[i * 2]     = ' '  ; 
        VIDEO[i * 2 + 1] = color;
    }
}
