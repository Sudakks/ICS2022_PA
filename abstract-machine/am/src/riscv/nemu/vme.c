#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
	//printf("run into set_satp\n");
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
	//printf("run into get_satp\n");
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }
  set_satp(kas.ptr);
  vme_enable = 1;
  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
	//ignore prot temporarily
	uint32_t _va = (uint32_t)va;
	uint32_t _pa = (uint32_t)pa;
	uint32_t VPN1 = _va >> 22;
	uint32_t VPN0 = (_va >> 12) & 0x3ff;
	//要经过两层转换
	//取出的是地址
	uint32_t** base = (uint32_t**)as->ptr;
	if(base[VPN1] == NULL)
	{
		uint32_t* npage = (uint32_t*)pgalloc_usr(PGSIZE);
		base[VPN1] = (uint32_t*)((uint32_t)npage | 0x1);//set valid
		/*
		在nemu里面要检查valid项，所以在这里就要预先设定好valid，使它为1
		！！！很重要！！！
		*/
		//base[VPN1] = (uint32_t*)pgalloc_usr(PGSIZE);
		//printf("alloc = %p\n", base[VPN1]);
	}
	/*
	嗯最后是通过一个具体的地址，一个一个比对，才发现translate错误了
	*/
	uint32_t* page_table = base[VPN1];
	//if(_va == 0x80001000)
		//printf("first = %x\n", page_table);
	page_table[VPN0] = (_pa & 0xfffff000) | 0x1;
	//if(_va == 0x80001000)
		//printf("2 addr = %x\n", &page_table[VPN0]);
		/*
	if(_va == 0x80001000)
	{
		printf("yinshe to %x\n", _pa & 0xfffff000);
	}
	*/
	//然后把这个位置映射到pa上
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
	//no arg
	Context* con = (Context*)(kstack.end - sizeof(Context));
	con->mepc = (uintptr_t)entry;
	con->pdir = as->ptr;
	/*在创建的用户进程上下文中设置地址空间描述符指针*/
  return con;
}
