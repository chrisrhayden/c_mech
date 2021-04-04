#include <SDL2/SDL.h>

#include "log.h"

#include "handle_input.h"

enum LoopState handle_input(SDL_Event *event) {
    enum LoopState loop_state = Nothing;

    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            loop_state = Quit;
            break;
        } else if (event->type == SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
                case SDLK_q:
                case SDLK_ESCAPE: {
                    loop_state = Quit;
                    break;
                }
                default: {
                    loop_state = Nothing;
                    // i dont think if this is necessary
                    break;
                }
            }
        }
    }

    return loop_state;
}
