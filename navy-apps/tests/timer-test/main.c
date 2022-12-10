#include <assert.h>
#include <stdio.h>
int main()
{
	printf("here in\n");
	struct timeval now_time;
	int before = 0;
	while(1)
	{
		gettimeofday(now_time, NULL);
		if(now_time.tv_sec - before == 0.5)
		{
			before = now_time.tv_sec;
			printf("Time: %lds\n", now_time.tv_sec);
		}
	}
	return 0;
}
