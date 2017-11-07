#include<linux/kernel.h>
#include<linux/module.h>
//杂项设备头文件
#include<linux/fs.h>
#include<linux/miscdevice.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>

//key1

irq_handler_t key1_handler(unsigned int irq,void* dev)
{
    int value;
    printk("key1 press \n");
    value=__gpio_get_value(EXYNOS4_GPX3(2));
    printk("key1 value =%d\n",value);
  return 0;
}
irq_handler_t key2_handler(unsigned int irq,void *dev)
{
    int value;
    printk("key2 press \n");
    value=__gpio_get_value(EXYNOS4_GPX3(3));
    printk("key2 value =%d\n",value);
 return 0;
}
irq_handler_t key3_handler(unsigned int irq,void *dev)
{
    int value;
    printk("key3 press \n");
    value=__gpio_get_value(EXYNOS4_GPX3(4));
    printk("key3 value =%d\n",value);
 return 0;
}
irq_handler_t key4_handler(unsigned int irq,void *dev)
{
    int value;
    printk("key4 press \n");
    value=__gpio_get_value(EXYNOS4_GPX3(5));
    printk("key4 value =%d\n",value);
 return 0;
}


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
	
int My_open (struct inode *file , struct file *name )
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
	.llseek  =My_lseek,
	.read 	 =My_read,
	.write 	 =My_write,
	.open	 = My_open,
	.release =My_close,
};


//初始化杂项设备核心结构体

struct miscdevice my_misc=
{
	.minor =255,
	.name="key_interupt",
	.fops=&misc_file,
	
};
//init irq
unsigned int key1_irq;
unsigned int key2_irq;
unsigned int key3_irq;
unsigned int key4_irq;


//加载函数

static int __init Misc_driver_init(void)
{
// init flags
    unsigned long flags=IRQF_DISABLED|IRQF_TRIGGER_FALLING; 
 	int ret;
//init request 
	key1_irq=gpio_to_irq(EXYNOS4_GPX3(2));
	request_irq(key1_irq,key1_handler,flags,"key1",NULL);
	key2_irq=gpio_to_irq(EXYNOS4_GPX3(3));
	request_irq(key2_irq,key2_handler,flags,"key2",NULL);
	key3_irq=gpio_to_irq(EXYNOS4_GPX3(4));
	request_irq(key3_irq,key3_handler,flags,"key3",NULL);
	key4_irq=gpio_to_irq(EXYNOS4_GPX3(5));
	request_irq(key4_irq,key4_handler,flags,"key4",NULL);
//enable interupt
/*
    enable_irq(key1_irq);
    enable_irq(key2_irq);
    enable_irq(key3_irq);
    enable_irq(key4_irq);
*/
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
 //disable interupt
    disable_irq(key1_irq);
    disable_irq(key2_irq);
    disable_irq(key3_irq);
    disable_irq(key4_irq);
 //free irq
    free_irq(key1_irq,NULL);
    free_irq(key2_irq,NULL);
    free_irq(key3_irq,NULL);
    free_irq(key4_irq,NULL);

     misc_deregister(&my_misc);
}

module_init(Misc_driver_init);
module_exit(Misc_driver_exit);
MODULE_LICENSE("GPL");






