#ifndef DOSMATCHER_KEYBOARD_H
#define DOSMATCHER_KEYBOARD_H

#include "types.h"
#include "utils.h"

#define KEY_ESCAPE (0x1b)
#define KEY_UP_ARROW (0x48)
#define KEY_LEFT_ARROW (0x4b)
#define KEY_DOWN_ARROW (0x50)
#define KEY_RIGHT_ARROW (0x4d)
#define KEY_W (0x11)
#define KEY_A (0x1e)
#define KEY_S (0x1f)
#define KEY_D (0x20)
#define KEY_ENTER (0x1c)
#define KEY_SPACE (0x39)

struct KEYBOARD {
    uint16_t KEY;
    uint8_t KEY_LOW;
    uint8_t KEY_HIGH;
    bool PRESSED;
};

/*
 * INT 16h AH = 01h - check for keystroke in the keyboard buffer.
 * ZF = 1 if keystroke is not available.
 * ZF = 0 if keystroke available.
 * AH = BIOS scan code.
 * AL = ASCII character
 */
struct KEYBOARD check_key()
{
    struct KEYBOARD kb;
    kb.KEY = 0;
    kb.KEY_LOW = 0xff;
    kb.KEY_HIGH = 0xff;
    kb.PRESSED = false;
    asm volatile (
        "mov $1, %%ah\n"            // scancode for keyboard check
        "int $0x16\n"               // do it
        "jnz is_not_zero%=\n"       // if ZeroFlag is not 0 then go to is_not_zero else continue
        "mov $0, %0\n"              // set 0 to output variable (kb.PRESSED)
        "mov $0, %%ax\n"            // set 0 to k.KEY
        "jmp done%=\n"              // go to return
        "is_not_zero%=:\n"          // is_not_zero:
        "mov $1, %0\n"              // set 1 to output variable (kb.PRESSED)
        "mov $0, %%ah\n"            // scancode for getkey
        "int $0x16\n"               // do it (put into kb.KEY)
        "done%=:\n"                 // return
    : "=rm"(kb.PRESSED), "=a"(kb.KEY) // output variable `r` register `m` memory `=` write only, `a` put into ax
    :                               // no inputs
    :                               // clobbers
    );

    kb.KEY_LOW = kb.KEY & 0xff;
    kb.KEY_HIGH = (kb.KEY >> 8) & 0xff;
    // http://stackoverflow.com/questions/2876275/what-are-the-ascii-values-of-up-down-left-right
    //int lo = k.KEY & 0xff;
    //int hi = (k.KEY & 0xFF00) >> 8;
    //k.KEY = (lo == 0) ? hi + 256 : lo;
    return kb;
}

char get_key(void) {
    short s;
    asm volatile(
        "xor %%ah, %%ah\n"
        "int $0x16"
    : "=al" (s) : );
    return (char)s;
}

#endif //DOSMATCHER_KEYBOARD_H
