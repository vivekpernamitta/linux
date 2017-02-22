
obj-m += virtual_dynamic_own.o

KERNEL_SRC=/lib/modules/${shell uname -r}/build

default:
	make -C $(KERNEL_SRC) M=$(PWD) modules	

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
