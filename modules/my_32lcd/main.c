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
#include "hzk32.h"

#define PERROR()  printf("Error��file:%s\r\nline:%d,function:%s\r\n",__FILE__,__LINE__,__FUNCTION__)



/********************************************************************
 *  Function��     main()
 *  Description:   ������ʾһ��bmpͼƬ
 *  Calls:         fb_get_fbmem��fb_get_xres��fb_get_yres
 *  Called By:
 *  Input��        argc������������ argv[0]Ӧ�ó�������argv[1]:bmpͼƬ
 *  Output��       ;
 *  Return :       0���ɹ�; -1��ʧ�ܣ�
 *  Author:        ChenZhiFa
 *  Others: this application only to show for input file bmp24 ,and display mode is 16bpp:565
 *          ./lcd_bmp 240x320.bmp   ͼƬ·�������Լ�ʵ������޸�
 *  date of completion:   2012-08-27
 *******************************************************************/
int main(int argc,char* argv[])
{
	int ret;
	
	/* lcd��ʼ�� */
	ret = fb_init();
	if(ret < 0)
	{
		PERROR();
		exit(-1);
	}

	/* ��ʾbmpͼƬ */
    ret = hzk32_init_hzk32_open(HZK32_PATH);
    if (ret < 0) {
        printf("Error : Can not open hzk32 \n");
        exit(-1);
    }

	//��ʾһ������
    lcd_printf_str_32(0, 100, 0xf800, 0x0, "��ʾһ�����ֹر��ļ��ر��ļ�Ӧ�ó�����");

	//�ر��ļ� 
   hzk32_end_hzk32_close();
	
	/* �ر��豸 */
	fb_close();

	return 0;
}

