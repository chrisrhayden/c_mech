#include <assert.h>

#include "log.h"

#include "font_cache.h"

uint64_t integer_hash_int(const int *x) {
    return integer_hash32((const uint32_t)*x);
}

uint64_t string_hash(const StrKey *key) {
    return data_hash64(key->str, sizeof(char) * key->len);
}

bool string_comp(const StrKey *key_1, const StrKey *key_2) {
    return (strncmp(key_1->str, key_2->str, INT_MAX) == 0);
}

uint64_t unicode_hash(const uint32_t *key) {
    return integer_hash32(*key);
}

bool unicode_comp(const uint32_t *key_1, const uint32_t *key_2) {
    return (*key_1 == *key_2);
}

bool int_comp(const int *key_1, const int *key_2) {
    return (*key_1 == *key_2);
}

void bitmap_drop(int *key, BitMap *bitmap) {
    free(key);

    for (int i = 0; i < bitmap->len; ++i) {
        free(bitmap->bitmaps[i]);
    }

    free(bitmap->bitmaps);

    free(bitmap);
}

void font_drop(StrKey *key, FaceCache *face_cache) {
    free(key->str);
    free(key);

    FT_Done_Face(face_cache->face);

    drop_hashmap(face_cache->sized_bitmaps);

    free(face_cache);
}

bool init_face_cache(FontCache *cache, char *font_path, wchar_t *initial_chars,
                     int initial_point_size) {
    // malloc here so we dont bother with the rest if we dont have memory
    FaceCache *face_cache = malloc(sizeof(*face_cache));

    if (face_cache == false) {
        return false;
    }

    StrKey *key = malloc(sizeof(*key));

    if (key == NULL) {
        free(face_cache);
        return false;
    }

    int len = strnlen(key->str, INT_MAX);

    if (len == INT_MAX) {
        free(key);
        return false;
    }

    key->len = len;

    key->str = malloc(sizeof(wchar_t *));
    if (key->str == NULL) {
        free(face_cache);
        free(key);

        return false;
    }
    strncpy(key->str, font_path, INT_MAX);

    FT_Face face;

    if (FT_New_Face(cache->library, font_path, -1, &face) != 0) {
        free(face_cache);
        free(key);
        return false;
    }

    face_cache->face = face;

    SizedBitMap *sized_bitmaps;
    init_hashmap(sized_bitmaps, integer_hash_int, int_comp, bitmap_drop);

    if (sized_bitmaps == NULL) {
        free(face_cache);
        free(key);
        return false;
    }

    face_cache->sized_bitmaps = sized_bitmaps;

    make_bitmap_cache(face_cache, initial_chars, initial_point_size);

    enum HashMapResult result = FailedToInsert;

    insert_hashmap(cache->fonts_map, key, face_cache, result);

    if (result != Success) {
        free(face_cache);
        free(key);

        return false;
    }

    return true;
}

FontCache *allocate_cache() {
    FontCache *cache = malloc(sizeof(*cache));

    if (cache == NULL) {
        return NULL;
    }

    init_hashmap(cache->fonts_map, string_hash, string_comp, font_drop);

    if (cache->fonts_map == NULL) {
        free(cache);

        return NULL;
    }

    return cache;
}

FontCache *init_font_cache(char *font_path, wchar_t *initial_chars,
                           int initial_point_size) {
    if (font_path == NULL) {
        log_error("font path is null");
        return NULL;
    } else if (initial_chars == NULL) {
        log_error("initial_chars is null");
        return NULL;
    }

    FontCache *cache = allocate_cache();

    if (cache == NULL) {
        return NULL;
    }

    if (FT_Init_FreeType(&cache->library) != 0) {
        drop_font_cache(cache);

        return NULL;
    }

    if (init_face_cache(cache, font_path, initial_chars, initial_point_size) ==
        false) {
        drop_font_cache(cache);

        return NULL;
    }

    return cache;
}

void drop_font_cache(FontCache *cache) {
    if (cache->fonts_map) {
        drop_hashmap(cache->fonts_map);
    }

    if (cache->library) {
        FT_Done_FreeType(cache->library);
    }

    free(cache);
}

bool make_bitmaps(FT_Face face, BitMap *bitmap, wchar_t *chars_to_make,
                  int point_size) {

    return true;
}

bool make_bitmap_cache(FaceCache *face_cache, wchar_t *chars_to_make,
                       int point_size) {

    BitMap *bitmap = malloc(sizeof(*bitmap));

    return true;
}
