//����汾������������:
//1) ����Linux�ں��жϱ�̷���
//2) ����dev_id��ʹ�÷���
//3) ����ʹ����������˼������д����������Լ򻯴���
//4) ����������̫���ƣ�����û�������������������Լ��첽֪ͨ�������ǡ��������ܡ�����Ŀγ̽��⡣
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>   //�ж�ע�� ��ע��,tasklet
#include <linux/gpio.h>        //gpio��صĺ���
#include <linux/delay.h>       //msleep����
#include <linux/poll.h>        //poll�ӿڵ���ض��弰����


//1)��һ��: ������ͷ�ļ�
#include <linux/wait.h>        //�ȴ�����������ݽṹ������
#include <linux/sched.h>       //����״̬�궨��




//2)����һ���ȴ�����ͷ���������ҳ�ʼ��
DECLARE_WAIT_QUEUE_HEAD(wq);

//��������
#define KEY_SIZE        (4)

//��д��������ȽϺ�һ�ֶ����ǲ��������������˼�롣
//��������4������ ��ÿ������������һ���ģ�дһ������������������Ҫ��Ԫ�ض���ͬ��
//���ԣ���һ��������Ҫ��Ԫ�ؽ��з�װ��Ȼ�����������Ƕ��������װ��ʵ��
//C����û���������ǽṹ�����Ϳ��԰Ѱ�������ɽṹ��
struct key_info {
    int key_gpio;          //����GPIO�ڱ��
    int key_num;           //������Ŵ�0��ʼ
    const char* keyname;   //��������
    struct tasklet_struct keytasklet; //ÿ��������Ӧ��tasklet
};

//����������4������������ struct key_info ���͵�����
static struct key_info keys[] = {
    {EXYNOS4_GPX3(2), 0, "key1"},
    {EXYNOS4_GPX3(3), 1, "key2"},
    {EXYNOS4_GPX3(4), 2, "key3"},
    {EXYNOS4_GPX3(5), 3, "key4"}
};



//����������
static char keybuf[KEY_SIZE] = {"0000"};    // '0'  ��ʾ����û�а�����'1'��ʾ����������


//����һ��ȫ�ֱ���������ʶ�Ƿ��а�������
static int press = 0;

/*
 * Kx�жϷ������:
 * �������ɿ���������жϣ���ִ�����������
 * ����Ҫ��ȡ�ð�����״̬���������������û�ͨ���û����API�ӿڶ�ȡ����״̬
 * ���ԣ����������:1)�жϰ��������ɿ�; 2)�Ѱ���״̬�洢����
 */
irqreturn_t   keys_handlers(int irq, void * dev_id)
{

	//��Ϊע��ʱ�򴫵ݽ�������ÿ��������Ԫ���׵�ַ�������� struct key_info *
    //���ԣ�������л�ԭ����ԭ��Ϳ��Ը������ָ��ȡ�ñ�������ȫ��Ϣ
    struct key_info *pdev_data  = (struct key_info *)dev_id;

	//�����жϵװ벿:����tasklet
    tasklet_schedule(&pdev_data->keytasklet);

    return IRQ_HANDLED;
}



//ʵ��tasklet��������:
//�������ִ�й������ǿ��Ա�Ӳ���ж�����ֹ�ģ���������Ӱ��ϵͳʵʱ��
void key_tasklet_func(unsigned long data)
{

    int dn;       //��Ű���״̬
    //��Ϊע��ʱ�򴫵ݽ�������ÿ��������Ԫ���׵�ַ�������� struct key_info *
    //���ԣ�������л�ԭ����ԭ��Ϳ��Ը������ָ��ȡ�ñ�������ȫ��Ϣ
    struct key_info *pdev_data  = (struct key_info *)data;

    //�ж��ǰ����жϻ����ɿ��ж�:����ͨ����ȡIO��ƽ״̬���ж�
    dn = gpio_get_value(pdev_data->key_gpio);    //Ӳ����ƣ����·��� 0���ɿ����� 1

    //�洢����״̬��������������
    keybuf[pdev_data->key_num] = '0' + !dn;     //���·��ء�0��

    //4) �ڵȴ����������ĵط�����  wake_up*�����������ߵĽ���
    press = 1;      //��־�а��������Ĳ���
    wake_up_interruptible(&wq);   //֪ͨ�ں˵�wq�������ͷ�ϵ�����ȥ��ѯÿһ�����ߵĽ��� �������Ƿ������档
    //������̵ȴ���������û�����棬��������ߡ�

}


//�������ļ����������ľ���ʵ�ִ���
static int xxx_open(struct inode *pinode, struct file *pfile )
{
    printk(KERN_EMERG"file:%s\r\nline:%d, %s is call\n", __FILE__, __LINE__, __FUNCTION__);

    return 0;
}

