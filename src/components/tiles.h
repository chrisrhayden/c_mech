#ifndef C_ROGUELIKE_COMPONENTS
#define C_ROGUELIKE_COMPONENTS

#include <wchar.h>

enum Tile {
    Floor,
    Wall,
    Player,
};

wchar_t match_tile(enum Tile tile);

#endif
