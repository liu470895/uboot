#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
//1�����ͷ�ļ�
#include <linux/workqueue.h>


#define METHER_0   0    //��ʽ1
#define METHER_1   1    //��ʽ2
#define METHER_2   2    //��ʽ3

#define METHER     METHER_2  

//����һ���ṹ������
#if (METHER == METHER_0)
struct mydata {
    struct work_struct work;
    int x;
    int y;
    int z;
};

#elif(METHER == METHER_1)
//˼��: work_struct ����Ƕ��ʽ�ڵ�һ����Ա�У���ô��?
struct mydata {
    int x;
    struct work_struct work;
    int y;
    int z;
};

#elif(METHER == METHER_2)
//˼��: work_struct ����Ƕ��ʽ�ڵ�һ����Ա�У���ô��?
struct mydata {
    int x;
    int y;
	struct work_struct work;

    int z;
};

#endif

struct mydata g_var;


#if(METHER == METHER_0)

//2)ʵ��һ��tasklet��������
void  mywork_func(struct work_struct *work)
{
	struct mydata  *p = (struct mydata*)work;
	
    printk("%s is call!  work:%p\r\n", __FUNCTION__, work);

	//���ʹ��ȫ�ֱ���������x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x,g_var.y,g_var.z);

	//���ʹ�ú�������ʽ����������x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x,p->y,p->z);
}

#elif(METHER == METHER_1)

//2)ʵ��һ��tasklet��������
void  mywork_func(struct work_struct *work)
{
	//�����������work��Ա������ mydata�ṹ�еĵ�һ����Ա��ǰ�滹��һ��int��Ա��
	struct mydata  *p = (struct mydata*)((unsigned int)work - 4);
	
    printk("%s is call!  work:%p\r\n", __FUNCTION__, work);

	//���ʹ��ȫ�ֱ���������x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x,g_var.y,g_var.z);

	//���ʹ�ú�������ʽ����������x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x,p->y,p->z);	
}


#elif(METHER == METHER_2)

//2)ʵ��һ��tasklet��������
void  mywork_func(struct work_struct *mywork)
{
	//�����������work��Ա������ mydata�ṹ�еĵ�һ����Ա��ǰ�滹��2��int��Ա��
	//struct mydata  *p = (struct mydata*)((unsigned int)mywork - 8);
	struct mydata  *p = (struct mydata*)container_of(mywork, struct mydata, work);
    printk("%s is call!  work:%p\r\n", __FUNCTION__, mywork);

	//���ʹ��ȫ�ֱ���������x,y,z
    printk("g_var.x:%d,g_var.y:%d,g_var.z:%d,\r\n", g_var.x,g_var.y,g_var.z);

	//���ʹ�ú�������ʽ����������x,y,z
    printk("p->x:%d,p->y:%d,p->z:%d,\r\n", p->x,p->y,p->z);	
}


#endif



static int __init myworkqueue_init(void)
{


    printk("%s is call!\r\n", __FUNCTION__);
    printk("&g_var.work:%p\r\n", &g_var.work);   //��� mywork������ַ

    g_var.x = 123;
    g_var.y = 456;
    g_var.z = 789;
	
    //3)ʹ�ö�̬��ʼ������
    INIT_WORK(&g_var.work, mywork_func);

    //4)һ��װģ��Ϳ�ʼ����
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
 