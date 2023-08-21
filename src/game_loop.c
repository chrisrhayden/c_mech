#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

#include "log.h"

#include "context.h"
#include "font/font_cache.h"
#include "game_loop.h"
#include "handle_input.h"

bool render_screen(GameData *data, SDL_Surface *sp, SDL_Surface *win_s) {
    SDL_SetRenderDrawColor(data->ctx->renderer, 0xff, 0xff, 0xff, 0xff);

    SDL_RenderClear(data->ctx->renderer);

    SDL_BlitSurface(sp, NULL, win_s, NULL);
    SDL_UpdateWindowSurface(data->ctx->window);

    SDL_RenderPresent(data->ctx->renderer);

    return true;
}

bool run_game(GameData *data) {
    log_info("running game loop");

    bool quit = false;

    log_info("making sprites game loop");
    FontSpriteCache *sprites = init_srpite_cache(data->font_cache);

    if (sprites == NULL) {
        return NULL;
    }

    SDL_Surface *sp = make_font_surface(sprites);
    SDL_Surface *win_s = SDL_GetWindowSurface(data->ctx->window);

    enum LoopState loop_state = Paused;

    SDL_Event event_adder;

    SDL_Event *event = &event_adder;

    while (!quit) {

        loop_state = handle_input(event);

        if (loop_state == Quit) {
            quit = true;
        } else {
            render_screen(data, sp, win_s);
        }
    }

    return true;
}
