#ifndef __COMMON_H__
#define __COMMON_H__

/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE/*PA3.1*/
//#define HAS_VME
//#define MULTIPROGRAM
//#define TIME_SHARING

#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <debug.h>

enum my_lseek{
	seek_set = 0, seek_cur, seek_end, seek_add
	//seek_add是我自己加的，即在这个文件的最初偏移量上加一个值
};
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);
#endif
