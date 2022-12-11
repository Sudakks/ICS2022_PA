#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
int main()
{
	//printf("here in\n");
	struct timeval now_time;
	//long before = 0;
	while(1)
	{
		gettimeofday(&now_time, NULL);
		printf("%ld\n", now_time.tv_usec);
		/*if(now_time.tv_usec - before == 500)
		{
			before = now_time.tv_sec;
			printf("Time: %lds\n", now_time.tv_sec);
		}*/
	}
	return 0;
}
