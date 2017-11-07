#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define  DEV_NAME    "/dev/timer_key"
#define KEY1    1
#define KEY2    2
#define KEY3    3
#define KEY4    4
#define KEY_READ_ON  _IOW('K',0,int)
#define LED_ALLON    _IO('L',0)
#define LED_ALLOFF   _IO('L',1)
#define LEDx_ON      _IOW('L',2,int)
#define LEDx_OFF     _IOW('L',3,int)
#define BEEP_ON      _IO('B',0)
#define BEEP_OFF     _IO('B',1)

int main(int argc, char *args[])
{
    int fd = 0;
    int ret = 0;
    int arg;
    int led_fd,beep_fd;
    fd = open("/dev/timer_key", O_RDWR);
    led_fd =open("/dev/ioctl_led",O_RDWR);
    beep_fd =open("/dev/ioctl_beep",O_RDWR);
    if(fd < 0) {
        perror("open");
    }
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
   ioctl(fd,KEY_READ_ON,&arg);
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
             ioctl(beep_fd,BEEP_OFF);
	     break;
     case KEY4:
     	     printf("KEY4=%d\n",arg);
	     ioctl(beep_fd,BEEP_ON);		     
	break;
     default:
     break;
   }
} 
    return 0;

}



