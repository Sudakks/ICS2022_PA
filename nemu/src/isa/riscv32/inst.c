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

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>

#define R(i) gpr(i)
#define Mr vaddr_read
#define Mw vaddr_write

enum {
  TYPE_I, TYPE_U, TYPE_S,
  TYPE_N, // none
	TYPE_JAL, TYPE_R, TYPE_B,
};

#define src1R() do { *src1 = R(rs1); } while (0)
#define src2R() do { *src2 = R(rs2); } while (0)
#define immI() do { *imm = SEXT(BITS(i, 31, 20), 12); } while(0)
#define immU() do { *imm = SEXT(BITS(i, 31, 12), 20) << 12; } while(0)
#define immS() do { *imm = (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7); } while(0)
//ATTENTION: 先对立即数左移2位，再立即数拓展!!!
#define immJAL() do { *imm = (SEXT(BITS(i, 31, 31), 1) << 19 | SEXT(BITS(i, 19, 12), 8) << 11 | SEXT(BITS(i, 20, 20), 1) << 8 | BITS(i, 30, 21)) << 1; } while(0)
#define immB() do { *imm = (SEXT(BITS(i, 31, 31), 1) << 11 | BITS(i, 7, 7) << 10 | SEXT(BITS(i, 30, 25), 6) << 4 | BITS(i, 11, 8)) << 1; } while(0)

//读出操作数
static void decode_operand(Decode *s, int *dest, word_t *src1, word_t *src2, word_t *imm, int type) {
  uint32_t i = s->isa.inst.val;
  int rd  = BITS(i, 11, 7);
  int rs1 = BITS(i, 19, 15);
  int rs2 = BITS(i, 24, 20);
  *dest = rd;
  switch (type) {
    case TYPE_I: src1R();          immI(); break;
    case TYPE_U:                   immU(); break;
    case TYPE_S: src1R(); src2R(); immS(); break;
		case TYPE_JAL:								 immJAL(); break;
		case TYPE_R: src1R(); src2R();       ; break;
		case TYPE_B: src1R(); src2R(); immB(); break;
  }
}

static int decode_exec(Decode *s) {
  int dest = 0;
  word_t src1 = 0, src2 = 0, imm = 0;
  s->dnpc = s->snpc;

#define INSTPAT_INST(s) ((s)->isa.inst.val)
#define INSTPAT_MATCH(s, name, type, ... /* execute body */ ) { \
  decode_operand(s, &dest, &src1, &src2, &imm, concat(TYPE_, type)); \
  __VA_ARGS__ ; \
}

  INSTPAT_START();
  INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui    , U, R(dest) = imm);
  INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw     , I, R(dest) = Mr(src1 + imm, 4));
  INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw     , S, Mw(src1 + imm, 4, src2));

	//add upper immediate to pc
	INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc  , U, R(dest) = s->pc + imm);//





	//load immediate
	INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi   , I, R(dest) = src1 + imm);//
	//jalr
	INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr   , I, R(dest) = s->pc + 4, s->dnpc = (imm + src1) & ~1);
	//sltiu set less than immediate unsigned
	//seqz伪命令，即imm = 1
	INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu  , I, (src1 < imm) ? (R(dest) = 1) : (R(dest) = 0));
	INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi   , I, R(dest) = imm & src1);
	INSTPAT("0000000 ????? ????? 001 ????? 00100 11", slli   , I, R(dest) = src1 << imm, printf("imm = %x, src1 = %x\n", imm, src1));


	//jump
	INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal    , JAL, R(dest) = s->pc + 4, s->dnpc = s->pc + imm);//

	//
	//mv 伪指令，相当于addi的立即数是0 
	//INSTPAT("0000000 00000 ????? 000 ????? 00100 11", mv     , I, R(dest) = R(src1) + imm);//

	//j伪指令 jal, rd = x0
	//INSTPAT("??????? ????? ????? ??? 00000 11011 11", j      , JAL, R(dest) = s->pc + 4, s->dnpc = s->pc + imm);


	//add
	INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add    , R, R(dest) = src1 + src2);
	//sub
	INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub    , R, R(dest) = src1 - src2);
	INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem    , R, R(dest) = src1 % src2);
//	INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul    , R, R(dest) = ((uint64_t)src1 * (uint64_t)src2), printf("src1 = %lx, src2 = %lx\n",(uint64_t) src1, (uint64_t)src2));
//	INSTPAT("0000001 ????? ????? 001 ????? 01100 11", mulh   , R, R(dest) = ((uint64_t)src1 * (uint64_t)src2) >> 32);
	INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor    , R, R(dest) = src1 ^ src2);
	INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or     , R, R(dest) = src1 | src2);
	INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu   , R, (src1 < src2) ? (R(dest) = 1) : (R(dest) = 0));




	//beq branch equal
	//beqz伪指令 branch equal to zero,src2 == 0
	INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq     , B,
		if(src1 == src2)	s->dnpc = s->pc + imm
	);
	//bne branch if not equal
	INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne      , B,
		if(src1 != src2)	s->dnpc = s->pc + imm
	);
	//blt branch is less than
	INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt      , B,
		if(src1 < src2)	s->dnpc = s->pc + imm
	);
	//条件分支！
	//P156 Instruction Set


	INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh        , S, Mw(src1 + imm, 2, src2));

  INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak , N, NEMUTRAP(s->pc, R(10))); // R(10) is $a0
  INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv    , N, INV(s->pc));
  INSTPAT_END();

  R(0) = 0; // reset $zero to 0

  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst.val = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}
