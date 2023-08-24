#include "../log.h"

#include "../components/entity.h"
#include "context.h"
#include "game_data.h"

GameState *init_game_state() {
    log_info("initing game state");

    GameState *state = malloc(sizeof(GameState));

    if (state == NULL) {
        log_error("cant init state");

        return NULL;
    }

    state->game_map = NULL;

    state->ent_cache = init_entity_cache();

    if (state->ent_cache == NULL) {
        free(state);
        return NULL;
    }

    return state;
}

void drop_game_state(GameState *state) {
    if (state->game_map) {
        drop_game_map(state->game_map);
    }

    if (state->ent_cache) {
        drop_entity_cache(state->ent_cache);
    }

    free(state);
}

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

    // data->font_cache = init_font_cache("./assets/unscii-8.ttf", 16);
    data->font_cache = init_font_cache("./assets/unscii-16-full.ttf", 24);

    if (data->font_cache == NULL) {
        free(data);
        return NULL;
    }

    data->state = init_game_state();

    if (data->state == NULL) {
        drop_game_data(data);
        return NULL;
    }

    log_info("finished game data");

    return data;
}

void drop_game_data(GameData *data) {
    if (data->ctx) {
        drop_context(data->ctx);
    }

    if (data->font_cache) {
        drop_font_cache(data->font_cache);
    }

    if (data->state) {
        drop_game_state(data->state);
    }

    free(data);
}

void drop_state(GameState *state) {
    free(state);
}
