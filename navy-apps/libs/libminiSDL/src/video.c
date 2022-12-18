#include <NDL.h>
#include <sdl-video.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
  assert(dst && src);
  assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);
	/*
		if srcrect == NULL, copy the whole src
		The width and height in srcrect determine the size of the copied rectangle
		Only the position is used in the dstrect
	*/
	
	int w, h;
	int src_x, src_y;
	int dst_x, dst_y;
	if(dstrect == NULL)
		dst_x = 0, dst_y = 0;
	else
		dst_x = dstrect->x, dst_y = dstrect->y;

	if(srcrect == NULL)
	{
		src_x = 0, src_y = 0;
		w = src->w, h = src->h;
	}
	else
	{
		src_x = srcrect->x, src_y = srcrect->y;
		w = srcrect->w, h = srcrect->h;	
	}

	if(dst->format->BitsPerPixel == 32)
	{
		int width = 4;
		for(int i = 0; i < h; i++)
		{
			memcpy(dst->pixels + ((dst_y + i) * dst->w + dst_x) * width, src->pixels + ((src_y + i) * src->w + src_x) * width, w * width);
		}
	}
	else if(dst->format->BitsPerPixel == 8)
	{
		uint8_t* sp = (uint8_t*) src->pixels;
		uint8_t* dp = (uint8_t*) dst->pixels;
		for(int i = 0; i < h; i++)
		{
			for(int j = 0; j < w; j++)
			{
				dp[(dst_y + i) * dst->w + dst_x + j] = sp[(src_y + i) * src->w + src_x + j];
			}
		}
		/*
		for(int i = 0; i < h; i++)
		{
			memcpy((uint8_t*)dst->pixels + ((dst_y + i) * dst->w + dst_x), (uint8_t*)src->pixels + ((src_y + i) * src->w + src_x), w * sizeof(uint8_t));
		}
		*/
	}
	else
	{
		printf("Wrong BytesPerPixel!\n");
		assert(0);
	}

/*
	uint32_t width = (dst->format->BytesPerPixel == 4) ? 4 : 1;
	for(int i = 0; i < h; i++)
	{
		memcpy(dst->pixels + ((dst_y + i) * dst->w + dst_x) * width, src->pixels + ((src_y + i) * src->w + src_x) * width, w * width);
		//printf("%d - %d - %d\n", (dst_y + i) * dst->w + dst_x, (src_y + i) * src->w + src_x, w);
*/
		/*
		Mark错误:
		是因为没有区分他的bytes数，所以导致这里无法正确绘图
		*/
}

void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
	//往画布的指定矩形区域中填充指定的颜色
	//即改变pixels里面的内容
	int x, y, w, h;
	if(dstrect == NULL)
	{
		//往整个屏幕填充color
		x = 0, y = 0;
		w = dst->w, h = dst->h;
	}
	else
	{
		x = dstrect->x, y = dstrect->y;
		w = dstrect->w, h = dstrect->h;
	}
	//advance pixels
	if(dst->format->BitsPerPixel == 32)
	{
		printf("Fill 4\n");
		/*
		for(int i = 0; i < h; i++)
		{
			for(int j = 0; j < w; j++)
			{
				*((uint32_t*)dst->pixels + ((y + i) * w + x + j)) = color;
			}
		}
		*/
		for(int i = 0; i < h; i++)
		{
			memset((uint32_t*)dst->pixels + (y + i) * dst->w + x, color, sizeof(uint32_t) * w);
		}
	}
	else if(dst->format->BitsPerPixel == 8)
	{
		printf("Fill 1\n");
		/*
		for(int i = 0; i < h; i++)
		{
			for(int j = 0; j < w; j++)
			{
				*((uint8_t*)dst->pixels + ((y + i) * w + x + j)) = color; 
			}
		}
		*/
		for(int i = 0; i < h; i++)
		{
			memset((uint8_t*)dst->pixels + (y + i) * dst->w + x, color, sizeof(uint8_t) * w);
		}
	}
	else
	{
		printf("Wrong BytesPerPixels!\n");
		assert(0);
	}
	
	/*
	uint32_t* pix = (uint32_t*)dst->pixels;
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			pix[(y + i) * w + x + j] = color;
		}
	}
	//不知道为啥，但这里不能一行一行赋值
	*/
}

