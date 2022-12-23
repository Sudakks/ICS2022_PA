#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
			case 0xb:
			//printf("GPR1 = %d\n", c->GPR1);
				if(c->GPR1 == -1)//GPR1用来存放异常号，通过其来判断YIELD和SYSCALL
					ev.event = EVENT_YIELD;
				else
					ev.event = EVENT_SYSCALL;
				break;
      default: ev.event = EVENT_ERROR; break;
    }
		//printf("mcause = %x, mepc = %x, mstatus = %x\n", c->mcause, c->mepc, c->mstatus);
			c->mepc += 4;
		//这里因为最后要将下一条指令的地址赋给pc，所以提前
		//将mepc+4
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
	//set the exception entry into mtvec register directly
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
	/*创建内核线程的上下文*/
	/*ignore arg temporarily*/
	Context* con = (Context*)(kstack.end - sizeof(Context));
	con->mepc = (uintptr_t)entry;
	/*刚好指的就是一个函数的入口地址*/
	/*should add other things?*/
	/*应该是PCB中的cp指针*/
	printf("kcontext: con = %p\n", con);
	return con;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
