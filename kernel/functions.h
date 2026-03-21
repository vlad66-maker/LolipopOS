#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>

void     print_at(const char *str, int x, int y, uint8_t color);
void     put_char(char c, int x, int y)                        ;
uint8_t  inb(uint16_t port)                                    ;
void     outb(uint16_t port, uint8_t data)                     ;
void     strcpy(char *dest, const char *src)                   ;
int8_t   strcmp(const char *str1, const char *str2)            ;
void     clear_screen(uint8_t color)                           ;
uint32_t strlen(const char *str)                               ;

#endif