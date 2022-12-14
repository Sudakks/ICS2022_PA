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
	//会复制结尾的'\0'
	assert(dst != NULL && src != NULL);
	char *ret = dst;
	/*while((*dest++ = *src++) != '\0')
	{}*/
	while(*src != '\0')
	{
		*dst++ = *src++;
	}
	*dst = '\0';//要复制最后一个'\0'!!!
	return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
	//Do not copy the '\0' in the tail
        assert(dst!=NULL && src!=NULL);  
        char* temp = dst;  
        int i = 0;  
        while(i++ < n)
        { 
        	if(*src != '\0')
        		*temp++ = *src++;
            	else
            		*temp++ = '\0';
	}
        return dst; 
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
		if(n == 0)
			return 0;
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
	char *d = (char*) dst;;
	char *s = (char*) src;
	if(dst < src)
	{
		while(n--)
		{
			*d++ = *s++;
		}
	}
	else
	{
		while(n > 0)
		{
			*(d + n - 1) = *(s + n - 1);
			n--;
		}
	}
	return ret;
}

void *memcpy(void *out, const void *in, size_t n) {
	//不会拷贝最后的'\0'，所以在使用时拷贝的长度为strlen(in) + 1 
	printf("usemymemcpy\n");
	if(n <= 0 || out == NULL || in == NULL)
		return out;
	void *ret = out;
	char* tmp_o = (char*)out;
	char* tmp_i = (char*)in;
	int i = 0;
	while(i++ < n)
	{
		*tmp_o++ = *tmp_i++;	
	}
	//当区间出现重叠时有UB行为产生
	return ret;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	unsigned char c1, c2;
	while (n-- > 0)
	{
    		c1 = *(unsigned char*)s1++;
    		c2 = *(unsigned char*)s2++;
    		if (c1 == '\0' || c1 != c2)
        		return c1 - c2;
	}
	return 0;
}

#endif
