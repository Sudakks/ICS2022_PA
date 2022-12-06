#include <common.h>
#include "syscall.h"

void sys_yield(Context *c)
{
	yield();
	c->GPRx = 0;
}

void sys_exit(Context *c)
{
	halt(0);
}

void sys_write(Context *c)
{
	int fd = c->GPR2;
	char *buf = (char*)(c->GPR3);
	size_t count = c->GPR4;

	if(fd == 1 || fd == 2)
	{
		//stand for stdout, stderr
		//printf("count = %d\n", count);
		for(size_t i = 0; i < count ; i++)
		 {
			putch(*buf);
			buf++;
		}
		//set return value
		c->GPRx = count;
	}
}

void sys_brk(Context* c)
{
	intptr_t addr = c->GPR2;//stand for the new program break address
	c->GPR3 = addr;
	c->GPRx = 0;
	//PA3.2, sys_brk always return 0
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
	case SYS_yield:
		sys_yield(c);
	case SYS_exit:
		sys_exit(c);
		break;
	case SYS_write:
		sys_write(c);
		break;
	case SYS_brk:
		sys_brk(c);
		break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: system call number = %d, return value = %d\n", c->GPR1, c->GPRx);
	#endif

}
