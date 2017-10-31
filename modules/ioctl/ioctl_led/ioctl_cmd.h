#ifndef IOCTL_CMD_H
#define IOCTL_CMD_H
#include<linux/ioctl.h>

#define LED_ALLON    _IO('L',0)
#define LED_ALLOFF   _IO('L',1)
#define LEDx_ON      _IOW('L',2,int)
#define LEDx_OFF     _IOW('L',3,int)
#define BEEP_ON    _IO('B',0)
#define BEEP_OFF   _IO('B',1)

#endif


