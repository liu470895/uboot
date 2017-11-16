
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

//lcd�豸 
#define FB_PATH  "/dev/fb0"

/********************************************************************
 *  Function��     fb_init()
 *  Description:   lcd ��ʼ��;
 *  Calls:
 *  Called By:
 *  Input��        none; 
 *  Output��       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
int fb_init (void );


/********************************************************************
 *  Function��     fb_close()
 *  Description:   lcd ȡ��ӳ��;
 *  Calls:
 *  Called By:
 *  Input��        none; 
 *  Output��       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
void fb_close (void );




/********************************************************************
 *  Function��     lcd_put_pixel()
 *  Description:   ���㺯��;
 *  Calls: 
 *  Called By:
 *  Input��        x:��ʾ��ʼ������    y:��ʾ��ʼ������  
                   color������ɫ; 
 *  Output��       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
void lcd_put_pixel(int x, int y, unsigned int color);



/********************************************************************
 *  Function��     fb_get_fbmem()
 *  Description:   ����lcd�Ļ������׵�ַ
 *  Calls:          
 *  Called By:
 *  Input��        none
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
unsigned char* fb_get_fbmem(void);



/********************************************************************
 *  Function��     fb_memmalloc()
 *  Description:   ����һ����lcd��������С��ͬ�Ļ�����;
 *  Calls:         malloc
 *  Called By:
 *  Input��         r,g,b�ϳɵ�rgb����
 *  Output��       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
void* fb_memmalloc(void);


/********************************************************************
 *  Function��     fb_get_bpp()
 *  Description:   ����lcd�Ļ���bpp
 *  Calls:          
 *  Called By:
 *  Input��        none��
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_bpp(void);


/********************************************************************
 *  Function��     fb_get_fb_size()
 *  Description:   ����lcd�Ļ���սռ�õ��ڴ��С
 *  Calls:          
 *  Called By:
 *  Input��        none��
 *  Output��       ;
 *  Return :       �ڴ��С 
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_fb_size(void);



/********************************************************************
 *  Function��     fb_get_xres()
 *  Description:   ����lcd�Ļ��������
 *  Calls:          
 *  Called By:
 *  Input��        none��
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_xres(void);


/********************************************************************
 *  Function��     fb_get_yres()
 *  Description:   ����lcd�Ļ������߶�
 *  Calls:          
 *  Called By:
 *  Input��        none��
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_yres(void);


/********************************************************************
 *  Function��     fb_get_fbmem()
 *  Description:   ����lcd�Ļ������׵�ַ
 *  Calls:          
 *  Called By:
 *  Input��        none
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
unsigned char* fb_get_fbmem(void);

/********************************************************************
 *  Function��     fb_get_fb_size()
 *  Description:   ����lcd�Ļ���սռ�õ��ڴ��С
 *  Calls:          
 *  Called By:
 *  Input��        none��
 *  Output��       ;
 *  Return :       �ڴ��С 
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_fb_size(void);

/********************************************************************
 *  Function��     fb_get_xres()
 *  Description:   ����lcd�Ļ��������
 *  Calls:          
 *  Called By:
 *  Input��        none��
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_xres(void);

/********************************************************************
 *  Function��     fb_get_yres()
 *  Description:   ����lcd�Ļ������߶�
 *  Calls:          
 *  Called By:
 *  Input��        none��
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *******************************************************************/
int fb_get_yres(void);


/********************************************************************
 *  Function��     make16color()
 *  Description:   ��r,g,b�ϳ�rgb565;
 *  Calls:
 *  Called By:
 *  Input��         r,g,b�ϳɵ�rgb����
 *  Output��       ;
 *  Return :       ;
 *  Author:        ChenZhiFa
 *  Others:
 *  date of completion:   2012-08-27
 *********************************************************************/
unsigned short make16color(unsigned char r, unsigned char g, unsigned char b);



#endif

