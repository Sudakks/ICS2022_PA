#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
// 以毫秒为单位返回系统时间
	struct timeval now_time;
	gettimeofday(&now_time, NULL);
	return now_time.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
	//read events from /dev/events to buf
	//only read one event each time
	int fd = open("/dev/events", O_RDONLY);
	int ret = read(fd, buf, len);
	return (ret == 0) ? 0 : 1;
}

void NDL_OpenCanvas(int *w, int *h) {
	printf("i am here\n");
	printf("here w = %d, h = %d\n", *w, *h);
	if(*w == 0 && *h == 0)
	{
		*w = screen_w;
		*h = screen_h;
	}
	else if(*w > screen_w || *h > screen_h)
		printf("Set size is bigger than screen size\n");

  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
	//should set screen_w and screen_h
  return 0;
}

void NDL_Quit() {
}
