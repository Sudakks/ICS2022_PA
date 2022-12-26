#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
	/*
  char *empty[] =  {NULL };
  environ = empty;
  exit(main(0, empty, empty));
  assert(0);
	*/
	/*
	把move改成mv后就能识别了？？？
	猜测可能riscv只有mv指令
	*/
	//printf("call----  %p\n", args);
	int argc = *(int*)args;
	//printf("in call_main:\nargc = %d\n", argc);
	//现在有argc个参数
	char** tmp = (char**)(args + 1);
	char** argv = tmp;

	while(*tmp != NULL)
		tmp++;
	tmp++;
	char** envp = tmp;

  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
