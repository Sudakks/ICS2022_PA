#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	inl(0xa0000060);
//	kbd->keycode = inl(KBD_ADDR);
/*	if(kbd->keycode == AM_KEY_NONE)
		kbd->keydown = false;
	else
		kbd->keydown = true;*/
}
