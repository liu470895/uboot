#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

//������ͨ����
static int num = 0;
static char *name = "DAVID";
static int age  = 30;
static int aa[10] = {1,2,3,4,5,6,7,8,9,0};


//module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
//�����ӵļ����Ϻ�
//ʹ�� S_IRUGO ��Ϊ�������Ա������˶�ȡ, ���ǲ��ܸı�; S_IRUGO|S_IWUSR ���� root ���ı����.
module_param(age, int ,           S_IRUGO | S_IWUSR);   //S_IWUSR���û�дȨ��
module_param(name, charp ,        S_IRUGO);
module_param_array(aa, int, &num, S_IRUGO);
MODULE_PARM_DESC(age, "A short integer");
MODULE_PARM_DESC(name, "A character string");
MODULE_PARM_DESC(aa, "An array of integers");


static int __init hello_init(void)
{
	int i = 0;
		
	printk(KERN_EMERG "Name:%s\n",name);
	printk(KERN_EMERG "Age:%d\n",age);
	
	for(i=0;i<10;i++)
		printk(KERN_EMERG "%d  ",aa[i]);
		
	printk(KERN_EMERG "num:%d  \n",num);	
	
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_EMERG"Module exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);
