#include <common.h>
#include <string.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
	//串口的写入
	char *tmp = (char*)buf;
	for(size_t i = 0; i < len; i++)
	{
		putch(*tmp);
		tmp++;
	} 
	return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
	AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
	if(ev.keycode == AM_KEY_NONE)
		return 0;
	//get events and status
	//already define 2 events
	
	char* kc = (char*)keyname[ev.keycode];
	char* status = (ev.keydown == 1) ? "kd " : "ku ";
	char* info = "\n";
	
	strcpy(info, status);
	strcat(info, kc);
	strcpy(buf, info);
	

	size_t tmp = strlen(info);
	size_t ret = (len < tmp) ? len : tmp;
	return ret;
	
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	
	size_t ret = 16;
	int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
	printf("io_read: w = %d, h = %d\n", w, h);
	char W[32];
	char H[32];
	ret += sprintf(W, "%d\n", w);
	ret += sprintf(H, "%d\n", h);
	char* info = "\n";
	//char* tmp = "\n";

  strcpy(info, "WEIGHT: ");
  strcat(info, W);
  //strcat(info, tmp);
  strcat(info, "HEIGHT: ");
  strcat(info, H);
	//strcat(info, tmp);
	strcpy(buf, info);
	assert(ret < len);
	return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	//write len bytes from buf to the offset of screen
	int w = io_read(AM_GPU_CONFIG).width;
  //int h = io_read(AM_GPU_CONFIG).height;
	for(int i = 0; i < len; i++)
	{
		int x = (offset + i) % w;
		int y = (offset + i) / w;
		void* tmp = (void*)buf;
		io_write(AM_GPU_FBDRAW, x, y, tmp + i, 1, 1, false); 
	}
	io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
	/*update the whole screen*/
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
