

/*
This odule takes commnad line arg while loading module

use this:
insmod <module.ko>  whom="<string>"


*/


#define info(fmt, arg...)                                           \
        pr_info("[ IPC :%-20.20s: %-1.1d ] [INF] " fmt "\n",  \
       __func__, __LINE__, ##arg)

#include <linux/module.h>
#include <linux/moduleparam.h>
#include<linux/module.h>
#include<linux/kernel.h>

static char *whom = "world";

static int __initdata  initt(void)
{
printk(" hellow world = %s\n",whom);
pr_info(" line-2 hey world = %s\n",whom);
info (" line-3 hi good world listen up=== %s", whom);

}


static int  __exitdata  exitt(void)
{
printk( " cruel world. you are a physco YOU KILLED ME!!!!\n");
}


module_param(whom, charp, S_IRUGO);


module_init(initt);

module_exit(exitt);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("vivek");
MODULE_DESCRIPTION("This module takes commnad line arg while loading module");