void SDL_UpdateRect(SDL_Surface *s, int x, int y, int w, int h) {
	//s->pixels是void*类型
		/*NDL_DrawRect receive pixels is uint32_t*
	so, we should set the pixles also as 32 bits
	*/

	//uint32_t idx = 0;
	if(s->format->BitsPerPixel == 32)
	{
		printf("update 4\n");
		if(x == 0 && y == 0 && w == 0 && h == 0)
		{
			w = s->w;
			h = s->h;
		}
		uint32_t* pix = malloc(w * h * sizeof(uint32_t));
		assert(pix);
		for(int i = 0; i < h; i++)
			memcpy(pix + i * w, (uint32_t*)s->pixels + (y + i) * s->w + x, w * sizeof(uint32_t));
		NDL_DrawRect(pix, x, y, w, h);
		free(pix);
	}
	else if(s->format->BitsPerPixel == 8)
	{
		//wrong here
		if(w == 0 && h == 0 && x == 0 && y == 0)
		{
			w = s->w, h = s->h;
		}
		printf("w = %d, h = %d, x = %d, y = %d\n", w, h, x, y);
		//printf("update 1\n");
		uint32_t* pix = malloc(w * h * sizeof(uint32_t));
		assert(pix);
		uint8_t* now_pix = (uint8_t*)s->pixels;
		for(int i = 0; i < h; i++)
		{
			for(int j = 0; j < w; j++)
			{
				uint32_t pos = (y + i) * s->w + x + j;
				SDL_Color col = s->format->palette->colors[now_pix[pos]];
//s->format->palette->colors[pixels[坐标索引]]
				if(col.r != 0 && col.g != 0)
					printf("pos = %d, r = %d, g = %d, b = %d\n", w *i + j, col.r, col.g, col.b);
				pix[w * i + j] = (col.a << 24) | (col.b << 16) | (col.g << 8) | col.b;
			}
		}
		NDL_DrawRect(pix, x, y, w, h);
		free(pix);
	}
	else
	{
		printf("Wrong BytesPerPixels!\n");
		assert(0);
	}
	/*
if(x == 0 && y == 0 && w == 0 && h == 0)
		{
			NDL_DrawRect((uint32_t*)s->pixels, x, y, s->w, s->h);
			return;
		}
		uint32_t* pix = malloc(w * h * sizeof(uint32_t));
		assert(pix);

		for(int i = 0; i < h; i++)
			memcpy(pix + i * w, (uint32_t*)s->pixels + (y + i) * s->w + x, w * sizeof(uint32_t));
		NDL_DrawRect(pix, x, y, w, h);
		free(pix);
*/
		//将画布中的指定矩形区域同步到屏幕上
	//要更新的区域不能超过屏幕
	//如果xywh都为0,那么更新整个屏幕
	//w和h都是以像素记录的
}

// APIs below are already implemented.

static inline int maskToShift(uint32_t mask) {
  switch (mask) {
    case 0x000000ff: return 0;
    case 0x0000ff00: return 8;
    case 0x00ff0000: return 16;
    case 0xff000000: return 24;
    case 0x00000000: return 24; // hack
    default: assert(0);
  }
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
    uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
  assert(depth == 8 || depth == 32);
  SDL_Surface *s = malloc(sizeof(SDL_Surface));
  assert(s);
  s->flags = flags;
  s->format = malloc(sizeof(SDL_PixelFormat));
  assert(s->format);
  if (depth == 8) {
    s->format->palette = malloc(sizeof(SDL_Palette));
    assert(s->format->palette);
    s->format->palette->colors = malloc(sizeof(SDL_Color) * 256);
    assert(s->format->palette->colors);
    memset(s->format->palette->colors, 0, sizeof(SDL_Color) * 256);
    s->format->palette->ncolors = 256;
  } else {
    s->format->palette = NULL;
    s->format->Rmask = Rmask; s->format->Rshift = maskToShift(Rmask); s->format->Rloss = 0;
    s->format->Gmask = Gmask; s->format->Gshift = maskToShift(Gmask); s->format->Gloss = 0;
    s->format->Bmask = Bmask; s->format->Bshift = maskToShift(Bmask); s->format->Bloss = 0;
    s->format->Amask = Amask; s->format->Ashift = maskToShift(Amask); s->format->Aloss = 0;
  }

  s->format->BitsPerPixel = depth;
  s->format->BytesPerPixel = depth / 8;

  s->w = width;
  s->h = height;
  s->pitch = width * depth / 8;
  assert(s->pitch == width * s->format->BytesPerPixel);

  if (!(flags & SDL_PREALLOC)) {
    s->pixels = malloc(s->pitch * height);
    assert(s->pixels);
  }

  return s;
}

