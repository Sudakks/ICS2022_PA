#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
	//open
	printf("start open\n");
	FILE *fp = fopen(filename, "r");
  if(fp == NULL) // 打开文件失败
	{
      return NULL;
	}

	//size
  fseek(fp, 0, SEEK_END);//定位文件指针到文件尾。
  size_t len = ftell(fp);//获取文件指针偏移量，即文件大小。

	//copy
	char* buf = malloc(len * sizeof(char));
	fseek(fp, 0, SEEK_SET);
	fread(buf, len, sizeof(char), fp);
	printf("len = %d\n", len);

	printf("before Load\n");
	SDL_Surface* ret = STBIMG_LoadFromMemory(buf, len);
	printf("end Load\n");

	//close
  fclose(fp);
	free(buf);
	printf("end open\n");
	return ret; 
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
