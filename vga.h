#ifndef DOSMATCHER_VGA_H
#define DOSMATCHER_VGA_H

#include "utils.h"
#include "types.h"

// width and height in pixels of mode 0x13
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200
// pointer to graphics memory start: 0x000A0000 end: 0x000FFFFF
unsigned char *VGA = (unsigned char *)(0xA0000);

/*void setMode()
{
    asm volatile (
        "mov $0x00, %ah\n"  // mode set
        "mov $0x13, %al\n"  // 0x13 = 256-color, 0x03 = text mode
        "int $0x10"         // do it
    );
}

void initGraphicsScreen()
{
    asm volatile (
        "mov $0x0A000, %ax\n"  // Set ES to graphics screen segment
        "mov %ax, %es"
    );
}*/

// http://www.petesqbsite.com/sections/tutorials/tuts/doorknob/asm_tutorial2.html

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

/*void drawPixelSlow()
{
    // draw using interrupts (slow)
    asm volatile (
    "mov $0x0C, %ah\n"  // pixel plot
            "mov $0x02, %al\n"  // what color (green dark)
            "mov $0x0A, %cx\n"  // x location, from 0..319
            "mov $0x0A, %dx\n"  // y location, from 0..199
            "int $0x10"         // do it
    );
}*/

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

/*
 * Write straight to memory pixel color from offset memory address
 */
void drawPixelAtOffset(uint16_t offset, uint8_t color)
{
    uint16_t address = 0xA000;
    asm volatile (
        "mov %%ax, %%fs\n"                      // set address, fs extra segment register for far pointer addressing like vid memory
        "mov %%cl, %%fs:(%%bx)"                 // move color into A000[offset]
    :                                           // no outputs
    : "c"(color), "b"(offset), "a"(address)     // inputs `c` cl, `b` bx, `a` ax
    :                                           // clobbers
    );
}

/*
 * Write straight to video memory filling all pixels with color
 */
void fillScreen()
{
    // this did some cool jailbars in green
    /*asm volatile (
        "movl $0x02, %eax\n"                // clear eax
        "mov $0x3E80, %cx\n"                 // set 16000 int cx counter
        "mov $0xA000, %di\n"                // set di to 0xA000
        "rep\n"
        "stosl\n"
    );*/

    // jailbars without interrupt line
    /*asm volatile (
        "movb $0x03, %al\n"                // clear eax
        "mov $0xFA00, %cx\n"                 // set 64000 int cx counter
        "mov $0xA000, %di\n"                // set di to 0xA000
        "rep\n"
        "stosl\n"
    );*/

    asm volatile (
        "movl $0x05050505, %eax\n"        // set color to eax
        "mov $0x7D00, %cx\n"              // set 64000 int cx counter
        "mov $0xA000, %di\n"              // set di to 0xA000
        "rep stosl"                       // will set the memory from 0xA000 to 0x19A00 to color in eax
    );

    // rep stosl = http://stackoverflow.com/a/17526611/1113788
}

void clearScreen(char color)
{
    // graphics memory starts at A000
    // we have 64,000 of video memory so fill with our color
    my_memset(VGA, color, 64000); // 0xFFFF or 0xBFFFF
}

static void my_clear(char fill_value) {
    /*asm volatile (
        "mov $16000, %cx\n" // 320*200 / 4 bytes = 16000
        "push  %%ax\n"
        "shl   $16, %%eax\n"
        "pop   %%ax\n"
        "mov $0x0A00, %ax\n"  // graphics start at 0A000
        "mov %ax, %es\n"      // move into es
        "xor %di, %di\n"      // es:di = A000:0000
        "mov $0x02, %eax\n"   // move color into eax
        "rep\n"
        "stosl"           // Process 4 bytes at a time
    : *//* no outputs *//*
    : "a"(color)
    : "cx", "di");*/
    /*asm volatile (
        "mov $0x0A000, %%di\n"
        "mov %0, %%eax\n"
        //"push %%ax\n"
        //"shl $16, %%eax\n"
        //"pop %%ax\n"
        "mov $16000, %%cx\n"
        "rep\n"
        "stosl\n"
    : *//* no outputs *//*
    : "g" (color)
    : "cx", "di");*/
    unsigned short count = 16;
    asm volatile (
    "cld\n"
            "rep\n"
            "stosl"
    : /* no output registers */
    : "c" (count), "a" (fill_value), "D" (&VGA)
    : "%cx", "%di" );

    //The above stores the value in fill_value count times to the pointer dest
}
/*
mov ecx, 16000          ; 320*200 / 4 bytes = 16000

:         mov ax, 0a000h
:         mov es, ax
        :         xor di, di              ; es:di = A000:0000
:         mov eax, couleur        ; Put the color in eax
        :         rep stosd               ; Process 4 bytes at a time*/