SDL_Surface* SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth,
    int pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
  SDL_Surface *s = SDL_CreateRGBSurface(SDL_PREALLOC, width, height, depth,
      Rmask, Gmask, Bmask, Amask);
  assert(pitch == s->pitch);
  s->pixels = pixels;
  return s;
}

void SDL_FreeSurface(SDL_Surface *s) {
  if (s != NULL) {
    if (s->format != NULL) {
      if (s->format->palette != NULL) {
        if (s->format->palette->colors != NULL) free(s->format->palette->colors);
        free(s->format->palette);
      }
      free(s->format);
    }
    if (s->pixels != NULL && !(s->flags & SDL_PREALLOC)) free(s->pixels);
    free(s);
  }
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
  if (flags & SDL_HWSURFACE) NDL_OpenCanvas(&width, &height);
  return SDL_CreateRGBSurface(flags, width, height, bpp,
      DEFAULT_RMASK, DEFAULT_GMASK, DEFAULT_BMASK, DEFAULT_AMASK);
}

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect) {
  assert(src && dst);
  assert(dst->format->BitsPerPixel == src->format->BitsPerPixel);
  assert(dst->format->BitsPerPixel == 8);

  int x = (srcrect == NULL ? 0 : srcrect->x);
  int y = (srcrect == NULL ? 0 : srcrect->y);
  int w = (srcrect == NULL ? src->w : srcrect->w);
  int h = (srcrect == NULL ? src->h : srcrect->h);

  assert(dstrect);
  if(w == dstrect->w && h == dstrect->h) {
    /* The source rectangle and the destination rectangle
     * are of the same size. If that is the case, there
     * is no need to stretch, just copy. */
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_BlitSurface(src, &rect, dst, dstrect);
  }
  else {
    assert(0);
  }
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, int firstcolor, int ncolors) {
  assert(s);
  assert(s->format);
  assert(s->format->palette);
  assert(firstcolor == 0);

  s->format->palette->ncolors = ncolors;
  memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

  if(s->flags & SDL_HWSURFACE) {
    assert(ncolors == 256);
    for (int i = 0; i < ncolors; i ++) {
      uint8_t r = colors[i].r;
      uint8_t g = colors[i].g;
      uint8_t b = colors[i].b;
    }
    SDL_UpdateRect(s, 0, 0, 0, 0);
  }
}

static void ConvertPixelsARGB_ABGR(void *dst, void *src, int len) {
  int i;
  uint8_t (*pdst)[4] = dst;
  uint8_t (*psrc)[4] = src;
  union {
    uint8_t val8[4];
    uint32_t val32;
  } tmp;
  int first = len & ~0xf;
  for (i = 0; i < first; i += 16) {
#define macro(i) \
    tmp.val32 = *((uint32_t *)psrc[i]); \
    *((uint32_t *)pdst[i]) = tmp.val32; \
    pdst[i][0] = tmp.val8[2]; \
    pdst[i][2] = tmp.val8[0];

    macro(i + 0); macro(i + 1); macro(i + 2); macro(i + 3);
    macro(i + 4); macro(i + 5); macro(i + 6); macro(i + 7);
    macro(i + 8); macro(i + 9); macro(i +10); macro(i +11);
    macro(i +12); macro(i +13); macro(i +14); macro(i +15);
  }

  for (; i < len; i ++) {
    macro(i);
  }
}

SDL_Surface *SDL_ConvertSurface(SDL_Surface *src, SDL_PixelFormat *fmt, uint32_t flags) {
  assert(src->format->BitsPerPixel == 32);
  assert(src->w * src->format->BytesPerPixel == src->pitch);
  assert(src->format->BitsPerPixel == fmt->BitsPerPixel);

  SDL_Surface* ret = SDL_CreateRGBSurface(flags, src->w, src->h, fmt->BitsPerPixel,
    fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

  assert(fmt->Gmask == src->format->Gmask);
  assert(fmt->Amask == 0 || src->format->Amask == 0 || (fmt->Amask == src->format->Amask));
  ConvertPixelsARGB_ABGR(ret->pixels, src->pixels, src->w * src->h);

  return ret;
}

uint32_t SDL_MapRGBA(SDL_PixelFormat *fmt, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  assert(fmt->BytesPerPixel == 4);
  uint32_t p = (r << fmt->Rshift) | (g << fmt->Gshift) | (b << fmt->Bshift);
  if (fmt->Amask) p |= (a << fmt->Ashift);
  return p;
}

int SDL_LockSurface(SDL_Surface *s) {
  return 0;
}

void SDL_UnlockSurface(SDL_Surface *s) {
}
