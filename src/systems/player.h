#ifndef C_ROGUELIKE_PLAYER
#define C_ROGUELIKE_PLAYER

#include "../components/entity.h"
#include "../game/game_data.h"

enum UserAction {
    MoveDown,
    MoveUp,
    MoveLeft,
    MoveRight,
    Nothing,
    Quit,
};

void player_handle_action(GameState *state, enum UserAction user_action);

#endif
