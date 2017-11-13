#include<linux/kernel.h>
#include<linux/module.h>
#include <linux/platform_device.h>

//定义平台核心结构
void my_release(struct device *dev)
{
  printk("platform exit success!\n");
}
struct resource my_resource[]=
{
  [0]=
  	{
		.start    = 0x110002e0,
		.end     = 0x110002e7,
		.name  = "platform",
		.flags   = IORESOURCE_MEM,
  	},

};
struct platform_device my_platform_device =
{
   	.name                  = "platform",
   	.id                        = -1,
   	.dev                     =
   	{
         .release =my_release,
	},
   	.num_resources  =1,
   	.resource             =&my_resource,
};

//注册函数

static int __init platform_driver_init(void)
{      
        int ret=0;
 	ret=platform_device_register(&my_platform_device);
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
      platform_device_unregister(&my_platform_device);
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);
MODULE_LICENSE("GPL");


