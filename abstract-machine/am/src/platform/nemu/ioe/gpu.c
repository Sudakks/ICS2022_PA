#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
	int i;
	uint32_t info = inl(VGACTL_ADDR);	
  int w = (info >> 16) & 0x0000ffff;  // TODO: get the correct width
  int h = (info & 0x0000ffff);  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
	uint32_t info = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = (info >> 16) & 0x0000ffff, .height = info & 0x0000ffff,
    .vmemsz = inl(FB_ADDR)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    //outl(SYNC_ADDR, 1);
		int x = ctl->x;
		int y = ctl->y;
		void *pixels = ctl->pixels;
		int w = ctl->w;
		int h = ctl->h;
		if(w == 0 || h == 0)
			return;//表示没有要写的东西

		uint32_t info = inl(VGACTL_ADDR);
		int W = (info >> 16) & 0x0000ffff;
		uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
		uint32_t base = x + y * W;
		uint32_t *p = (uint32_t*)pixels;
		printf("x = %d, y = %d, w = %d, h = %d\n", x, y, w, h);
		for(int i = 0; i < h; i++)
		{
			for(int j = 0; j < w; j++)
			{
				fb[base + j + i * W] = *p;
				//p++;
			}
		}
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
