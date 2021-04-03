#include "log.h"

#include "context.h"
#include "game_data.h"

GameData *init_game_data(const char *title, int width, int height) {
    log_trace("initializing game data");

    GameData *data = malloc(sizeof(*data));

    if (data == NULL) {
        return NULL;
    }

    data->ctx = init_context(title, width, height);

    if (data->ctx == NULL) {
        free(data);

        return NULL;
    }

    return data;
}
