#ifndef DOSMATCHER_DOS_H
#define DOSMATCHER_DOS_H

#include "types.h"

static void print(char *string)
{
    asm volatile (
        "mov $0x09, %%ah\n"     // set mode
        "int $0x21\n"           // do it
    :                           // no outputs
    : "d"(string)               // input variable `d` register dx
    : "ah");                    // clobber ah
}

static void * my_memset(void *dst, int c, size_t n)
{
    if (n) {
        char *d = dst;
        do {
            *d++ = c;
        } while (--n);
    }
    return dst;
}

#endif //DOSMATCHER_DOS_H
