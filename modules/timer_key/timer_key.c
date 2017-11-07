#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/io.h>

//wait 
#include <linux/wait.h>
#include <linux/sched.h>

#include <linux/poll.h>
// 按下 ‘1’  抬起 ‘0’
//°ŽŒüÊýÁ¿
#define BTN_SIZE   4
//定义等待队列队列头变量
wait_queue_head_t  my_wq;
//定义是否有结果的标志变量
int press =0;
//°ŽŒüÃû
static char*keyname[] = {"key1", "key2", "key3", "key4"};

//°ŽŒü»º³åÇø£¬'0'±íÊŸÃ»ÓÐ°ŽŒü£¬'1'±íÊŸ°ŽÏÂÁË
static char keybuf[] = {"0000"};
int n;
//¶šÒågpio±àºÅ
static int gpio[] = {
    EXYNOS4_GPX3(2), EXYNOS4_GPX3(3),
    EXYNOS4_GPX3(4), EXYNOS4_GPX3(5)
};

//ÖÐ¶Ï·þÎñº¯Êý
irqreturn_t key_isr(int irq, void* dev)
{
    //Žæ·Å°ŽŒü×ŽÌ¬
    int dn = 0;

    //×¢²áÊ±ºòŽ«µÝÁË0~3Êý×Ö×ª»»³ÉµÄÖžÕë¡£ÕâÀïœøÐÐ»¹Ô­
    int index = (int)dev;

    //°Ñ¶ÁÈ¡µœµÄœá¹ûÈ¡ÂßŒ­·Ç£¬ÒòÎª³ÌÐòÉèŒÆÊ¹ÓÃÕýÂßŒ­¡£'1'±íÊŸ°ŽÏÂ¡£
    dn = !gpio_get_value(gpio[index]);

    //°Ñ°ŽŒü×ŽÌ¬žüÐÂµœ¶ÔÓŠµÄ°Ž»º³åÖÐ
    keybuf[index] = dn + '0';
    n=index+1;
    //Êä³ö°ŽŒüÌáÊŸ
    //printk("key%d %s\r\n", index+1, dn ? "down" : "up");
   
     //唤醒进程
   wake_up_interruptible(&my_wq);
   press =1; //结果标志非0
   return IRQ_HANDLED;
}


static ssize_t tiny4412_read (struct file *flp,
                              char __user *buff,
                              size_t count,
                              loff_t * off)
{
/*
    int ret = 0;

    //ÓÃ»§Ž«µÝ0£¬Ö±œÓ·µ»Ø
    if(!count) {
        return 0;
    }

    //ÐÞÕý²ÎÊý
    if(count > BTN_SIZE ) {
        count = BTN_SIZE;
    }

    //žŽÖÆÊýŸÝµœÓÃ»§¿ÕŒä
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
      press =0;
      copy_to_user((unsigned long*)arg,&n,1);
      n=0;
      return 0;
}

unsigned int My_poll(struct file *myfile, struct poll_table_struct *my_poll)
{
   //  调取把进程添加到等待队列上，但不休眠
   poll_wait(myfile,&my_wq,my_poll);
   if(press ==1)//有按键动作
   {
       return POLLIN | POLLRDNORM;
   }
   else
     return 0;
}

static const struct file_operations dev_fops = {
    .read   =   tiny4412_read,
    .owner  =   THIS_MODULE,
    .unlocked_ioctl = My_ioctl,
    .poll    = My_poll,
};


#define LEDS_MAJOR  255   //255

#define DEVICE_NAME  "timer_key"
static struct miscdevice misc = {
    .minor = LEDS_MAJOR, //ŽÎÉè±žºÅ
    .name  = DEVICE_NAME,//Éè±žÃû
    .fops  = &dev_fops,  //ÎÄŒþ²Ù×÷·œ·š
};



static int __init btn_init(void)
{
    int ret;
    int irq;
    int i;
    int flags;

    flags = IRQ_TYPE_EDGE_BOTH; //ÉèÖÃÎªË«±ßŽ¥·¢
    for ( i = 0; i < 4 ; i++ ) {
        //µÃµœÖÐ¶ÏºÅ
        irq = gpio_to_irq(gpio[i]); //keyX

        //×¢²áÖÐ¶Ï
        ret = request_irq(irq, key_isr, flags, keyname[i], (void*)i);
        if(ret < 0) {
            break;
        }
    }

    //Èç¹û²»ÊÇÈ«²¿³É¹Š£¬Ôò·ŽÏò×¢ÏúÒÑŸ­×¢²áµÄÖÐ¶Ï
    if(ret < 0) {
        for ( --i; i; i-- ) {
            irq = gpio_to_irq(gpio[i]); //keyX
            disable_irq(irq);
            free_irq(irq, (void*)i);
        }

        return ret;
    }
    //初始化等待队列头
   init_waitqueue_head(&my_wq);
    //×¢²áÔÓÏîÉè±ž
    ret = misc_register(&misc);       //×¢²á»ìÔÓÉè±ž

    return ret;
}

static void __exit btn_exit(void)
{
    int i = 0;
    int irq;

    //×¢ÏúÖÐ¶Ï
    for (i = 0; i < 4; i++) {
        irq = gpio_to_irq(gpio[i]); //keyX
        disable_irq(irq);
        free_irq(irq, (void*)i);
    }

    //×¢ÏúÔÓÏîÉè±ž
    misc_deregister(&misc);

    printk(KERN_EMERG "Goodbye,cruel world!, priority = 0\n");
}


module_init(btn_init);
module_exit(btn_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("XYD");
MODULE_DESCRIPTION("button");

