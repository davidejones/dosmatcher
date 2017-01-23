#ifndef DOSMATCHER_MOUSE_H
#define DOSMATCHER_MOUSE_H

/*
 * INT 33h / AX=0000 - mouse ininialization. any previous mouse pointer is hidden.
 * AX = 0FFFFh if successful
 * AX = 0 if failed
 * BX = number of mouse buttons
*/
void mouse_init()
{
    asm volatile(
        "mov $0, %ax\n"     // set mode
        "int $0x33"         // do it
    );
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

#endif //DOSMATCHER_MOUSE_H
