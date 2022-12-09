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
} Finfo;


//the file descriptor of stdout is 1
enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

//indicate the number of file_table
#define MAX_FILE_SZ 25
size_t open_offset[MAX_FILE_SZ];
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
			open_offset[i] = file_table[i].disk_offset;
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
	//Finfo info = file_table[fd];	
	//size_t sz = info.size;
	//size_t disoff = info.disk_offset;
	//read from this fd's open_offset
	/*if(len + open_offset[fd] > disoff + sz)
	{
		//out of range
		len = disoff + sz - open_offset[fd];
	}

	if(len <= 0)
		return 0;*/
	size_t read_sz = ramdisk_read(buf, open_offset[fd], len);
	open_offset[fd] += read_sz;
	//advanced

	return read_sz;
}


int fs_close(int fd)
{
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	assert(fd < file_table_sz);
	//indicate the number of file_table
	//the sfs doesn't maintain the status of openning file, return 0 indicate always close successfully
	return 0;
}

size_t fs_lseek(int fd, size_t offset, int whence)
{
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	//adjust fd's open_offset 
	assert(fd < file_table_sz);
	Finfo info = file_table[fd];	
	size_t sz = info.size;
	size_t disoff = info.disk_offset;

	switch(whence)
	{
		case 0:
			open_offset[fd] = offset;
			break;
		case 1:
			if(offset + open_offset[fd] >= sz + disoff)
			{
				open_offset[fd] = sz + disoff - 1;
			}
			else
			{
				open_offset[fd] += offset;
			}
			break;
		case 2:
			open_offset[fd] = sz + disoff - 1;
			break;
		default:
			assert(0);
	}
	return open_offset[fd];
}

//extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t fs_write(int fd, const void *buf, size_t len)
{
	int file_table_sz = sizeof(file_table) / sizeof(Finfo);
	assert(fd < file_table_sz);
	Finfo info = file_table[fd];	
	size_t sz = info.size;
	size_t disoff = info.disk_offset;

	if(open_offset[fd] + len > sz + disoff)
	{
		len = sz + disoff - open_offset[fd];
	}
	if(len <= 0)
		return 0;
	size_t ret = ramdisk_write(buf, open_offset[fd], len);
	return ret;
}
