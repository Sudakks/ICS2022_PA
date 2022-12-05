#include <proc.h>
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


extern size_t ramdisk_read(void* buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
//magic number 7f 45 4c 46
	//检查魔数和ISA类型

	Elf_Ehdr ehdr;//elf headr table
	size_t off = ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
	assert(off != 0);

	//check magic number
	assert(ehdr.e_ident[0] == 0x7f && ehdr.e_ident[1] == 0x45 && ehdr.e_ident[2] == 0x4c && ehdr.e_ident[3] == 0x46);
	assert(ehdr.e_machine == EXPECT_TYPE);

	Elf_Phdr phdr[ehdr.e_phnum];
	ramdisk_read(phdr, ehdr.e_phoff, sizeof(Elf_Phdr) * ehdr.e_phnum);
	for (size_t i = 0; i < ehdr.e_phnum; i++)
	{
		if (phdr[i].p_type == PT_LOAD)
		{
			//judge whether it should be loaded
			ramdisk_read((void*)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
			//clear[VirtAddr + FileSize, VirtAddr + MemSiz), as .bss part
			memset((void*)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
		}
	}
	printf("the address = %p\n", ehdr.e_entry);
	return ehdr.e_entry;//Entry point virtual address
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

