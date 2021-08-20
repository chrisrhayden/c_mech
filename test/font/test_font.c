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

    int width = 16 * 10;
    int height = 16 * 15;

    unsigned char *buffer =
        malloc(sizeof(unsigned char) * ((width * 4) * height));

    SourceData data = {
        .x = 0,
        .y = 0,
        .width = width,
        .height = height,
        .buffer = buffer,
    };

    Color red = {.red = 178, .green = 34, .blue = 34};
    Color green = {.red = 10, .green = 233, .blue = 0};

    for (int i = 0; i < ((width * 4) * height); i += 4) {
        buffer[i] = green.red;
        buffer[i + 1] = green.green;
        buffer[i + 2] = green.blue;
        buffer[i + 3] = (unsigned char)254;
    }

    int count = 1;
    for (int c = 33; c < 127; ++c) {
        char cc = c;
        get_value_hashmap(bitmap_cache, &cc, bitmap);

        int offset_w = (16 - bitmap->width) / 2;
        int offset_h = (16 - bitmap->rows) / 2;

        data.x += offset_w * 4;
        int temp_y = data.y;
        data.y += offset_h;

        // printf("off w %d off h %d\n", offset_w, offset_h);

        draw_character(bitmap, &red, &green, &data);

        data.y = temp_y;

        if (count == 10) {
            data.x = 0;
            data.y += 16;

            count = 1;
        } else {
            data.x += (16 - offset_w) * 4;

            ++count;
        }
    }

    drop_font_cache(font_cache);

    char *filename = "./out/ascii_test.png";

    int success = stbi_write_png(filename, data.width, data.height, 4, buffer,
                                 data.width * 4);

    free(buffer);

    if (success) {
        return true;
    } else {
        return false;
    }
}
