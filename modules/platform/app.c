#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//验证
//当应用层调用相关接口函数时，是否会执行驱动中
//对应添加的函数

int main()
{
   int fd;
   char buf[40];
   fd =open("/dev/platform",O_RDWR);
   if(fd <0)
   {
     printf("open error\n");
     return 0;
   }
   read(fd,buf,30);
   lseek(fd,0,SEEK_SET);
   write(fd,buf,20);
   close(fd);
   return 0;
   
}



