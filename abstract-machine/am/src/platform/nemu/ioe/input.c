#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	kbd->keycode = inl(KBD_ADDR);
	//1 byte
//	kbd->keycode = inl(KBD_ADDR);
	if(kbd->keycode == AM_KEY_NONE)
		kbd->keydown = false;
	else
		kbd->keydown = true;
}
