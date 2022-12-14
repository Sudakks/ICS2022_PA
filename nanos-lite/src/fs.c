#include <fs.h>
#include <common.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
	size_t open_offset;
} Finfo;


//the file descriptor of stdout is 1
enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_KBD, FD_DISPINFO};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

//相应报错函数
size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
	[FD_FB]     = {" ", 0, 0},
	[FD_KBD]    = {"/dev/events", 0, 0, events_read, invalid_write},
	[FD_DISPINFO] = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
	
#include "files.h"
};

//indicate the number of file_table
//#define MAX_FILE_SZ 50
//size_t open_offset[MAX_FILE_SZ];
//advanced every time the file is read

void init_fs() {
  // TODO: initialize the size of /dev/fb
}


int fs_open(const char *pathname, int flags, int mode)
{
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	//find the same filename in file_table
	for(int i = 0; i < file_table_sz; i++)
	{
		char* tmp = file_table[i].name;
		if(strcmp(tmp, pathname) == 0)
		{
			//equal and find
			file_table[i].open_offset = 0;
			printf("name = %s\n", pathname);
			return i;
		}
	}
	assert(0);
	//if can't find the file, assert
}


size_t fs_read(int fd, void *buf, size_t len)
{
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	assert(fd < file_table_sz);


	if(file_table[fd].read != NULL)
	{
		size_t ret = file_table[fd].read(buf, file_table[fd].open_offset, len);
		file_table[fd].open_offset += ret;
		return ret;
	}

	Finfo info = file_table[fd];	
	size_t sz = info.size;
	size_t disoff = info.disk_offset;
	size_t off = info.open_offset;
	//read from this fd's open_offset
	size_t ret = (len + off > sz) ? sz - off : len;
	if(ret < 0)
		return -1;
	printf("fs_read: len = %d, ret = %d\n", len, ret);
	size_t read_sz = ramdisk_read(buf, disoff + off, ret);
	file_table[fd].open_offset += read_sz;
	assert(read_sz == ret);
	//这个是相对于这个文件头的偏移量
	//advanced
	return read_sz;
}


int fs_close(int fd)
{
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	assert(fd < file_table_sz);
	file_table[fd].open_offset = 0;
	//indicate the number of file_table
	//the sfs doesn't maintain the status of openning file, return 0 indicate always close successfully
	return 0;
}

size_t fs_lseek(int fd, size_t offset, int whence)
{
	/*
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	//adjust fd's open_offset 
	assert(fd < file_table_sz);
	Finfo info = file_table[fd];	
	size_t sz = info.size;

	switch(whence)
	{
		case SEEK_SET:
			offset = (offset > sz) ? sz : offset;
			file_table[fd].open_offset = offset;
			break;
		case SEEK_CUR:
			file_table[fd].open_offset = (file_table[fd].open_offset + offset > sz) ? sz : file_table[fd].open_offset + offset;
			break;
		case SEEK_END:
			file_table[fd].open_offset = (sz + offset < 0) ? 0 : sz + offset;
			
			break;
		default:
			assert(0);
	}
	*/
	size_t curpos = file_table[fd].open_offset;
	size_t filesize = file_table[fd].size;
	switch (whence) {                                
		case SEEK_END: curpos = filesize; break;
		case SEEK_SET: curpos = offset; break;
		case SEEK_CUR: curpos += offset; break;
		}
		if (curpos < 0) curpos = 0;
		else if (curpos > filesize) curpos = filesize;
		file_table[fd].open_offset = curpos;

		return file_table[fd].open_offset;
}

size_t fs_write(int fd, const void *buf, size_t len)
{
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	assert(fd < file_table_sz);

	if(file_table[fd].write != NULL)
	{
		size_t ret = file_table[fd].write(buf, file_table[fd].open_offset, len);
		file_table[fd].open_offset += ret;
		return ret;
	}

	Finfo info = file_table[fd];	
	size_t sz = info.size;
	size_t disoff = info.disk_offset;
	size_t off = info.open_offset;
	size_t ret = (len + off > sz) ? sz - off : len;

	if(ret < 0)
		return -1;
	size_t write_sz = ramdisk_write(buf, disoff + off, ret);
	assert(write_sz == ret);
	file_table[fd].open_offset += write_sz;
	return write_sz;

}
