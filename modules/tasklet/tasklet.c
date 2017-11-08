//这个版本按键驱动程序:
//1) 掌握Linux内核中断编程方法
//2) 掌握dev_id的使用方法
//3) 掌握使用面向对象的思想来编写驱动程序可以简化代码
//4) 本驱动还不太完善，比如没有做按键消除抖动，以及异步通知，阻塞非、阻塞功能。后面的课程讲解。
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>   //中断注册 ，注销,tasklet
#include <linux/gpio.h>        //gpio相关的函数
#include <linux/delay.h>       //msleep函数
#include <linux/poll.h>        //poll接口的相关定义及函数


//1)第一步: 添加相关头文件
#include <linux/wait.h>        //等待队列相关数据结构及函数
#include <linux/sched.h>       //进程状态宏定义




//2)定义一个等待队列头变量，并且初始化
DECLARE_WAIT_QUEUE_HEAD(wq);

//按键数量
#define KEY_SIZE        (4)

//编写驱动程序比较好一种断想是采用面向对象的设计思想。
//开发板有4个按键 ，每个按键功能是一样的，写一个按键驱动代码所需要的元素都相同。
//所以，把一个按键需要的元素进行封装，然后多个按键就是定义这个封装的实例
//C语言没有类概念，但是结构体概念，就可以把按键定义成结构体
struct key_info {
    int key_gpio;          //按键GPIO口编号
    int key_num;           //按键编号从0开始
    const char* keyname;   //按键名字
    struct tasklet_struct keytasklet; //每个按键对应的tasklet
};

//开发板上有4个按键，定义 struct key_info 类型的数组
static struct key_info keys[] = {
    {EXYNOS4_GPX3(2), 0, "key1"},
    {EXYNOS4_GPX3(3), 1, "key2"},
    {EXYNOS4_GPX3(4), 2, "key3"},
    {EXYNOS4_GPX3(5), 3, "key4"}
};



//按键缓冲区
static char keybuf[KEY_SIZE] = {"0000"};    // '0'  表示按键没有按键，'1'表示按键按下了


//定义一个全局变量用来标识是否有按键动作
static int press = 0;

/*
 * Kx中断服务程序:
 * 当按下松开都会产生中断，都执行这个函数。
 * 程序要把取得按键的状态，保存起来，让用户通过用户编程API接口读取按键状态
 * 所以，函数中完成:1)判断按键还是松开; 2)把按键状态存储起来
 */
irqreturn_t   keys_handlers(int irq, void * dev_id)
{

	//因为注册时候传递进来的是每个按键的元素首地址，类型是 struct key_info *
    //所以，这里进行还原，还原后就可以根据这个指针取得本按键的全信息
    struct key_info *pdev_data  = (struct key_info *)dev_id;

	//启动中断底半部:调度tasklet
    tasklet_schedule(&pdev_data->keytasklet);

    return IRQ_HANDLED;
}



//实现tasklet工作函数:
//这个函数执行过程中是可以被硬件中断所中止的，这样不会影响系统实时性
void key_tasklet_func(unsigned long data)
{

    int dn;       //存放按键状态
    //因为注册时候传递进来的是每个按键的元素首地址，类型是 struct key_info *
    //所以，这里进行还原，还原后就可以根据这个指针取得本按键的全信息
    struct key_info *pdev_data  = (struct key_info *)data;

    //判断是按键中断还是松开中断:可以通过读取IO电平状态来判断
    dn = gpio_get_value(pdev_data->key_gpio);    //硬件设计，按下返回 0，松开返回 1

    //存储按键状态到按键缓冲区中
    keybuf[pdev_data->key_num] = '0' + !dn;     //按下返回‘0’

    //4) 在等待条件变成真的地方调用  wake_up*函数唤醒休眠的进程
    press = 1;      //标志有按键动作的产生
    wake_up_interruptible(&wq);   //通知内核到wq这个队列头上的链表去查询每一个休眠的进程 的条件是否变成了真。
    //如果进程等待的条件还没有是真，则继续休眠。

}


//以下是文件操作方法的具体实现代码
static int xxx_open(struct inode *pinode, struct file *pfile )
{
    printk(KERN_EMERG"file:%s\r\nline:%d, %s is call\n", __FILE__, __LINE__, __FUNCTION__);

    return 0;
}

