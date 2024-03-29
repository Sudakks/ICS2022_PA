/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <cpu/cpu.h>
#include <stdlib.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
static int is_batch_mode = false;
word_t paddr_read(paddr_t addr, int len);
void init_regex();
word_t expr(char *e, bool *success);
void init_wp_pool();
bool del_wp(int idx);
struct watchpoint* new_wp(char* args, word_t temp);//remember to add "struct"!!!
void print_wps();
#define EOF (-1)
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);


static int cmd_si(char *args)
{
	int time;
	char *arg = strtok(NULL, " ");
  if (arg == NULL)
		time = 1;
	else
	{
		sscanf(arg, "%d", &time);
		if(time < 0)
		{
			printf("Invalid argument!\n");
			return 0;
		}
	} 
//parse the times cpu should execute
	cpu_exec(time);
	return 0;
}

static int cmd_info(char *args){
	char *arg = strtok(NULL, " ");
	if(strcmp(arg, "r") == 0)
	{//arg is not a pointer, *arg is a pointer
		//print the registers
		isa_reg_display();
	}
	else if(strcmp(arg, "w") == 0)
	{
		print_wps();
	}
	else 
		printf("Invalid argument!\n");
	return 0;
}


static int cmd_x(char *args){
  char *arg = strtok(NULL, " ");
	if(arg == NULL)
	{
		printf("Lack enough arguments!\n");
		return 0;
	}
	int time = atoi(arg);
	arg = strtok(NULL, " ");
	if(arg == NULL)
	{
		printf("Lack enough arguments!\n");
		return 0;
	}
	bool suc = true;
	paddr_t addr = expr(arg, &suc);
	if(suc == false)
	{
		printf("Invalid argument!\n");
		return 0;
	}
	//sscanf(arg, "%x", &addr);
	int len = 4;
	for(int i = 1; i <= time; i++)
	{
		printf("%#X %#8X\n", addr, vaddr_read(addr, len));
		addr += 4;
	}
	return 0;
}

static int cmd_p(char *args){
	if(args == NULL)
	{
		printf("Lack enough arguments!\n");
		return 0;
	}
	bool suc = true;
	word_t ans = expr(args, &suc);
	if(suc == true)
		printf("%u\n", ans);
	else
		printf("Invalid exprssion! Can't calculate!\n");
	return 0;
}

static int cmd_w(char *args)
{
	if(args == NULL)
	{ 
		printf("Lack enough arguments!\n");
		return 0;
	}
	bool suc = true;
	word_t temp = expr(args, &suc);
	if(suc == false)
	{
		printf("Invalid expression! Can't watch this value!\n");
		return 0;
	}
	new_wp(args, temp);
	return 0;
}
static int cmd_d(char *args)
{
	char *arg = strtok(args, " ");
	if(arg == NULL)
	{
		printf("Lack enough arguments!\n");
		return 0;
	}
	int wp = atoi(arg);
  bool suc = del_wp(wp);
	if(suc == false)
		printf("Failed! No such watchpoint!\n");
	else
		printf("Watchpoint %d deleted!\n", wp);
	return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Excute step by step", cmd_si},
	{ "info", "Print the information of registers and the watchpoints", cmd_info},
	{ "x", "Scan the memory", cmd_x},
	{ "p", "Calculate the given expression", cmd_p},
	{ "w", "Set watchpoints", cmd_w},
	{ "d", "Delete a certain watchpoint", cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);//compare whether it's the same order
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");//the first use of strtok
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { nemu_state.state = NEMU_QUIT; return; }//it shows we press q
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
