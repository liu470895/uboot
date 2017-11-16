
#ifndef __HZK32_H__
#define __HZK32_H__

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



/*************************************************************
***** 函数  名：  init_hzk32_open
***** 功    能：  从字库文件中读取一个汉字的字模点阵数据
***** 参    数：  path:HZK32文件存放路径
***** 返 回 值：  -1:失败;  0:成功 
***** 创 建 者:
***** 创建时间：
***** 最后更新:
***** 说    明：
*************************************************************/
int hzk32_init_hzk32_open(const char * path );


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
int hzk32_end_hzk32_close(void);


/****************************************************************
***** 函数  名：  lcd_show_ascii()
***** 功    能：  显示32X8字符
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
    unsigned char ch);


/*************************************************************
***** 函数  名：  lcd_show_chinese_32x32
***** 功    能：  显示32X32汉字
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
                               const unsigned char* chs);


/*************************************************************
***** 函数  名：  lcd_printf_str_32
***** 功    能：  混合显示32X32汉字和32＊8字符
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
                       const unsigned char *str);
                       
                       

#endif