//用户空间: ssize_t read(int fd, void *buf, size_t count);
//内核空间不能直接使用用户空间传递下来的指针 ，因为会有安全隐患
//要复制数据给用户空间，必须使用内核专用的复制函数 copy_to_user
//这个函数会先检测指针是否合法，合法再进行复制
static ssize_t xxx_read(struct file *pfile,
                        char __user *buf,
                        size_t count, loff_t *poff)
{
    //进行参数修正
    if(count > KEY_SIZE) {
        count = KEY_SIZE;
    }

    //判断当前是否有按键动作
    if(press == 0) {
        //判断 pfile->f_flags 成员是否设置 O_NONBLOCK
        if(pfile->f_flags & O_NONBLOCK) { //表示用户空间使用非阻塞打开
            return - EAGAIN;              //返回一个错误码，告诉用户空间你可以再尝试读取
        }
        //阻塞方式打开，没有数据就休眠，不马上返回
        else {
            //休眠，等待有按键动作唤醒进程。
            //wait_event(wq,press);  //进程休眠，直到press变成真
            wait_event_interruptible(wq, press); //进程休眠，直到press变成真
        }
    }

    //清按键处理标志:能运行到这里的表示已经发生了按键动作
    press = 0;  //一定在清0，否则 下次read时候出会问题




    //复制数据给用户空间
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


//轮询接口
//这个函数执行时候不会引起阻塞，
unsigned int xxx_poll (struct file *pfile, struct poll_table_struct *wait)
{
    unsigned int mask = 0;       //一定要初始化为0，因为mask是局部变量

    //1)调用 poll_wait 把当前进程添加到等待队列中
    poll_wait(pfile, &wq, wait); //这个函数不会引起进程的阻塞

    //2)返回设备状态掩码(是否可读可写标志)
    if(press) {
        mask = POLLIN | POLLRDNORM;
    }

    return mask;
}



//文件操作方法集合指针
static const struct file_operations mymisc_fops = {
    .open           =   xxx_open,
    .read           =   xxx_read,
    .release        =   xxx_release,
    .poll           =   xxx_poll,
};


//定义核心结构
static struct miscdevice  misc = {
    .minor = 255,
    .name  = "mybtns",     ///dev目录下的设备名
    .fops  = &mymisc_fops,
};


static int __init mymisdevice_init(void)
{
    int ret;
    int irq;
    int i;

    //设置中断属性:非共享中断，上升沿，下降触发
    unsigned long flags = IRQF_DISABLED  |
                          IRQF_TRIGGER_FALLING |
                          IRQF_TRIGGER_RISING;

    //1)开发板上有4个按键 ，要注册4次
    for(i = 0; i < 4; i++) {

		//初始化每个按键 tasklet结构
		tasklet_init(&keys[i].keytasklet, key_tasklet_func,&keys[i]);
		
        //得到K1中断编号
        irq = gpio_to_irq( keys[i].key_gpio);

        //注册key1中断:dev_id分别绑定 1~4这4个地址
        //keys_handlers(irq,(void*)&keys[0])
        //keys_handlers(irq,(void*)&keys[1])
        //keys_handlers(irq,(void*)&keys[2])
        //keys_handlers(irq,(void*)&keys[3])
        ret = request_irq(irq, keys_handlers, flags, keys[i].keyname, &keys[i]);
        if(ret < 0) {
            break;
        }
    }

    //如果前面注册有失败的，则全部释放中断:注意只释放已经注册成功的中断。
    if(ret < 0) {
        for(--i; i > 0 ; i--) {
            //1)开发板上有4个按键 ，要注册4次
            irq = gpio_to_irq( keys[i].key_gpio);               //得到Kx中断编号
            free_irq(irq, &keys[i]);                            //释放中断
        }

        //失败了，释放中断号后，直接返回
        return ret;
    }



    //注册核心结构
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

    //注销核心结构
    ret = misc_deregister(&misc);
    if(ret < 0) {
        printk(KERN_EMERG"misc_deregister error\n");
        return ;
    }

    for(i = 0; i < 4; i++) {
        irq = gpio_to_irq( keys[i].key_gpio);               //得到Kx中断编号
        free_irq(irq, &keys[i]);                            //释放中断

		tasklet_kill(&keys[i].keytasklet);                  //取销tasklet
    }

    printk(KERN_EMERG"misc_deregister ok\n");

}

module_init(mymisdevice_init);
module_exit(mymisdevice_exit);
MODULE_LICENSE("GPL");



