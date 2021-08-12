#ifndef C_MECH_ASSETS
#define C_MECH_ASSETS

#include <stdint.h>
#include <wchar.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <SDL2/SDL.h>

#include "hashmap.h"

typedef struct {
    int len;
    void **bitmaps;
} BitMap;

HASHMAP(SizedBitMap, int, BitMap);

typedef struct {
    FT_FaceRec *face;
    SizedBitMap *sized_bitmaps;
} FaceCache;

typedef struct {
    int len;
    // Freetype expects a char string fro path no wchar_t
    char *str;
} StrKey;

HASHMAP(FontFaceMap, StrKey, FaceCache);

typedef struct {
    FT_Library library;
    FontFaceMap *fonts_map;
} FontCache;

FontCache *init_font_cache(char *font_path, wchar_t *initial_chars,
                           int initial_point_size);

void drop_font_cache(FontCache *cache);

bool make_bitmap_cache(FaceCache *face_cache, wchar_t *chars_to_make,
                       int point_size);

#endif
