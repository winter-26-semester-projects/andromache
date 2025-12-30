#ifndef _TTY_TTY0_H
#define _TTY_TTY0_H

#include<stdint.h>
#include "tty/vga.h"

static uint32_t tty_row;
static uint32_t tty_col;
static uint32_t tty_color;
static uintptr_t tty_buf;

void tty_init(void);
void tty_setcolor(vga_color);
void tty_scroll(uint32_t);
void tty_putc(char);
void tty_writeline(char*);
void tty_clearline(void);
void tty_putc_at(char, enum vga_color, uint32_t, uint32_t);

#endif /* _TTY_TTY0_H */
