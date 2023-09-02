#ifndef C_ROGUELIKE_FOV
#define C_ROGUELIKE_FOV

#include <stdbool.h>
#include <stdlib.h>

#include "../common.h"
#include "../game_map/game_map.h"

typedef struct {
    size_t width;
    size_t height;
    size_t len;
    bool *data;
    size_t radius;

} Fov;

Fov *init_fov(GameMap *game_map);

bool calculate_fov(Fov *fov, GameMap *game_map, Point *start_point);

#endif
