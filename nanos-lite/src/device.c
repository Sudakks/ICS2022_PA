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
	int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
	printf("i am here\n");
	printf("w = %d, h = %d\n", w, h);
	char* W = "\0";
	char* H = "\0";
  snprintf(W, 100, "%d", w);
	snprintf(H, 100, "%d", h);
  char* tmp = "\n";
  strcpy(buf, "WEIGHT: ");
  strcat(buf, W);
  strcat(buf, tmp);
  strcat(buf, "HEIGHT: ");
  strcat(buf, H);
  return (strlen(buf) < len) ? strlen(buf) : len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
