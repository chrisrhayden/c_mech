#include "log.h"

#include "context.h"
#include "game_data.h"

GameData *init_game_data(const char *title, int width, int height) {
    log_info("initializing game data");

    GameData *data = malloc(sizeof(*data));

    if (data == NULL) {
        return NULL;
    }

    data->ctx = init_context(title, width, height);

    if (data->ctx == NULL) {
        free(data);

        return NULL;
    }

    data->font_cache = init_font_cache("./assets/unscii-8.ttf", 16);

    if (data->font_cache == NULL) {
        free(data);
        return NULL;
    }

    data->state = NULL;

    return data;
}

void drop_game_data(GameData *data) {
    if (data->ctx) {
        drop_context(data->ctx);
    }

    if (data->state != NULL) {
        drop_state(data->state);
    }

    free(data);
}

void drop_state(GameState *state) {
    free(state);
}
