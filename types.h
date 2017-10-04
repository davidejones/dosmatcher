#ifndef DOSMATCHER_TYPES_H
#define DOSMATCHER_TYPES_H

typedef unsigned char          uint8_t;
typedef char                   int8_t;
typedef unsigned short         uint16_t;
typedef short                  int16_t;
typedef unsigned long int      uint32_t;
typedef long int               int32_t;
typedef unsigned long long int uint64_t;
typedef long long int          int64_t;
typedef unsigned short         size_t;

#define true  1
#define false 0
typedef char                   bool;

enum VGA_COLOR {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GRAY, DARK_GRAY,
    LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_MAGENTA,
    YELLOW, WHITE
};

struct card {
    uint8_t color;
    uint16_t x;
    uint16_t y;
    uint8_t mode;  // 0x01 = ready, 0x02 = highlighted with mouse/keyboard, 0x03 = matched, 0x04 = temporary revealed
    uint8_t shape; // 0x01 = square, 0x02 = triangle, 0x03 = circle
};

#endif //DOSMATCHER_TYPES_H
