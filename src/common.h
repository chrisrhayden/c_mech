#ifndef C_ROGUELIKE_COMMON
#define C_ROGUELIKE_COMMON

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t x1;
    size_t y1;
    size_t x2;
    size_t y2;
} Rect;

typedef struct {
    int x;
    int y;
} Point;

bool rect_intersects(Rect a, Rect b);

Point rect_center(Rect a);

size_t rand_range(size_t min, size_t max);

size_t min(size_t a, size_t b);
size_t max(size_t a, size_t b);

#endif
