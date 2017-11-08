#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//0.���ͷ�ļ�
#include <linux/workqueue.h>

//1)ʵ��һ����������
void  mywork_func(struct work_struct *mywork)
{
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);
}


//2)����һ����ʱ����
//struct delayed_work mydelayed_work;            //��̬����
DECLARE_DELAYED_WORK(mydelayed_work, mywork_func); //��̬����


static int __init myworkqueue_init(void)
{

    printk("%s is call!\r\n", __FUNCTION__);
    //3.һ��װģ��Ϳ�ʼ����,1���ʼ���ȹ�������
    schedule_delayed_work(&mydelayed_work, msecs_to_jiffies(1000));   //������ʱ����������

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

