#ifndef DOSMATCHER_UTILS_H
#define DOSMATCHER_UTILS_H

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



//void * x_memset (void* dest, int nByte, unsigned short count)
//{
//    asm volatile (
//        "mov $0x0C, %ah\n"  // pixel plot
//        "mov $0x02, %al\n"  // what color (green dark)
//        "mov $0x2F, %cx\n"  // x location, from 0..319
//        "mov $0x2F, %dx\n"  // y location, from 0..199
//        "int $0x10"         // do it
//    );
//    asm volatile (
//        "mov ecx, [count]\n"
//        "mov edi, [dest]\n"
//        "mov eax, [nByte]\n"
//        "mov ebx, ecx\n"
//        "rep stosb\n"
//        : /* no output */
//        : "a"(nByte), "d"(dest), "c"(count)
//        :
//    );
//}

/*void *memset(void *s, int c, size_t n)
{
    int i;
    char *ss = (char *) s;

    for (i = 0; i < (int)n; i++)
        ss[i] = c;
    asm volatile (
    "cld\n"
            "rep\n"
            "stosb"
    :
    : "a"(c), "D"(ss), "c"(n)
    );
    return s;
}*/

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
/*
void* my_memset(void *s, int c, size_t len) {
    unsigned char *dst = s;
    while (len > 0) {
        *dst = (unsigned char) c;
        dst++;
        len--;
    }
    return s;
}*/

#endif //DOSMATCHER_UTILS_H
