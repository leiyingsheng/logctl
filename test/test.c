#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(void)
{
	int fd,count=0;;
	
	fd = open("/dev/test",O_RDWR);
	
	char str[10]="my_log";
	
	while(1)
	{
		count ++;
		printf("%d\n",count);
		write(fd,str,strlen(str));
		if(count>(1000))
		{
			break;
		}
		usleep(1000);
	}
	
	close(fd);
	
	return 0;
}
