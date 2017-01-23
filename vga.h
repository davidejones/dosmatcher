#ifndef DOSMATCHER_VGA_H
#define DOSMATCHER_VGA_H

#include "utils.h"
#include "types.h"

// width and height in pixels of mode 0x13
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200
// pointer to graphics memory start: 0x000A0000 end: 0x000FFFFF
unsigned char *VGA = (unsigned char *)(0xA0000);

void setGraphicsMode()
{
    // might just be able to do last 2 rows without first
    asm volatile (
        "mov $0x00, %%ah\n"     // mode set
        "mov $0x13, %%ax\n"     // 256-color
        "int $0x10\n"           // do it
        "mov $0xA000, %%ax\n"   // graphics start at 0xA000 set address into es
        "mov %%ax, %%es\n"
    : /* no outputs */
    : /* no inputs */
    : "ax");
}


void setTextMode()
{
    asm volatile (
        "mov $0x00, %ah\n"  // mode set
        "mov $0x03, %al\n"  // text mode is mode 3
        "int $0x10"         // do it
    );
}

/*
 * INT 10h AH = 0Ch - change color for a single pixel, using interrupt (slow)
 * AL = pixel color
 * CX = column, (x location, from 0..319/0x00..0x13F)
 * DX = row, (y location, from 0..199/0x00..0xC7)
 */
void drawPixelSlow(uint16_t x, uint16_t y, uint8_t color)
{
    asm volatile (
        "mov $0x0C, %%ah\n"         // pixel plot mode
        "movb %0, %%al\n"           // set color byte we are using
        "int $0x10"                 // do it
    :                               // no outputs
    : "q"(color), "c"(x), "d"(y)    // `q` Registers a, b, c or d (%0). `c` into cx `d` into dx
    :                               // clobbers
    );
}

/*
 * Write straight to memory pixel color
 */
void drawPixelFast(uint16_t x, uint16_t y, uint8_t color)
{
    uint16_t address = 0xA000;
    uint16_t offset = (SCREEN_WIDTH * y) + x;
    asm volatile (
        "mov %%ax, %%fs\n"                      // set address, fs extra segment register for far pointer addressing like vid memory
        "mov %%cl, %%fs:(%%bx)"                 // move color into A000[offset]
    :                                           // no outputs
    : "c"(color), "b"(offset), "a"(address)     // inputs `c` cl, `b` bx, `a` ax
    :                                           // clobbers
    );
}

void clearScreen(char color)
{
    // graphics memory starts at A000
    // we have 64,000 of video memory so fill with our color
    my_memset(VGA, color, 64000); // 0xFFFF or 0xBFFFF
}

void rect_fill(int x, int y, int width, int height, unsigned char color)
{
    unsigned short top_offset, bottom_offset, i, temp, new_width;

    if (y > height) {
        temp = y;
        y = height;
        height = temp;
    }
    if (x > width) {
        temp = x;
        x = width;
        width = temp;
    }

    top_offset = (y<<8)+(y<<6)+x;
    bottom_offset = (height<<8)+(height<<6)+x;
    new_width = width-x+1;

    for(i = top_offset; i <= bottom_offset; i += SCREEN_WIDTH)
    {
        my_memset(&VGA[i], color, new_width);
    }
}

static void vga_clear(char color)
{
    //"mov   $0xA000, %%ax\n"
    //"mov   %%ax, %%es\n"
    asm volatile ("mov   %%al, %%ah\n"
            "mov   $0, %%di\n"
            "push  %%ax\n"
            "shl   $16, %%eax\n"
            "pop   %%ax\n"
            "mov   $16000, %%cx\n"
            "rep\n"
            "stosl\n"
    : /* no outputs */
    : "a"(color)
    : "cx", "di");

   // As an example, rep stosl with %eax set to 0, %edi set to 0x4000 and %cx set to 4, will set the memory from 0x4000 to %0x4010 to zero.
}

static void vga_vsync()
{
    asm volatile ("mov   $0x03DA, %%dx\n"
            "current%=:"
            "in    %%dx, %%al\n"
            "and   $0x8, %%al\n"
            "jnz   current%=\n"
            "restart%=:"
            "in    %%dx, %%al\n"
            "and   $0x8, %%al\n"
            "jz    restart%=\n"
    : /* no outputs */
    : /* no inputs */
    : "al", "dx");
}

#endif //DOSMATCHER_VGA_H
