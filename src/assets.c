#include "assets.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

Glyphs *build_glyph_cache(char *font_path) {
    Glyphs *glyphs = malloc(sizeof(*glyphs));

    if (glyphs == NULL) {
        return NULL;
    }

    return glyphs;
}
