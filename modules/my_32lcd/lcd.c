
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include "lcd.h"

static int fd_fb;                        //lcd�ļ�������
static unsigned char *fbp;               //lcd ��ʾ��������ַ
static unsigned long screensize;  //lcd ��ʾ������ռ�õ����ڴ��С
static struct fb_var_screeninfo vinfo;   //lcd �ɱ����
static struct fb_fix_screeninfo finfo;   //lcd �̶�����



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
int fb_init (void )
{
	// Open the file for reading and writing
	fd_fb = open(FB_PATH, O_RDWR);
	if (!fd_fb)
	{
		printf("Error cannot open framebuffer device.\n");
		return(-1);
	}

	// Get fixed screen information
	if (ioctl(fd_fb, FBIOGET_FSCREENINFO, &finfo))
	{
		printf("Error reading fixed information.\n");
		return(-1);
	}

	// Get variable screen information
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &vinfo))
	{
		printf("Error reading variable information.\n");
		return(-1);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (unsigned char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
		fd_fb, 0);
	if ((int)fbp == -1)
	{
		printf("Error failed to map framebuffer device to memory.\n");
		return(-1);
	}

	return 0;

}




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
void fb_close (void )
{
	munmap(fbp,screensize);
}




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
void lcd_put_pixel(int x, int y, unsigned int color)
{
	unsigned char  *bpp8;
	unsigned short *bpp16;	
	unsigned int   *bpp32;	
	unsigned int red, green, blue;	
	
  bpp8  = fbp + y * finfo.line_length + x * vinfo.bits_per_pixel / 8;
	bpp16 = (unsigned short *)bpp8;
	bpp32 = (unsigned int *)bpp8;

	switch (vinfo.bits_per_pixel)
	{
    //8bpp
		case 8:
		{
			*bpp8 = color;
			break;
		}

		case 16:
		{
			/*rgb565 */
			red   = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue  = (color >> 0) & 0xff;

	  	*bpp16 = ((red >> 3) << 11) | ((green >> 2) << 5) | ((blue >> 3)<<0);
			break;
		}

		case 32:
		{
			*bpp32 = color;
			break;
		}

		default:
		{
			printf("err %dbpp val\n", vinfo.bits_per_pixel);
			break;
		}
	}
}







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
void* fb_memmalloc(void)
{
	return  malloc(screensize);
}


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
int fb_get_bpp(void)
{
	return 	vinfo.bits_per_pixel;
}


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
int fb_get_fb_size(void)
{
	return 	screensize;
}

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
int fb_get_xres(void)
{
	return 	vinfo.xres;
}

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
int fb_get_yres(void)
{
	return 	vinfo.yres;
}

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
unsigned char* fb_get_fbmem(void)
{
	return 	fbp;
}






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
unsigned short make16color(unsigned char r, unsigned char g, unsigned char b)
{
	return (
		(((r >> 3) & 31) << 11) |
		(((g >> 2) & 63) << 5)  |
		((b >> 3) & 31)        );
}









