/*  printf()  */
#include <stdio.h>

/* exit(0),否则会出现:
 * warning: incompatible implicit declaration of built-in function 'exit'
 */
#include <stdlib.h>

/*open(),*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*read(),write(),lseek(),sleep()*/
#include <unistd.h>

/*  ioctl() */
#include <sys/ioctl.h>
#include <sys/types.h>

#define  DEV_NAME    "/dev/key_btn"
#define KEY1    1
#define KEY2    2
#define KEY3    3
#define KEY4    4
#define KEY_READ_ON  _IOW('K',0,int)
int main(int argc, char *args[])
{
    int fd = 0;
    int ret = 0;
    int arg;

    fd = open(DEV_NAME, O_RDWR);
    if(fd < 0) {
        perror("open");
    }

   while(1)
{
   ioctl(fd,KEY_READ_ON,&arg);
   switch(arg)
   {
     case KEY1: 
	     printf("KEY1=%d\n",arg);
	     break;
     case KEY2: 
	     printf("KEY2=%d\n",arg);
	     break;
     case KEY3:
	     printf("KEY3=%d\n",arg);
	     break;
     case KEY4:
     	     printf("KEY4=%d\n",arg);		     
	break;
     default:
     break;
   }
} 
    return 0;

}



