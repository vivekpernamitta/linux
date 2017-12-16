#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>




int main()

{
	int fd, i;
	ssize_t ret;
	char mybuf[4000];

	// filling buffers with *'s
	for(i=0;i<4000;i++)
	{
		mybuf[i] = '*';
	}

	/* open the device for  write*/

	printf("%d - Opening device vivek_dev\n", getpid());

	fd=open("/dev/vivek_test", O_RDWR);
	if(fd<=0)
	{
		perror("ERROR in file open");
		exit(0);
	}
	printf(" PID = %d\n", getpid());
	getchar(); 

	for(i=0;i<4;i++){
		
		ret = write( fd, "__vivek__", 12);
		sleep(2);
		ret = write( fd, mybuf, 4000);
	}
	close (fd);

}
