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

// 以32BPP形式 显示一个点
void put_pixle(int x, int y, unsigned int c);


int main(int argc, char **argv)
{
    int fd;
    int x, y;
    unsigned int size;



    /* 打开文件 */
    if ((fd = open("/dev/fb0", O_RDWR)) < 0) {
        perror("open");
    }


    /* 获取可变参数和固定参数 */
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);


    /* 计算显存大小 */
    size = vinfo.xres_virtual * vinfo.yres_virtual * (vinfo.bits_per_pixel / 8);
    printf("vinfo.xres_virtual:%d\r\nvinfo.yres_virtual :%d\r\nbpp:%d \r\n",
           vinfo.xres_virtual, vinfo.yres_virtual, vinfo.bits_per_pixel);

    /* 将文件映射至进程的地址空间 */
    if ((fb_addr = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0)) == (void *) - 1) {
        perror("mmap");
    }


    /* 清屏 */
    memset(fb_addr, 0, size);


    /* 在lcd 屏显示一些数据  */
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

    /* 映射完后, 关闭文件也可以操纵内存 */
    close(fd);

    /* 释放存储映射区 */
    if ((munmap((void *)fb_addr, size)) == -1) {
        perror("munmap");
    }

    return 0;
}


// 以32BPP形式 显示一个点
void put_pixle(int x, int y, unsigned int c)
{
    int pos;
    pos = y * finfo.line_length + x * (vinfo.bits_per_pixel / 8);
    *(unsigned int*)(fb_addr + pos) = c;
}


