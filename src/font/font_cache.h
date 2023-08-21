#ifndef C_ROGUELIKE_FONT_CACHE
#define C_ROGUELIKE_FONT_CACHE

#include <SDL2/SDL_surface.h>
#include <stdint.h>
#include <wchar.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include "../collections/robin_hood_hashing/robin_hood.h"

typedef struct {
    FT_Library library;
    FT_Face face;
    Map *bitmap_cache;
    size_t max_char_width;
    size_t max_char_height;
} FontCache;

extern int CHARACTER_LEN;
extern wchar_t CHARACTER_ARRAY[256];

FontCache *init_font_cache(char font_path[], int point_size);
void drop_font_cache(FontCache *cache);

typedef struct {
    size_t width;
    size_t height;
    uint32_t *data;
} FontSpriteCache;

FontSpriteCache *init_srpite_cache(FontCache *font_cache);

SDL_Surface *make_font_surface(FontSpriteCache *sprites);

#endif
