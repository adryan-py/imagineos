#include "print.h"

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

static struct limine_framebuffer *fb = NULL;

static size_t cols;
static size_t rows;

static size_t cursor_x = 0;
static size_t cursor_y = 0;

static uint32_t fg = 0xFFFFFFFF;
static uint32_t bg = 0x00000000;

/*
 * Você precisará fornecer:
 *
 * extern const uint8_t font8x16[256][16];
 */
extern const uint8_t font8x16[256][16];

void print_init(struct limine_framebuffer *framebuffer)
{
    fb = framebuffer;

    cols = fb->width / FONT_WIDTH;
    rows = fb->height / FONT_HEIGHT;

    cursor_x = 0;
    cursor_y = 0;
}

static inline void putpixel(size_t x, size_t y, uint32_t color)
{
    volatile uint32_t *pixel =
        (uint32_t *)((uint8_t *)fb->address +
        y * fb->pitch +
        x * 4);

    *pixel = color;
}

void print_clear(void)
{
    if (!fb)
        return;

    for (size_t y = 0; y < fb->height; y++) {
        for (size_t x = 0; x < fb->width; x++) {
            putpixel(x, y, bg);
        }
    }

    cursor_x = 0;
    cursor_y = 0;
}

static void draw_char(char c, size_t cx, size_t cy)
{
    uint8_t *glyph = (uint8_t *)font8x16[(uint8_t)c];

    size_t px = cx * FONT_WIDTH;
    size_t py = cy * FONT_HEIGHT;

    for (size_t y = 0; y < FONT_HEIGHT; y++) {
        for (size_t x = 0; x < FONT_WIDTH; x++) {

            uint32_t color = (glyph[y] & (1 << (7 - x)))
                ? fg
                : bg;

            putpixel(px + x, py + y, color);
        }
    }
}

static void scroll(void)
{
    uint32_t *pixels = (uint32_t *)fb->address;

    size_t line_pixels =
        FONT_HEIGHT * fb->pitch / 4;

    size_t total_pixels =
        fb->height * fb->pitch / 4;

    for (size_t i = 0;
         i < total_pixels - line_pixels;
         i++)
    {
        pixels[i] = pixels[i + line_pixels];
    }

    for (size_t i = total_pixels - line_pixels;
         i < total_pixels;
         i++)
    {
        pixels[i] = bg;
    }
}

void print_newline(void)
{
    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= rows) {
        scroll();
        cursor_y = rows - 1;
    }
}