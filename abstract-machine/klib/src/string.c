#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	if(*s == '\0')
		return 0;
	return (1 + strlen(++s));
}

char *strcpy(char *dst, const char *src) {
	assert(dst != NULL && src != NULL);
	char *ret = dst;
	while(*src != '\0')
	{
		*dst++ = *src++;
	}
	*dst = *src;//要复制最后一个'\0'!!!
	return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
	assert(dst != NULL && src != NULL);
	char *ret = dst;
	while(n--)
	{
		if(*src != '\0')
		{
			*dst++ = *src++;
		}
		else
		{
			*dst++ = '\0';
		}
	}
	return ret;
}

char *strcat(char *dst, const char *src) {
	char *ret = dst;
	while(*dst != '\0')
		dst++;
	while(*src != '\0')
	{ 
		*dst++ = *src++;
	}
	*dst = *src;//将最后一个'\0'也拷贝过去
	return ret;
}

int strcmp(const char *s1, const char *s2) {
	assert(s1 != NULL && s2 != NULL);
	/*
	while(*s1 && *(unsigned char*)s1 == *(unsigned char*)s2)
	{ 
		s1++;
		s2++;
	}
	return *s1 - *s2;
	*/
	while(*(unsigned char*)s1 == *((unsigned char*)s2))
	{
		if(*s1 == '\0')
			return 0;
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	if(n <= 0)
		return 0;
	while(n-- && *s1 != '\0' && *(unsigned char*)s1 == *(unsigned char*)s2)
	{
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

void *memset(void *s, int c, size_t n) {
	void *ret = s;
	if(n <= 0 || s == NULL)
		return ret;
	char* ss = (char*)s;
	while(n--)
	{
		*ss++ = c;
	}
	return ret;
}

void *memmove(void *dst, const void *src, size_t n) {
	//复制的地方重合不影响原先的值
	void *ret = dst;
	if(n == 0)
		return ret;
	char* d = (char*)dst;
	char* s = (char*)src;
	char* temp = (char*)malloc(n);
	for(int i = 0; i < n; i++)
		temp[i] = s[i];
	for(int i = 0; i < n; i++)
		d[i] = temp[i];
	free(temp);
	return ret;
	/*if(src >  dst)
	{//这里说明不会重叠
		while(n--)//遇到'\0'不会停下来
		{
			*d++ = *s++;
		}
	}
	else
	{//会重叠，从后往前进行交换
		d = d + n;
		s = s + n;
		while(n--)
		{
			//这个要先强制转化成类型char，才能继续加值
			*d-- = *s--;
		}
	}
	return ret;*/
}

void *memcpy(void *out, const void *in, size_t n) {
	if(n <= 0 || out == NULL || in == NULL)
		return out;
	void *ret = out;
	char* o = (char*)out;
	char* i = (char*)in;
	/*while(n--)
	{
		*o++ = *i++;
	}
	return ret;*/
	//判断是向后拷贝，还是向前拷贝
	if(o < i)
	{
		while(n--)
			*o++ = *i++;
	}
	else
	{
		o = o + n;
		i = i + n;
		while(n--)
		{
			*o-- = *i--;
		}
	}
	return ret;
}

int memcmp(const void *s1, const void *s2, size_t n) {
/*	char* ss1 = (char*) s1;
	char* ss2 = (char*) s2;
	return strncmp(ss1, ss2, n);*/
	unsigned char c1, c2;
	while (n > 0)
	{
    c1 = *(unsigned char*)s1++;
    c2 = *(unsigned char*)s2++;
    if (c1 == '\0' || c1 != c2)
        return c1 - c2;
    n--;
	}
	return 0;
}

#endif
