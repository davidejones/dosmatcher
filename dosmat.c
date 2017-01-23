#include "init.h"
#include "types.h"
#include "utils.h"
#include "vga.h"
#include "mouse.h"
#include "keyboard.h"

enum VGA_COLOR {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GRAY, DARK_GRAY,
    LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_MAGENTA,
    YELLOW, WHITE
};

int dosmain(void)
{
    setGraphicsMode();
    vga_clear(WHITE);
    bool isRunning = true;
    struct KEYBOARD kb;

    while(isRunning)
    {
        vga_vsync();
        kb = check_key();

        if(kb.PRESSED)
        {
            if(kb.KEY_LOW == KEY_ESCAPE) {
                isRunning = false;
            }

            if(kb.KEY_HIGH == KEY_A || kb.KEY_HIGH == KEY_LEFT_ARROW) {
                print("left key\n$");
            }

            if(kb.KEY_HIGH == KEY_D || kb.KEY_HIGH == KEY_RIGHT_ARROW) {
                print("right key\n$");
            }

            if(kb.KEY_HIGH == KEY_W || kb.KEY_HIGH == KEY_UP_ARROW) {
                print("up key\n$");
            }

            if(kb.KEY_HIGH == KEY_S || kb.KEY_HIGH == KEY_DOWN_ARROW) {
                print("down key\n$");
            }

            if(kb.KEY_HIGH == KEY_ENTER) {
                print("enter\n$");
            }

            if(kb.KEY_HIGH == KEY_SPACE) {
                print("space\n$");
            }
        }
    }

    setTextMode();
    return 0;
}