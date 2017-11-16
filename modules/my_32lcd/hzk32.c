
/*******************************************************************************
** 程序名称：演示使用hzk32点阵字库的程序
** 程序描述：使用hzk32实现显示16*16点阵汉字
** 性能提升：
** 程序版本：V1.0
** 程序作者：
** 最后修改：2011年8月16日
*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>                    //exit()函数在这个头文件 中
#include <sys/time.h>
//#include <errno.h>
#include <sys/wait.h>
//#include <string.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include "lcd.h"

#define HZK32_PATH        "HZK32"     //汉字库存文件 路径

extern  const unsigned char fontdata_8x16[]; //8x16文件点阵数组

FILE *fp_hzk;                         //汉字库文件的指针 ，用来指向打开的hzk32文件


/*************************************************************
***** 函数  名：  init_hzk32_open
***** 功    能：  从字库文件中读取一个汉字的字模点阵数据
***** 参    数：  path:hzk32文件存放路径
***** 返 回 值：  -1:失败;  0:成功 
***** 创 建 者:
***** 创建时间：
***** 最后更新:
***** 说    明：
*************************************************************/
int hzk32_init_hzk32_open(const char * path )
{
   // FILE *fp_hzk;
    if(!path ) {
        return NULL;
    }

    //打开汉字库存字模文件
    if((fp_hzk = fopen(path, "r")) == NULL) {
    	 printf("Error：file:%s\r\nline:%d,function:%s\r\n",__FILE__,__LINE__,__FUNCTION__);
       return -1;    //打开字库文件
    }

    return 0;
}


/*************************************************************
***** 函数  名：  hzk32_end_hzk32_close
***** 功    能：  关闭 hzk32文件
***** 参    数：  fp_hzk:要关闭的文件指针
***** 返 回 值：  -1:失败;  0 :成功
***** 创 建 者:
***** 创建时间：
***** 最后更新:
***** 说    明：
*************************************************************/
int hzk32_end_hzk32_close()
{
    fclose(fp_hzk);
    return 0;
}



/*************************************************************
***** 函数  名：  hzk32_get_data_32byte
***** 功    能：  从字库文件中读取一个汉字的字模点阵数据
***** 参    数：  unsigned char *s 指向目标汉字的指针,
                  char* const chs 用于存储字模的数组首地址
                  fp_hzk : 指向已经打开的hzk32文件指针
***** 返 回 值：  -1:失败，参数非法； 0>=:成功；
***** 创 建 者:
***** 创建时间：
***** 最后更新:
***** 说    明：
*************************************************************/
static int hzk32_get_data_32byte(unsigned char * buffer, const unsigned char *s, FILE *fp_hzk)
{

    unsigned long offset;
    int ret;
    if(!s || !buffer || !fp_hzk ) {
        return -1;
    }

    //根据内码找出汉字在hzk32中的偏移位置
    offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 128;

    fseek(fp_hzk, offset, SEEK_SET);                   //文件指针偏移到要找的汉字处
    ret = fread(buffer, 1, 128, fp_hzk);                //读取该汉字的字模
    return ret;
}




/****************************************************************
***** 函数  名：  lcd_show_ascii()
***** 功    能：  显示16X8字符
***** 参    数：  fbmem_start:fb缓冲区起始地址
                  x:显示起始横坐标    y:显示起始纵坐标
                  forecolor:前景色，也就是字符颜色； backcolor：背景色
***** 返 回 值：  无
***** 创 建 者:
***** 创建时间：
***** 最后更新:
***** 说    明：
*************************************************************/
void lcd_show_ascii(
    int x, int y,
    unsigned int forecolor,
    unsigned int backcolor,
    unsigned char ch)
{

    unsigned char *dots = (unsigned char *)&fontdata_8x16[ch * 16];
    int i, b;
    unsigned char byte;

    for (i = 0; i < 16; i++) {

        byte = dots[i];

        for (b = 7; b >= 0; b--) {

            if (byte & (1 << b)) {
                lcd_put_pixel(x + 7 - b, y + i, forecolor);
            }

            else {
                lcd_put_pixel(x + 7 - b, y + i, backcolor);
            }
        }
    }
}



/*************************************************************
***** 函数  名：  show_chinese_32x32
***** 功    能：  显示32x32汉字
***** 参    数：
                  x:显示起始横坐标    y:显示起始纵坐标 ；
                  chs：要显示的中文字符串（注意是GB2312编码）
                  forecolor:前景色，也就是字符颜色； backcolor：背景色
***** 返 回 值：  无
***** 创 建 者:
***** 创建时间：
***** 最后更新:
***** 说    明：使用到了全局变量  fp_hzk
*************************************************************/
void lcd_show_chinese_32x32(unsigned int x, unsigned int y,
                               unsigned int fc, unsigned int bc,
                               const unsigned char* buf)
{
    int i, j;
    unsigned char data[128]; 
    int temp;                     
    hzk32_get_data_32byte(data,buf,fp_hzk);      //去字库中读取汉字字模

     for(i=0;i<32;i++)
	{
		
		temp = data[4*i]<<24 |data[4*i+1]<<16 |data[4*i +2]<<8 |data[4*i+3];
		for(j=0;j<32;j++)
		{
			 if(temp &(0x80000000>>j))
			 {
				lcd_put_pixel(x + j, y + i, fc);
			 }
			 else 
			 {

				lcd_put_pixel(x + j, y + i, bc);
			 }
		}
	}
}



/*************************************************************
***** 函数  名：  lcd_printf_str_16
***** 功    能：  混合显示32x32汉字和16＊8字符
***** 参    数：  fbmem_start:fb缓冲区起始地址
                  x:显示起始横坐标    y:显示起始纵坐标 ；
                  str：要显示的中文字符串（注意是GB2312编码）
                  forecolor:前景色，也就是字符颜色； backcolor：背景色
***** 返 回 值：  无
***** 创 建 者:
***** 创建时间：
***** 最后更新:
***** 说    明：
*************************************************************/
void lcd_printf_str_32(int x, int y,
                       unsigned int forecolor,
                       unsigned int backcolor,
                       const unsigned char *str)
{

    const unsigned char *ptr_hz;


    ptr_hz = str;

    /*显示字符串*/
    while(*ptr_hz != '\0') {
        /*判断是否是汉字*/
        if ((*ptr_hz & 128) != 0) {
            lcd_show_chinese_32x32(x, y, forecolor, backcolor, ptr_hz);
            x      += 32;
            ptr_hz += 2;
            if(x>1024)
             y=+32;
        }

        /*显示字符*/
        else {
            lcd_show_ascii(x, y, forecolor, backcolor, *ptr_hz);
            x +=16;
            ptr_hz++;
        }
    }
}










