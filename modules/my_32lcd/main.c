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

#define PERROR()  printf("Error：file:%s\r\nline:%d,function:%s\r\n",__FILE__,__LINE__,__FUNCTION__)



/********************************************************************
 *  Function：     main()
 *  Description:   居中显示一张bmp图片
 *  Calls:         fb_get_fbmem，fb_get_xres，fb_get_yres
 *  Called By:
 *  Input：        argc；参数数量； argv[0]应用程序名，argv[1]:bmp图片
 *  Output：       ;
 *  Return :       0：成功; -1：失败；
 *  Author:        ChenZhiFa
 *  Others: this application only to show for input file bmp24 ,and display mode is 16bpp:565
 *          ./lcd_bmp 240x320.bmp   图片路径根据自己实际情况修改
 *  date of completion:   2012-08-27
 *******************************************************************/
int main(int argc,char* argv[])
{
	int ret;
	
	/* lcd初始化 */
	ret = fb_init();
	if(ret < 0)
	{
		PERROR();
		exit(-1);
	}

	/* 显示bmp图片 */
    ret = hzk32_init_hzk32_open(HZK32_PATH);
    if (ret < 0) {
        printf("Error : Can not open hzk32 \n");
        exit(-1);
    }

	//显示一个汉字
    lcd_printf_str_32(0, 100, 0xf800, 0x0, "显示一个汉字关闭文件关闭文件应用程序名");

	//关闭文件 
   hzk32_end_hzk32_close();
	
	/* 关闭设备 */
	fb_close();

	return 0;
}

