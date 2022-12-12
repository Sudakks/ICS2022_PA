#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <NDL.h>

int main()
{
	/*struct timeval now_time;
	while(1)
	{
		gettimeofday(&now_time, NULL);
		printf("%ld - %ld\n", now_time.tv_usec % 500000, now_time.tv_usec);
		if(now_time.tv_usec % 500000 == 0)
		{
			printf("Time: %lds\n", now_time.tv_sec);
		}
	}*/
	NDL_Init(1);
	uint32_t sec = 0;
	while (1) {
    		while(NDL_GetTicks() / 1000 < sec);
    		if (sec == 1) {
      		printf("%d second).\n", sec);
    		} else {
      		printf("%d seconds).\n", sec);
    		}
    		sec ++;
  	}
	NDL_Quit();
	return 0;
}
