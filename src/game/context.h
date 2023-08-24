#ifndef C_ROGUELIKE_CONTEXT
#define C_ROGUELIKE_CONTEXT

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct {
    int width;
    int height;
} WindowSize;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    WindowSize size;
} Context;

Context *init_context(const char *title, int width, int length);

void drop_context(Context *ctx);

#endif
