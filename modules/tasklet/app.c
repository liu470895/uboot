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

char save_buf[10] = {"0000"};   //�������ʹ��

int main(void)
{
    int fd;                      //����ļ���������
    int ret;
    int i;
    struct pollfd fds[1];


    //�Է�������ʽ��
    //fd = open("/dev/mybtns", O_RDWR | O_NONBLOCK );
    fd = open("/dev/mybtns", O_RDWR ); //�Զ�д��ʽ���д򿪣�Ĭ����������ʽ�򿪵�
    if(fd < 0) {
        printf("open error\r\n");
        return -1;
    }


    //ʵ�ʳ�����Ҫѭ����ȡ����������Ȼ����ݶ�����ɲ�ͬ����
    while(1) {
        char cur_buf[10] = {0};   //��ʱ�������ʹ��

        fds[0].fd     = fd;
        fds[0].events = POLLIN;  //Ҫ�����¼�

        //ret = poll(fds, 1, -1);   //   ��Զ����ֱ���б仯 
        //ret = poll(fds, 1, 0);      //   ������
        ret = poll(fds, 1, 2000); //   2�볬ʱ
        //�жϲ�ѯ���
        if(ret < 0) {
            perror("poll");
            exit(0);
        } else if(ret == 0) {
            printf("timeout\r\n");
			continue;
        } else {
            //�ֱ��ж�ÿ��fd
            if(fds[0].revents & POLLIN) {
                //�ض���ǰ��4����״̬
                read(fd, cur_buf, 4);
                for(i = 0; i < 4; i++) {
                    if(cur_buf[i] != save_buf[i]) {
                        save_buf[i] = cur_buf[i] ; //���µ�ǰ����״̬

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


    //�ر��ļ�
    close(fd);

    return 0;
}



