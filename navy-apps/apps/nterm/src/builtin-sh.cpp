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
	char str[128];
	strcpy(str, (char*)cmd);
	str[strlen(cmd) - 1] = '\0';
	//printf("cmd = %s\n", str);
	//overwrite is set to 0

	char* argv[16];
	//char** envp;
	const char del[2] = " ";

	int argc = 0;
	char* token = strtok(str, del);
	while(token != NULL ) 
	{
		argv[argc++] = token;
    printf("%s\n", argv[argc-1]);
    token = strtok(NULL, str);
  }
	argv[argc] = NULL;
	//我不理解为啥，他就是解析不了p这个字符

	//setenv("PATH", "/bin:/usr/bin", 0);
	execvp(argv[0], argv);
	/*
	注意传给execvp的argv参数列表，也包括cmd命令！
	*/
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

	setenv("PATH", "/bin:/usr/bin", 0);

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
