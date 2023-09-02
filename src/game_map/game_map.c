#include <stdlib.h>

#include "../components/tiles.h"
#include "../log.h"
#include "game_map.h"

GameMap *init_game_map(size_t w, size_t h) {
    GameMap *map = malloc(sizeof(GameMap));

    if (map == NULL) {
        log_error("coul not alloacte map");
        return NULL;
    }

    size_t len = w * h;

    map->data = malloc(sizeof(MapTile) * len);

    if (map->data == NULL) {
        log_error("could not alloacte map data");
        free(map);
        return NULL;
    }

    map->width = w;
    map->height = h;
    map->len = len;

    return map;
}

void drop_game_map(GameMap *map) {
    if (map->data) {
        free(map->data);
    }

    free(map);
}
