#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>    //lseek
#include <sys/ioctl.h> //ioctl
#include <poll.h>      //poll


//int poll(struct pollfd fd[], nfds_t nfds, int timeout);

char save_buf[10] = {"0000"};   //存放数据使用

int main(void)
{
    int fd;                      //存放文件描述符号
    int ret;
    int i;
    struct pollfd fds[1];


    //以非阻塞方式打开
    //fd = open("/dev/mybtns", O_RDWR | O_NONBLOCK );
    fd = open("/dev/mybtns", O_RDWR ); //以读写方式进行打开，默认是阻塞方式打开的
    if(fd < 0) {
        printf("open error\r\n");
        return -1;
    }


    //实际程序需要循环读取按键动作，然后根据动作完成不同事情
    while(1) {
        char cur_buf[10] = {0};   //临时存放数据使用

        fds[0].fd     = fd;
        fds[0].events = POLLIN;  //要监测读事件

        //ret = poll(fds, 1, -1);   //   永远阻塞直到有变化 
        //ret = poll(fds, 1, 0);      //   非阻塞
        ret = poll(fds, 1, 2000); //   2秒超时
        //判断查询结果
        if(ret < 0) {
            perror("poll");
            exit(0);
        } else if(ret == 0) {
            printf("timeout\r\n");
			continue;
        } else {
            //分别判断每个fd
            if(fds[0].revents & POLLIN) {
                //回读当前的4个灯状态
                read(fd, cur_buf, 4);
                for(i = 0; i < 4; i++) {
                    if(cur_buf[i] != save_buf[i]) {
                        save_buf[i] = cur_buf[i] ; //更新当前按键状态

                        if(save_buf[i] == '1') {
                            printf("K%d press\r\n", i + 1);
                        } else {
                            printf("K%d up\r\n", i + 1);
                        }
                        printf("keys:%s\r\n", save_buf);
                    }
                }
                printf("keys:%s\r\n", save_buf);
            }
        }
    }


    //关闭文件
    close(fd);

    return 0;
}



