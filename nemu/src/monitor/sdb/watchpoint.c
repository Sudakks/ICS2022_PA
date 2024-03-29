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

#include "sdb.h"
#include <cpu/cpu.h>

#define NR_WP 32
word_t expr(char *e, bool *success);
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
	struct watchpoint *last;//point to last one
	word_t now;//mark the now value and before value to compare whether the two are changed
	word_t before;
	//char* expression;//the expression to calculate
	char expression[512];
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
void print_expr(WP *sta)
{
		int i = 0;
		while(sta->expression[i] != '\0')
		{
			printf("%c", (sta->expression[i]));
			i++;
		}
		printf("\n");
}

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
		wp_pool[i].last = (i == 0 ? NULL : &wp_pool[i - 1]);
		//printf("addr %d = %p\n", i, &wp_pool[i]);
  }
  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char* args, int temp)
{
	//give a free wp
	//indicate there lacks enough free wp
	//printf("free addr = %p\n", free_);
	if(free_ == NULL)
		assert(0);
	else
	{  
		WP* fr = free_;
		fr->before = temp;
		memset(fr->expression, 0, sizeof(fr->expression));
		strcpy(fr->expression, args);
		if(head == NULL)
		{
			//there is no wp exists
			head = fr;
			free_ = free_->next;
			free_->last = NULL;
			head->last = NULL;
			head->next = NULL;
			//!!!我在这里改变了它的直！omgomg
		}
		else
		{
			//head insert
			free_ = free_->next;
			fr -> next = head;
			head->last = fr;
			head = fr;
			head->last = NULL;
		} 
		//printf("next addr = %p\n", free_);
		printf("Set watchpoint %d:  ", fr->NO);
		print_expr(fr);
		return fr;
		//may need modify the info
	}  
}

void free_wp(WP *wp)
{
	//return the wp back to free_
	if(wp->next != NULL)
	{
		wp->next->last = wp->last;
	}
	if(wp->last != NULL)
	{
		wp->last->next = wp->next;
	}
	if(head == wp)
		head = head->next;
	wp->next = free_;
	free_->last = wp;
	wp->last = NULL;
	free_ = wp;
	//put the return one to the head of free_
}

void scan_wps()
{
	int flag = 0;//indicate whether there has some values to be changed
	WP* sta = head;
	while(sta != NULL)
	{
		bool suc = true;
		sta->now = expr(sta->expression, &suc);
			if(sta->now != sta->before)
			{
				flag = 1;
				printf("Hit watchpoint %d:  ", sta->NO);
				print_expr(sta);
				printf("Old value = %u\n", sta->before);
				printf("New value = %u\n", sta->now);
				sta->before = sta->now;
		  	}
		sta = sta->next;
	}  
	if(flag == 1)
		nemu_state.state = NEMU_STOP;
}

void print_wps()
{
	//print the information of wps
	WP* sta = head;
	printf("NO       Old Value      What\n");
	while(sta != NULL)
	{
		printf("%-9d", sta->NO);
		printf("%-15u", sta->before);
		print_expr(sta);
		sta = sta->next;
	}
}

bool del_wp(int idx)
{
	WP* sta = head;
	while(sta != NULL)
	{
		if(sta->NO == idx)
		{
			//find the target
			free_wp(sta);
			return true;
		}
		sta = sta->next;
	}
	return false;
}
