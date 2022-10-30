#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
	/*
	实现：
	利用sprintf将数据读到一个out里面，利用putch函数输出
	*/
	char out[100];
	sprintf(out, fmt, ...);
	i==;
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char* s;
	int num;
	int ret = 0;
	int val[20];
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
						//printf("out = %c\n", *out);
						out++;
					}
					//自己写一个int转为char*的函数？
					ret += len;
					break;
				case 's':
					s = va_arg(ap, char*);//因为写string没有高亮，，，
					//这里！要主动给s后面添加一个'\0'!
					memcpy(out, s, strlen(s));
					out += strlen(s);
					ret += strlen(s);
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
