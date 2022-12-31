#include <proc.h>
#include <common.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
# define EXPECT_TYPE EM_RISCV 
#else
# error Unsupported ISA 
#endif

#define mmin(a, b) ((a) < (b)) ? (a) : (b)
static uintptr_t loader(PCB *pcb, const char *filename) {
//magic number 7f 45 4c 46
	//检查魔数和ISA类型

//above we use ramdisk_read directly, now we use another API to loader the file


///////////////////////////////////////////
/*
	Elf_Ehdr ehdr;//elf headr table
//	size_t off = ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
	int fd = fs_open(filename, 0, 0);
	//ignore flags and mode
	size_t off = fs_read(fd, &ehdr, sizeof(Elf_Ehdr)); 
	assert(off != 0);

	//check magic number
	assert(ehdr.e_ident[0] == 0x7f && ehdr.e_ident[1] == 0x45 && ehdr.e_ident[2] == 0x4c && ehdr.e_ident[3] == 0x46);
	assert(ehdr.e_machine == EXPECT_TYPE);

	Elf_Phdr phdr[ehdr.e_phnum];
	fs_lseek(fd, ehdr.e_phoff, 0);
	fs_read(fd, phdr, sizeof(Elf_Phdr) * ehdr.e_phnum);
	//ramdisk_read(phdr, ehdr.e_phoff, sizeof(Elf_Phdr) * ehdr.e_phnum);
	for (size_t i = 0; i < ehdr.e_phnum; i++)
	{ 
		if (phdr[i].p_type == PT_LOAD)
		 {
			//judge whether it should be loaded
			//ramdisk_read((void*)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_filesz);
			fs_lseek(fd, phdr[i].p_offset, 0);
			fs_read(fd, (void*)phdr[i].p_vaddr, phdr[i].p_filesz);
			//clear[VirtAddr + FileSize, VirtAddr + MemSiz), as .bss part
			memset((uint8_t*)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
		}
		//需要及时调整open_offset的位置，因为有的内容没有读，需要跳过
	}
	return ehdr.e_entry;//Entry point virtual address

*/
//////////////////////////////
	//这个时候要用到pcb的addrspace了
	AddrSpace* as = &pcb->as;

	Elf_Ehdr ehdr;//elf headr table
	int fd = fs_open(filename, 0, 0);
	//ignore flags and mode
	size_t off = fs_read(fd, &ehdr, sizeof(Elf_Ehdr)); 
	assert(off != 0);

	//check magic number
	assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
	assert(ehdr.e_machine == EXPECT_TYPE);

	Elf_Phdr phdr[ehdr.e_phnum];
	fs_lseek(fd, ehdr.e_phoff, 0);
	fs_read(fd, phdr, sizeof(Elf_Phdr) * ehdr.e_phnum);
	//这是将头表的内容先全部读入
	for (size_t i = 0; i < ehdr.e_phnum; i++)
	{ 
		if (phdr[i].p_type == PT_LOAD)
		{
			printf("i = %d\n", i);
			//加载这一段，并看它有多少页，使用for循环map
			fs_lseek(fd, phdr[i].p_offset, 0);
			uint32_t read_vaddr = phdr[i].p_vaddr;
			uint32_t left_len = phdr[i].p_filesz;
			uint32_t read_len;
			for( ;read_vaddr < phdr[i].p_vaddr + phdr[i].p_filesz;)
			{
				void* npage = new_page(1);
				//预先将其全部设置为0
				memset(npage, 0, PGSIZE);
				if(read_vaddr != (read_vaddr & ~0xfff))
				{
					printf("not aligned\n");
					//一开始不对齐，在一页中有偏移量
					map(as, (void*)read_vaddr, npage, 1);
					read_len = mmin(PGSIZE - (read_vaddr & 0xfff), left_len);
					fs_read(fd, npage + (read_vaddr & 0xfff), read_len);
				}
				else
				{
					printf("aligned\n");
					printf("map to %x\n", npage);
					map(as, (void*)read_vaddr, npage, 1);
					read_len = mmin(PGSIZE, left_len);
					fs_read(fd, npage, read_len);
				}
				printf("read_len = %d, read_vaddr = %x\n", read_len, read_vaddr);
				left_len -= read_len;
			}
			left_len = phdr[i].p_memsz - phdr[i].p_filesz;
			for(;read_vaddr < phdr[i].p_vaddr + phdr[i].p_memsz; read_vaddr += PGSIZE)
			{
				void* npage = new_page(1);
				memset(npage, 0, PGSIZE);
				map(as, (void*)read_vaddr, npage, 1);	
			}
			//还需要把后面一段空间清0
			//以一页为单位，从文件中读入一页
			//不满一页按少于一页的读
		}
	}
	return ehdr.e_entry;//Entry point virtual address
}

