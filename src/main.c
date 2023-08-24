#include <locale.h>
#include <stdbool.h>
#include <stdio.h>

#include "log.h"

#include "game/game_data.h"
#include "game/game_loop.h"

bool init_logging(int log_level) {
    log_set_level(log_level);

    return true;
}

int main() {
    setlocale(LC_ALL, "");

    if (init_logging(LOG_TRACE) == false) {
        fprintf(stderr, "could not init logging, quiting now");

        return 1;
    }

    log_info("starting roguelike");

    char title[] = "c_roguelike";

    int width = 1200;
    int height = 900;

    GameData *data = init_game_data(title, width, height);

    if (data == NULL) {
        log_error("could not initialize game data");

        return 1;
    }

    run_game(data);

    drop_game_data(data);

    return 0;
}
