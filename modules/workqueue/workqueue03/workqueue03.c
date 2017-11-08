#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//0.���ͷ�ļ�
#include <linux/workqueue.h>


struct mydata {
    int x;
    int y;
    struct work_struct work;

    int z;
};

struct mydata g_var;


//1.����һ����������ָ��
struct workqueue_struct * mywq;


//ʵ��һ��tasklet��������
void  mywork_func(struct work_struct *mywork)
{

    struct mydata  *p = (struct mydata*)container_of(mywork, struct mydata, work);
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);

    //���ʹ��ȫ�ֱ���������x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x, g_var.y, g_var.z);

    //���ʹ�ú�������ʽ����������x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x, p->y, p->z);
}

static int __init myworkqueue_init(void)
{

    printk("%s is call!\r\n", __FUNCTION__);
    printk("&g_var.work:%p\r\n", &g_var.work);   //��� mywork������ַ

    g_var.x = 123;
    g_var.y = 456;
    g_var.z = 789;

    //2. ������������
    mywq = create_workqueue("mywq");
    if(mywq == NULL) {
        printk("create_workqueue error\r\n");
        return -ENOMEM;
    }

    printk("create_workqueue ok\r\n");


    //4.ʹ�ö�̬��ʼ������
    INIT_WORK(&g_var.work, mywork_func);

    //5.һ��װģ��Ϳ�ʼ����
    //schedule_work(&g_var.work);    //���ȹ���������
    queue_work(mywq, &g_var.work);   //�����Զ��幤������


    return 0;
}

static void __exit myworkqueue_exit(void)
{
    //3. ���ٹ�������
    destroy_workqueue(mywq);

}

module_init(myworkqueue_init);
module_exit(myworkqueue_exit);
MODULE_ALIAS("alternate_name");
MODULE_VERSION("version_string");
MODULE_DESCRIPTION("myworkqueue test ");
MODULE_AUTHOR("XYD");
MODULE_LICENSE("GPL");

