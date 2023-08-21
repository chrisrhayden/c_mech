#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "../log.h"

#include "draw_font.h"
#include "font_cache.h"

#include "../collections/robin_hood_hashing/hash_algos.h"
#include "../collections/robin_hood_hashing/robin_hood.h"

int CHARACTER_LEN = 256;
wchar_t CHARACTER_ARRAY[256] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
    0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0011,
    0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001a,
    0x001b, 0x001c, 0x001d, 0x001e, 0x001f, 0x0020, 0x0021, 0x0022, 0x0023,
    0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x002b, 0x002c,
    0x002d, 0x002e, 0x002f, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035,
    0x0036, 0x0037, 0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e,
    0x003f, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
    0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f, 0x0050,
    0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059,
    0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f, 0x0060, 0x0061, 0x0062,
    0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006a, 0x006b,
    0x006c, 0x006d, 0x006e, 0x006f, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074,
    0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d,
    0x007e, 0x007f, 0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5,
    0x00e7, 0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
    0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9, 0x00ff,
    0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192, 0x00e1, 0x00ed,
    0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba, 0x00bf, 0x2310, 0x00ac,
    0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb, 0x2591, 0x2592, 0x2593, 0x2502,
    0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255d,
    0x255c, 0x255b, 0x2510, 0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c,
    0x255e, 0x255f, 0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c,
    0x2567, 0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
    0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580, 0x03b1,
    0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4, 0x03a6, 0x0398,
    0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229, 0x2261, 0x00b1, 0x2265,
    0x2264, 0x2320, 0x2321, 0x00f7, 0x2248, 0x00b0, 0x2219, 0x00b7, 0x221a,
    0x207f, 0x00b2, 0x25a0, 0x00a0};

// uint64_t char_hash(wchar_t *x) {
//     return integer_hash64(*x);
// }
//
// bool char_comp(wchar_t *key_1, wchar_t *key_2) {
//     return (*key_1 == *key_2);
// }
//
// void bitmap_drop(wchar_t *key, FT_Bitmap *bitmap) {
//     free(key);
//     free(bitmap->buffer);
//     free(bitmap->palette);
//     free(bitmap);
// }
//
// void drop_face_cache(FT_Library library, FaceCache *face_cache) {
//     if (face_cache->bitmap_cache) {
//         drop_hashmap(face_cache->bitmap_cache);
//     }
//
//     FT_Done_Face(face_cache->face);
//
//     free(face_cache);
// }
//
// void drop_font_cache(FontCache *cache) {
//     if (cache->face) {
//         FT_Done_Face(cache->face);
//     }
//
//     if (cache->library) {
//         FT_Done_FreeType(cache->library);
//     }
//
//     free(cache);
// }
//
// bool make_bitmap(FontCache *font_cache, wchar_t c) {
//     printf("making char bitmap '%C'\n", c);
//     FT_UInt glyph_index = FT_Get_Char_Index(font_cache->face_cache->face, c);
//
//     if (FT_Load_Glyph(font_cache->face_cache->face, glyph_index,
//                       FT_LOAD_DEFAULT) != 0) {
//         log_error("could not load glyph for %c", c);
//         return false;
//     }
//
//     if (FT_Render_Glyph(font_cache->face_cache->face->glyph,
//                         FT_RENDER_MODE_NORMAL) != 0) {
//         log_error("could not render glyph for %c", c);
//         return false;
//     }
//
//     // {
//     //     unsigned int rows;
//     //     unsigned int width;
//     //     int pitch;
//     //     unsigned char *buffer;
//     //     unsigned short num_grays;
//     //     unsigned char pixel_mode;
//     //     unsigned char palette_mode;
//     //     void *palette;
//     // }
//     FT_Bitmap *bitmap = malloc(sizeof(*bitmap));
//     bitmap->buffer = NULL;
//     bitmap->palette = NULL;
//     bitmap->palette_mode = 0;
//     bitmap->rows = 0;
//     bitmap->width = 0;
//     bitmap->num_grays = 0;
//     bitmap->pixel_mode = 0;
//     bitmap->pitch = 0;
//
//     if (FT_Bitmap_Copy(font_cache->library,
//                        &font_cache->face_cache->face->glyph->bitmap,
//                        bitmap) != 0) {
//         log_error("could not copy bitmap\n");
//         free(bitmap);
//
//         return false;
//     }
//
//     wchar_t *key = malloc(sizeof(wchar_t));
//
//     if (key == NULL) {
//         log_error("could not allocate key\n");
//         free(bitmap);
//         return false;
//     }
//
//     *key = c;
//
//     enum HashMapResult result = FailedToInsert;
//
//     insert_hashmap(font_cache->face_cache->bitmap_cache, key, bitmap,
//     result);
//
//     if (result != Success) {
//         log_error("could not insert in to hashmap");
//         free(bitmap);
//
//         free(key);
//
//         return false;
//     }
//
//     if (bitmap->width > font_cache->face_cache->max_width) {
//         font_cache->face_cache->max_width = bitmap->width;
//     }
//
//     if (bitmap->rows > font_cache->face_cache->max_height) {
//         font_cache->face_cache->max_height = bitmap->rows;
//     }
//
//     return true;
// }
//
// bool init_face_cache(FontCache *cache, char face_path[], int point_size)
// {
//     FT_Face face;
//     if (FT_New_Face(cache->library, face_path, 0, &face) != 0) {
//         return false;
//     }
//
//     cache->face_cache->face = face;
//
//     return make_bitmaps(cache, point_size);
// }

