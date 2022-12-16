#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
#define MAX_STR_SIZE 100000

void put_str(size_t cnt, const char* str)
{
	//putch str
	for(int i = 0; i < cnt; i++)
	{
		putch(*(i + str));
		
	}
}

/*int printNum(unsigned int num, int base, char* out)
{
		int val[67];
		int len = 0;
		while(1)
		{
				if(num == 0)
					break;
				len++;
				val[len] = num % base;
				num = num / base;
		}					
		if(len == 0)
				val[++len] = 0; //相当于特判了是0的情况 
									
		for(int i = len; i >= 1; i--)
		{
				*out = val[i] + '0';
				out++;
		}
		return len;
}*/

int printf(const char *fmt, ...) {
	/*
	实现：
	利用sprintf将数据读到一个out里面，利用putch函数输出
	*/
	va_list ap;
	va_start(ap, fmt);
	int num;
	int str_cnt = 0;
	int ret = 0;
	static char* mystr;
	static char mynum[67];
	static char myc;
	unsigned int myp;
	unsigned int myu;
	while(*fmt != '\0')
	{ 
		if(*fmt == '%')
		{
			fmt++;
			//说明有要读到的格式了
			switch(*fmt)
			{
				case 'd':
				//读数字
			  		num = va_arg(ap, int);			  		
						str_cnt = sprintf(mynum, "%d", num);
						putstr(mynum);
						ret += str_cnt;
						//put_str(str_cnt, mynum);
						break;
				case 's':
						mystr = va_arg(ap, char*);//它会显示char*转到char数组不行
						str_cnt = strlen(mystr);
						putstr(mystr);
						//put_str(str_cnt, mystr);
						ret += str_cnt;
						break;
				case 'c':
						myc = va_arg(ap, int);
						str_cnt = 1;
						ret += 1;
						putch(myc);
						break;
				case 'p':
						myp = va_arg(ap, unsigned int);
						putch('0');
						putch('x');
						str_cnt = sprintf(mynum, "%p", myp);
						putstr(mynum);
						//put_str(str_cnt, mynum);
						ret += str_cnt;
						//相当于打印16进制地址
						break;			
				case 'u':
						myu = va_arg(ap, unsigned int);
						str_cnt = sprintf(mynum, "%u", myu);
						putstr(mynum);
						//put_str(str_cnt, mynum);
						ret += str_cnt;
						break;
				case 'x':
						myu = va_arg(ap, unsigned int);
						str_cnt = sprintf(mynum, "%p", myu);
						putstr(mynum);
						ret += str_cnt;
						break;
				default:
						panic("Not implemented in printf");
						assert(0);
			}
		}
		else
		{
			putch(*fmt);
		}
		fmt++;
	}
	va_end(ap);
	return ret;
  //panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char* s;
	char c;
	int num;
	int ret = 0;
	int val[67];
	int len; 
	bool neg = false; 
	unsigned int unum;
	while(*fmt != '\0')
	{
		if(*fmt == '%')
		{
			//说明有要读到的格式了
			fmt++;
			switch(*fmt)
			{
				case 'd':
				//读数字
			  	num = va_arg(ap, int);			  		
					len = 0;
					//似乎没有判断为负的情况 
					if(num < 0)
					{
						neg = true;	
						*out++ = '-';					
					}							
					while(1)
					{
						if(num == 0)
							break;
						len++;
						val[len] = num % 10;
						num = num / 10;
						if(neg == true)
							val[len] = -val[len];						
					}					
					if(len == 0)
						val[++len] = 0; //相当于特判了是0的情况 
									
					for(int i = len; i >= 1; i--)
					{
						*out = val[i] + '0';
						out++;
					}
					//int2string
					ret += len;
					break;
				case 's':
					s = va_arg(ap, char*);//因为写string没有高亮，，，
					//这里！要主动给s后面添加一个'\0'!
					memcpy(out, s, strlen(s));
					out += strlen(s);
					ret += strlen(s);
					break;
				case 'c':
					c = va_arg(ap, int);
					*out = c;
					out++;
					ret += 1;
					break;
				case 'u':
					unum = va_arg(ap, unsigned int);
					//len = printNum(unum, 10, out);
					//ret += len;
					len = 0;
					while(1)
					{
						if(unum == 0)
							break;
						len++;
						val[len] = unum % 10;
						unum = unum / 10;
					}					
					if(len == 0)
						val[++len] = 0; //相当于特判了是0的情况 					
					for(int i = len; i >= 1; i--)
					{
						*out = val[i] + '0';
						out++;
					}
					ret += len;
					break;
				case 'p':
				//地址是16进制显示
					unum = va_arg(ap, unsigned int);
					//len = printNum(unum, 16, out);
					//ret += len;
					len = 0;
					while(1)
					{
						if(unum == 0)
							break;
						len++;
						val[len] = unum % 16;
						unum = unum / 16;
					}					
					if(len == 0)
						val[++len] = 0; //相当于特判了是0的情况 					
					for(int i = len; i >= 1; i--)
					{
						if(val[i] < 10)
						{
							*out = val[i] + '0';
						}
						else
						{
							*out = 'a' + val[i] - 10;
						}					
						//omg我好傻，我之前写了两次输出。。。
						//难怪觉得不对头的感觉
						out++;
						
					}
					ret += len;
					break;
				default:
					panic("Not implemented in sprintf");
			}
		}
		else
		{
			*out = *fmt;
			out++;
		}
		fmt++;
	}
	printf("end snprintf\n");
	va_end(ap);
	*out = '\0';
	return ret;
}


int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
