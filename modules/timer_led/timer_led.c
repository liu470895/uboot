//模块头文件
#include <linux/module.h>
#include <linux/kernel.h>
//linux2.6标准设备头文件
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include<linux/timer.h>


//存放物理地址对应的虚拟地址
unsigned int __iomem *   base_addr;  // __iomem 可选择的，告诉你是虚拟地址而已

//寄存器定义
#define BASE_ADDR    (0x110002E0)
#define GPM4CON     (*(volatile unsigned int *)(base_addr + 0))
#define GPM4DAT     (*(volatile unsigned int *)(base_addr + 1))


struct timer_list my_timelist;

void mytimer_func(long data)
{
  printk("%s is call! data:%d\r\n",__FUNCTION__,data);
  GPM4DAT  ^=( 1 << 0);
  GPM4DAT  ^=( 1 << 1);
  GPM4DAT  ^=( 1 << 2);
  GPM4DAT  ^=( 1 << 3);
  mod_timer(&my_timelist,jiffies+HZ*2);
  /*
  GPM4DAT  ^=( 1 << 0);
  GPM4DAT  ^=( 1 << 1);
  GPM4DAT  ^=( 1 << 2);
  GPM4DAT  ^=( 1 << 3);
 */
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

size_t My_write (struct file *mufile, const char __user *buff , size_t size , loff_t *conut)
	{	 
  return 0;
	}


int My_open (struct inode *file , struct file * name )
{
    return 0;
}

	
int  My_close(struct inode *file , struct file *name)
{
   printk("My_close is ok\n");
   return 0;
}

//初始化文件操作结构体
struct file_operations  cdev_file =
{
   .llseek  = My_lseek,
   .read    = My_read,
   .write   = My_write,
   .open    = My_open,
   .release = My_close,
};


//定义核心结构体指针
struct cdev * pcdev; 
//定义存放系统自分的设备号变量(主   + 次)
dev_t  my_dev; 
//次设备号连续个数
#define MY_MINOR_CONUT   2
//设备名、任意 无需和 /dev/ 相同
#define MY_CDEV_NAME  "timer_led"
//主设备号 次设备号
dev_t major,minor; 
//定义结构体类指针
struct class * my_class;
struct device *my_device;


//加载函数
static int __init Cdev_driver_init(void)
{
   int ret=0 ;
    //把 BASE_ADDR 物理地址映射成虚拟地址，大小为0x18
    base_addr = (unsigned long *) ioremap(BASE_ADDR, 0x8);

    GPM4CON  &= ~0xffff;  //清0
    GPM4CON  |=  0x1111;  //配置为输出方向
    GPM4DAT  |=  0xf ;    //配置为输出方向

    init_timer(&my_timelist);
    setup_timer(&my_timelist,mytimer_func,10);
    my_timelist.expires=jiffies+msecs_to_jiffies(10);
    add_timer(&my_timelist);
    printk("%s is call !\r\n",__FUNCTION__);

   //1、采用动态方式申请结构体内存空间
   pcdev = cdev_alloc();
   if(pcdev ==NULL)
   {
       printk("cdev_alloc error\n");
       goto ret_cdev_alloc ;
   }
   //2、采用函数方式初始化核心结构体成员
   cdev_init(pcdev,&cdev_file);
   //3、采用动态方式申请设备号
   ret =alloc_chrdev_region(&my_dev,0,MY_MINOR_CONUT,MY_CDEV_NAME);
   if(ret <0)
   {
     printk("alloc_chrdev_region error\n");
     goto ret_alloc_chrdev_region;
   }
   //4、注册标准字符设备
   ret =cdev_add(pcdev,my_dev,MY_MINOR_CONUT);
   if(ret <0)
   {
     printk("cdev_add error\n");
     goto ret_cdev_add;
   }
   //5、自动创建设备名
   //给类指针赋值
   my_class =class_create(THIS_MODULE,"timer_led");
   if(IS_ERR(my_class)) {
		ret = PTR_ERR(my_class);
		goto  ret_my_class ;
	}
   my_device=device_create(my_class,NULL,my_dev,NULL,"%s",MY_CDEV_NAME);
   if(IS_ERR(my_device)) { 
		ret = PTR_ERR(my_device);
		goto  ret_my_device ;
	}
   major =MAJOR(my_dev);//提取主设备号
   minor =MINOR(my_dev);//提取次设备号
   printk("major:%d ;minor :%d  minor_count :%d\n ",major,minor,MY_MINOR_CONUT);  
   return 0;

ret_my_device :
  class_destroy(my_class); //取消类初始化
ret_my_class: 
   cdev_del(pcdev);   //1、注销标准设备
ret_cdev_add :
  unregister_chrdev_region(my_dev,MY_MINOR_CONUT); //释放设备号
ret_alloc_chrdev_region :
  kfree((struct cdev *)pcdev);//释放申请内存空间 
ret_cdev_alloc:
       return -ret; 
}
//卸载函数

static void __exit Cdev_driver_exit(void)
{      
    device_destroy(my_class,my_dev);
    class_destroy(my_class);
    //1、注销标准设备
    cdev_del(pcdev);
    //2、释放申请的设备号
    unregister_chrdev_region(my_dev,MY_MINOR_CONUT); //释放设备号
    //3、释放核心结构体内存空间
    kfree((struct cdev *)pcdev);//释放申请内存空间 
   
    del_timer(&my_timelist);
   
    printk("timer_led  exit success! \n");
}

module_init(Cdev_driver_init);
module_exit(Cdev_driver_exit);
MODULE_LICENSE("GPL");


