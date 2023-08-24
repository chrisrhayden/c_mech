#include "tiles.h"
#include "../log.h"

wchar_t match_tile(enum Tile tile) {
    switch (tile) {
        case Floor:
            return 0x0020;
        case Wall:
            return 0x0023;
        case Player:
            return 0x263A;
        default:
            return 0x0020;
    }
}
