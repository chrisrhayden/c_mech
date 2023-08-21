// robin_hood.c

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../log.h"

#include "./hash_algos.h"
#include "./robin_hood.h"

// swap two pointers
void swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

Map *_create_map(hash_func_t hash, uint64_t size) {
    Map *map = (Map *)malloc(sizeof(Map));

    if (map == NULL) {
        return NULL;
    }

    map->buckets = (Item **)calloc(size, sizeof(Item *));

    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }

    map->bucket_len = size;
    map->bucket_load = 0;

    map->hash_func = hash;

    return map;
}

Map *create_map(hash_func_t hash) {
    return _create_map(hash, STARTING_SIZE);
}

Item *create_item(void *key, void *value, uint64_t hash, uint64_t probe) {
    Item *item = (Item *)malloc(sizeof(Item));

    if (item == NULL) {
        return NULL;
    }

    item->key = key;
    item->value = value;
    item->probe = probe;
    item->hash = hash;

    return item;
}

bool _insert_value(Item **buckets, uint64_t bucket_len, Item *new_item) {
    uint64_t pos = new_item->hash & (bucket_len - 1);

    uint64_t current_pos = pos;

    Item *item = NULL;
    bool success = true;
    bool finding_bucket = true;

    while (finding_bucket) {
        item = buckets[current_pos];

        if (item == NULL) {
            buckets[current_pos] = new_item;

            finding_bucket = false;
        } else {
            // we found a "rich" item so we will swap it with the poor item
            if (new_item->probe > item->probe) {
                swap(&new_item->key, &item->key);
                swap(&new_item->value, &item->value);

                uint64_t temp_hash = item->hash;
                item->hash = new_item->hash;
                new_item->hash = temp_hash;

                uint64_t temp_probe = item->probe;
                item->probe = new_item->probe;
                new_item->probe = temp_probe;
            }

            current_pos = (current_pos + 1) & (bucket_len - 1);
            new_item->probe++;
        }
    }

    return success;
}

bool rehash_map(Map *map) {
    uint64_t new_len = map->bucket_len * GROWTH_FACTOR;

    Item **new_buckets = (Item **)calloc(new_len, sizeof(Item *));

    if (new_buckets == NULL) {
        return false;
    }

    bool success = true;

    Item *item = NULL;
    for (uint64_t i = 0; i < map->bucket_len && success; i++) {
        item = map->buckets[i];

        if (item) {
            success = _insert_value(new_buckets, new_len, item);
        }
    }

    if (success) {
        free(map->buckets);
        map->buckets = new_buckets;
        map->bucket_len = new_len;
    } else {
        free(new_buckets);
    }

    return success;
}

bool insert_value(Map *map, void *key, void *value) {
    if (key == NULL || value == NULL) {
        return false;
    }

    if (map->bucket_load + 1 >= floor(map->bucket_len * MAX_LOAD_FACTOR)) {
        if (rehash_map(map) == false) {
            return false;
        }
    }

    uint64_t hash = map->hash_func(key);
    Item *item = create_item(key, value, hash, 0);

    if (item == NULL) {
        return false;
    }

    if (_insert_value(map->buckets, map->bucket_len, item)) {
        map->bucket_load++;

        return true;
    }

    return false;
}

void backward_shift(Map *map, uint64_t pos) {
    uint64_t current_pos = pos;

    Item *item = NULL;

    bool move_backward = true;

    while (move_backward) {
        current_pos = (current_pos + 1) & (map->bucket_len - 1);
        item = map->buckets[current_pos];

        if (item == NULL || item->probe == 0) {
            move_backward = false;
        } else {
            item->probe--;
            map->buckets[current_pos - 1] = item;
            map->buckets[current_pos] = NULL;
        }
    }
}

Item *delete_item(Map *map, void *key) {
    uint64_t hash = map->hash_func(key);
    uint64_t pos = hash & (map->bucket_len - 1);
    uint64_t probe = 0;

    Item *item = NULL;
    uint64_t current_pos = 0;

    bool finding_bucket = true;

    while (finding_bucket) {
        current_pos = (pos + probe) & (map->bucket_len - 1);

        item = map->buckets[current_pos];

        if (item == NULL || probe > item->probe) {
            item = NULL;
            finding_bucket = false;
        } else if (item->hash == hash) {
            finding_bucket = false;
            map->buckets[current_pos] = NULL;

            backward_shift(map, current_pos);
        } else {
            probe++;
        }
    }

    return item;
}

Item *lookup_key(Map *map, void *key) {
    uint64_t hash = map->hash_func(key);
    uint64_t pos = hash & (map->bucket_len - 1);

    Item *item = NULL;

    uint64_t probe = 0;
    uint64_t current_pos = pos;

    bool finding_bucket = true;

    while (finding_bucket) {
        item = map->buckets[current_pos];

        // if the item probe is larger then we would have found the key
        // we are looking for by now
        if (item == NULL || probe > item->probe) {
            item = NULL;
            finding_bucket = false;
            // if this is true then we found the correct bucket
        } else if (item->hash == hash) {
            finding_bucket = false;
        }

        probe++;
        current_pos = (current_pos + 1) & (map->bucket_len - 1);
    }

    return item;
}

IterMap *create_iter_map(Map *map) {
    IterMap *iter_map = (IterMap *)malloc(sizeof(IterMap));

    if (iter_map == NULL) {
        return NULL;
    }

    iter_map->current_pos = 0;
    iter_map->buckets = map->buckets;
    iter_map->bucket_len = map->bucket_len;

    return iter_map;
}

Item *next_item(IterMap *iter_map) {
    Item *item = NULL;

    bool looking = true;

    while (looking && iter_map->current_pos <= iter_map->bucket_len) {
        item = iter_map->buckets[iter_map->current_pos];

        if (item != NULL) {
            looking = false;
        }

        iter_map->current_pos++;
    }

    return item;
}

void drop_map(Map *map) {
    for (int i = 0; i < map->bucket_len; ++i) {
        if (map->buckets[i]) {
            free(map->buckets[i]->key);
            free(map->buckets[i]->value);
            free(map->buckets[i]);
        }
    }

    free(map);
}
