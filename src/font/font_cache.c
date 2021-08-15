#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include "log.h"

#include "font_cache.h"

uint64_t char_hash(const char *x) {
    return integer_hash64(*x);
}

bool char_comp(const char *key_1, const char *key_2) {
    return (*key_1 == *key_2);
}

void drop_bitmap(char *key, void *value) {
    free(key);

    free(value);

    return;
}

void drop_face_cache(FaceCache *face_cache) {
    if (face_cache->bitmap_cache) {
        drop_hashmap(face_cache->bitmap_cache);
    }

    FT_Done_Face(face_cache->face);

    free(face_cache);
}

void drop_font_cache(FontCache *cache) {
    if (cache->face_cache) {
        drop_face_cache(cache->face_cache);
    }

    if (cache->library) {
        FT_Done_FreeType(cache->library);
    }

    free(cache);
}

bool make_bitmaps(FontCache *font_cache, int initial_point_size) {
    FT_FaceRec *face = font_cache->face_cache->face;

    if (FT_Set_Char_Size(face, 0, 16 * 64, 0, 72) != 0) {
        return false;
    }

    int index = 0;
    bool glyph_error = false;

    for (char c = 0x21; c < 0x7F && !glyph_error; ++c) {
        FT_UInt glyph_index = FT_Get_Char_Index(face, c);

        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) != 0) {
            log_error("could not load glyph for %c", c);
            glyph_error = true;
            break;
        }

        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) != 0) {
            log_error("could not render glyph for %c", c);
            glyph_error = true;
            break;
        }

        FT_Bitmap *bitmap = malloc(sizeof(*bitmap));

        if (bitmap == NULL) {
            log_error("could not allocate new FT_Bitmap for %c", c);
            glyph_error = true;
            break;
        }

        FT_Bitmap_Init(bitmap);

        if (FT_Bitmap_Copy(font_cache->library, &face->glyph->bitmap, bitmap) !=
            0) {
            log_error("could not copy to new bitmap for %c", c);
            glyph_error = true;
            break;
        }

        char *key = malloc(sizeof(char));

        if (key == NULL) {
            log_error("could not allocate new key for %c", c);
            glyph_error = true;
            break;
        }

        *key = c;

        enum HashMapResult result = FailedToInsert;

        insert_hashmap(font_cache->face_cache->bitmap_cache, key, bitmap,
                       result);

        if (result != Success) {
            log_error("could not insert in to bitmap_cache for %c", c);
            glyph_error = true;
            break;
        }

        ++index;
    }

    return !glyph_error;
}

bool init_face_cache(FontCache *cache, char face_path[],
                     int initial_point_size) {
    FT_Face face;
    if (FT_New_Face(cache->library, face_path, 0, &face) != 0) {
        return false;
    }

    cache->face_cache->face = face;

    return make_bitmaps(cache, initial_point_size);
}

FontCache *allocate_cache() {
    FontCache *cache = malloc(sizeof(*cache));

    if (cache == NULL) {
        return NULL;
    }

    cache->library = NULL;
    cache->face_cache = malloc(sizeof(*cache->face_cache));

    if (cache->face_cache == NULL) {
        return NULL;
    }

    cache->face_cache->bitmap_cache = NULL;
    cache->face_cache->face = NULL;

    init_hashmap(cache->face_cache->bitmap_cache, char_hash, char_comp,
                 drop_bitmap);

    if (cache->face_cache->bitmap_cache == NULL) {
        free(cache);

        return NULL;
    }

    return cache;
}

FontCache *init_font_cache(char font_path[], int initial_point_size) {
    if (font_path == NULL) {
        log_error("font path is null");
        return NULL;
    }

    FontCache *cache = allocate_cache();

    if (cache == NULL) {
        drop_font_cache(cache);
        return NULL;
    }

    if (FT_Init_FreeType(&cache->library) != 0) {
        drop_font_cache(cache);

        return NULL;
    }

    if (init_face_cache(cache, font_path, initial_point_size) == false) {
        drop_font_cache(cache);

        return NULL;
    }

    return cache;
}
