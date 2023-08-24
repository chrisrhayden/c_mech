#ifndef C_ROGUELIKE_HANDLE_INPUT
#define C_ROGUELIKE_HANDLE_INPUT

#include <SDL2/SDL.h>

#include "player.h"

enum UserAction handle_input(SDL_Event *event);

#endif
