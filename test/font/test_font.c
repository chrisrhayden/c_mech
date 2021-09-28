#include <hashmap.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "test_font.h"

bool test_font() {
    size_t size = 16;

    char path[] = "assets/unscii-8.ttf";

    FontCache *font_cache = init_font_cache(path, size);

    if (font_cache->face_cache == NULL) {
        return false;
    }

    BitMapCache *bitmap_cache = font_cache->face_cache->bitmap_cache;

    if (bitmap_cache == NULL) {
        return false;
    }

    FT_Bitmap *bitmap;

    char key = 33;

    get_value_hashmap(bitmap_cache, &key, bitmap);

    if (bitmap == NULL) {
        return false;
    }

    // 16 pixels for each character by 10 characters wide
    int width = 16 * 10;
    // 16 pixels for each character by 15 characters down
    int height = 16 * 15;

    uint32_t *buffer = malloc(sizeof(uint32_t) * (width * height));

    SourceData data = {
        .x = 0,
        .y = 0,
        .width = width,
        .height = height,
        .buffer = buffer,
    };

    Color red = {.red = 178, .green = 34, .blue = 34};
    Color green = {.red = 10, .green = 255, .blue = 0};

    uint32_t color =
        (0xFF << 24) + (green.blue << 16) + (green.green << 8) + green.red;

    for (int i = 0; i < (width * height); ++i) {
        data.buffer[i] = color;
    }

    draw_characters(font_cache->face_cache->bitmap_cache, &data, &red, &green);

    drop_font_cache(font_cache);

    char *filename = "./out/ascii_test.png";

    int success = stbi_write_png(filename, data.width, data.height, 4,
                                 data.buffer, data.width * sizeof(uint32_t));

    free(buffer);

    if (success) {
        return true;
    } else {
        return false;
    }
}
