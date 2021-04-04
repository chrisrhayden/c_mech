#include "context.h"

#include "log.h"

Context *init_context(const char *title, int width, int height) {
    log_trace("initializing context");

    Context *ctx = malloc(sizeof(*ctx));

    if (ctx == NULL) {
        return NULL;
    }

    ctx->size.width = width;
    ctx->size.height = height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        log_error("SDL Error -- %s", SDL_GetError());

        free(ctx);

        return NULL;
    }

    ctx->window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         ctx->size.width, ctx->size.height, SDL_WINDOW_SHOWN);

    if (ctx->window == NULL) {
        log_error("SDL Error -- %s", SDL_GetError());

        free(ctx);

        SDL_Quit();

        return NULL;
    }

    ctx->renderer = SDL_CreateRenderer(
        ctx->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (ctx->renderer == NULL) {
        log_error("SDL Error -- %s", SDL_GetError());

        free(ctx);

        return NULL;
    }

    return ctx;
}

void drop_context(Context *ctx) {
    SDL_Quit();

    free(ctx);
}
