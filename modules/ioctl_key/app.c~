#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ioctl_cmd.h"
int main()
{
   int led_fd,beep_fd,i,key_fd;
   unsigned long arg=0;
   led_fd =open("/dev/ioctl_led",O_RDWR);
  // beep_fd =open("/dev/ioctl_beep",O_RDWR);
   key_fd =open("/dev/ioctl_key",O_RDWR);
   if(led_fd <0 )
   {
     printf("open led_fd error\n");
     return 0;
   }
/*
  if(beep_fd <0)
   {
     printf("open beep_fd error\n");
     return 0;
   }
*/
   if(key_fd <0)
   {
     printf("open key_fd error\n");
     return 0;
   }
   ioctl(led_fd,LED_ALLOFF);
   ioctl(beep_fd,BEEP_OFF);
    
while(1)
{
   ioctl(key_fd,KEY_READ_ON,&arg);
   switch(arg)
   {
     case KEY1: 
	     printf("KEY1=%d\n",arg);
	     ioctl(led_fd,LED_ALLON);
	     break;
     case KEY2: 
	     printf("KEY2=%d\n",arg);
             ioctl(led_fd,LED_ALLOFF);
	     break;
     case KEY3:
	     printf("KEY3=%d\n",arg);
	     ioctl(led_fd,LED_ALLON);
	     break;
     case KEY4:
     	     printf("KEY4=%d\n",arg);
             ioctl(led_fd,LED_ALLOFF);		     
	break;
     default:
     break;
   }
} 
   
   close(led_fd); 
   close(beep_fd);
   return 0;
   
}



