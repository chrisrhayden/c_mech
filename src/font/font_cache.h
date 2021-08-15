#ifndef C_MECH_ASSETS
#define C_MECH_ASSETS

#include <stdint.h>
#include <wchar.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <SDL2/SDL.h>

#include "hashmap.h"

HASHMAP(BitMapCache, char, void);

typedef struct {
    FT_FaceRec *face;
    BitMapCache *bitmap_cache;
} FaceCache;

typedef struct {
    FT_Library library;
    FaceCache *face_cache;
} FontCache;

FontCache *init_font_cache(char font_path[], int initial_point_size);

void drop_font_cache(FontCache *cache);
void drop_face_cache(FaceCache *face);

#endif
