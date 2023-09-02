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

/** draw font in to a buffer
 *
 * this is pretty naive as it will break on more complicated font i think
 */
bool draw_character(FT_Bitmap *bitmap, FontSpriteCache *sprites, Cursor *cursor,
                    Color *fg) {
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
                     wchar_t *chars, size_t char_len) {
    bool success = true;

    Cursor cursor = {0, 0};

    Item *item = NULL;
    FT_Bitmap *bitmap = NULL;
    int count = 1;

    for (int i = 0; i < char_len && success; ++i) {
        item = lookup_key(cache->bitmap_cache, &chars[i]);

        if (item == NULL || item->value == NULL) {
            log_error("bitmap cache is empty or there is an error");
            success = false;
            break;
        }

        bitmap = (FT_Bitmap *)item->value;

        success = draw_character(bitmap, sprites, &cursor, fg);

        SDL_Rect *rect = malloc(sizeof(SDL_Rect));

        rect->x = cursor.x;
        rect->y = cursor.y;
        rect->w = cache->max_char_width;
        rect->h = cache->max_char_height;

        wchar_t *key = malloc(sizeof(wchar_t));
        *key = chars[i];
        success = insert_value(cache->sprite_rects, key, rect);

        // when the count reaches 10 move to the next row
        // this is display the whole cache in a window
        if (count == 10) {
            cursor.x = 0;
            cursor.y += cache->max_char_height;

            count = 1;

        } else {
            cursor.x += cache->max_char_width;

            ++count;
        }
    }

    return success;
}
