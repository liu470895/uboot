#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//0.添加头文件
#include <linux/workqueue.h>

//1)实现一个工作函数
void  mywork_func(struct work_struct *mywork)
{
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);
}


//2)定义一个延时工作
//struct delayed_work mydelayed_work;            //动态定义
DECLARE_DELAYED_WORK(mydelayed_work, mywork_func); //静态定义


static int __init myworkqueue_init(void)
{

    printk("%s is call!\r\n", __FUNCTION__);
    //3.一安装模块就开始调度,1秒后开始调度工作函数
    schedule_delayed_work(&mydelayed_work, msecs_to_jiffies(1000));   //调度延时共享工作队列

    return 0;
}


static void __exit myworkqueue_exit(void)
{

}

module_init(myworkqueue_init);
module_exit(myworkqueue_exit);
MODULE_ALIAS("alternate_name");
MODULE_VERSION("version_string");
MODULE_DESCRIPTION("myworkqueue test ");
MODULE_AUTHOR("XYD");
MODULE_LICENSE("GPL");

