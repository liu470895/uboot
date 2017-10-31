#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
   int led_fd;
   int fd;
   char ch='0';
   int key=0;
   fd =open("/dev/key",O_RDWR);
   led_fd =open("/dev/led",O_RDWR);
   if(fd <0)
   {
     printf("open key_led error\n");
     return 0;
   }
  if(led_fd <0)
   {
     printf("open led_fd error\n");
     return 0;
   }
   while(1)
   {
    read(fd,&ch,1);
    key=atoi(&ch); // key =ch-'0'
    switch(key)
     {
     case 1:write(led_fd,"1100",4);break;
     case 2:write(led_fd,"0011",4);break;
     case 3:write(led_fd,"1001",4);break;
     case 4:write(led_fd,"0000",4);break;
     }
   }
   close(fd);
   close(led_fd);
   return 0;
   
}



