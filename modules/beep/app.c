#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
   int fd;
   char buf[2]="11";
   fd =open("/dev/beep",O_RDWR);
   if(fd <0)
   {
     printf("open error\n");
     return 0;
   }
   write(fd,buf,2);
   close(fd);
   return 0;
   
}



