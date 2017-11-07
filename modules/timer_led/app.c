#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
   int fd;
   char buf[4]="1001";
   fd =open("/dev/timer_led",O_RDWR);
   if(fd <0)
   {
     printf("open error\n");
     return 0;
   }
   write(fd,buf,4);
   close(fd);
   return 0;
   
}



