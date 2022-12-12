#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
int main()
{
	struct timeval now_time;
	long before = 0;
	while(1)
	{
		gettimeofday(&now_time, NULL);
		//printf("usec = %ld\n", now_time.tv_usec);
		printf("sec = %ld\n", now_time.tv_sec);
		if(now_time.tv_sec - before == 1)
		{
			before = now_time.tv_sec;
			printf("Time: %lds\n", now_time.tv_sec);
		}
	}
	return 0;
}