void rect_fill(int x, int y, int width, int height, unsigned char color)
{
    unsigned short top_offset, bottom_offset, i, j, temp, new_width;

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


void card_square(int x, int y, int width, int height, int borderXStart, int borderXEnd, int borderYStart, int borderYEnd, unsigned char color,  unsigned char borderColor) {
    int padding = 10;
    for(unsigned short i = borderXStart; i < borderXEnd; i++) {
        for(unsigned short j = borderYStart; j < borderYEnd; j++) {
            if( (j < y || j > y+height) || (i < x || i > x+width))
            {
                // border
                drawPixelFast(i, j, borderColor);
            } else {
                // box
                if(i < (borderXEnd - padding) && i > (borderXStart + padding) && j < (borderYEnd/2 + padding) && j > (borderYStart + 15)) {
                    drawPixelFast(i, j, color);
                } else {
                    drawPixelFast(i, j, LIGHT_GRAY);
                }
            }
        }
    }
}

void card_triangle(int x, int y, int width, int height, int borderXStart, int borderXEnd, int borderYStart, int borderYEnd, unsigned char color,  unsigned char borderColor) {
    int padding = 10;
    int aspectRatio = 10 / 5;
    for(unsigned short i = borderXStart; i < borderXEnd; i++) {
        for(unsigned short j = borderYStart; j < borderYEnd; j++) {
            if( (j < y || j > y+height) || (i < x || i > x+width))
            {
                // border
                drawPixelFast(i, j, borderColor);
            } else {
                // box
                if(j * 0.5  < (i * 0.5) * aspectRatio) {
                    drawPixelFast(i, j, color);
                } else {
                    drawPixelFast(i, j, LIGHT_GRAY);
                }
            }
        }
    }
}

void card_circle(int x, int y, int width, int height, int borderXStart, int borderXEnd, int borderYStart, int borderYEnd, unsigned char color,  unsigned char borderColor) {
    int padding = 10;
    for(unsigned short i = borderXStart; i < borderXEnd; i++) {
        for(unsigned short j = borderYStart; j < borderYEnd; j++) {
            if( (j < y || j > y+height) || (i < x || i > x+width))
            {
                // border
                drawPixelFast(i, j, borderColor);
            } else {
                // box
                if(i < (borderXEnd - padding) && i > (borderXStart + padding) && j < (borderYEnd - padding) && j > (borderYStart + padding)) {
                    drawPixelFast(i, j, color);
                } else {
                    drawPixelFast(i, j, LIGHT_GRAY);
                }
            }
        }
    }
}

void my_rect_fill(int x, int y, int width, int height, unsigned char color, int borderSize, unsigned char borderColor, uint8_t shape)
{
    int borderXStart = x - borderSize;
    int borderYStart = y - borderSize;
    int borderXEnd = x+width+borderSize;
    int borderYEnd = y+height+borderSize;

    switch(shape) {
        case 0x02:
            // draw the card triangle
            card_triangle(x, y, width, height, borderXStart, borderXEnd, borderYStart, borderYEnd, color, borderColor);
            break;
        case 0x03:
            // draw the circle card
            card_circle(x, y, width, height, borderXStart, borderXEnd, borderYStart, borderYEnd, color, borderColor);
            break;
        case 0x01:
            // draw the card normally
            card_square(x, y, width, height, borderXStart, borderXEnd, borderYStart, borderYEnd, color, borderColor);
            break;
        case 0x00:
        default:
            for(unsigned short i = borderXStart; i < borderXEnd; i++) {
                for(unsigned short j = borderYStart; j < borderYEnd; j++) {
                    if( (j < y || j > y+height) || (i < x || i > x+width))
                    {
                        // border
                        drawPixelFast(i, j, borderColor);
                    } else {
                        // box
                        drawPixelFast(i, j, color);
                    }
                }
            }
            break;
    }


    // http://stackoverflow.com/a/6808764/1113788
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