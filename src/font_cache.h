#ifndef C_MECH_ASSETS
#define C_MECH_ASSETS

#include <stdint.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

#include <SDL2/SDL.h>

#include "hashmap.h"

HASHMAP(BitMapCache, uint32_t, void);

typedef struct {
    FT_FaceRec *face;
    BitMapCache *bit_maps;
} FaceCache;

typedef struct {
    int len;
    char *str;
} StrKey;

HASHMAP(FontFaceMap, StrKey, FaceCache);

typedef struct {
    FT_Library library;
    FontFaceMap *fonts_map;
} FontCache;

HASHMAP(GlyphMap, uint32_t, SDL_Rect);

typedef struct {
    SDL_Texture *glyph_texture;
    GlyphMap *glyphs;
} Glyphs;

FontCache *init_font_cache(char *font_path, char *initial_chars);

void drop_font_cache(FontCache *cache);

Glyphs *build_glyph_cache(FT_Face face);

#endif
