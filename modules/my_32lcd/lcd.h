
#ifndef __LCD_H__
#define __LCD_H__


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

//lcd设备 
#define FB_PATH  "/dev/fb0"

/********************************************************************
 *  Function：     fb_init()
 *  Description:   lcd 初始化;
 *  Calls:
 *  Called By:
 *  Input：        none; 
 *  Output：       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
int fb_init (void );


/********************************************************************
 *  Function：     fb_close()
 *  Description:   lcd 取消映射;
 *  Calls:
 *  Called By:
 *  Input：        none; 
 *  Output：       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
void fb_close (void );




/********************************************************************
 *  Function：     lcd_put_pixel()
 *  Description:   画点函数;
 *  Calls: 
 *  Called By:
 *  Input：        x:显示起始横坐标    y:显示起始纵坐标  
                   color：点颜色; 
 *  Output：       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
void lcd_put_pixel(int x, int y, unsigned int color);



/********************************************************************
 *  Function：     fb_get_fbmem()
 *  Description:   返回lcd的缓冲区首地址
 *  Calls:          
 *  Called By:
 *  Input：        none
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
unsigned char* fb_get_fbmem(void);



/********************************************************************
 *  Function：     fb_memmalloc()
 *  Description:   开辟一个和lcd缓冲区大小相同的缓冲区;
 *  Calls:         malloc
 *  Called By:
 *  Input：         r,g,b合成的rgb数据
 *  Output：       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
void* fb_memmalloc(void);


/********************************************************************
 *  Function：     fb_get_bpp()
 *  Description:   返回lcd的缓冲bpp
 *  Calls:          
 *  Called By:
 *  Input：        none；
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_bpp(void);


/********************************************************************
 *  Function：     fb_get_fb_size()
 *  Description:   返回lcd的缓冲战占用的内存大小
 *  Calls:          
 *  Called By:
 *  Input：        none；
 *  Output：       ;
 *  Return :       内存大小 
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_fb_size(void);



/********************************************************************
 *  Function：     fb_get_xres()
 *  Description:   返回lcd的缓冲区宽度
 *  Calls:          
 *  Called By:
 *  Input：        none；
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_xres(void);


/********************************************************************
 *  Function：     fb_get_yres()
 *  Description:   返回lcd的缓冲区高度
 *  Calls:          
 *  Called By:
 *  Input：        none；
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_yres(void);


/********************************************************************
 *  Function：     fb_get_fbmem()
 *  Description:   返回lcd的缓冲区首地址
 *  Calls:          
 *  Called By:
 *  Input：        none
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
unsigned char* fb_get_fbmem(void);

/********************************************************************
 *  Function：     fb_get_fb_size()
 *  Description:   返回lcd的缓冲战占用的内存大小
 *  Calls:          
 *  Called By:
 *  Input：        none；
 *  Output：       ;
 *  Return :       内存大小 
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_fb_size(void);

/********************************************************************
 *  Function：     fb_get_xres()
 *  Description:   返回lcd的缓冲区宽度
 *  Calls:          
 *  Called By:
 *  Input：        none；
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_xres(void);

/********************************************************************
 *  Function：     fb_get_yres()
 *  Description:   返回lcd的缓冲区高度
 *  Calls:          
 *  Called By:
 *  Input：        none；
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_yres(void);


/********************************************************************
 *  Function：     make16color()
 *  Description:   把r,g,b合成rgb565;
 *  Calls:
 *  Called By:
 *  Input：         r,g,b合成的rgb数据
 *  Output：       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
unsigned short make16color(unsigned char r, unsigned char g, unsigned char b);



#endif

