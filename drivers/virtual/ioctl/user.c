
/*
   simple user procram to r/w using ioctl
 */


#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<errno.h>
#include "virtual_ioctl.h"




int main()
{
	int fd, i;
	ssize_t ret=0,r_ret=0;
	char mybuf[4000];
	char r_buf[4000];
	unsigned int size;

	// filling buffers with *'s
	for(i=0;i<4000;i++)
	{
		mybuf[i] = '*';
	}

	/* open the device for  write*/

	printf("%d - Opening device \n", getpid());

	fd=open("/dev/vivek_iocto_test", O_RDWR);
	if(fd<=0)
	{
		perror("ERROR in file open");
		exit(0);
	}
	printf(" PID = %d\n", getpid());
	getchar(); 

	ret = write( fd, mybuf, 4000);
	lseek(fd, 0, SEEK_SET);
	r_ret = read(fd, r_buf, 4000);
	printf("the data read is = %s", r_buf);


	//lseek(fd,0 ,SEEK_SET); 	
	r_ret = ioctl(fd, VIVEK_GET_SIZE,&size);
	if (r_ret <0)
		perror(" ioctl failed");
	printf( "the size of buffer= %u \n",size);

	r_ret = ioctl(fd, VIVEK_FILL_ZERO);
	lseek(fd,0 ,SEEK_SET); 	
	r_ret = read(fd, r_buf, 4000);
	if(r_ret <=0)
	{
		perror("data failed to read");
	}	
//	r_buf[r_ret]='\0';
	printf("the data read count=%d \n data= %s\n", r_ret, r_buf );
	sleep (1);

	printf("\n printing IOCTL numbers\n VIVEK_FILL_ZERO = %u\t VIVEK_GET_SIZE = %u\n",VIVEK_FILL_ZERO,VIVEK_GET_SIZE);
	close (fd);

}
