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
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {

/*
	if(cpu.satp & 0x80000000)
		printf("should MMU\n");
	else
		printf("cpu.satp = %x\n", cpu.satp);
		*/
	//vaddr_t bug = addr;
	//printf("ifetch = %x\n", addr);
	addr = (isa_mmu_check(addr, len, 0) == MMU_TRANSLATE) ? isa_mmu_translate(addr, len, 0) : addr;
	//assert(addr == bug);
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
	/*
	if(cpu.satp & 0x80000000)
		printf("should MMU\n");
	else
		printf("cpu.satp = %x\n", cpu.satp);
		*/
		/*
	else
		printf("satp = %d\n", cpu.satp);
		*/
		/*
	if(isa_mmu_check(vaddr, len, 0) == MMU_TRANSLATE)
		printf("should MMU\n");
	else if(isa_mmu_check(vaddr, len, 0) == MMU_DIRECT)
		printf("should not MMU\n");
		*/

	//printf("vaddr_read = %x\n", addr);
	//vaddr_t bug = addr;
	addr = (isa_mmu_check(addr, len, 0) == MMU_TRANSLATE) ? isa_mmu_translate(addr, len, 0) : addr;
	//assert(addr == bug);
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
/*
	if(cpu.satp & 0x80000000)
		printf("should MMU\n");
	else
		printf("cpu.satp = %x\n", cpu.satp);
		*/
	//printf("vaddr_write = %x\n", addr);
	//vaddr_t bug = addr;
	addr = (isa_mmu_check(addr, len, 0) == MMU_TRANSLATE) ? isa_mmu_translate(addr, len, 0) : addr;
	//assert(addr == bug);
  paddr_write(addr, len, data);
}
