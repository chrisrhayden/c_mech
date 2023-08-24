#include "player.h"
#include "../log.h"

void move(GameState *state, int x_off, int y_off) {
    Entity *player = state->ent_cache->player;

    size_t new_x = player->pos.x + x_off;
    size_t new_y = player->pos.y + y_off;

    size_t idx = new_x + (new_y * state->game_map->width);

    if (state->game_map->data[idx].blocking) {
        return;
    }

    bool no_conflicts = true;

    for (size_t i = i; i < state->ent_cache->entitys_len; ++i) {
        Entity *ent = state->ent_cache->entitys[i];

        if (ent) {
            if (new_x == ent->pos.x && new_y == ent->pos.y) {
                no_conflicts = false;
                break;
            }
        }
    }

    if (no_conflicts) {
        player->pos.x = new_x;
        player->pos.y = new_y;
    }

    return;
}

void player_handle_action(GameState *state, enum UserAction user_action) {
    switch (user_action) {
        case MoveUp: {
            move(state, 0, -1);
            break;
        }
        case MoveDown: {
            move(state, 0, 1);
            break;
        }
        case MoveLeft: {
            move(state, -1, 0);
            break;
        }
        case MoveRight: {
            move(state, 1, 0);
            break;
        }
        default:
            break;
    }
}
