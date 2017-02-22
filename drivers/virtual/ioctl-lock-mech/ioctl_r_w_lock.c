
/*
   Simple virtual char driver which can perform
	1.read, write, lseek( all three) and	 ioctl 	(a.setting all char to zero
							 b. checking the size of kernel buffer
							 c. and .d not tested 
							)
	2. dynamic allocating the number and create manually mknod
		cat /proce devices	check majot number
		mknod /dev/vivek_ioctl_test  c <major no> <minor no>
	3. tested working perfectly with ioctl toow functions
	
	contents in pack	:	ioctl_r_w.c --> driver
					Makefile
					user.c
	 
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <asm/current.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <asm/ioctl.h>
#include "virtual_ioctl_lock.h"

/* 
   LIST of MACROS
 */

#define MAJORNO 300
#define MINORNO 0
#define CHAR_DEV_NAME "vivek_module"
#define MAX_LENGTH 4000
#define SUCCESS 0

static char  char_device_buf[MAX_LENGTH];
struct cdev *vivek_cdev;
dev_t mydev;


int count=1, inuse=0;
//int nbytes=0;


/*
   File operation open. close ,read, write

 */
static int char_dev_open( struct inode *inode, struct file *file)
{
	if (inuse)
	{
		printk(KERN_ERR " \t DEVICE BUSY %s\n", CHAR_DEV_NAME);
		//		return _EBUSY;

	}
	printk(KERN_INFO "\t DEVICE OPEN");
	inuse=1;
	return SUCCESS;

}


static int char_dev_release(struct inode *inode, struct file* file)
{

	inuse=0;
	printk(KERN_INFO "\t DEVICE CLOSED");
	return SUCCESS;

}


/*
   Reding and writting
 */
/*
static int char_dev_read(struct file *file, char *buf, size_t lbuf, loff_t *ppos)
{

	int nbytes = lbuf - copy_to_user(buf, char_device_buf+ *ppos  , lbuf);
	*ppos +=nbytes;
	printk(KERN_INFO "\t READING nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	printk ("data written to user space = %s ", char_device_buf);

	return nbytes;

}

static int char_dev_write(struct file *file, const char* buf, size_t lbuf, loff_t *ppos)
{

	int nbytes = lbuf - copy_from_user(char_device_buf + *ppos, buf, lbuf);
	*ppos += nbytes;
	printk( KERN_INFO "\t WRITING finction, nbyes=%d, pos=%d\n", nbytes, (int)*ppos);
	printk ("data read from user space%s ", char_device_buf);


	return nbytes;
}

*/
/*
   This function is called when lseek is called on device
   This function should place the ppos pointer of 'file'
   at the offset of  'offset' of 'orig'
 ************
 if orig = SET_SEEK
 ppos = offset
 if orig =SEEK_END
 ppos = MAX_LENGTH +offset
 if orig = SEEK_CUR
 ppos += offset

 returns the new position
 ***********'
 */

/*
static loff_t	char_dev_lseek( struct file *file, loff_t offset, int orig)
{
	loff_t	testpos;
	switch (orig)
	{

		case 0:
			testpos = offset;
			break;
		case 1:
			testpos = file->f_pos + offset;
			break;
		case 2:
			testpos =MAX_LENGTH + offset;
			break;
		default:
			return -EINVAL;

	}
	testpos = testpos < MAX_LENGTH ? testpos : MAX_LENGTH;
	testpos = testpos >= 0 ? testpos :0;
	file->f_pos = testpos;
	printk ( KERN_INFO "Seeking to pos=%d\n", (long)testpos);
	return testpos;
}

*/

static struct my_data my_data = {
	.i = -100,
	.x = 100,
	.s = "LINUX KERNEL",
};

/* IOCTL function  */

static int	char_dev_ioctl(struct file *filp,
		unsigned int cmd,
		unsigned long arg)


