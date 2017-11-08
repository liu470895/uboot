#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//1�����ͷ�ļ� 
#include <linux/workqueue.h>


//2)ʵ��һ��tasklet��������
void  mywork_func(struct work_struct *work)
{
	printk("befor:%s is call!  work:%p\r\n",__FUNCTION__,work);
	ssleep(1);  
	printk("after:%s is call!  work:%p\r\n",__FUNCTION__,work);

	
}


//3)����һ�� work_struct �ṹ����,���ҽ��г�ʼ��
DECLARE_WORK(mywork, mywork_func);   //���岢�ҳ�ʼ����



static int __init myworkqueue_init(void)
{

	printk("%s is call!\r\n",__FUNCTION__);

	printk("&mywork:%p\r\n",&mywork);    //��� mywork������ַ
	
	//4)һ��װģ��Ϳ�ʼ���� 
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
* ���ն����� tail /var/log/messages   ���Բ鿴����־�ļ� ���10��
*/
