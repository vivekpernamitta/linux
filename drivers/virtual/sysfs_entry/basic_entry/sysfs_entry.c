#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/kobject.h> 
#include <linux/device.h> 
#include <linux/sysfs.h> 


static struct kobject * example_kobject, * subdir_example_kobject;


static ssize_t mydev_do_read (struct kobject *kobj,
			 struct kobj_attribute * attr, char *buf, size_t count){
	printk("%s",__func__);
	//strncpy(buf,"vivek",5);
	sprintf(buf, "%s", "vivek");
}
// Device attributes 
struct  kobj_attribute vivek_subdir_device={
	.attr.name = "vivek",
	.attr.mode = 0444,
	.show = mydev_do_read,
	.store = NULL,
};
	

static int __init mymodule_init (void) { 
	pr_debug("Module initialized successfully \n"); 
	//example_kobject = kobject_create_and_add("kobject_example", "/sys/class"); 
	example_kobject = kobject_create_and_add("kobject_example", NULL); 
	//example_kobject = kobject_create_and_add("kobject_example", kernel_kobj); 
	subdir_example_kobject = kobject_create_and_add("subdir_kobject_example", example_kobject); 

	if(!example_kobject) {
		return -ENOMEM;
	}
	// Adding  file to /sys/kernel/kobject_example/subdir_kobject_example foler
	sysfs_create_file ( subdir_example_kobject , &vivek_subdir_device.attr);
	return 0;
}




static void __exit mymodule_exit (void) { 
	pr_debug ("Module un initialized successfully \n");
	//
	sysfs_remove_file( subdir_example_kobject, &vivek_subdir_device.attr);
	//kobject_put(example_kobject);
	kobject_put(subdir_example_kobject);
	kobject_put(example_kobject);
} 
module_init(mymodule_init); 
module_exit(mymodule_exit); 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("vivek");
