#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include <linux/miscdevice.h>
#define PRE_DEVICE_NAME   "pre"

loff_t  My_lseek (struct file *myfile, loff_t size, int conut)
{
   printk("My_lseek is ok\n");
   return 0;
}

ssize_t My_read (struct file *myfile, char __user *name , size_t size, loff_t * count)
{
   printk("My_read is ok\n");
   return 0;
}

size_t My_write (struct file *mufile, const char __user *name , size_t size , loff_t *conut)
{
   printk("My_write is ok\n");
   return 0;
}
	
int My_open (struct inode *file , struct file * name )
{
   printk("My_open is ok\n");
   return 0;
}

	
int  My_close(struct inode *file , struct file *name)
{
   printk("My_close is ok\n");
   return 0;
}



struct file_operations  pre_file=
{
	.llseek 	=My_lseek,
	.read 	=My_read,
	.write 	=My_write,
	.open	= My_open,
	.release 	=My_close,
};


int ret;

static int __init Pre_driver_init(void)
{
 	
       ret=register_chrdev(0,PRE_DEVICE_NAME,&pre_file);
 	if(ret<0)
 	{
	printk("pre_device failed!\n");
	return 0;
 	}
	printk("pre_device init ok!\n");
	printk("ret:%d\n",ret);
	return 0;
}
static void __exit Pre_driver_exit(void)
{
    unregister_chrdev(ret,PRE_DEVICE_NAME);
    printk("pre_device exit ok\n");
}

module_init(Pre_driver_init);
module_exit(Pre_driver_exit);
MODULE_LICENSE("GPL");




























