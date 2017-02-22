


/*
**********************************************************************************************************
	AUTHOR: VIVEK
	DESCRIPTION:	Simple virual char driver that has ioctl functions
        
	ioctl constants are defined in this header file for out
        "ioctl_virtual_r_w" driver. 

        This file explains how to constants are defined with
        the use of inline comments

        Constants have to be globally unique. Though this is not
        compulasarly, it is necessary. This is because of simple
        reason: we do not want our commands to clash with the
        commands in other drivers. We don't want right command
        to go to wrong driver or vice-versa
*********************************************************************************************************i
*/





#ifndef         _VIVEK_CHAR_DEVICE_H_
#define         _VIVEK_CHAR_DEVICE_H_

#include <asm/ioctl.h>



/*
	structure our constants based on majic number
*/
#define		 VIVEK_MAGIC	0x34


/* define constants require us define us to decide four things
	i) 	type or magic nO (type)
	ii)	Sequence number which is length eight buits wide. 
		this means we can decide upto 256 ioctl commands (nr)
	iii)	Direction wether reading/writing
	iv)	size it is size of user data involved
	

	To arrive at unique number easily, we use the following macros

		_IO(type, nr)
		_TOW( type, nr, dataitem)  The fouth item can be calculated sizeof
		_IOR(type, nr, dataitem)
		_IOWR(type, nr, dataitem)
 

*/

/*
	VIVEK_FILL_ZERO: To fill our charactor array with char.
	As we can see there is no dtae transfer. The movement
	driver gets this command. It executes the folowing for loop
	*************************************************

	for (i=0; i< MAX_LENGTH ; i++)
	{
       		 char_device_buf[i]= 0
	}

	************************************************
*/
#define	 	 VIVEK_FILL_ZERO	  _IO( VIVEK_MAGIC, 1)


/*

	VIVEK_FILL_CHAR: To fill our charactor array with char.
	As we can see there is no dtae transfer. The movement
	driver gets this command. It executes the folowing for loop
	*************************************************

	for (i=0; i< MAX_LENGTH ; i++)
	{
		char_device_buf[i]= char
	}
	************************************************
*/
#define		VIVEK_FILL_CHAR		_IOW( VIVEK_MAGIC, 2, char )


/*
	VIVEK_SET_SIZE: To set size of charactor array
*/

#define 	VIVEK_SET_SIZE		_IOW( VIVEK_MAGIC, 3, uint)


/*
	VIVEK_GET_SIZE: To get size fo charactor array
*/

#define 	VIVEK_GET_SIZE		_IOR( VIVEK_MAGIC, 4, uint)

#define 	VIVEK_MAX_CMDS		4

#endif




