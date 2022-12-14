#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main() {
printf("B----\n");
  NDL_Init(0);
  printf("A----\n");
  int w, h;
  printf("reach here\n");
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  printf("not here\n");
  assert(bmp);
  NDL_OpenCanvas(&w, &h);
  NDL_DrawRect(bmp, 0, 0, w, h);
  free(bmp);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}
