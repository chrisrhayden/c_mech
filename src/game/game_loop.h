#ifndef C_ROGUELIKE_GAME_LOOP
#define C_ROGUELIKE_GAME_LOOP

#include "game_data.h"

enum LoopState {
    Running,
    Paused,
};

typedef struct {
    enum LoopState loop_state;
} GameLoop;

bool run_game(GameData *data);

#endif
