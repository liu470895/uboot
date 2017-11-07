#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/io.h>
//按键数量
#define BTN_SIZE   4

//按键名
static char*keyname[] = {"key1", "key2", "key3", "key4"};

//按键缓冲区，'0'表示没有按键，'1'表示按下了
static char keybuf[] = {"0000"};
int n;
//定义gpio编号
static int gpio[] = {
    EXYNOS4_GPX3(2), EXYNOS4_GPX3(3),
    EXYNOS4_GPX3(4), EXYNOS4_GPX3(5)
};

//中断服务函数
irqreturn_t key_isr(int irq, void* dev)
{
    //存放按键状态
    int dn = 0;

    //注册时候传递了0~3数字转换成的指针。这里进行还原
    int index = (int)dev;

    //把读取到的结果取逻辑非，因为程序设计使用正逻辑。'1'表示按下。
    dn = !gpio_get_value(gpio[index]);

    //把按键状态更新到对应的按缓冲中
    keybuf[index] = dn + '0';
    n=index+1;
    //输出按键提示
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

    //用户传递0，直接返回
    if(!count) {
        return 0;
    }

    //修正参数
    if(count > BTN_SIZE ) {
        count = BTN_SIZE;
    }

    //复制数据到用户空间
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
    .minor = LEDS_MAJOR, //次设备号
    .name  = DEVICE_NAME,//设备名
    .fops  = &dev_fops,  //文件操作方法
};



static int __init btn_init(void)
{
    int ret;
    int irq;
    int i;
    int flags;

    flags = IRQ_TYPE_EDGE_BOTH; //设置为双边触发
    for ( i = 0; i < 4 ; i++ ) {
        //得到中断号
        irq = gpio_to_irq(gpio[i]); //keyX

        //注册中断
        ret = request_irq(irq, key_isr, flags, keyname[i], (void*)i);
        if(ret < 0) {
            break;
        }
    }

    //如果不是全部成功，则反向注销已经注册的中断
    if(ret < 0) {
        for ( --i; i; i-- ) {
            irq = gpio_to_irq(gpio[i]); //keyX
            disable_irq(irq);
            free_irq(irq, (void*)i);
        }

        return ret;
    }

    //注册杂项设备
    ret = misc_register(&misc);       //注册混杂设备

    return ret;
}

static void __exit btn_exit(void)
{
    int i = 0;
    int irq;

    //注销中断
    for (i = 0; i < 4; i++) {
        irq = gpio_to_irq(gpio[i]); //keyX
        disable_irq(irq);
        free_irq(irq, (void*)i);
    }

    //注销杂项设备
    misc_deregister(&misc);

    printk(KERN_EMERG "Goodbye,cruel world!, priority = 0\n");
}


module_init(btn_init);
module_exit(btn_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("XYD");
MODULE_DESCRIPTION("button");

