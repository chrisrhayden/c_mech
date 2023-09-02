#ifndef C_ROGUELIKE_GAME_MAP
#define C_ROGUELIKE_GAME_MAP

#include "../common.h"
#include <stddef.h>

typedef struct {
    bool blocking;
    bool visited;
    wchar_t sprite;
} MapTile;

typedef struct {
    size_t width;
    size_t height;
    size_t len;
    MapTile *data;
} GameMap;

GameMap *init_game_map(size_t width, size_t height);

void drop_game_map(GameMap *map);

#endif
