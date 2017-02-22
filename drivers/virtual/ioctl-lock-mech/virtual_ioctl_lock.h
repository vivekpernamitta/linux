


/*
Example -2 IOCTL function
*/
#ifndef 	_VIVEK_CHAR_DEVICE
#define 	_VIVEK_CHAR_DEVICE

#include <linux/ioctl.h>


struct my_data
{
	int i;
	long x;
	char s[256];

};

#define		VIVEK_MAGIC	'L'
#endif	
