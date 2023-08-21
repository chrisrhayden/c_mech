#ifndef C_ROGUELIKE_HANDLE_INPUT
#define C_ROGUELIKE_HANDLE_INPUT

#include <SDL2/SDL.h>

#include "game_loop.h"

enum LoopState handle_input(SDL_Event *event);

#endif
