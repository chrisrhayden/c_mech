#ifndef C_MACH_GAME_DATA
#define C_MACH_GAME_DATA

#include "context.h"

typedef struct {
    Context *ctx;
} GameData;

GameData *init_game_data(const char *title, int width, int height);

#endif
