/*************************************************************************
	> File Name: lcd.c
	> Author: 
	> Mail: 
	> Created Time: Tue 14 Nov 2017 03:30:47 AM PST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/fb.h> //fb_fix_screeninfo,fb_var_screeninfo
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
void put_pixel(int x, int y, unsigned int c);
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
unsigned char *lcd_addr;
int main()
{
int fd;
unsigned int lcd_size;
int x, y;
//1. ��/dev/fbX �豸
fd = open("/dev/fb0", O_RDWR);
if(fd < 0) {
perror("open");
exit(1);
}
//2. ʹ�� ioctl �������ƽ̨�� framebuffer �豸����Ϣ
ioctl(fd, FBIOGET_FSCREENINFO, &finfo); //�����Ļ�̶���Ϣ
ioctl(fd, FBIOGET_VSCREENINFO, &vinfo); //�����Ļ�ɱ���Ϣ
printf("vinfo.xres:%d,vinfo.yres:%d,bpp:%d\r\n",
vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
//3. ʹ��ǰ���õ� framebuffer ��Ϣ���� LCD �Դ�Ĵ�С
lcd_size = finfo.line_length * vinfo.yres_virtual;
//4. ���� mmap �������ں˿ռ��п��ٵ��Դ�ӳ�䵽���̿ռ���
lcd_addr = mmap(NULL, lcd_size, PROT_READ | PROT_WRITE, PROT_READ, fd, 0);
if(lcd_addr == (void *) - 1) {
perror("mmap");
exit(1);
}
//5. ʹ�� mmap �õ��ĵ�ַ�����Դ�ռ�����ݣ��Ӷ��ı� LCD ������ʾ?
// (x�� y)����ƫ������
//pos = line_length*y + x*(bits_per_pixel/8)
//�ڴ��ַ��
//lcd_addr + pos������ӯ��Ƽ����޹�˾����־�� Framebuffer �豸Ӧ�ó����� Linux �ʼ� 2016-11-08

memset(lcd_addr, 0, lcd_size); //���Դ滺������Ϊ��ɫ
for(y = 10; y < 1024; y++)
for(x = 10; x < 1024; x++) {
#if 0
unsigned int pos;
pos = finfo.line_length * y + x * (vinfo.bits_per_pixel / 8);
//24BPP ,32BPP
*(int *)(lcd_addr + pos) = 0xff0000; //��ɫ
#endif
put_pixel(x, y, 0xff0000);//��ɫ
}
//6. ����ʹ�� framebuffer �豸ʱ��ʹ�� munmap ȡ��ӳ�䡣
munmap(lcd_addr, lcd_size);
return 0;
}
//��(x,y)����ʾһ����ɫΪ c �ĵ�
void put_pixel(int x, int y, unsigned int c)
{
unsigned int pos;
pos = finfo.line_length * y + x * (vinfo.bits_per_pixel / 8);
//24BPP ,32BPP
*(int *)(lcd_addr + pos) = c; //��ɫ
}