//�û��ռ�: ssize_t read(int fd, void *buf, size_t count);
//�ں˿ռ䲻��ֱ��ʹ���û��ռ䴫��������ָ�� ����Ϊ���а�ȫ����
//Ҫ�������ݸ��û��ռ䣬����ʹ���ں�ר�õĸ��ƺ��� copy_to_user
//����������ȼ��ָ���Ƿ�Ϸ����Ϸ��ٽ��и���
static ssize_t xxx_read(struct file *pfile,
                        char __user *buf,
                        size_t count, loff_t *poff)
{
    //���в�������
    if(count > KEY_SIZE) {
        count = KEY_SIZE;
    }

    //�жϵ�ǰ�Ƿ��а�������
    if(press == 0) {
        //�ж� pfile->f_flags ��Ա�Ƿ����� O_NONBLOCK
        if(pfile->f_flags & O_NONBLOCK) { //��ʾ�û��ռ�ʹ�÷�������
            return - EAGAIN;              //����һ�������룬�����û��ռ�������ٳ��Զ�ȡ
        }
        //������ʽ�򿪣�û�����ݾ����ߣ������Ϸ���
        else {
            //���ߣ��ȴ��а����������ѽ��̡�
            //wait_event(wq,press);  //�������ߣ�ֱ��press�����
            wait_event_interruptible(wq, press); //�������ߣ�ֱ��press�����
        }
    }

    //�尴�������־:�����е�����ı�ʾ�Ѿ������˰�������
    press = 0;  //һ������0������ �´�readʱ���������




    //�������ݸ��û��ռ�
    if (copy_to_user(buf, keybuf, count) ) {
        printk(KERN_EMERG" error: copy_to_user\r\n");
        return -EFAULT;
    }
    return count;
}


static int xxx_release (struct inode * pinode, struct file * pfile)
{
    printk(KERN_EMERG"file:%s\r\nline:%d, %s is call\n", __FILE__, __LINE__, __FUNCTION__);
    return 0;
}


//��ѯ�ӿ�
//�������ִ��ʱ�򲻻�����������
unsigned int xxx_poll (struct file *pfile, struct poll_table_struct *wait)
{
    unsigned int mask = 0;       //һ��Ҫ��ʼ��Ϊ0����Ϊmask�Ǿֲ�����

    //1)���� poll_wait �ѵ�ǰ������ӵ��ȴ�������
    poll_wait(pfile, &wq, wait); //�����������������̵�����

    //2)�����豸״̬����(�Ƿ�ɶ���д��־)
    if(press) {
        mask = POLLIN | POLLRDNORM;
    }

    return mask;
}



//�ļ�������������ָ��
static const struct file_operations mymisc_fops = {
    .open           =   xxx_open,
    .read           =   xxx_read,
    .release        =   xxx_release,
    .poll           =   xxx_poll,
};


//������Ľṹ
static struct miscdevice  misc = {
    .minor = 255,
    .name  = "mybtns",     ///devĿ¼�µ��豸��
    .fops  = &mymisc_fops,
};


static int __init mymisdevice_init(void)
{
    int ret;
    int irq;
    int i;

    //�����ж�����:�ǹ����жϣ������أ��½�����
    unsigned long flags = IRQF_DISABLED  |
                          IRQF_TRIGGER_FALLING |
                          IRQF_TRIGGER_RISING;

    //1)����������4������ ��Ҫע��4��
    for(i = 0; i < 4; i++) {

		//��ʼ��ÿ������ tasklet�ṹ
		tasklet_init(&keys[i].keytasklet, key_tasklet_func,&keys[i]);
		
        //�õ�K1�жϱ��
        irq = gpio_to_irq( keys[i].key_gpio);

        //ע��key1�ж�:dev_id�ֱ�� 1~4��4����ַ
        //keys_handlers(irq,(void*)&keys[0])
        //keys_handlers(irq,(void*)&keys[1])
        //keys_handlers(irq,(void*)&keys[2])
        //keys_handlers(irq,(void*)&keys[3])
        ret = request_irq(irq, keys_handlers, flags, keys[i].keyname, &keys[i]);
        if(ret < 0) {
            break;
        }
    }

    //���ǰ��ע����ʧ�ܵģ���ȫ���ͷ��ж�:ע��ֻ�ͷ��Ѿ�ע��ɹ����жϡ�
    if(ret < 0) {
        for(--i; i > 0 ; i--) {
            //1)����������4������ ��Ҫע��4��
            irq = gpio_to_irq( keys[i].key_gpio);               //�õ�Kx�жϱ��
            free_irq(irq, &keys[i]);                            //�ͷ��ж�
        }

        //ʧ���ˣ��ͷ��жϺź�ֱ�ӷ���
        return ret;
    }



    //ע����Ľṹ
    ret = misc_register(&misc);
    if(ret < 0) {
        printk(KERN_EMERG"misc_register error\n");
        return ret;
    }

    printk(KERN_EMERG"misc_register  ok\n");


    return 0;
}


static void __exit mymisdevice_exit(void)
{
    int ret;
    int irq;
    int i;

    //ע�����Ľṹ
    ret = misc_deregister(&misc);
    if(ret < 0) {
        printk(KERN_EMERG"misc_deregister error\n");
        return ;
    }

    for(i = 0; i < 4; i++) {
        irq = gpio_to_irq( keys[i].key_gpio);               //�õ�Kx�жϱ��
        free_irq(irq, &keys[i]);                            //�ͷ��ж�

		tasklet_kill(&keys[i].keytasklet);                  //ȡ��tasklet
    }

    printk(KERN_EMERG"misc_deregister ok\n");

}

module_init(mymisdevice_init);
module_exit(mymisdevice_exit);
MODULE_LICENSE("GPL");



