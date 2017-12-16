obj-m += simple_semaphore.o

KERNEL_SRC=/lib/modules/${shell uname -r}/build 

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules
	gcc user_1.c -o user_1
	gcc user_2.c -o user_2
clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
	rm user_1 user_2
	

