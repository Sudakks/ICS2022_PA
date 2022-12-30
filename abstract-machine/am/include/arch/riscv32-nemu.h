#ifndef ARCH_H__
#define ARCH_H__

struct Context {
	/*不同架构之间上下文信息的差异过大，所以只能由不同的am自己来设定*/
  // TODO: fix the order of these members to match trap.S
  //uintptr_t mepc, mcause, gpr[32], mstatus;
	//PA3.1
	uintptr_t gpr[32], mcause, mstatus, mepc;
  void *pdir;
	/*这是进程的地址空间描述符指针*/
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[10] // a0
#define GPR3 gpr[11] // a1
#define GPR4 gpr[12] // a2
#define GPRx gpr[10] // a0

#endif
