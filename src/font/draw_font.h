#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include <stdbool.h>
#include <stdint.h>

#include "font_cache.h"

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    uint32_t *buffer;
} SourceData;

bool draw_character(FT_Bitmap *bitmap, SourceData *source_data, Color *fg,
                    Color *bg);

bool draw_characters(BitMapCache *bitmap_cache, SourceData *data, Color *fg,
                     Color *bg);
