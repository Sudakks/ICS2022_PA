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
		for(size_t i = 0; i < count ; i++)
		 {
			putch(*buf);
			buf++;
		}
		//set return value
		c->GPRx = count;
	}
	else
	{
		//这是文件的写
		c->GPRx = fs_write(fd, buf, count);
	}
}

void sys_read(Context *c)
{
	int fd = c->GPR2;
	char* buf = (char*)c->GPR3;
	size_t len = c->GPR4;
	c->GPRx = fs_write(fd, buf, len);
}

void sys_brk(Context* c)
{
	intptr_t addr = c->GPR2;//stand for the new program break address
	c->GPR3 = addr;
	c->GPRx = 0;
	//PA3.2, sys_brk always return 0
}

void sys_open(Context *c)
{
	char* path = (char*)c->GPR2;
	int flags = c->GPR3;
	int mode = c->GPR4;
	c->GPRx = fs_open(path, flags, mode);
}

void sys_lseek(Context *c)
{
	int fd = c->GPR2;
	size_t offset = c->GPR3;
	int whence = c->GPR4;
	c->GPRx = fs_lseek(fd, offset, whence);
}

void sys_close(Context *c)
{
	int fd = c->GPR2;
	c->GPRx = fs_close(fd);
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
	case SYS_open:
		sys_open(c);
		break;
	case SYS_read:
		sys_read(c);
		break;
	case SYS_lseek:
		sys_lseek(c);
		break;
	case SYS_close:
		sys_close(c);
		break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: system call number = %d, return value = %d\n", c->GPR1, c->GPRx);
	#endif

}
