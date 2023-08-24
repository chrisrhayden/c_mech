#ifndef C_ROGUELIKE_DRAW_FONT
#define C_ROGUELIKE_DRAW_FONT

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include <stdbool.h>
#include <stdint.h>

#include "font_cache.h"

typedef struct {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
} Color;

// bool draw_character(FT_Bitmap *bitmap, FontSpriteCache *sprite, Color *fg,
// Color *bg);

bool draw_characters(FontCache *font_cache, FontSpriteCache *sprite, Color *fg,
                     wchar_t *characters, size_t char_len);

#endif
