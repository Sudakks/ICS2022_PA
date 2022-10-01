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
#include "local-include/reg.h"
enum IDX
{
	$0 = 0, ra, sp, gp, tp, t0, t1, t2,
  s0, s1, a0, a1, a2, a3, a4, a5,
  a6, a7, s2, s3, s4, s5, s6, s7,
  s8, s9, s10, s11, t3, t4, t5, t6,
};
const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
	int len = sizeof(regs) / sizeof(regs[0]);
	//printf("len = %d\n", len);
	for(int i = 0; i < len; i++)
	{
		printf("%s  %-16p  %u\n", regs[i], cpu.gpr+i, cpu.gpr[i]);
	}
}

word_t isa_reg_str2val(const char *s, bool *success) {
	if(!strcmp(s, "$0"))
		return cpu.gpr[$0];

	if(!strcmp(s, "ra"))
		return cpu.gpr[ra];

	if(!strcmp(s, "sp"))
		return cpu.gpr[sp];

	if(!strcmp(s, "gp"))
		return cpu.gpr[gp];

	if(!strcmp(s, "tp"))
		return cpu.gpr[tp];

	if(!strcmp(s, "t0"))
		return cpu.gpr[t0];

	if(!strcmp(s, "t1"))
		return cpu.gpr[t1];

	if(!strcmp(s, "t2"))
		return cpu.gpr[t2];

	if(!strcmp(s, "s0"))
		return cpu.gpr[s0];

	if(!strcmp(s, "s1"))
		return cpu.gpr[s1];

	if(!strcmp(s, "a0"))
		return cpu.gpr[a0];

	if(!strcmp(s, "a1"))
		return cpu.gpr[a1];

	if(!strcmp(s, "a2"))
		return cpu.gpr[a2];

	if(!strcmp(s, "a3"))
		return cpu.gpr[a3];

	if(!strcmp(s, "a4"))
		return cpu.gpr[a4];

	if(!strcmp(s, "a5"))
		return cpu.gpr[a5];

	if(!strcmp(s, "a6"))
		return cpu.gpr[a6];

	if(!strcmp(s, "a7"))
		return cpu.gpr[a7];

	if(!strcmp(s, "s2"))
		return cpu.gpr[s2];

	if(!strcmp(s, "s3"))
		return cpu.gpr[s3];

	if(!strcmp(s, "s4"))
		return cpu.gpr[s4];

	if(!strcmp(s, "s5"))
		return cpu.gpr[s5];

	if(!strcmp(s, "s6"))
		return cpu.gpr[s6];

	if(!strcmp(s, "s7"))
		return cpu.gpr[s7];
		
	if(!strcmp(s, "s8"))
		return cpu.gpr[s8];

	if(!strcmp(s, "s9"))
		return cpu.gpr[s9];

	if(!strcmp(s, "s10"))
		return cpu.gpr[s10];

	if(!strcmp(s, "s11"))
		return cpu.gpr[s11];

	if(!strcmp(s, "t3"))
		return cpu.gpr[t3];

	if(!strcmp(s, "t4"))
		return cpu.gpr[t4];

	if(!strcmp(s, "t5"))
		return cpu.gpr[t5];

	if(!strcmp(s, "t6"))
		return cpu.gpr[t6];
	*success = false;
	return 0;
}
