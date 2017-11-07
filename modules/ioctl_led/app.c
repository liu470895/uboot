#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ioctl_cmd.h"
int main()
{
   int led_fd,beep_fd,i;
   led_fd =open("/dev/ioctl_led",O_RDWR);
   beep_fd =open("/dev/beep",O_RDWR);
   if(led_fd <0 )
   {
     printf("open led_fd error\n");
     return 0;
   }
  if(beep_fd <0)
   {
     printf("open beep_fd error\n");
     return 0;
   }
   ioctl(led_fd,LED_ALLOFF);
   ioctl(beep_fd,BEEP_OFF);
   while(1)
   {
   	 for (i=0; i<4; i++)
   	 {
   	 	ioctl(led_fd,LEDx_ON,i);
   	 	sleep(1);
   	 	ioctl(led_fd,LEDx_OFF,i);
   	 	sleep(1);
                ioctl(beep_fd,BEEP_ON);
	        sleep(1);
 		ioctl(beep_fd,BEEP_OFF);
                sleep(1);
   	 }
   }
   close(led_fd); 
   close(beep_fd);
   return 0;
   
}



