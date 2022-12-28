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
	strcpy(str, cmd);
	str[strlen(cmd) - 1] = '\0';
	//printf("cmd = %s\n", str);
	//overwrite is set to 0

	char* argv[16];
	//char** envp;
	const char del[2] = " ";
	char* token;
	int argc = 0;

	token = strtok(str, del);

	while(token != NULL ) 
	{
		argv[argc++] = token;
		//printf("%s\n", token);
    token = strtok(NULL, del);
		//我要哭出来了，这里写成了str分割，所以一直解析不对,,,,
		//天哪，这个小小的地方，竟然耗费了我一天的时间，真的会哭出来啊啊啊啊！！！！
		//但是这里解决了，busybox也可以运行了终于！
		//还是有点开心在里面的哈哈哈哈
  }
	argv[argc] = NULL;
	//我不理解为啥，他就是解析不了p这个字符

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
