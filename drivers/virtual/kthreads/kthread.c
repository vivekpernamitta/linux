#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/sched.h>
//#include<linux/module.h>



static pid_t my_thread (void){

	for(;;){
		printk (KERN_INFO " thread function\n "); 

	}
}


static int __init my_driver(void ){
	int ret ;
	kernel_thread(&my_thread,NULL ,CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
	if(ret = 0){
		printk(KERN_ERR " kernek thread creation faili\n");	
	}
	for(;;){
		printk (KERN_INFO " my_driver function\n "); 
	}
}

static int __exit my_driver_exit(void ){
	printk (KERN_INFO " module called back");
}
module_init(my_driver);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("VIVEK");
MODULE_DESCRIPTION ("Kernel thread  Test");

