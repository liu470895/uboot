#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <linux/fb.h>
#include <string.h>

unsigned char *fb_addr;
struct fb_var_screeninfo   vinfo;
struct fb_fix_screeninfo   finfo;

// ��32BPP��ʽ ��ʾһ����
void put_pixle(int x, int y, unsigned int c);


int main(int argc, char **argv)
{
    int fd;
    int x, y;
    unsigned int size;



    /* ���ļ� */
    if ((fd = open("/dev/fb0", O_RDWR)) < 0) {
        perror("open");
    }


    /* ��ȡ�ɱ�����͹̶����� */
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);


    /* �����Դ��С */
    size = vinfo.xres_virtual * vinfo.yres_virtual * (vinfo.bits_per_pixel / 8);
    printf("vinfo.xres_virtual:%d\r\nvinfo.yres_virtual :%d\r\nbpp:%d \r\n",
           vinfo.xres_virtual, vinfo.yres_virtual, vinfo.bits_per_pixel);

    /* ���ļ�ӳ�������̵ĵ�ַ�ռ� */
    if ((fb_addr = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0)) == (void *) - 1) {
        perror("mmap");
    }


    /* ���� */
    memset(fb_addr, 0, size);


    /* ��lcd ����ʾһЩ����  */
    for(y = 100; y < 200; y++) {
        for(x = 100; x < 300; x++) {
#if 0
            int pos;
            pos = y * finfo.line_length + x * (vinfo.bits_per_pixel / 8);
            *(unsigned int*)(fb_addr + pos) = 0xff00;
#endif
            put_pixle(x, y, 0xff00);
        }
    }

    /* ӳ�����, �ر��ļ�Ҳ���Բ����ڴ� */
    close(fd);

    /* �ͷŴ洢ӳ���� */
    if ((munmap((void *)fb_addr, size)) == -1) {
        perror("munmap");
    }

    return 0;
}


// ��32BPP��ʽ ��ʾһ����
void put_pixle(int x, int y, unsigned int c)
{
    int pos;
    pos = y * finfo.line_length + x * (vinfo.bits_per_pixel / 8);
    *(unsigned int*)(fb_addr + pos) = c;
}


