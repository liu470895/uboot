#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//0.添加头文件
#include <linux/workqueue.h>


struct mydata {
    int x;
    int y;
    struct work_struct work;

    int z;
};

struct mydata g_var;


//1.定义一个工作队列指针
struct workqueue_struct * mywq;


//实现一个tasklet工作函数
void  mywork_func(struct work_struct *mywork)
{

    struct mydata  *p = (struct mydata*)container_of(mywork, struct mydata, work);
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);

    //这个使用全局变量来访问x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x, g_var.y, g_var.z);

    //这个使用函数的形式参数来访问x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x, p->y, p->z);
}

static int __init myworkqueue_init(void)
{

    printk("%s is call!\r\n", __FUNCTION__);
    printk("&g_var.work:%p\r\n", &g_var.work);   //输出 mywork变量地址

    g_var.x = 123;
    g_var.y = 456;
    g_var.z = 789;

    //2. 创建工作队列
    mywq = create_workqueue("mywq");
    if(mywq == NULL) {
        printk("create_workqueue error\r\n");
        return -ENOMEM;
    }

    printk("create_workqueue ok\r\n");


    //4.使用动态初始化方法
    INIT_WORK(&g_var.work, mywork_func);

    //5.一安装模块就开始调度
    //schedule_work(&g_var.work);    //调度共享工作队列
    queue_work(mywq, &g_var.work);   //调度自定义工作队列


    return 0;
}

static void __exit myworkqueue_exit(void)
{
    //3. 销毁工作队列
    destroy_workqueue(mywq);

}

module_init(myworkqueue_init);
module_exit(myworkqueue_exit);
MODULE_ALIAS("alternate_name");
MODULE_VERSION("version_string");
MODULE_DESCRIPTION("myworkqueue test ");
MODULE_AUTHOR("XYD");
MODULE_LICENSE("GPL");

