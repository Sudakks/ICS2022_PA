#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
int main()
{
	struct timeval now_time;
	while(1)
	{
		gettimeofday(&now_time, NULL);
		printf("%ld - %ld\n", now_time.tv_usec % 500000, now_time.tv_usec);
		if(now_time.tv_usec % 500000 == 0)
		{
			printf("Time: %lds\n", now_time.tv_sec);
		}
	}
	return 0;
}