bool make_bitmap(FontCache *cache, wchar_t c) {

    FT_UInt glyph_index = FT_Get_Char_Index(cache->face, c);

    if (FT_Load_Glyph(cache->face, glyph_index, FT_LOAD_DEFAULT) != 0) {
        log_error("could not load glyph for %C", c);
        return false;
    }

    if (FT_Render_Glyph(cache->face->glyph, FT_RENDER_MODE_NORMAL) != 0) {
        log_error("could not render glyph for %C", c);
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

    if (FT_Bitmap_Copy(cache->library, &cache->face->glyph->bitmap, bitmap) !=
        FT_Err_Ok) {
        log_error("could not copy bitmap\n");
        free(bitmap);

        return false;
    }

    wchar_t *key = malloc(sizeof(wchar_t));

    if (key == NULL) {
        log_error("could not allocate key\n");
        free(bitmap);
        return false;
    }

    *key = c;

    if (insert_value(cache->bitmap_cache, key, bitmap) == false) {
        log_error("could not insert in to hashmap");
        free(bitmap);
        free(key);
        return false;
    }

    if (bitmap->width > cache->max_char_width) {
        cache->max_char_width = bitmap->width;
    }

    if (bitmap->rows > cache->max_char_height) {
        cache->max_char_height = bitmap->rows;
    }

    return true;
}

bool make_bitmaps(FontCache *font_cache, int point_size) {
    if (FT_Set_Char_Size(font_cache->face, 0, point_size * 64, 0, 72) != 0) {
        log_error("cant set font size");
        return false;
    }

    bool success = true;

    for (int i = 0; i < CHARACTER_LEN && success; ++i) {
        log_debug("%d -- making char bitmap '%C'\n", i, CHARACTER_ARRAY[i]);
        success = make_bitmap(font_cache, CHARACTER_ARRAY[i]);
    }

    return success;
}

uint64_t hash(void *key) {
    return integer_hash64(*(wchar_t *)key);
}

FontCache *allocate_cache() {
    FontCache *cache = malloc(sizeof(*cache));

    if (cache == NULL) {
        return NULL;
    }

    cache->library = NULL;
    cache->face = NULL;

    cache->bitmap_cache = create_map(hash);

    return cache;
}

FontCache *_init_font_cache(char font_path[], int point_size) {
    FontCache *cache = allocate_cache();

    if (cache == NULL) {
        drop_font_cache(cache);
        return NULL;
    }

    if (FT_Init_FreeType(&cache->library) != 0) {
        drop_font_cache(cache);
        return NULL;
    }

    if (FT_New_Face(cache->library, font_path, 0, &cache->face) != 0) {
        drop_font_cache(cache);
        return NULL;
    }

    return cache;
}

FontCache *init_font_cache(char font_path[], int point_size) {
    if (font_path == NULL) {
        log_error("font path is null");
        return NULL;
    }

    FontCache *cache = _init_font_cache(font_path, point_size);

    if (cache == NULL) {
        log_error("could not initalize font cache");
    }

    if (make_bitmaps(cache, 16) == false) {
        log_error("could not make bitmaps");

        drop_font_cache(cache);

        return NULL;
    }

    return cache;
}

void drop_font_cache(FontCache *cache) {
    if (cache->face) {
        FT_Done_Face(cache->face);
    }

    if (cache->library) {
        FT_Done_FreeType(cache->library);
    }

    if (cache->bitmap_cache) {
        drop_map(cache->bitmap_cache);
    }

    free(cache);
}

FontSpriteCache *init_srpite_cache(FontCache *cache) {
    FontSpriteCache *sprites = malloc(sizeof(FontSpriteCache));

    if (sprites == NULL) {
        return NULL;
    }

    size_t char_width = cache->max_char_width;
    size_t char_height = cache->max_char_height;

    sprites->width = 10 * char_width;
    sprites->height = (CHARACTER_LEN / 2) * char_height;

    size_t total = sprites->width * sprites->height;

    sprites->data = malloc(sizeof(uint32_t) * total);

    if (sprites->data == NULL) {
        log_error("could not make sprite cache");
        free(sprites);
        return NULL;
    }

    for (int i = 0; i < total; ++i) {
        sprites->data[i] = 0xffffffff;
    }

    Color fg = {0, 0, 0};
    Color bg = {255, 255, 255};

    if (draw_characters(cache, sprites, &fg, &bg, CHARACTER_ARRAY,
                        CHARACTER_LEN) == false) {
        log_error("could not make sprites");
        free(sprites->data);
        free(sprites);
        return NULL;
    }

    return sprites;
}

typedef struct {
    size_t r;
    size_t b;
    size_t g;
    size_t a;
} ColorMask;

ColorMask *make_color_mask() {
    ColorMask *cm = malloc(sizeof(ColorMask));

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    cm->r = 0xff000000;
    cm->g = 0x00ff0000;
    cm->b = 0x0000ff00;
    cm->a = 0x000000ff;
#else
    cm->r = 0x000000ff;
    cm->g = 0x0000ff00;
    cm->b = 0x00ff0000;
    cm->a = 0xff000000;
#endif

    return cm;
}

SDL_Surface *make_font_surface(FontSpriteCache *sprites) {
    ColorMask *cm = make_color_mask();
    SDL_Surface *sp;

    sp = SDL_CreateRGBSurfaceFrom(sprites->data, sprites->width,
                                  sprites->height, 32, sprites->width * 4,
                                  cm->r, cm->g, cm->b, cm->a);

    free(cm);
    return sp;
}