{
	printk( KERN_INFO "IOCTL function entered\n");
	unsigned int i , size, direction;
	char * new_buf;
	char c;
	int retbytes;
	void __user *ioargp = (void __user *)arg;	// need to debug
/*
	if( _IOC_TYPE(cmd) != VIVEK_MAGIC )
	{
		printk(KERN_ERR "IOCTL type check failed\n");
		return -ENOTTY;
	}
	if( _IOC_NR (cmd) > VIVEK_MAX_CMDS ) 
	{
		printk(KERN_ERR "IOCTL MAX commands exceded\n");
		return -ENOTTY;
	}
	if( _IOC_DIR(cmd) & _IOC_READ)
		if( ! access_ok( VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd) ) )
			return -EFAULT;
	if(_IOC_DIR(cmd) & _IOC_WRITE)
		if( ! access_ok(VERIFY_READ, (void *) arg, _IOC_SIZE(cmd)))
			return -EFAULT;
	printk( KERN_INFO "ready to switch function\n");
*/
	if (_IOC_TYPE (cmd) != VIVEK_MAGIC){
	
		printk(KERN_ERR "IOCTL invalid case\n");
		return -ENOTTY;
	}
	/*
	
	calling the correct ioctl function using switch case

	*/
	direction = _IOC_DIR (cmd);
	size = _IOC_SIZE(cmd);

	switch(direction)
	{
		case _IOC_WRITE:	// APP (write) --> kernel (read)	so IOC_WRITE (always user level specific) and [copy_from_user] is used
			printk( KERN_INFO  " reading  = %d Bytes from user space writting to device", size);
			retbytes =  copy_from_user (&my_data, ioargp, size);
			printk ( KERN_INFO "my_data.i =%d \n	(int) my_data.x = %ld \n 	my_dat.s = %s\n", my_data.i, my_data.x, my_data.s);
			return retbytes;
			break;
	
		case _IOC_READ:
			printk( KERN_INFO  " reading bytes and writting  = %d Bytes to user space ", size);
	
			printk ( KERN_INFO "my_data.i =%d \n	(int) my_data.x = %ld \n 	my_dat.s = %s\n", my_data.i, my_data.x, my_data.s);
			retbytes =  copy_to_user ( ioargp, &my_data, size);
			return retbytes;
			break;
			
	}
	return SUCCESS;


}




static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
//	.read = char_dev_read,
//	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release,
//	.llseek = char_dev_lseek,
	.unlocked_ioctl = char_dev_ioctl,
};









/*
   Char device initilation and uninitilazation
 */ 

static int __init char_dev_init(void)
{
	int ret;

	/* registering device in charactor space*/
	//mydev=MKDEV(MAJORNO,MINORNO);
	//register_chrdev_region(mydev, count, CHAR_DEV_NAME);

	ret=alloc_chrdev_region( &mydev, 0, count, CHAR_DEV_NAME);
	if( ret <0){
		printk( KERN_ERR "failed to reserve majar/minor dynamically");
		return -1;
	}



	/*
	   creting cdev memory and and allocating file operations to that memory
TERM:  Creating instance of c-dev for VFS
	 */
	vivek_cdev = cdev_alloc();
	cdev_init(vivek_cdev, &char_dev_fops);


	/*
	   linking cdev(fileoperation) and device number and making count is no of minor devices(currently 0)
TERM: registering c-ev object with functions
	 */
	ret= cdev_add(vivek_cdev , mydev , count);

	if(ret<0)
	{
		printk("Error registering device driver\n");
		return ret;

	}

	printk(KERN_INFO "\t Device Registered %S\n", CHAR_DEV_NAME);

	memset(char_device_buf, '\0', MAX_LENGTH);
	return 0;




}

static int __exit char_dev_exit(void)
{
	// exectly reverse way
	/*
	   deleting c-dev object from VFS
	 */
	cdev_del(vivek_cdev);

	/*
	   remove major no and minor no for charr device
	 */
	unregister_chrdev_region(mydev, 1);

	printk(KERN_INFO"\t Device unregistered\n");


}








module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("vivek");
MODULE_DESCRIPTION("Virtaul charactor driver-test");
MODULE_LICENSE("GPL");


