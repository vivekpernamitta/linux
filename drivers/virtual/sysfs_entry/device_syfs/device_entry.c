#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/kobject.h> 
#include <linux/device.h>



struct kobject * example_kobject;



// Kobject functions 
static int ldd_match(struct device *dev)
{	
	printk(KERN_WARN " device_release");
//#	return driver->name;
		//return !strncmp(dev->bus_id, driver->name, strlen(driver->name));
}
/*
static int ldd_hotplug(struct device *dev, char **envp, int num_envp,
		char *buffer, int buffer_size)
{
	envp[0] = buffer;
	if (snprintf(buffer, buffer_size, "LDDBUS_VERSION=%s",
				Version) >= buffer_size)
		return -ENOMEM;
	envp[1] = NULL;
	return 0;
}

*/

struct device  ldd_device_type = {
        .bus_id = "ldd",
        .release = ldd_match,
//        .hotplug = ldd_hotplug,
};





static int __init mymodule_init (void) { 
	int ret;	
	pr_debug("Module initialized successfully \n"); 
	ret = device_register(&ldd_device_type);
	if (ret)
		return ret;

	return 0;
}

static void __exit mymodule_exit (void) { 
	pr_debug ("Module un initialized successfully \n");
	//kobject_put(example_kobject);
	//kobject_put(example_kobject);
	void device_unregister(&ldd_device_type);
} 
module_init(mymodule_init); 
module_exit(mymodule_exit); 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("vivek");
