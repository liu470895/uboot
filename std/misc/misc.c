#include<linux/kernel.h>
#include<linux/module.h>
//杂项设备头文件
#include<linux/fs.h>
#include <linux/miscdevice.h>
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

//初始化文件造操作结构体

struct file_operations misc_file=
{
	.llseek 	=My_lseek,
	.read 	=My_read,
	.write 	=My_write,
	.open	= My_open,
	.release 	=My_close,
};


//初始化杂项设备核心结构体

struct miscdevice my_misc=
{
	.minor =255,
	.name="test",
	.fops=&misc_file,
	
};

//加载函数

static int __init Misc_driver_init(void)
{
 	int ret;
        ret =misc_register(&my_misc);
 	if(ret<0)
 	{
	printk("misc failed!\n");
	return 0;
 	}
	return 0;
}
static void __exit Misc_driver_exit(void)
{
     misc_deregister(&my_misc);
}

module_init(Misc_driver_init);
module_exit(Misc_driver_exit);
MODULE_LICENSE("GPL");






