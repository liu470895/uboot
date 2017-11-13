#include<linux/kernel.h>
#include<linux/module.h>
#include <linux/platform_device.h>
#include<linux/cdev.h>
#include<linux/device.h>
/***

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
	int my_probe(struct platform_device *dev)
{
       printk("my_probe is ok!\n");
       return 0;
}
	int my_remove(struct platform_device *dev)
{
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



