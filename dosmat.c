#include "init.h"
#include "types.h"
#include "alloc.h"
#include "time.h"
#include "utils.h"
#include "vga.h"
#include "mouse.h"
#include "keyboard.h"

#define BACKGROUND         17



static struct card *cards;
static uint8_t card_cols = 9;
static uint8_t card_rows = 4;
static size_t cards_max = 36;
static uint8_t card_width = 22;
static uint8_t card_height = 30;
static uint8_t card_spacing = 10;
struct KEYBOARD kb;
struct MOUSE ms;
int highlighted_card_index = 0;
bool isRunning = true;
int temporaryRevealCount = 0;
int highlightbordersize = 5;

void drawCards()
{
    for(uint8_t i = 0; i < cards_max; i++) {
        switch (cards[i].mode)
        {
            case 0x02:
                // draw the card with highlight
                my_rect_fill(cards[i].x, cards[i].y, card_width, card_height, cards[i].color, highlightbordersize, WHITE, 0x00);
                break;
            case 0x03:
                // a matched card keep showing revealed
                my_rect_fill(cards[i].x, cards[i].y, card_width, card_height, BLUE, highlightbordersize, BACKGROUND, cards[i].shape);
                break;
            case 0x04:
                // temporary reveal
                my_rect_fill(cards[i].x, cards[i].y, card_width, card_height, BLUE, highlightbordersize, BACKGROUND, cards[i].shape);
                break;
            case 0x05:
                // temporary reveal but highlighted
                my_rect_fill(cards[i].x, cards[i].y, card_width, card_height, BLUE, highlightbordersize, WHITE, cards[i].shape);
                break;
            case 0x01:
            default:
                // draw the card normally
                my_rect_fill(cards[i].x, cards[i].y, card_width, card_height, cards[i].color, highlightbordersize, BACKGROUND, 0x00);
                break;
        }
    }
}

void setHighlightCard(int index)
{
    for(uint8_t i = 0; i < cards_max; i++) {
        if(cards[i].mode == 0x02) {
            cards[i].mode = 0x01;
        } else if(cards[i].mode == 0x05) {
            cards[i].mode = 0x04;
        }
    }
    cards[index].mode = 0x02;
    drawCards();
}

void revertCards()
{
    for(uint8_t i = 0; i < cards_max; i++) {
        if(cards[i].mode == 0x04) {
            cards[i].mode = 0x01;
        } else if(cards[i].mode == 0x05) {
            cards[i].mode = 0x02;
        }
    }
    temporaryRevealCount = 0;
    drawCards();
}

void revealCard(int index)
{
    temporaryRevealCount++;
    cards[index].mode = 0x05;
    drawCards();
    if(temporaryRevealCount == 2) {
        // this is last one if no match set timer to revert cards
        msleep(1000);
        revertCards();
    }
}

void init()
{
    free();
    cards = sbrk(cards_max * sizeof(cards[0]));
    int count = 0;
    uint16_t startxpos = 10;
    uint16_t startypos = 5;
    for(uint16_t i = 0; i < card_rows; i++) {
        for(uint16_t j = 0; j < card_cols; j++) {
            uint16_t x = startxpos + (card_spacing + card_width) * j;
            if(x == 0) {
                x = card_spacing;
            } else {
                x += card_spacing;
            }
            uint16_t y = startypos + (card_spacing + card_height) * i;
            if(y == 0) {
                y = card_spacing;
            } else {
                y += card_spacing;
            }
            cards[count] = (struct card) {
                .x = x,
                .y = y,
                .color = RED,
                .mode = 0x01,
                .shape = 0x02
            };
            if(i == 0 && j == 0) {
                cards[count].mode = 0x02;
            }
            count++;
        }
    }
    vga_clear(BACKGROUND);
}

int dosmain(void)
{
    //print("Hello, World!\n$");

    /*
     * This is mapped 0 to 319 on the x axis and 0 to 199 on the y axis, with the origin (0,0) at the top-left corner
     * Since this is a 256-color mode, each pixel represents 8 bits (28=256) or one byte, so the memory needed is 320*200 or 64,000 bytes.
    */

    // http://www.eecg.toronto.edu/~amza/www.mindsec.com/files/x86regs.html on registers
    // https://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture

    setGraphicsMode();
    //clearScreen(WHITE);
    //vga_clear(BLACK);

    //char string[3] = {'a', '\n', '$'};
    //bool mouse_initialized = mouse_init();
    //show_mouse_pointer();

    // initialize objects
    init();

    while(isRunning)
    {
        // vsync
        vga_vsync();

        // render cards from state if something changed
        drawCards();

        // lets read the keyboard input
        kb = check_key();

        /*if(mouse_initialized) {
            //ms = get_mouse_status();
            //print("mouse ready\n$");
            get_mouse_release();
        }*/

        //drawPixelAtOffset(ms.OFFSET, BLACK);

        /*if(ms.CLICK_INDEX == -1) {
            print("nope\n$");
        }
        if(ms.CLICK_INDEX == 1) {
            print("left click\n$");
        }
        if(ms.CLICK_INDEX == 0x01) {
            print("lefty clicky\n$");
        }
        if(ms.CLICK_INDEX == 0x10) {
            print("lefty2 clicky2\n$");
        }*/

        /*if(ms.LEFT) {
            print("left click\n$");
        }
        if(ms.RIGHT) {
            print("right click\n$");
        }
        if(ms.BOTH) {
            print("both click\n$");
        }*/

        //string[0] = (char)kb.KEY;
        //print(string);

        //if(kb.PRESSED) {
        //isRunning = false;
        //}

        if(kb.PRESSED)
        {
            if(kb.KEY_LOW == KEY_ESCAPE) {
                isRunning = false;
            }

            if(kb.KEY_HIGH == KEY_A || kb.KEY_HIGH == KEY_LEFT_ARROW) {
                highlighted_card_index--;
                if(highlighted_card_index < 0) {
                    highlighted_card_index = cards_max-1;
                }
                setHighlightCard(highlighted_card_index);
            }

            if(kb.KEY_HIGH == KEY_D || kb.KEY_HIGH == KEY_RIGHT_ARROW) {
                highlighted_card_index++;
                if(highlighted_card_index > cards_max-1) {
                    highlighted_card_index = 0;
                }
                setHighlightCard(highlighted_card_index);
            }

            if(kb.KEY_HIGH == KEY_W || kb.KEY_HIGH == KEY_UP_ARROW) {
                highlighted_card_index = highlighted_card_index - card_cols;
                if(highlighted_card_index < 0) {
                    highlighted_card_index = cards_max+highlighted_card_index;
                }
                setHighlightCard(highlighted_card_index);
            }

            if(kb.KEY_HIGH == KEY_S || kb.KEY_HIGH == KEY_DOWN_ARROW) {
                highlighted_card_index = highlighted_card_index + card_cols;
                if(highlighted_card_index > cards_max-1) {
                    highlighted_card_index = (highlighted_card_index - (cards_max-1)) - 1;
                }
                setHighlightCard(highlighted_card_index);
            }

            if(kb.KEY_HIGH == KEY_ENTER || kb.KEY_HIGH == KEY_SPACE) {
                revealCard(highlighted_card_index);
            }
        }
    }

    setTextMode();
    return 0;
}