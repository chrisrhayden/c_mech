#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

#include "../log.h"

// #include "../common.h"
#include "../font/font_cache.h"
#include "../game_map/basic_wall.h"
#include "../systems/handle_input.h"
#include "../systems/player.h"
#include "context.h"
#include "game_loop.h"

bool render_map(GameData *data, SDL_Texture *sprites) {
    GameMap *game_map = data->state->game_map;

    size_t curent_x = 0;

    size_t start_x_px = data->font_cache->max_char_width;
    size_t start_y_px = data->font_cache->max_char_height;

    for (size_t i = 0; i < game_map->len; ++i) {
        MapTile *tile = &game_map->data[i];

        SDL_Rect *src_rect = get_sprite_rect(data->font_cache, &tile->sprite);

        if (src_rect == NULL) {
            return NULL;
        }

        SDL_Rect dest_rect;

        dest_rect.x = start_x_px;
        dest_rect.y = start_y_px;
        dest_rect.w = src_rect->w;
        dest_rect.h = src_rect->h;

        SDL_SetTextureColorMod(sprites, 0, 0, 0);

        SDL_RenderCopy(data->ctx->renderer, sprites, src_rect, &dest_rect);

        curent_x += 1;
        if (curent_x < game_map->width) {
            start_x_px += data->font_cache->max_char_width;
        } else {
            curent_x = 0;
            start_x_px = data->font_cache->max_char_width;

            start_y_px += data->font_cache->max_char_height;
        }
    }

    return true;
}

bool render_entitys(GameData *data, SDL_Texture *sprites) {
    FontCache *cache = data->font_cache;

    Entity *player = data->state->ent_cache->player;

    Point player_pos = player->pos;

    size_t start_x_px =
        (player_pos.x * cache->max_char_width) + cache->max_char_width;

    size_t start_y_px =
        (player_pos.y * cache->max_char_height) + cache->max_char_height;

    SDL_Rect *src_rect = get_sprite_rect(data->font_cache, &player->sprite);

    if (src_rect == NULL) {
        return false;
    }

    SDL_Rect dest_rect;
    dest_rect.x = start_x_px;
    dest_rect.y = start_y_px;
    dest_rect.w = src_rect->w;
    dest_rect.h = src_rect->h;

    SDL_SetTextureColorMod(sprites, 255, 0, 0);
    SDL_RenderCopy(data->ctx->renderer, sprites, src_rect, &dest_rect);

    return true;
}

bool render_screen(GameData *data, SDL_Texture *sprites,
                   enum LoopState loop_state) {
    bool successes = false;
    SDL_SetRenderDrawColor(data->ctx->renderer, 255, 255, 255, 255);
    SDL_RenderClear(data->ctx->renderer);

    successes = render_map(data, sprites);
    successes = render_entitys(data, sprites);

    SDL_RenderPresent(data->ctx->renderer);

    return successes;
}

bool run_game(GameData *data) {
    data->state->game_map = init_basic_wall(data->state->ent_cache, 66, 28);

    if (data->state->game_map == NULL) {
        return false;
    }

    FontSpriteCache *sprites = init_sprite_cache(data->font_cache);

    if (sprites == NULL) {
        return NULL;
    }

    SDL_Surface *sp = make_font_surface(sprites);

    SDL_SetColorKey(sp, SDL_TRUE, SDL_MapRGB(sp->format, 255, 00, 255));

    SDL_Texture *sp_texture =
        SDL_CreateTextureFromSurface(data->ctx->renderer, sp);

    free(sp);

    enum LoopState loop_state = Paused;
    enum UserAction user_action = Nothing;

    SDL_Event event_adder;

    SDL_Event *event = &event_adder;

    log_info("entering game loop");

    while (true) {
        render_screen(data, sp_texture, loop_state);

        user_action = handle_input(event);

        if (user_action == Quit) {
            break;
        }

        if (user_action != Nothing) {
            player_handle_action(data->state, user_action);
        }
    }

    return true;
}
