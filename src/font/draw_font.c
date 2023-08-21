#include <freetype/ftbitmap.h>

// #include <SDL2/SDL_endian.h>
#include <SDL2/SDL.h>

#include "../log.h"

#include "../collections/robin_hood_hashing/robin_hood.h"
#include "draw_font.h"
#include "font_cache.h"

typedef struct {
    size_t x;
    size_t y;
} Cursor;

uint32_t pack_color(Color *color, uint32_t alpha) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return (color->red << 24) + (color->green << 16) + (color->blue << 8) +
           alpha;
#else
    return (alpha << 24) + (color->blue << 16) + (color->green << 8) +
           color->red;
#endif
}

bool draw_character(FT_Bitmap *bitmap, FontSpriteCache *sprites, Cursor *cursor,
                    Color *fg, Color *bg) {
    int bm_rows = bitmap->rows;
    int bm_width = bitmap->width;
    int s_width = sprites->width;

    size_t s_x = cursor->x;
    size_t s_y = cursor->y;

    for (size_t y = 0; y < bm_rows; ++y) {
        for (size_t x = 0; x < bm_width; ++x) {
            size_t index_bitmap = x + (y * bm_width);
            size_t index_source = s_x + (s_y * s_width);

            if (bitmap->buffer[index_bitmap] > 0) {
                sprites->data[index_source] =
                    pack_color(fg, bitmap->buffer[index_bitmap]);
            }

            ++s_x;
        }

        ++s_y;
        s_x = cursor->x;
    }

    return true;
}

bool draw_characters(FontCache *cache, FontSpriteCache *sprites, Color *fg,
                     Color *bg, wchar_t *chars, size_t char_len) {

    log_debug("making character sprites");
    bool success = true;

    Cursor cursor = {0, 0};

    Item *item = NULL;
    FT_Bitmap *bitmap = NULL;
    int count = 1;

    wchar_t *key = malloc(sizeof(wchar_t));

    for (int i = 50; i < char_len && success; ++i) {
        *key = chars[i];
        item = lookup_key(cache->bitmap_cache, key);

        if (item == NULL || item->value == NULL) {
            log_error("bitmap cache is empty or there is an error");
            success = false;
            break;
        }

        bitmap = (FT_Bitmap *)item->value;

        // set the character in the middle of the character box
        /* int offset_w = (face_cache->max_width - bitmap->width) / 2; */
        /* int offset_h = (face_cache->max_height - bitmap->rows) / 2; */
        int offset_w = 0;
        int offset_h = 0;

        cursor.x += offset_w;

        // keep the current y value as we need to reset the y to the same place
        // so it can draw from the top of the row each time
        int temp_y = cursor.y;
        cursor.y += offset_h;

        success = draw_character(bitmap, sprites, &cursor, fg, bg);

        cursor.y = temp_y;

        // when the count reaches 10 move to the next row
        if (count == 10) {
            cursor.x = 0;
            cursor.y += cache->max_char_width;

            count = 1;
            // we are in the same row so move the x value and increment the
            // count
        } else {
            cursor.x += (cache->max_char_width - offset_w);

            ++count;
        }
    }

    free(key);

    return success;
}
