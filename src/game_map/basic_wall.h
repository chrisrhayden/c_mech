#ifndef C_ROGUELIKE_BASIC_WALL
#define C_ROGUELIKE_BASIC_WALL

#include "../components/entity.h"
#include "game_map.h"

GameMap *init_basic_wall(EntityCache *ent_cache, size_t width, size_t hight);

#endif
