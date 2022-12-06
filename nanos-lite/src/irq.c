#include <common.h>

extern void do_syscall(Context* c);
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
		case EVENT_YIELD:
			Log("Run into EVENT_YIELD");
			break;
		case EVENT_SYSCALL:
			do_syscall(c);
			assert(0);
			printf("--- strace: system call number = %s, return value = %d\n", c->GPR1, c->GPRx);
			break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
