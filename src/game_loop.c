#include <SDL2/SDL.h>

#include "log.h"

#include "context.h"
#include "game_loop.h"
#include "handle_input.h"

bool render_screen(Context *ctx) {
    SDL_SetRenderDrawColor(ctx->renderer, 0xff, 0xff, 0xff, 0xff);

    SDL_RenderClear(ctx->renderer);

    SDL_RenderPresent(ctx->renderer);

    return true;
}

bool run_game(GameData *data) {
    log_trace("running game loop");

    bool quit = false;

    enum LoopState loop_state = Paused;

    SDL_Event event_adder;

    SDL_Event *event = &event_adder;

    while (!quit) {
        loop_state = handle_input(event);

        if (loop_state == Quit) {
            quit = true;
        } else {
            render_screen(data->ctx);
        }
    }

    return true;
}
