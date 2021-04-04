#ifndef C_MECH_ASSETS
#define C_MECH_ASSETS

#include <stdint.h>

#include <SDL2/SDL.h>

#include "hashmap.h"

HASHMAP(GlyphMap, uint32_t, SDL_Rect);

typedef struct {
    SDL_Texture *f_cache;
    GlyphMap *glyphs;
} Glyphs;

Glyphs *build_glyph_cache(char *font_path);

SDL_Rect *get_glyph(uint32_t glyph_code);

#endif
