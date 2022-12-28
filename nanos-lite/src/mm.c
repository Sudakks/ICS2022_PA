#include <memory.h>

static void *pf = NULL;
const int page_sz = 4096;
void* new_page(size_t nr_page) {
	void* ret = pf;
	pf = (void*)((char*)pf + nr_page * 4096);
	return ret;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
	printf("yes  jj\n");
	size_t num = n / page_sz;
	void* ret = new_page(num);
	//对分配的页面进行清零操作
	memset(ret, 0, n);
	return ret;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  return 0;
}

void init_mm() {
	/*将TRM提供的堆区起始地址作为空闲物理页的首地址*/
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
