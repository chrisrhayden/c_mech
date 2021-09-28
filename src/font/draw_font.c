// #include <freetype2/ft2build.h>
// #include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include <SDL2/SDL_endian.h>

#include "draw_font.h"
#include "font_cache.h"
#include "hashmap.h"

bool draw_character(FT_Bitmap *bitmap, SourceData *source_data, Color *fg,
                    Color *bg) {
    int rows = bitmap->rows;
    int bm_width = bitmap->width;
    int s_width = source_data->width;

    size_t s_x = source_data->x;
    size_t s_y = source_data->y;

    for (size_t y = 0; y < rows; ++y) {
        for (size_t x = 0; x < bm_width; ++x) {
            size_t index_bitmap = x + (y * bm_width);
            size_t index_source = s_x + (s_y * s_width);

            if (bitmap->buffer[index_bitmap] > 0) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                uint32_t color = +(fg->red << 24) + (fg->green << 16) +
                                 (fg->blue << 8) + bitmap->buffer[index_bitmap];
#else
                uint32_t color = +(bitmap->buffer[index_bitmap] << 24) +
                                 (fg->blue << 16) + (fg->green << 8) + fg->red;
#endif
                source_data->buffer[index_source] = color;
            }
            ++s_x;
        }

        ++s_y;
        s_x = source_data->x;
    }

    return true;
}

bool draw_characters(BitMapCache *bitmap_cache, SourceData *data, Color *fg,
                     Color *bg) {
    bool success = false;

    FT_Bitmap *bitmap;
    int count = 1;

    for (char c = 33; c < 127; ++c) {
        get_value_hashmap(bitmap_cache, &c, bitmap);

        // set the character in the middle of the character box
        int offset_w = (16 - bitmap->width) / 2;
        int offset_h = (16 - bitmap->rows) / 2;

        data->x += offset_w;

        // keep the current y value as we need to reset the y to the same place
        // so it can draw from the top of the row each time
        int temp_y = data->y;
        data->y += offset_h;

        success = draw_character(bitmap, data, fg, fg);

        data->y = temp_y;

        // when the count reaches 10 move to the next row
        if (count == 10) {
            data->x = 0;
            data->y += 16;

            count = 1;
            // we are in the same row so move the x value and increment the
            // count
        } else {
            data->x += (16 - offset_w);

            ++count;
        }
    }

    return success;
}
