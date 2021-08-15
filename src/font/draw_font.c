#include "draw_font.h"

bool draw_character(FT_Bitmap *bitmap, Color *fg, Color *bg,
                    SourceData *source_data) {
    int rows = bitmap->rows;
    int bm_width = bitmap->width;
    int s_width = source_data->width * 4;

    size_t s_x = source_data->x;
    size_t s_y = source_data->y;

    for (size_t y = 0; y < rows; ++y) {
        for (size_t x = 0; x < bm_width; ++x) {
            size_t index_bitmap = x + (y * bm_width);

            size_t index_source = s_x + (s_y * s_width);

            if (bitmap->buffer[index_bitmap] > 0) {
                source_data->buffer[index_source] = fg->red;
                source_data->buffer[index_source + 1] = fg->green;
                source_data->buffer[index_source + 2] = fg->blue;
                source_data->buffer[index_source + 3] = (char)255;
            }

            s_x += 4;
        }

        ++s_y;
        s_x = source_data->x;
    }

    return true;
}
