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

int sprintf(char *out, const char *fmt, ...) {
	//先读到%
	va_list ap;
	va_start(ap, fmt);
	char* s;
	int num;
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
			  	num = va_arg(ap, int);
					len = 0;
					while(1)
					{
						if(num == 0)
							break;
						len++;
						val[len] = num % 10;
						num = num / 10;
					}
					for(int i = len; i >= 1; i--)
					{
						*out++ = val[i] + '0';
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
	return ret;
  //panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
