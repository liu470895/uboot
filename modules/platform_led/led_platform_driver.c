#include<linux/kernel.h>
#include<linux/module.h>
#include <linux/platform_device.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <asm/io.h>

/*

struct platform_driver {
	int (*probe)(struct platform_device *);
	int (*remove)(struct platform_device *);
	void (*shutdown)(struct platform_device *);
	int (*suspend)(struct platform_device *, pm_message_t state);
	int (*resume)(struct platform_device *);
	struct device_driver driver;
	const struct platform_device_id *id_table;
};
***/
unsigned int __iomem *	 base_addr;  
int size;	
struct resource *ret;
#define BASE_ADDR    (0x110002E0)
#define GPM4CON     (*(volatile unsigned int *)(base_addr + 0))
#define GPM4DAT     (*(volatile unsigned int *)(base_addr + 1))
	
	
	
	loff_t	My_lseek (struct file *myfile, loff_t size, int conut)
	{
	   printk("My_lseek is ok\n");
	   return 0;
	}
	
	ssize_t My_read (struct file *myfile, char __user *name , size_t size, loff_t 
	* count)
	{
	   printk("My_read is ok\n");
	   return 0;
	}
	
	size_t My_write (struct file *mufile, const char __user *buff , size_t size , 
	loff_t *conut)
		{	 
		int i = 0;
		char kbuf[4] = {0};
		if (copy_from_user(kbuf, buff, size) ) {
			printk(KERN_EMERG" error: copy_from_user\r\n");
			return -EFAULT;
		}
	
		for(i = 0; i < size; i++) {
			if(kbuf[i] == '0') {
				GPM4DAT  |=  1 << (i) ;  
			}
	
			else if(kbuf[i ] == '1') {
				GPM4DAT  &= ~( 1 << (i));  
			}
		}
	  return 0;
		}
	
	
	int My_open (struct inode *file , struct file * name )
	{
		base_addr = (unsigned long *) ioremap(ret->start, size);
	
		GPM4CON  &= ~0xffff;  
		GPM4CON  |=  0x1111;  
		GPM4DAT  |=  0xf ;	  
		return 0;
	}
	
		
	int  My_close(struct inode *file , struct file *name)
	{
	   printk("My_close is ok\n");
	   return 0;
	}

	
	
	struct file_operations misc_file=
	{
		.llseek 	=My_lseek,
		.read	=My_read,
		.write	=My_write,
		.open	= My_open,
		.release	=My_close,
	};
	
	
	
	struct miscdevice misc_led=
	{
		.minor =255,
		.name="platform_led",
		.fops=&misc_file,
		
	};

	int my_probe(struct platform_device *dev)
{
	int value;
     ret = platform_get_resource(dev,IORESOURCE_MEM,0);
	printk("my_probe is ok!\n");
	size=ret->end-ret->start+1;
	ret=request_mem_region(ret->start,size,ret->name);
	printk("request_mem_region is ok!\n");
	value=misc_register(&misc_led);
	if(value<0)
		{
       printk("misc_register error!\n");
      release_mem_region(ret->start, size) ;
       return 0;
	}
       return 0;
}
	int my_remove(struct platform_device *dev)
{
       misc_deregister(&misc_led);
       printk("my_removeis ok!\n");
       return 0;
}


//定义一个平台结构体变量

struct platform_driver my_platform_driver=
{
  	.probe      = &my_probe,
  	.remove   = &my_remove,
  	.driver =
  	{
  	  .name="my_platform",
  	},

};

//注册函数

static int __init platform_driver_init(void)
{      
        int ret=0;
 	ret=platform_driver_register(&my_platform_driver);
	if(ret<0)
		{
	printk("my_platform_device error !\n");
	return 0;
	}
	else
	printk("my_platform_device init success !\n");	
	return 0;
}
static void __exit platform_driver_exit(void)
{
      platform_driver_unregister(&my_platform_driver);
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);
MODULE_LICENSE("GPL");



