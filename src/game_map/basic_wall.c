// basic walls

#include "../common.h"
#include "../components/entity.h"
#include "../components/tiles.h"
#include "../log.h"
#include "game_map.h"

#define MIN_WIDTH 4
#define MIN_HEIGHT 4
#define MAX_WIDTH 8
#define MAX_HEIGHT 8

#define MAX_ROOMS 50

void carve_out_inner(GameMap *map, Rect rect) {
    // only carve out the inner room leaving the walls
    for (int y = rect.y1 + 1; y < rect.y2; ++y) {
        for (int x = rect.x1 + 1; x < rect.x2; ++x) {
            size_t idx = x + (y * map->width);

            map->data[idx].blocking = false;
            map->data[idx].visited = false;
            map->data[idx].sprite = 0x0020;
        }
    }
}

void carve_out_tunnel(GameMap *map, Rect cur, Rect last) {
    Point cur_center = rect_center(cur);
    Point last_center = rect_center(last);

    size_t start_x;
    size_t start_y;

    // start from either the past room or current room
    if (rand_range(1, 100) < 50) {
        start_x = cur_center.x;
        start_y = last_center.y;
    } else {
        start_x = last_center.x;
        start_y = cur_center.y;
    }

    size_t min_x = min(cur_center.x, last_center.x);
    size_t max_x = max(cur_center.x, last_center.x);

    for (size_t x = min_x; x < max_x; ++x) {
        size_t idx = x + (start_y * map->width);

        map->data[idx].blocking = false;
        map->data[idx].visited = false;
        map->data[idx].sprite = 0x0020;
    }

    size_t min_y = min(cur_center.y, last_center.y);
    size_t max_y = max(cur_center.y, last_center.y);

    for (size_t y = min_y; y < max_y; ++y) {
        size_t idx = start_x + (y * map->width);
        map->data[idx].blocking = false;
        map->data[idx].visited = false;
        map->data[idx].sprite = 0x0020;
    }
}

GameMap *init_basic_wall(EntityCache *ent_cache, size_t w, size_t h) {
    log_info("initng basic walls");

    GameMap *map = init_game_map(w, h);

    if (map == NULL) {
        return NULL;
    }

    for (int i = 0; i < map->len; ++i) {

        map->data[i].blocking = true;
        map->data[i].visited = false;
        map->data[i].sprite = 0x0023;
    }

    size_t prev_len = 0;
    Rect prev_rooms[MAX_ROOMS];

    for (int i = 0; i < MAX_ROOMS; ++i) {
        size_t room_width = rand_range(MIN_WIDTH, MAX_WIDTH);
        size_t room_height = rand_range(MIN_HEIGHT, MAX_HEIGHT);

        size_t x = rand_range(1, map->width - room_width - 1);
        size_t y = rand_range(1, map->height - room_height - 1);

        Rect cur = {
            .x1 = x, .x2 = x + room_width, .y1 = y, .y2 = y + room_height};

        bool intersects = false;
        for (int prev_idx = 0; prev_idx < prev_len && !intersects; ++prev_idx) {
            if (rect_intersects(prev_rooms[prev_idx], cur)) {
                intersects = true;

                break;
            }
        }

        if (intersects) {
            continue;
        }

        if (prev_len == 0) {
            ent_cache->player->pos.x = x + 1;
            ent_cache->player->pos.y = y + 1;
        } else {
            Rect last = prev_rooms[prev_len - 1];
            carve_out_tunnel(map, cur, last);
        }

        carve_out_inner(map, cur);

        prev_rooms[prev_len] = cur;
        prev_len += 1;
    }

    return map;
}
