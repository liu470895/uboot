#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/io.h>
//��������
#define BTN_SIZE   4

//������
static char*keyname[] = {"key1", "key2", "key3", "key4"};

//������������'0'��ʾû�а�����'1'��ʾ������
static char keybuf[] = {"0000"};
int n;
//����gpio���
static int gpio[] = {
    EXYNOS4_GPX3(2), EXYNOS4_GPX3(3),
    EXYNOS4_GPX3(4), EXYNOS4_GPX3(5)
};

//�жϷ�����
irqreturn_t key_isr(int irq, void* dev)
{
    //��Ű���״̬
    int dn = 0;

    //ע��ʱ�򴫵���0~3����ת���ɵ�ָ�롣������л�ԭ
    int index = (int)dev;

    //�Ѷ�ȡ���Ľ��ȡ�߼��ǣ���Ϊ�������ʹ�����߼���'1'��ʾ���¡�
    dn = !gpio_get_value(gpio[index]);

    //�Ѱ���״̬���µ���Ӧ�İ�������
    keybuf[index] = dn + '0';
    n=index+1;
    //���������ʾ
    //printk("key%d %s\r\n", index+1, dn ? "down" : "up");
   
    return IRQ_HANDLED;
}


static ssize_t tiny4412_read (struct file *flp,
                              char __user *buff,
                              size_t count,
                              loff_t * off)
{
/*
    int ret = 0;

    //�û�����0��ֱ�ӷ���
    if(!count) {
        return 0;
    }

    //��������
    if(count > BTN_SIZE ) {
        count = BTN_SIZE;
    }

    //�������ݵ��û��ռ�
    ret = copy_to_user(buff, keybuf, count);
    if(ret) {
        printk("error:copy_to_user\r\n");
        return -EFAULT;
    }
*/
    return count;

}
long My_ioctl(struct file *file,unsigned int cmd, unsigned long arg)
{       
      copy_to_user((unsigned long*)arg,&n,1);
      n=0;
      return 0;
}
static const struct file_operations dev_fops = {
    .read   =   tiny4412_read,
    .owner  =   THIS_MODULE,
    .unlocked_ioctl = My_ioctl,
};


#define LEDS_MAJOR  255   //255

#define DEVICE_NAME  "key_btn"
static struct miscdevice misc = {
    .minor = LEDS_MAJOR, //���豸��
    .name  = DEVICE_NAME,//�豸��
    .fops  = &dev_fops,  //�ļ���������
};



static int __init btn_init(void)
{
    int ret;
    int irq;
    int i;
    int flags;

    flags = IRQ_TYPE_EDGE_BOTH; //����Ϊ˫�ߴ���
    for ( i = 0; i < 4 ; i++ ) {
        //�õ��жϺ�
        irq = gpio_to_irq(gpio[i]); //keyX

        //ע���ж�
        ret = request_irq(irq, key_isr, flags, keyname[i], (void*)i);
        if(ret < 0) {
            break;
        }
    }

    //�������ȫ���ɹ�������ע���Ѿ�ע����ж�
    if(ret < 0) {
        for ( --i; i; i-- ) {
            irq = gpio_to_irq(gpio[i]); //keyX
            disable_irq(irq);
            free_irq(irq, (void*)i);
        }

        return ret;
    }

    //ע�������豸
    ret = misc_register(&misc);       //ע������豸

    return ret;
}

static void __exit btn_exit(void)
{
    int i = 0;
    int irq;

    //ע���ж�
    for (i = 0; i < 4; i++) {
        irq = gpio_to_irq(gpio[i]); //keyX
        disable_irq(irq);
        free_irq(irq, (void*)i);
    }

    //ע�������豸
    misc_deregister(&misc);

    printk(KERN_EMERG "Goodbye,cruel world!, priority = 0\n");
}


module_init(btn_init);
module_exit(btn_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("XYD");
MODULE_DESCRIPTION("button");

