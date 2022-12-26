#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
	/*
	一开始参数传不进来，是传参类型的原因？
	*/
  while (1) {
    //Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    //Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (char*)arg, j);
    j ++;
    yield();
  }
}

extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB* pcb, void (*entry)(void*), void* arg);
extern void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);

void init_proc() {
  Log("Initializing processes...");
	/*
  switch_boot_pcb();
	//naive_uload(NULL, NULL);
	naive_uload(NULL, "/bin/nterm");
  // load program here
	*/
	
	char* arg1 = "arg1";
	context_kload(&pcb[0], hello_fun, arg1);
	//context_kload(&pcb[1], hello_fun, arg2);
	char *const argv[] = {"--skip"};
	context_uload(&pcb[1], "/bin/pal", argv, NULL);
	//context_uload(&pcb[1], "/bin/pal");
  switch_boot_pcb();
}

Context* schedule(Context *prev) {
	// save the context pointer
	current->cp = prev;

// always select pcb[0] as the new process
//在两个内核线程之间来回切换
	current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
	//printf("current->cp = %p\n", current->cp);

// then return the new context
	return current->cp;
}