void naive_uload(PCB *pcb, const char *filename) {
	printf("Start to loader file \"%s\"\n", filename);
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB* pcb, void (*entry)(void*), void* arg)
{
	//先给其分配Area空间
	//仿照其他地方，先分配一段空间
	//并且里面的变量是union，所以只用分配一个变量的空间
	Area area = RANGE(pcb, (uint8_t*)pcb + STACK_SIZE);
	pcb->cp = kcontext(area, entry, arg);
	//printf("kload: pcb->cp = %p\n", pcb->cp);
}


const int nr_page = 8;
const int nr_page_sz = 4096;
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[])
{

/*
需要在创建用户进程前为其创建地址空间
*/
	protect(&pcb->as);

	/*
	把heap.end作为用户进程的栈顶, 然后把这个栈顶赋给用户进程的栈指针寄存器
	*/
	//先得出argc的值
	/*nr_page = 8, nr_page_sz = 4096*/
	int argc = 0, envc = 0;
	if(argv)
	{
		while(argv[argc])
		{
			argc++;
		}
	}
	if(envp)
	{
		while(envp[envc])
			envc++;
	}
	//printf("in uload:\nargc = %d\n", argc);
	//printf("in uload:\nenvc = %d\n", envc);

	char* now = (char*)new_page(nr_page) + nr_page * nr_page_sz;
	for(int i = 1; i <= 8; i++)
	{
		//把用户栈的物理页映射到[as.area.end - 32KB, as.area.end)这段虚拟地址空间
		map(&pcb->as, pcb->as.area.end - i * PGSIZE, now - i * PGSIZE, 1);
	}
	//char* now = (char*)heap.end;
	char* ar[argc];
	char* en[envc];
	char* str = (char*)now - 4;
	for(int i = 0; i < argc; i++)
	{
		str = str - (strlen(argv[i]) + 1);
		strcpy(str, argv[i]);
		ar[i] = str;
	}
	for(int i = 0; i < envc; i++)
	{
		str = str - (strlen(envp[i]) + 1);
		strcpy(str, envp[i]);
		en[i] = str;
	}
	//开始分配下面的指针区域
	char** ptr = (char**)str;
	ptr--;
	*ptr = NULL;
	ptr--;
	for(int i = envc - 1; i >= 0; i--)
	{
		*ptr = en[i];
		ptr--;
	}
	*ptr = NULL;
	//不知道为啥，他不能赋值为NULL
	ptr--;
	for(int i = argc - 1; i >= 0; i--)
	{
		*ptr = ar[i];
		ptr--;
	}

	Area area = RANGE(pcb, (uint8_t*)pcb + STACK_SIZE);
	void* entry = (void*)loader(pcb, filename);
	pcb->cp = ucontext(NULL, area, entry);
	//pcb->cp->GPRx = (uintptr_t)heap.end;

	int* tmp = (int*)(ptr);
	*tmp = argc;
	pcb->cp->GPRx = (uintptr_t)(tmp);
}
