#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "log.h"

#include "font_cache.h"

uint64_t char_hash(char *x) {
    return integer_hash64(*x);
}

bool char_comp(char *key_1, char *key_2) {
    return (*key_1 == *key_2);
}

void bitmap_drop(char *key, FT_Bitmap *bitmap) {
    free(key);
    free(bitmap->buffer);
    free(bitmap->palette);
    free(bitmap);
}

void drop_face_cache(FT_Library library, FaceCache *face_cache) {
    if (face_cache->bitmap_cache) {
        drop_hashmap(face_cache->bitmap_cache);
    }

    FT_Done_Face(face_cache->face);

    free(face_cache);
}

void drop_font_cache(FontCache *cache) {
    if (cache->face_cache) {
        drop_face_cache(cache->library, cache->face_cache);
    }

    if (cache->library) {
        FT_Done_FreeType(cache->library);
    }

    free(cache);
}

bool make_bitmap(FontCache *font_cache, char c) {
    FT_UInt glyph_index = FT_Get_Char_Index(font_cache->face_cache->face, c);

    if (FT_Load_Glyph(font_cache->face_cache->face, glyph_index,
                      FT_LOAD_DEFAULT) != 0) {
        log_error("could not load glyph for %c", c);
        return false;
    }

    if (FT_Render_Glyph(font_cache->face_cache->face->glyph,
                        FT_RENDER_MODE_NORMAL) != 0) {
        log_error("could not render glyph for %c", c);
        return false;
    }

    // {
    //     unsigned int rows;
    //     unsigned int width;
    //     int pitch;
    //     unsigned char *buffer;
    //     unsigned short num_grays;
    //     unsigned char pixel_mode;
    //     unsigned char palette_mode;
    //     void *palette;
    // }
    FT_Bitmap *bitmap = malloc(sizeof(*bitmap));
    bitmap->buffer = NULL;
    bitmap->palette = NULL;
    bitmap->palette_mode = 0;
    bitmap->rows = 0;
    bitmap->width = 0;
    bitmap->num_grays = 0;
    bitmap->pixel_mode = 0;
    bitmap->pitch = 0;

    if (FT_Bitmap_Copy(font_cache->library,
                       &font_cache->face_cache->face->glyph->bitmap,
                       bitmap) != 0) {
        log_error("could not copy bitmap");
        free(bitmap);

        return false;
    }

    char *key = malloc(sizeof(char));

    if (key == NULL) {
        log_error("could not allocate key");
        free(bitmap);
        return false;
    }

    *key = c;

    enum HashMapResult result = FailedToInsert;

    insert_hashmap(font_cache->face_cache->bitmap_cache, key, bitmap, result);

    if (result != Success) {
        log_error("could not insert in to hashmap");
        free(bitmap);

        free(key);

        return false;
    }

    if (bitmap->width > font_cache->face_cache->max_width) {
        font_cache->face_cache->max_width = bitmap->width;
    }

    if (bitmap->rows > font_cache->face_cache->max_height) {
        font_cache->face_cache->max_height = bitmap->rows;
    }

    return true;
}

bool make_bitmaps(FontCache *font_cache, int point_size) {
    if (FT_Set_Char_Size(font_cache->face_cache->face, 0, point_size * 64, 0,
                         72) != 0) {
        return false;
    }

    bool success = true;

    for (char c = 0x21; c < 0x7F && success; ++c) {
        success = make_bitmap(font_cache, c);
    }

    return success;
}

bool init_face_cache(FontCache *cache, char face_path[],
                     int initial_point_size) {
    FT_Face face;
    if (FT_New_Face(cache->library, (char *)face_path, 0, &face) != 0) {
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
                 bitmap_drop);

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
