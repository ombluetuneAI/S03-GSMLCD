

#ifndef __SCROLLBAR_H
#define __SCROLLBAR_H

/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "stm32F10x.h"
#include "sys.h"
#include "System.h"



//�������ߴ��趨
#define ScrollBarMinWidth		20			//���˰�ť�ĳ���(�����)
#define ScrollBarMinHigh		10			//��С�ĺ��(���߶�)

#define ScrollbarSignLooseColor 	0X0000	    //�����ɿ���ɫΪ��ɫ


//ͼ����
#define ScrollbarsignUP	 		0X00	    //��ͼ��
#define ScrollbarsignDown 	0X01	    //��ͼ��
#define ScrollbarsignLeft 	0X02	    //��ͼ��
#define ScrollbarsignRight 	0X03	    //��ͼ��
#define ScrollbarsignHOR	 	0X04	    //ˮƽͼ��
#define ScrollbarsignVER 		0X05	    //��ֱͼ��



#define ScrollbarHorizontal		0X00		//ˮƽscrollbar
#define ScrollbarVertical			0X80		//��ֱscrollbar

//������Ĭ����ɫ 
#define ScrollbarBackColor				0XFFFF		//���ɫA��ɫ
#define ScrollbarButtonColor			0X2DDA //0X031F		//���ɫB��ɫ
#define ScrollbarFrameColor				0X4A49		//�߿���ɫ


typedef struct
{
	u16 x;
	u16 y;
	u16 xSize;
	u16 ySize;
	u16 BackColor;//��ť����ɫ
	u16 FrameColor;//�߿���ɫ
	u16 SignColor;//������ɫ
	u16 Signx;//���ű�� 0,��;1,��;2,��;3,��;4,��ֱ;5,ˮƽ;
}ScrollbarButton;

typedef struct
{
	//����ߴ�
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
	
	uint8_t ID;
	uint8_t State;//��ť״̬ bit0:���/������ť;bit1:�м䰴ť;bit2:�Ҷ�/�ײ���ť.	 
	uint8_t Type;//���ͱ���ֽ� [bit7]:����,0,ˮƽ;1,��ֱ;bit[6:2]:����;[bit1:0]:��ť����,0,��׼������;1,û�ж˰�ť�Ĺ�����;2,3,����.
	uint16_t TotalItems;//����Ŀ
	uint16_t PerpageItems;//ÿҳ��ʾ����Ŀ
	uint16_t TopItems;//��˵���Ŀ
	uint16_t scrollbarLength;//����������
	
	uint16_t BackColor;
	uint16_t ButtonColor;   //��ť��ɫ
	uint16_t FrameColor;//�߿���ɫ
	
}Scrollbar;

/*---------------------------------------------����--------------------------------------*/


#endif

