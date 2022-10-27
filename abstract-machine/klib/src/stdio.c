#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int mysprintf(char *out, const char *fmt, ...) {
	//先读到%
	char* tmp = out;
	va_list ap;
	va_start(ap, fmt);
	char* s;
	int num;
	long long llnum;
	int ret = 0;
	int val[20];
	int len; 
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
			  		num = va_arg(ap, long long);			  		
					len = 0;
					llnum = (long long)num;
					//似乎没有判断为负的情况 
					if(num < 0)
					{
						llnum = -llnum;	
						*tmp++ = '-';					
					}							
					while(1)
					{
						if(llnum == 0)
							break;
						val[++len] = llnum % 10;
						llnum = llnum / 10;
					}					
					if(len == 0)
						val[++len] = 0; //相当于特判了是0的情况 
									
					for(int i = len; i >= 1; i--)
					{
						*tmp = val[i] + '0';
						tmp++;
					}
					//自己写一个int转为char*的函数？
					ret += len;
					break;
				case 's':
					s = va_arg(ap, char*);//因为写string没有高亮，，，
					//这里！要主动给s后面添加一个'\0'!
					memcpy(tmp, s, strlen(s));
					tmp += strlen(s);
					ret += strlen(s);
					break;
			}
		}
		else
		{
			*tmp = *fmt;
			tmp++;
		}
		fmt++;
	}
	va_end(ap);
	*tmp = '\0';
	return ret;
}


int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
