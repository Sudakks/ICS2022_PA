#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
//#include <malloc.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
#define MAX_STR_SIZE 1024 

void put_str(size_t cnt, const char* str)
{
	//putch str
	for(int i = 0; i < cnt; i++)
	{
		putch(*(i + str));
	}
}

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
	static char mystr[MAX_STR_SIZE;
	static char* cp;
	static char myc;
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
						str_cnt = sprintf(&mystr[0], "%d", num);
						ret += str_cnt;
						put_str(str_cnt, &mystr[0]);
						break;
				case 's':
						cp = va_arg(ap, char*);//它会显示char*转到char数组不行
						strcpy(mystr, cp);
						str_cnt = strlen(mystr);
						ret += str_cnt;
						put_str(str_cnt, mystr);
						break;
				case 'c':
						myc = va_arg(ap, int);
						str_cnt = 1;
						ret += 1;
						putch(myc);
						break;
				default: ;
			}
		}
		else
		{
			putch(*fmt);
		}
		fmt++;
	}
  //panic("Not implemented");
	va_end(ap);
	return ret;
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
						//printf("now = %d\n", val[len]);
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
			}
		}
		else
		{
			*out = *fmt;
			out++;
		}
		fmt++;
	}
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
