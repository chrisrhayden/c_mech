#include <stdbool.h>
#include <stdio.h>

#include "log.h"

#include "context.h"
#include "game_data.h"

bool init_logging(int log_level) {
    log_set_level(log_level);

    return true;
}

int main() {
    if (init_logging(LOG_TRACE) == false) {
        fprintf(stderr, "could not init logging, quiting now");

        return 1;
    }

    log_trace("starting c_mech");

    char title[] = "c_mech game";

    int width = 400;
    int height = 600;

    GameData *data = init_game_data(title, width, height);

    return 0;
}
