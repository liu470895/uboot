
/*******************************************************************************
** �������ƣ���ʾʹ��hzk32�����ֿ�ĳ���
** ����������ʹ��hzk32ʵ����ʾ16*16������
** ����������
** ����汾��V1.0
** �������ߣ�
** ����޸ģ�2011��8��16��
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

FILE *fp_hzk;                         //���ֿ��ļ���ָ�� ������ָ��򿪵�hzk32�ļ�


/*************************************************************
***** ����  ����  init_hzk32_open
***** ��    �ܣ�  ���ֿ��ļ��ж�ȡһ�����ֵ���ģ��������
***** ��    ����  path:hzk32�ļ����·��
***** �� �� ֵ��  -1:ʧ��;  0:�ɹ� 
***** �� �� ��:
***** ����ʱ�䣺
***** ������:
***** ˵    ����
*************************************************************/
int hzk32_init_hzk32_open(const char * path )
{
   // FILE *fp_hzk;
    if(!path ) {
        return NULL;
    }

    //�򿪺��ֿ����ģ�ļ�
    if((fp_hzk = fopen(path, "r")) == NULL) {
    	 printf("Error��file:%s\r\nline:%d,function:%s\r\n",__FILE__,__LINE__,__FUNCTION__);
       return -1;    //���ֿ��ļ�
    }

    return 0;
}


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
int hzk32_end_hzk32_close()
{
    fclose(fp_hzk);
    return 0;
}



/*************************************************************
***** ����  ����  hzk32_get_data_32byte
***** ��    �ܣ�  ���ֿ��ļ��ж�ȡһ�����ֵ���ģ��������
***** ��    ����  unsigned char *s ָ��Ŀ�꺺�ֵ�ָ��,
                  char* const chs ���ڴ洢��ģ�������׵�ַ
                  fp_hzk : ָ���Ѿ��򿪵�hzk32�ļ�ָ��
***** �� �� ֵ��  -1:ʧ�ܣ������Ƿ��� 0>=:�ɹ���
***** �� �� ��:
***** ����ʱ�䣺
***** ������:
***** ˵    ����
*************************************************************/
static int hzk32_get_data_32byte(unsigned char * buffer, const unsigned char *s, FILE *fp_hzk)
{

    unsigned long offset;
    int ret;
    if(!s || !buffer || !fp_hzk ) {
        return -1;
    }

    //���������ҳ�������hzk32�е�ƫ��λ��
    offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 128;

    fseek(fp_hzk, offset, SEEK_SET);                   //�ļ�ָ��ƫ�Ƶ�Ҫ�ҵĺ��ִ�
    ret = fread(buffer, 1, 128, fp_hzk);                //��ȡ�ú��ֵ���ģ
    return ret;
}




/****************************************************************
***** ����  ����  lcd_show_ascii()
***** ��    �ܣ�  ��ʾ16X8�ַ�
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
***** ����  ����  show_chinese_32x32
***** ��    �ܣ�  ��ʾ32x32����
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
                               const unsigned char* buf)
{
    int i, j;
    unsigned char data[128]; 
    int temp;                     
    hzk32_get_data_32byte(data,buf,fp_hzk);      //ȥ�ֿ��ж�ȡ������ģ

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
***** ����  ����  lcd_printf_str_16
***** ��    �ܣ�  �����ʾ32x32���ֺ�16��8�ַ�
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
                       const unsigned char *str)
{

    const unsigned char *ptr_hz;


    ptr_hz = str;

    /*��ʾ�ַ���*/
    while(*ptr_hz != '\0') {
        /*�ж��Ƿ��Ǻ���*/
        if ((*ptr_hz & 128) != 0) {
            lcd_show_chinese_32x32(x, y, forecolor, backcolor, ptr_hz);
            x      += 32;
            ptr_hz += 2;
            if(x>1024)
             y=+32;
        }

        /*��ʾ�ַ�*/
        else {
            lcd_show_ascii(x, y, forecolor, backcolor, *ptr_hz);
            x +=16;
            ptr_hz++;
        }
    }
}










