#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//1）添加头文件 
#include <linux/workqueue.h>


//2)实现一个tasklet工作函数
void  mywork_func(struct work_struct *work)
{
	printk("befor:%s is call!  work:%p\r\n",__FUNCTION__,work);
	ssleep(1);  
	printk("after:%s is call!  work:%p\r\n",__FUNCTION__,work);

	
}


//3)定义一个 work_struct 结构变量,并且进行初始化
DECLARE_WORK(mywork, mywork_func);   //定义并且初始化了



static int __init myworkqueue_init(void)
{

	printk("%s is call!\r\n",__FUNCTION__);

	printk("&mywork:%p\r\n",&mywork);    //输出 mywork变量地址
	
	//4)一安装模块就开始调度 
	schedule_work(&mywork);

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



/*
* 在终端输入 tail /var/log/messages   可以查看到日志文件 最后10行
*/
