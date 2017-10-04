#ifndef DOSMATCHER_MOUSE_H
#define DOSMATCHER_MOUSE_H

#include "types.h"

struct MOUSE {
    bool LEFT;
    bool RIGHT;
    bool BOTH;
    uint16_t OFFSET;
    uint16_t X;
    uint16_t Y;
    int8_t CLICK_INDEX;
};

// http://www.gabrielececchetti.it/Teaching/CalcolatoriElettronici/Docs/i8086_and_DOS_interrupts.pdf
// http://stanislavs.org/helppc/int_33.html
// http://devdocs.inightmare.org/x86-assembly-mouse-int-33h/
// http://asm.inightmare.org/index.php?tutorial=3&location=11
// http://www.programmingforums.org/post221532.html
// https://github.com/AhmadNaserTurnkeySolutions/emu8086/blob/master/examples/mouse2.asm



/*
 * INT 33h / AX=0000 - mouse ininialization. any previous mouse pointer is hidden.
 * AX = 0FFFFh if successful
 * AX = 0 if failed
 * BX = number of mouse buttons
*/
bool mouse_init()
{
    uint16_t axvalue;
    uint8_t number_buttons;
    asm volatile(
        "mov $0, %%ax\n"     // set mode
        "int $0x33"         // do it
    : "=a"(axvalue), "=b"(number_buttons)
    :
    :
    );
    return (axvalue == 0xFFFF) ? true : false;
}

/*
 * INT 33h / AX=0001 - show mouse pointer.
*/
void show_mouse_pointer()
{
    asm volatile(
        "mov $1, %ax\n"     // set mode
        "int $0x33"         // do it
    );
}

/*
 * INT 33h / AX=0002 - hide visible mouse pointer.
*/
void hide_mouse_pointer()
{
    asm volatile(
        "mov $2, %ax\n"     // set mode
        "int $0x33"         // do it
    );
}

/*void get_mouse_release()
{
    uint8_t button;
    asm volatile(
        "mov $6, %%ax\n"        // set mode
        "int $0x33\n"           // do it
    : "=a"(button)
    :
    :
    );

    if(button == 0x01) {
        print("LEFT\n$");
    }
    if(button == 0x02) {
        print("RIGHT\n$");
    }
}*/

/*
 * INT 33h / AX=0003 - get mouse position and status of its buttons
 * BX = 1 if left button is down
 * BX = 2 if right button is down
 * BX = 3 if both buttons are down
 * CX = x
 * DX = y
 * note: in graphical 320x200 mode the value of CX is doubled.
*/
struct MOUSE get_mouse_status()
{
    struct MOUSE m;
    m.LEFT = false;
    m.RIGHT = false;
    m.BOTH = false;
    m.OFFSET = 0;
    m.X = 0;
    m.Y = 0;
    m.CLICK_INDEX = -1;

    asm volatile(
        "mov $0x03, %%ax\n"     // set mode
        "int $0x33\n"       // do it
        "shr $1, %%bx\n"
        "jc is_left%=\n"
        "is_left%=:\n"
        "mov $1, %0\n"
    : "=q"(m.LEFT)
    :
    :
    );

    /*asm volatile(
        "mov $3, %%ax\n"     // set mode
        "int $0x33\n"       // do it
        //"shr $1, %cx\n"     // x / 2 - in this mode the value of CX is doubled.
        //"cmp $1, %%bx\n"
        //"je is_left%=\n"
        //"is_left%=:\n"
        //"mov $1, %0\n"
        *//*"cmp $2, %%bx\n"
        "je is_right%=\n"
        "cmp $3, %%bx\n"
        "je is_both%=\n"
        "jmp done%=\n"*//*
        //"is_left%=:\n"
        //"mov $1, %0\n"
       *//* "jmp done%=\n"
        "is_right%=:\n"
        "mov $0, %0\n"
        "mov $1, %1\n"
        "mov $0, %2\n"
        "jmp done%=\n"
        "is_both%=:\n"
        "mov $0, %0\n"
        "mov $0, %1\n"
        "mov $1, %2\n"
        "jmp done%=\n"*//*
        //"done%=:\n"
    : //"=r"(m.LEFT), "=q"(m.RIGHT), "=q"(m.BOTH) // output variable
    :                               // no inputs
    :                               // clobbers
    );*/

    /*asm volatile(
        "mov $3, %%ax\n"
        "int $0x33\n"

        *//*"test $1, %%bx\n"
        "je is_left%=\n"
        "jmp done%=\n"
        "is_left%=:\n"
        "mov $1, %0\n"
        "done%=:\n"*//*

        //"shr $1, %%cx\n"  // divide x by 2 because in 13h mode CX is doubled
        //"mov %%cx, %0\n"  // set x into m.X
        //"mov %%dx, %1\n"    // set y into m.Y

        *//*"test $1, %%bx\n"  // is left click?
        "je is_left%=\n"
        "mov $0, %2\n"
        "jmp done%=\n"
        "is_left%=:\n"
        "mov $1, %2\n"
        "done%=:\n"*//*
        //"shr $1, %%cx\n" // x/2 in this mode the value of CX is doubled
        //"mov %%dx %%ax\n" // Y coord to AX
        //"mov $320 %%dx\n"
        //"mul %%dx\n"    // multiply AX by 320
        //"add %%ax, %%cx\n" //add X coord
        // cursor position now in ax
        *//*"cmp $1, %%bx\n"
        "je is_left%=\n"
        "jmp done%=\n"
        "is_left%=:\n"
        "mov $1, %0\n"
        "done%=:\n"*//*
    : "=c"(m.X), "=d"(m.Y), "=b"(m.CLICK_INDEX)
    :
    :
    );*/

    //m.OFFSET = (320 * m.Y) + m.X;

    //m.X = m.X / 2;
    // outputs x, y, left, right, both
    // be nice to get pixel color under mouse?
    return m;
}

#endif //DOSMATCHER_MOUSE_H