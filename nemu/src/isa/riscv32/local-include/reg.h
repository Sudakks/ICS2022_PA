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

#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include <common.h>

static inline int check_reg_idx(int idx) {
  IFDEF(CONFIG_RT_CHECK, assert(idx >= 0 && idx < 32));
  return idx;
}

static inline int check_sr_idx(int idx)
{
  IFDEF(CONFIG_RT_CHECK, assert(idx >= 0 && idx < 4));
	return idx;
}

/*
word_t Read_mycsr(word_t addr)
{
	switch(addr)
	{
		case 0x341:
			return cpu.mepc;
		case 0x342:
			return cpu.mcause;
		case 0x305:
			return cpu.mtvec;
		case 0x300:
			return cpu.mstatus;
		default:
			panic("No more accessible SRs can read!");
	}
}
void Write_mycsr(word_t addr, word_t val)
{
	switch(addr)
	{
		case 0x341:
			cpu.gpr[0] = val;
			break;
		case 0x342:
			cpu.mcause = val;
			break;
		case 0x305:
			cpu.mtvec = val;
			break;
		case 0x300:
			cpu.mstatus = val;
			break;
		default:
			panic("No more accessible SRs can write!");
	}

}
*/
//#define csr(idx) cpu.mycsr[check_sr_idx(idx)]
//Pa3.1，目前只用了4个寄存器，后续可能更改
#define gpr(idx) cpu.gpr[check_reg_idx(idx)]

static inline const char* reg_name(int idx, int width) {
  extern const char* regs[];
  return regs[check_reg_idx(idx)];
}

#endif
