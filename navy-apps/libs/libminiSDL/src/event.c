#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
	//success return 1, otherwise return 0
	//有type和sym两个变量
	char buf[64];
	char key_sym[32];
  int ret = NDL_PollEvent(buf, sizeof(buf)); 
	if(ret == 0)
		return 0;// no key

	uint8_t sym, type;
	char* key_ptr = &buf[2];
	while(*key_ptr == ' ')
			key_ptr++;
	//此时key_ptr指向的就应该是按键的类型

	for(int i = 0; i < 32; i++, key_ptr++)
	{
		key_sym[i] = *key_ptr;
		if(*key_ptr == '\0' || *key_ptr == '\n')
		{
			key_sym[i] = '\0';
			break;
		}
	}

	if(buf[0] == 'k' && buf[1] == 'd')
	{
		//keydown
		type = SDL_KEYDOWN;
	}
	else if(buf[0] == 'k' && buf[1] == 'u')
	{
		type = SDL_KEYUP;
	}
	int sz = sizeof(keyname) / sizeof(char*);
	for(int i = 0; i <= sz; i++)
	{
		if(strcmp(keyname[i], key_sym) == 0)
		{
			//find sym
			sym = i;
			break;
		}
	}
	event->type = type;
	printf("type = %d\n, sym = %d\n", type, sym);
	event->key.keysym.sym = sym;
	//这个应该对应着什么按键
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
