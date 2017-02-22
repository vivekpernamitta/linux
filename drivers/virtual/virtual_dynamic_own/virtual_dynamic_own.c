


/* Thi si bacis driver that can inserted in the kernel  which can take major no dynamically or statically

 */


#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>


// Macros defined

#define CHAR_DRIVER_NAME "scull"
#define MINOR_ADDRESS 1
#define COUNT 1



unsigned int scull_major=0;
unsigned int scull_minor;
struct file *filp






struct scull_dev {
	struct scull_qset *data; /* Pointer to first quantum set */
	int quantum; /* the current quantum size */
	int qset; /* the current array size */
	unsigned long size; /* amount of data stored here */
	unsigned int access_key; /* used by sculluid and scullpriv */
	struct semaphore sem; /* mutual exclusion semaphore */
	struct cdev cdev; /* Char device structure */

};


static int scull_open(struct inode *inode, struct file *filep) {
	struct scull_dev *dev; /* device information */
	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev; /* for other methods */
	/* now trim to 0 the length of the device if open was write-only */
	if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {
		scull_trim(dev); /* ignore errors */
	}
	return 0; /* success */

}

static int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}



struct file_operations scull_fops = {

	.open = scull_open,
};


static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err, devno,result;
	if (scull_major)        {
		devno = MKDEV (scull_major, scull_minor);
		result = register_chrdev_region( &devno, COUNT, CHAR_DRIVER_NAME);
	}
	else    {
		result = alloc_chrdev_region(&devno, scull_minor, COUNT, CHAR_DRIVER_NAME);
		scull_major = MAJOR(dev);
	}


	if (result < 0) {
		printk (KERN_WARNING " scull: can't get major %d \n", scull_major);
		return result;
	}
	else
		printk (KERN_INFO , "device numebr =%u \n",&devno);


	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add (&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}



static void   __initdata init_function(void){

	struct scull_dev *dev;

	scull_setup_cdev(&dev, COUNT)

		struct cdev *my_cdev = cdev_alloc( );	
	my_cdev->ops = &my_fops;

	// you should initialize the structure that you have already allocated
	//	cdev_init (my_cdev, fops);

	//the final step is to tell the kernel about it with a call to
	if (cdev_add( my_cdev, MINOR_ADDRESS, COUNT) < 0)
	{
		printk(KERN_ERROR, "cdev_add failed : module not inserted"); 
	}


	int 	iminor = iminor(struct inode *inode);
	int imajor = iminor(struct inode *inode);

}


static void  __exitdata exit_function(void) {
	cdev_del(my_cdev);
	unregister_chrdev_region(scull_minor, COUNT, CHAR_DRIVER_NAME);
}







module_init ( init_function);
module_exit ( exit_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIVEK");
MODULE_DESCRIPTION(" vitual char driver");




}






