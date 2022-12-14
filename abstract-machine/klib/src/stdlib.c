#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;
char* hbrk;
bool init_orn = false;
struct mal_block
{
	void* start;
	void* end;
	void* now;
}my_block;
int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

/*already exits atoi function*/
int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}


void *malloc(size_t size) {
  // On native, malloc() will be called during initializaion of C runtime.
  // Therefore do not call panic() here, else it will yield a dead recursion:
  //   panic() -> putchar() -> (glibc) -> malloc() -> panic()
	printf("here\n");
#if !(defined(__ISA_NATIVE__) && defined(__NATIVE_USE_KLIB__))
	printf("in\n");
	if(init_orn == false)
	{
		init_orn = true;
		hbrk = (void*) ROUNDUP(heap.start, 8);
	}
	size = (size_t)ROUNDUP(size, 8);
	char* old = hbrk;
	hbrk += size;
	assert((uintptr_t)heap.start <= (uintptr_t)hbrk && (uintptr_t)hbrk < (uintptr_t)heap.end);
	for(uint64_t *p = (uint64_t*)old; p != (uint64_t*)hbrk; p++)
		*p = 0;
	return old;
#endif
  return NULL;
}

void free(void *ptr) {
}

#endif
