#include <SDL_bmp.h>
#include <BMP.h>
#include <assert.h>

SDL_Surface* SDL_LoadBMP(const char *filename) {
  int w, h;
	printf("111\n");
  void *pixels = BMP_Load(filename, &w, &h);
  assert(pixels);
	printf("222\n");
  SDL_Surface *s = SDL_CreateRGBSurfaceFrom(pixels, w, h, 32, w * sizeof(uint32_t),
      DEFAULT_RMASK, DEFAULT_GMASK, DEFAULT_BMASK, DEFAULT_AMASK);
  s->flags &= ~SDL_PREALLOC;
	printf("333\n");
  return s;
}
