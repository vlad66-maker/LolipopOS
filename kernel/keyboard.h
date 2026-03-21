#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void        keyboard_handler(void);
char        getchar(void)         ;
static void buffer_put(char c)    ;

#endif
