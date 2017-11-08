#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//0.���ͷ�ļ�
#include <linux/workqueue.h>

//�Զ������ݽṹ
struct mydata {
	int x;
	int y;
    struct delayed_work mydelayed_work;     //Ƕ��һ����ʱ����
	int z;  
};



//����һ���Զ������ݽṹ����
struct mydata  g_var;


//1)ʵ��һ����������
void  mywork_func(struct work_struct *mywork)
{
    //�ȵõ�struct mydata�ṹ�� mydelayed_work��Ա �ĵ�ַ
    struct delayed_work  *pmdw = (struct delayed_work*)container_of(mywork, struct delayed_work, work);

    //�ȵõ�struct mydata�ṹ�� mydelayed_work��Ա �ĵ�ַ
    struct mydata  *p = (struct mydata*)container_of(pmdw, struct mydata, mydelayed_work);

	
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);

    //���ʹ��ȫ�ֱ���������x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x, g_var.y, g_var.z);

    //���ʹ�ú�������ʽ����������x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x, p->y, p->z);
}





static int __init myworkqueue_init(void)
{

	//��ʼ���û��ṹ��Ա
	g_var.x=123;
	g_var.y=456;
	g_var.z=789;


    //3. ��̬��ʼ����ʱ�����ṹ
    INIT_DELAYED_WORK(&g_var.mydelayed_work, mywork_func);
	
    printk("%s is call!\r\n", __FUNCTION__);
    //3.һ��װģ��Ϳ�ʼ����,1���ʼ���ȹ�������
    schedule_delayed_work(&g_var.mydelayed_work, msecs_to_jiffies(1000));   //������ʱ����������

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

