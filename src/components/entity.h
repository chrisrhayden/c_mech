#ifndef C_ROGUELIKE_ENTITY
#define C_ROGUELIKE_ENTITY

#include "../common.h"

#define ENT_ARRAY_START 200

typedef struct {
    Point pos;
    wchar_t sprite;
} Entity;

typedef struct {
    size_t entitys_len;
    Entity **entitys;
    Entity *player;
} EntityCache;

EntityCache *init_entity_cache();

void drop_entity_cache(EntityCache *ent_cache);

#endif
