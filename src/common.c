#include "common.h"
#include <assert.h>
#include <stdlib.h>

bool rect_intersects(Rect a, Rect b) {
    return (a.x1 < b.x2) && (a.x2 > b.x1) && (a.y1 < b.y2) && (a.y2 > b.y1);
}

Point rect_center(Rect a) {
    size_t c_x = (a.x1 + a.x2) / 2;
    size_t c_y = (a.y1 + a.y2) / 2;

    Point pos = {.x = c_x, .y = c_y};

    return pos;
}

size_t rand_range(size_t min, size_t max) {
    assert(min < max);

    // i think i need + 1 for inclusion
    size_t to_mod = (max + 1) - min;

    return (rand() % to_mod) + min;
}

size_t min(size_t a, size_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

size_t max(size_t a, size_t b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}
