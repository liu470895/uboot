#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//1）添加头文件
#include <linux/workqueue.h>


#define METHER_0   0    //方式1
#define METHER_1   1    //方式2
#define METHER_2   2    //方式3

#define METHER     METHER_2  

//定义一个结构体类型
#if (METHER == METHER_0)
struct mydata {
    struct work_struct work;
    int x;
    int y;
    int z;
};

#elif(METHER == METHER_1)
//思考: work_struct 不是嵌入式在第一个成员中，怎么办?
struct mydata {
    int x;
    struct work_struct work;
    int y;
    int z;
};

#elif(METHER == METHER_2)
//思考: work_struct 不是嵌入式在第一个成员中，怎么办?
struct mydata {
    int x;
    int y;
	struct work_struct work;

    int z;
};

#endif

struct mydata g_var;


#if(METHER == METHER_0)

//2)实现一个tasklet工作函数
void  mywork_func(struct work_struct *work)
{
	struct mydata  *p = (struct mydata*)work;
	
    printk("%s is call!  work:%p\r\n", __FUNCTION__, work);

	//这个使用全局变量来访问x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x,g_var.y,g_var.z);

	//这个使用函数的形式参数来访问x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x,p->y,p->z);
}

#elif(METHER == METHER_1)

//2)实现一个tasklet工作函数
void  mywork_func(struct work_struct *work)
{
	//由于这个测试work成员不是在 mydata结构中的第一个成员，前面还有一个int成员。
	struct mydata  *p = (struct mydata*)((unsigned int)work - 4);
	
    printk("%s is call!  work:%p\r\n", __FUNCTION__, work);

	//这个使用全局变量来访问x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x,g_var.y,g_var.z);

	//这个使用函数的形式参数来访问x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x,p->y,p->z);	
}


#elif(METHER == METHER_2)

//2)实现一个tasklet工作函数
void  mywork_func(struct work_struct *mywork)
{
	//由于这个测试work成员不是在 mydata结构中的第一个成员，前面还有2个int成员。
	//struct mydata  *p = (struct mydata*)((unsigned int)mywork - 8);
	struct mydata  *p = (struct mydata*)container_of(mywork, struct mydata, work);
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);

	//这个使用全局变量来访问x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x,g_var.y,g_var.z);

	//这个使用函数的形式参数来访问x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x,p->y,p->z);	
}


#endif



static int __init myworkqueue_init(void)
{


    printk("%s is call!\r\n", __FUNCTION__);
    printk("&g_var.work:%p\r\n", &g_var.work);   //输出 mywork变量地址

    g_var.x = 123;
    g_var.y = 456;
    g_var.z = 789;
	
    //3)使用动态初始化方法
    INIT_WORK(&g_var.work, mywork_func);

    //4)一安装模块就开始调度
    schedule_work(&g_var.work);

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
 