
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
//#include "virtual_ioctl.h"
#include "virtual_ioctl_lock.h"


#define		DEVICE_NAME	"/dev/vivek_virtual_test"

struct my_data	my_data;


int main(int argc, int *argv[])
{
	int fd, i;
	ssize_t ret=0,r_ret=0;
	char mybuf[4000];
	char r_buf[4000];
	unsigned int size;
	int IOCTL_NO;
	// filling buffers with *'s
	for(i=0;i<4000;i++)
	{
		mybuf[i] = '*';
	}

	/* open the device for  write*/

	printf("%d - Opening device \n", getpid());

	fd=open(DEVICE_NAME, O_RDWR);
	if(fd<=0)
	{
		perror("ERROR in file open");
		exit(0);
	}
	printf(" PID = %d\n", getpid());
	getchar(); 

	/* retrivig the actual/initial values*/
	IOCTL_NO = (int) _IOR (VIVEK_MAGIC, 1, struct my_data);
	
	r_ret = ioctl(fd, IOCTL_NO, &my_data );
	if (r_ret <0)
		perror(" ioctl failed");
	printf( "retrun count of ioctl read= %d \n\n",r_ret);

	/* Fill the data structue*/
	strcpy (my_data.s, "A  Sting from user space");
	my_data.i = 10;
	my_data.x = -700;

	printf(" Sending from user space");
	printf( " my_data.i = %d\n	my_data.x = %ld\n	my_data.s = %s\n",
			my_data.i, my_data.x, my_data.s);


	/* send to device*/
	IOCTL_NO = (int) _IOW(VIVEK_MAGIC, 1, struct my_data);
	r_ret = ioctl (fd, IOCTL_NO, &my_data);
	printf( "\n Return count from ioctl = %d \n\n", r_ret);


	/* re-read device and see if it got through */
	IOCTL_NO = (int) _IOR (VIVEK_MAGIC, 1, struct my_data);
	r_ret = ioctl (fd, IOCTL_NO, &my_data);
	printf( "\n Return count from ioctl = %d \n\n", r_ret);
	printf( " my_data.i = %d\n	my_data.x = %ld\n	my_data.s = %s\n",
			my_data.i, my_data.x, my_data.s);

	close (fd);

}
