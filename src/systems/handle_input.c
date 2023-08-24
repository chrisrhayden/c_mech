#include <SDL2/SDL.h>

#include "../log.h"

#include "handle_input.h"
#include "player.h"

enum UserAction handle_input(SDL_Event *event) {
    enum UserAction user_action = Nothing;

    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            user_action = Quit;
            break;
        } else if (event->type == SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
                case SDLK_q:
                case SDLK_ESCAPE: {
                    user_action = Quit;
                    break;
                }
                case SDLK_h:
                case SDLK_LEFT: {
                    user_action = MoveLeft;
                    break;
                }
                case SDLK_l:
                case SDLK_RIGHT: {
                    user_action = MoveRight;
                    break;
                }
                case SDLK_j:
                case SDLK_DOWN: {

                    user_action = MoveDown;
                    break;
                }
                case SDLK_k:
                case SDLK_UP: {

                    user_action = MoveUp;
                    break;
                }
                default: {
                    user_action = Nothing;
                    // i dont think this break is necessary
                    break;
                }
            }
        }
    }

    return user_action;
}
