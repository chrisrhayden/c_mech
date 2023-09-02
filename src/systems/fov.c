#include "fov.h"
#include "../log.h"
#include <assert.h>

Fov *init_fov(GameMap *game_map) {
    Fov *fov = malloc(sizeof(GameMap));

    if (fov == NULL) {
        log_error("could not init fov");
        return NULL;
    }

    size_t len = game_map->width * game_map->height;

    fov->data = calloc(sizeof(bool), len);

    if (fov->data == NULL) {
        log_error("could not init fov data");

        free(fov);

        return NULL;
    }

    fov->len = len;
    fov->width = game_map->width;
    fov->height = game_map->height;
    fov->radius = 5;

    return fov;
}
