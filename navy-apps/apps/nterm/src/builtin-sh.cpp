#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>
#include <stdlib.h>
#include <string.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
	//这个是读到的路径，调用这个文件
	//set PATH=/bin
	//先要把'\n'去掉
	//现在要解析参数
	char* tmp = (char*) cmd;
	tmp[strlen(cmd) - 1] = '\0';
	printf("cmd = %s\n", cmd);
	//overwrite is set to 0
	char* my_cmd;
	char str[64];
	char* argv;
	//char** envp;
	strcpy(str, tmp);
	const char del[2] = " ";
	my_cmd = strtok(tmp, del);
	printf("my_cmd = %s\n", my_cmd);
	//if(my_cmd != NULL)
		//argv = (char**)strtok(NULL, del);
	argv = tmp;
	printf("argv = %s\n", argv);
	//if(argv != NULL)
		//envp = (char**)strtok(NULL, del);
		
	setenv("PATH", "/bin", 0);
	//execvpe(my_cmd, (char**const)argv, (char**const)envp);
	//execvp(my_cmd, (char**const) argv);
	execvp(cmd, NULL);
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
