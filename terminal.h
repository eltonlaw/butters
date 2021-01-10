#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace terminal {

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_WHITE,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
    return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
    return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

/* taken from https://wiki.osdev.org/Detecting_Colour_and_Monochrome_Monitors */
enum video_type
{
    VIDEO_TYPE_NONE = 0x00,
    VIDEO_TYPE_COLOUR = 0x20,
    VIDEO_TYPE_MONOCHROME = 0x30,
};

uint16_t detect_bios_area_hardware(void)
{
    const uint16_t* bda_detected_hardware_ptr = (const uint16_t*) 0x410;
    return *bda_detected_hardware_ptr;
}

enum video_type get_bios_area_video_type(void)
{
    return (enum video_type) (detect_bios_area_hardware() & 0x30);
}

class Terminal
{
    private:
        size_t vga_width;
        size_t vga_height;
        size_t row;
        size_t column;
        uint8_t color;
        uint16_t* buffer;
        void next_row(void);
        void next_column(void);
        void putentryat(char c, uint8_t color, size_t x, size_t y);
        void putchar(char c);
    public:
        Terminal(size_t width, size_t height);
        ~Terminal(void);
        void setcolor(uint8_t color);
        void write(const char* data, size_t size);
        void write(const char* data);
};

Terminal::Terminal(size_t width, size_t height)
{
    vga_width = width;
    vga_height = height;
    row = 0;
    column = 0;

    color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    switch (get_bios_area_video_type()) {
        case VIDEO_TYPE_COLOUR:
            /* for colour monitors text screen mem at 0xB8000 */
            buffer = (uint16_t*) 0xB8000;
            break;
        case VIDEO_TYPE_MONOCHROME:
            /* for monochrome monitors text screen mem at address 0xB0000 */
            buffer = (uint16_t*) 0xB0000;
            break;
        default:
            /* FIXME: to quiet warning, this needs to be handled properly */
            buffer = (uint16_t*) 0xB8000;
            break;
    }
    for (size_t y = 0; y < vga_height; y++) {
        for (size_t x = 0; x < vga_width; x++) {
            const size_t index = y * vga_width + x;
            buffer[index] = vga_entry(' ', color);
        }
    }
}

Terminal::~Terminal(void)
{
}
 
void Terminal::setcolor(uint8_t color) 
{
    color = color;
}
 
void Terminal::putentryat(char c, uint8_t color, size_t x, size_t y) 
{
    const size_t index = y * vga_width + x;
    buffer[index] = vga_entry(c, color);
}

void Terminal::next_column(void)
{
    if (++column == vga_width) {
        column = 0;
        next_row();
    }
}

void Terminal::next_row(void)
{
    if (++row == vga_height)
        row = 0;
}
 
void Terminal::putchar(char c) 
{
    /* if newline is passed in don't put anything */
    if (c == '\n') {
        column = 0;
        next_row();
    } else {
        putentryat(c, color, column, row);
        next_column();
    }
}
 
void Terminal::write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; i++)
        putchar(data[i]);
}
 
void Terminal::write(const char* data) 
{
    write(data, strlen(data));
}

}
