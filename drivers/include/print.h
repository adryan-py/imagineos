#pragma once

#include <stdint.h>
#include <limine.h>

void print_init(struct limine_framebuffer *fb);
void print_clear(void);

void print_char(char c);
void print_str(const char *str);

void print_set_color(uint32_t fg, uint32_t bg);

void print_uint64_dec(uint64_t value);
void print_uint64_hex(uint64_t value);