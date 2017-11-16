
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
#include <stdlib.h>                    //exit()���������ͷ�ļ� ��
#include <sys/time.h>
//#include <errno.h>
#include <sys/wait.h>
//#include <string.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>

#include "lcd.h"

#define HZK32_PATH        "HZK32"     //���ֿ���ļ� ·��

extern  const unsigned char fontdata_8x16[]; //8x16�ļ���������



/*************************************************************
***** ����  ����  init_hzk32_open
***** ��    �ܣ�  ���ֿ��ļ��ж�ȡһ�����ֵ���ģ��������
***** ��    ����  path:HZK32�ļ����·��
***** �� �� ֵ��  -1:ʧ��;  0:�ɹ� 
***** �� �� ��:
***** ����ʱ�䣺
***** ������:
***** ˵    ����
*************************************************************/
int hzk32_init_hzk32_open(const char * path );


/*************************************************************
***** ����  ����  hzk32_end_hzk32_close
***** ��    �ܣ�  �ر� hzk32�ļ�
***** ��    ����  fp_hzk:Ҫ�رյ��ļ�ָ��
***** �� �� ֵ��  -1:ʧ��;  0 :�ɹ�
***** �� �� ��:
***** ����ʱ�䣺
***** ������:
***** ˵    ����
*************************************************************/
int hzk32_end_hzk32_close(void);


/****************************************************************
***** ����  ����  lcd_show_ascii()
***** ��    �ܣ�  ��ʾ32X8�ַ�
***** ��    ����  fbmem_start:fb��������ʼ��ַ
                  x:��ʾ��ʼ������    y:��ʾ��ʼ������
                  forecolor:ǰ��ɫ��Ҳ�����ַ���ɫ�� backcolor������ɫ
***** �� �� ֵ��  ��
***** �� �� ��:
***** ����ʱ�䣺
***** ������:
***** ˵    ����
*************************************************************/
void lcd_show_ascii(
    int x, int y,
    unsigned int forecolor,
    unsigned int backcolor,
    unsigned char ch);


/*************************************************************
***** ����  ����  lcd_show_chinese_32x32
***** ��    �ܣ�  ��ʾ32X32����
***** ��    ����
                  x:��ʾ��ʼ������    y:��ʾ��ʼ������ ��
                  chs��Ҫ��ʾ�������ַ�����ע����GB2312���룩
                  forecolor:ǰ��ɫ��Ҳ�����ַ���ɫ�� backcolor������ɫ
***** �� �� ֵ��  ��
***** �� �� ��:
***** ����ʱ�䣺
***** ������:
***** ˵    ����ʹ�õ���ȫ�ֱ���  fp_hzk
*************************************************************/
void lcd_show_chinese_32x32(unsigned int x, unsigned int y,
                               unsigned int fc, unsigned int bc,
                               const unsigned char* chs);


/*************************************************************
***** ����  ����  lcd_printf_str_32
***** ��    �ܣ�  �����ʾ32X32���ֺ�32��8�ַ�
***** ��    ����  fbmem_start:fb��������ʼ��ַ
                  x:��ʾ��ʼ������    y:��ʾ��ʼ������ ��
                  str��Ҫ��ʾ�������ַ�����ע����GB2312���룩
                  forecolor:ǰ��ɫ��Ҳ�����ַ���ɫ�� backcolor������ɫ
***** �� �� ֵ��  ��
***** �� �� ��:
***** ����ʱ�䣺
***** ������:
***** ˵    ����
*************************************************************/
void lcd_printf_str_32(int x, int y,
                       unsigned int forecolor,
                       unsigned int backcolor,
                       const unsigned char *str);
                       
                       

#endif







