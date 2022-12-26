.globl  _start
_start:
	move a0, sp
	//栈指针存放在GPRx，也即a0中，即把它设置到栈指针寄存器中
  move s0, zero
  jal call_main
