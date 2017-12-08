/*
 * This is basic Interrput program that will genrete interrupts using 
 * pre-existing (NIC) IRQ line through  "request_irq" function.
 * 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as published by
 * the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include<linux/module.h>
#include<linux/init.h>
#include<linux/interrupt.h>

#define SHARED_IRQ 10

static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param (irq, int, S_IRUGO);

static irqreturn_t my_interrupt (int irq, void *dev_id){
	irq_counter++;
	printk (KERN_INFO " IN the ISR:counter = %d\n", irq_counter);
	return IRQ_NONE;	// This is just for observation
}

static int __init my_init (void){

	if(request_irq (irq, my_interrupt, IRQF_SHARED, "test_interrupt", &my_dev_id))
		return -1;
	printk (KERN_INFO " Successfully loading ISR handle\n");
	return 0;

}

static void my_exit(void){
	synchronize_irq(irq);
	free_irq(irq, &my_dev_id);
	printk( KERN_INFO " Successfully Unloading \n");
}



module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("VIVEK");
MODULE_DESCRIPTION ("Interrupt handler Test");
MODULE_LICENSE("GPL");
