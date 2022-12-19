#include <common.h>
#include "syscall.h"
#include <sys/time.h>
#include <proc.h>

void sys_execve(Context *c);
void naive_uload(PCB *pcb, const char *filename);

void sys_yield(Context *c)
{
	
	yield();
	c->GPRx = 0;
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_yield\n");
	#endif

}

void sys_exit(Context *c)
{
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_exit\n");
	#endif
	c->GPR2 = (uintptr_t)"/bin/menu";
	sys_execve(c);
	//halt(0);
}

void sys_write(Context *c)
{
	int fd = c->GPR2;
	char *buf = (char*)(c->GPR3);
	size_t count = c->GPR4;

	c->GPRx = fs_write(fd, buf, count);
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_write\n");
	#endif

//利用VFS，不需要让系统调用处理函数关心这些特殊文件的情况了
	/*if(fd == 1 || fd == 2)
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
	}*/
}

void sys_read(Context *c)
{
	int fd = c->GPR2;
	char* buf = (char*)c->GPR3;
	size_t len = c->GPR4;
	c->GPRx = fs_read(fd, buf, len);
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_read\n");
	#endif

}

void sys_brk(Context* c)
{
	//intptr_t addr = c->GPR2;//stand for the new program break address
	//c->GPR3 = addr;
	c->GPRx = 0;
	//PA3.2, sys_brk always return 0
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_brk\n");
	#endif

}

void sys_open(Context *c)
{
	char* path = (char*)c->GPR2;
	int flags = c->GPR3;
	int mode = c->GPR4;
	c->GPRx = fs_open(path, flags, mode);
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_open\n");
	#endif

}

void sys_lseek(Context *c)
{
	int fd = c->GPR2;
	size_t offset = c->GPR3;
	int whence = c->GPR4;
	c->GPRx = fs_lseek(fd, offset, whence);
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_lseek\n");
	#endif

}

void sys_close(Context *c)
{
	int fd = c->GPR2;
	c->GPRx = fs_close(fd);
	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_close\n");
	#endif

}

void sys_gettimeofday(Context *c)
{
	struct timeval* tmp = (struct timeval*)c->GPR2;

	tmp->tv_usec = io_read(AM_TIMER_UPTIME).us;
	tmp->tv_sec = tmp->tv_usec / 1000000; 
	c->GPRx = 0;

	#ifdef CONFIG_STRACE_COND
	printf("STRACE: sys_gettimeofday\n");
	#endif
}


void sys_execve(Context *c)
{
	char* filename = (char*)c->GPR2;
	//暂时忽略后面两个参数
	//execve无返回值
	printf("execve = %s\n", filename);
	naive_uload(NULL, filename);
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
	case SYS_gettimeofday:
		sys_gettimeofday(c);
		break;
	case SYS_execve:
		sys_execve(c);
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

}
