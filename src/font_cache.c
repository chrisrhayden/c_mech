#include <assert.h>

#include "log.h"

#include "font_cache.h"

/* lets see if this all works the way i think is does */
SDL_Surface *new_rgb_surface(int width, int height) {
    /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
       as expected by OpenGL for textures */
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface =
        SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

    if (surface == NULL) {
        return NULL;
    }
    return surface;
}

SDL_Texture *make_glyph_cache() {
    SDL_Surface *surface = new_rgb_surface(0, 0);

    return NULL;
}

uint64_t string_hash(const StrKey *key) {
    return data_hash64(key->str, sizeof(char) * key->len);
}

bool string_comp(const StrKey *key_1, const StrKey *key_2) {
    return (strcmp(key_1->str, key_2->str) == 0);
}

uint64_t unicode_hash(const uint32_t *key) {
    return integer_hash32(*key);
}

bool unicode_comp(const uint32_t *key_1, const uint32_t *key_2) {
    return (*key_1 == *key_2);
}

void bitmap_drop(uint32_t *key, void *bitmap) {
    free(key);

    free(bitmap);
}

void font_drop(StrKey *key, FaceCache *face_cache) {
    free(key->str);
    free(key);

    drop_hashmap(face_cache->bit_maps);

    FT_Done_Face(face_cache->face);

    free(face_cache);
}

bool init_face(FontCache *cache, char *font_path) {
    StrKey *key = malloc(sizeof(*key));

    int len = strnlen(key->str, INT_MAX);

    if (len == INT_MAX) {
        return false;
    }

    key->len = len;

    key->str = malloc(sizeof(char *));
    strncpy(key->str, font_path, INT_MAX);

    FT_Face face;

    if (FT_New_Face(cache->library, font_path, -1, &face) != 0) {
        return false;
    }

    FaceCache *face_cache = malloc(sizeof(*face_cache));

    if (face_cache == false) {
        return false;
    }

    init_hashmap(face_cache->bit_maps, unicode_hash, unicode_comp, bitmap_drop);

    if (face_cache->bit_maps == false) {
        return false;
    }

    face_cache->face = face;

    enum HashMapResult result = FailedToInsert;

    insert_hashmap(cache->fonts_map, key, face_cache, result);

    if (result != Success) {
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

FontCache *init_font_cache(char *font_path, char *initial_chars) {
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

    if (init_face(cache, font_path) == false) {
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
