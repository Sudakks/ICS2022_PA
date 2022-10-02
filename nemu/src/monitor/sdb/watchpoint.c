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
	bool *suc;
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
		wp_pool[i].last = (i == 0 ? NULL : &wp_pool[i - 1]);
  }
  head = NULL;
  free_ = wp_pool;
	if((free_)->next == &wp_pool[1])
		printf("yes next\n");
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char* args, int temp)
{
	//give a free wp
	//indicate there lacks enough free wp
	if(free_ == NULL)
		assert(0);
	else
	{  
		if(free_ == &wp_pool[0])
			printf("yes 0\n");
		WP* fr = free_;
		fr->before = temp;
		strcpy(fr->expression, args);
		if(head == NULL)
		{
			//there is no wp exists
			head = fr;
			head->last = NULL;
			head->next = NULL;
		}
		else
		{
			printf("have one\n");
			//head insert
			fr -> next = head;
			head->last = fr;
			head = fr;
			head->last = NULL;
		} 
		free_ = free_ -> next;
		if(free_ == &wp_pool[1])
			printf("yes 1\n");
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
	wp->next = free_;
	free_->last = wp;
	wp->last = NULL;
	free_ = wp;
	//put the return one to the head of free_
}

void scan_wps()
{
	WP* sta = head;
	while(sta != NULL)
	{
		bool suc = true;
		sta->now = expr(sta->expression, &suc);
	//	printf("now = %d, before = %d\n", sta->now, sta->before);
	//	printf("reach here!\n");
			if(sta->now != sta->before)
			{
				printf("Watchpoint %d\n", sta->NO);
				printf("Old value = %u\n", sta->before);
				printf("New value = %u\n", sta->now);
				sta->before = sta->now;
				nemu_state.state = NEMU_STOP;
		 	}
		sta = sta->next;
	} 
}
