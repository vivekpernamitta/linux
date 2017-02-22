
/*
   Simple virtual char driver with 
	keys:	dynamic register in kernel
	  	manual /dev/proc creation
		read from user space and print on terminal usign dmesg
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
		printk(KERN_INFO " \t DEVICE BUSY %s\n", CHAR_DEV_NAME);
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

static int char_dev_read(struct file *file, char *buf, size_t lbuf, loff_t *ppos)
{

	int nbytes = lbuf - copy_to_user(buf, char_device_buf + *ppos, lbuf);
	*ppos +=nbytes;
	printk(KERN_INFO "\t READING nbytes=%d, pos=%d\n", nbytes, (int)*ppos);

	return nbytes;

}

static int char_dev_write(struct file *file, const char* buf, size_t lbuf, loff_t *ppos)
{

//int nbytes = lbuf - copy_from_user(char_device_buf+ *ppos, buf, lbuf);
	int nbytes = lbuf - copy_from_user(char_device_buf, buf, lbuf);
	*ppos += nbytes;
	printk( KERN_INFO "\t WRITING function, nbyes=%d, pos=%d \n DATE=%s\n", nbytes, (int)*ppos,char_device_buf);

	printk(" READ DATA ");
	return nbytes;
}





static struct file_operations char_dev_fops = {
	.owner = THIS_MODULE,
	.read = char_dev_read,
	.write = char_dev_write,
	.open = char_dev_open,
	.release = char_dev_release,

};









/*
   Char device initilation and uninitilazation
 */ 

static int __init char_dev_init(void)
{
	int ret;

	/* registering device in charactor space
	   alloc_chrdev_region
	   1 -->   address to major nad mnor no
	   2 -->	start minor no (12bit size) 
	   3 --> 	count: how many no.of device(i.e., called nodes) you can connect. currenly coutn is only one
	   4 -->	Name of driver name (see cat /proc/devices)

	 */

	ret=alloc_chrdev_region( &mydev, 0, count, CHAR_DEV_NAME);
	if( ret <0){
		printk( KERN_ERR "failed to reserve majar/minor dynamically");
		return -1;
	}

	/*
	   creting cdev memory and and allocating file operations to that memory
		TERM:  Creating instance of c-dev for VFS
	 */
	if(!(vivek_cdev = cdev_alloc())){
		printk(KERN_ERR "\t cdev_alloc instance memory allocation fail");
	}

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


