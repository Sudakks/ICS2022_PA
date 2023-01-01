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
#include <memory/vaddr.h>

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
	// 对内存区间为[vaddr, vaddr + len), 类型为type的内存访问进行地址转换
	//Log("translate: %x\n", vaddr);
	vaddr_t VPN1 = ((vaddr >> 22) & 0x3ff);
	vaddr_t VPN0 = ((vaddr >> 12) & 0x3ff);
	//printf("VPN1 = %x, VPN0 = %x\n", VPN1, VPN0);
	vaddr_t offset = vaddr & 0xfff;

	//first level
	vaddr_t PPN = cpu.satp & 0x3fffff;
	vaddr_t pte_addr = (PPN << 12) + (VPN1 << 2);
	uint32_t pte = paddr_read(pte_addr, 4);//这里是读出目录特定位置的内容
	if(pte == 0)
	{
		printf("addr = %x\n", vaddr);
		printf("first = %x\n", pte);
	}
	assert(pte != 0 && (pte & 0x1));
	/*
	assert是有必要的，检验map是否正确，这个是要自己设置valid位的
	*/
	//说明已经有过映射了！
	//valid位有效

	//second level
	//PPN = pte & 0x3fffff;
	PPN = pte;
	vaddr_t leaf_addr = PPN + (VPN0 << 2); 
	//printf("2 addr = %x\n", leaf_addr);
	//printf("leaf_addr = %x\n", leaf_addr);
	uint32_t leaf = paddr_read(leaf_addr, 4);
	assert(leaf != 0 && (leaf & 0x1));
	vaddr_t ret = (leaf & 0xfffff000) | offset;
	//printf("yinshe to %x\n", ret);

	if(type == VME_WRITE)
	{
		//表示这个位置被写过
		//set dirty
		paddr_write(leaf_addr, 4, leaf | 0x80);
	}
	else if(type == VME_READ)
	{
		//set access
		paddr_write(leaf_addr, 4, leaf | 0x40);
	}
	return ret;
}
