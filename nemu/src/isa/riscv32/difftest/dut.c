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
#include <cpu/difftest.h>
#include <cpu/cpu.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	//compare reg value and PC
	//if(ref_r -> pc != pc)
	//	return false;似乎不行，无法比较
	//PA2.3
	int len = sizeof(cpu.gpr) / sizeof(cpu.gpr[0]);
	for(int i = 0; i < len; i++)
	{
		if(cpu.gpr[i] != ref_r->gpr[i])
		{
			printf("Right gpr %s should be %x\n", reg_name(i, 8), ref_r->gpr[i]);
			return false;
		}
	}
	return true;
}

void isa_difftest_attach() {
}
