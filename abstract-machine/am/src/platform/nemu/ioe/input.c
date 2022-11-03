#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	
	uint8_t input = inb(KBD_ADDR);
	//1 byte
  //inl(KBD_ADDR);
	if(input == AM_KEY_NONE)
		kbd->keydown = false;
	else
		kbd->keydown = true;
}
