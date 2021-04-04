#ifndef C_MECH_LOOP
#define C_MECH_LOOP

#include "game_data.h"

enum LoopState {
    Running,
    Paused,
    Nothing,
    Quit,
};

typedef struct {
    enum LoopState loop_state;
} GameLoop;

bool run_game(GameData *data);

#endif
