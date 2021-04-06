# c_mech

## resources

- freetype & font cache <br>
  holy shit does freetype have terrible documentation, these were needed to
  even understand how the cache should be instantiated let alone be used

  so i think the idea is that the FT_Library object hold the allocator and
  modules for dealing with the fonts while the FT_Face hold the actual data,
  meaning that if i want to keep a font in memory I need to also keep the face
  - https://www.sivachandran.in/2009/04/freetype-2-caching.html
  - https://lists.gnu.org/archive/html/freetype/2005-06/msg00002.html
