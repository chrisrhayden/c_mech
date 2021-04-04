#ifndef C_MACH_GAME_DATA
#define C_MACH_GAME_DATA

#include "context.h"

/* this will be where the, map object and entities live eventually
 */
typedef struct {
    int x;
    int y;
} GameState;

typedef struct {
    Context *ctx;
    GameState *state;
} GameData;

GameData *init_game_data(const char *title, int width, int height);

void drop_game_data(GameData *data);

void drop_state(GameState *state);

#endif
