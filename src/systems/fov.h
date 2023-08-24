#ifndef C_ROGUELIKE_FOV
#define C_ROGUELIKE_FOV

#include <stdbool.h>
#include <stdlib.h>

#include "../common.h"

typedef struct {
    size_t width;
    size_t height;
    size_t len;
    bool *data;
} Fov;

Fov *init_fov();

bool calculate_fov(Fov *fov, Point *start_point);

#endif
