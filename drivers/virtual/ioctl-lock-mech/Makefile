
obj-m += ioctl_r_w_lock.o

KERNEL_SRC=/lib/modules/${shell uname -r}/build 

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules
clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean

