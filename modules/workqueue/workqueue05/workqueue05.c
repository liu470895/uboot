#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//0.添加头文件
#include <linux/workqueue.h>

//自定义数据结构
struct mydata {
	int x;
	int y;
    struct delayed_work mydelayed_work;     //嵌入一个延时工作
	int z;  
};



//定义一个自定义数据结构变量
struct mydata  g_var;


//1)实现一个工作函数
void  mywork_func(struct work_struct *mywork)
{
    //先得到struct mydata结构中 mydelayed_work成员 的地址
    struct delayed_work  *pmdw = (struct delayed_work*)container_of(mywork, struct delayed_work, work);

    //先得到struct mydata结构中 mydelayed_work成员 的地址
    struct mydata  *p = (struct mydata*)container_of(pmdw, struct mydata, mydelayed_work);

	
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);

    //这个使用全局变量来访问x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x, g_var.y, g_var.z);

    //这个使用函数的形式参数来访问x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x, p->y, p->z);
}





static int __init myworkqueue_init(void)
{

	//初始化用户结构成员
	g_var.x=123;
	g_var.y=456;
	g_var.z=789;


    //3. 动态初始化延时工作结构
    INIT_DELAYED_WORK(&g_var.mydelayed_work, mywork_func);
	
    printk("%s is call!\r\n", __FUNCTION__);
    //3.一安装模块就开始调度,1秒后开始调度工作函数
    schedule_delayed_work(&g_var.mydelayed_work, msecs_to_jiffies(1000));   //调度延时共享工作队列

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

