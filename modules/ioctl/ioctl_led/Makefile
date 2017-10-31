name=ioctl_led
obj-m :=$(name).o 
#KDIR :=/lib/modules/$(shell uname -r)/build
KDIR  :=/home/ubuntu/Desktop/linux-3.5/
#模块最终存放
rootfs_dir :=/root/rootfs/home/
all:
	mkdir -p $(rootfs_dir)/$(name)
	@make -C $(KDIR) M=$(PWD) modules  
	@rm -f *.o *.mod.o *.mod.c *.symvers *.markers *.unsigned *.order *~ *.bak
	@rm -fr  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
	@make -v  		>readme.txt
	date                    >>readme.txt
	@echo  $(func)		>>readme.txt	
	@echo  $(PWD)  		>>readme.txt	
	arm-linux-gcc -o app app.c
	mv app *.ko *.txt  ${rootfs_dir}$(name) 
	ls ${rootfs_dir}$(name)
clean:
	@make -C  $(KDIR)   M=$(PWD)  modules  clean
	@rm -f *.ko *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~
