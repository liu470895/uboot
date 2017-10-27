#include<linux/kernel.h>
#include<linux/module.h>
//杂项设备头文件
#include<linux/fs.h>
#include <linux/cdev.h>
#include<linux/slab.h>
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

struct file_operations cdev_file=
{
	.llseek 	=My_lseek,
	.read 	=My_read,
	.write 	=My_write,
	.open	= My_open,
	.release 	=My_close,
};

//定义一个核心结构体指针
struct cdev *pcdev;
//定义一个系统自分配的设备号变量（主+次）
dev_t   my_dev;
//次备号连续的个数
#define MY_MIN_COUT 2
//设备名、任意无需要和/dev下相同
#define MY_CDEV_NAME "linux26"
//主设备号 //次设备号
dev_t major,minor;


//加载函数

static int __init Cdev_driver_init(void)
{
 	int ret;
//1、采用动态方式申请结构体空间
 	pcdev =cdev_alloc();
 	if(pcdev==NULL)
 		{
 			printk("cdev_alloc error !\n");
 		}
 	//2、采用函数初始化方式结构体成员
	cdev_init(pcdev,&cdev_file );
	//3、采用动态方式申请设备号
	ret=alloc_chrdev_region(&my_dev,8,MY_MIN_COUT,MY_CDEV_NAME); 	
	if(ret<0)
		{
			printk("alloc_chrdev_region error!\n");
			kfree((struct cdev*)pcdev);
			return 0;
		}
	//4、注册标准字符设备
	ret=cdev_add(pcdev,my_dev,MY_MIN_COUT);
	if(ret<0)
		{
			printk("cdev_add error!\n");
		  unregister_chrdev_region(my_dev,MY_MIN_COUT);
		  return 0;
		}
	major=MAJOR(my_dev); //提取主设备号
	minor=MINOR(my_dev); //提取次设备号
	printk("major : %d \nminor : %d \ncount : %d \n",major,minor,MY_MIN_COUT);
	return 0;
}
static void __exit Cdev_driver_exit(void)
{
  //1、注销标准设备
  cdev_del(pcdev);
  //2、释放设备号
  unregister_chrdev_region(my_dev,MY_MIN_COUT);
  //3、释放内存
  kfree((struct cdev*)pcdev);
}

module_init(Cdev_driver_init);
module_exit(Cdev_driver_exit);
MODULE_LICENSE("GPL");






