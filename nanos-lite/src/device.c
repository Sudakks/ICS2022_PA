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
	char* info = (ev.keydown == 1) ? "kd " : "ku ";
	printf("%s - %s\n", info, kc);
	strcat(info, kc);
	size_t tmp = strlen(info);
	size_t ret = (len < tmp) ? len : tmp;
	strcpy(buf, info);
	return ret;
	
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
