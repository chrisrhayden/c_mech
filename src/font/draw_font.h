#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char red;
    char green;
    char blue;
} Color;

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    char *buffer;
} SourceData;

bool draw_character(FT_Bitmap *bitmap, Color *fg, Color *bg,
                    SourceData *source_data);
