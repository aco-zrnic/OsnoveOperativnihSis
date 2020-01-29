#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "vga.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);

static const size_t VGA_WIDTH = 80;  //80.karakter je new line
static const size_t VGA_HEIGHT = 25;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c, uint8_t colorOfLetter);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
#endif
