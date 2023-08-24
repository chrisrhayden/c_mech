#include <stdlib.h>

#include "../log.h"
#include "entity.h"

EntityCache *init_entity_cache() {
    log_info("initing entiy cache");
    EntityCache *ent_cache = malloc(sizeof(EntityCache));
    Entity *player = malloc(sizeof(Entity));
    Entity **entitys = malloc(sizeof(Entity) * ENT_ARRAY_START);

    if (ent_cache == NULL || player == NULL || entitys == NULL) {
        log_error("coul not get entity cache");

        if (ent_cache) {
            free(ent_cache);
        }
        if (player) {
            free(player);
        }
        if (entitys) {
            free(entitys);
        }

        return NULL;
    }

    player->pos.x = 0;
    player->pos.y = 0;
    player->sprite = 0x263A;

    ent_cache->player = player;
    ent_cache->entitys = entitys;
    ent_cache->entitys_len = ENT_ARRAY_START;

    return ent_cache;
}

void drop_entity_cache(EntityCache *ent_cache) {
    if (ent_cache->player) {
        free(ent_cache->player);
    }

    if (ent_cache->entitys) {
        for (int i = 0; i < ent_cache->entitys_len; ++i) {
            if (ent_cache->entitys[i]) {
                free(ent_cache->entitys[i]);
            }
        }
    }

    free(ent_cache);
}
