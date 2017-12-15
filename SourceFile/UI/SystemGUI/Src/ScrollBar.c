



/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "ScrollBar_.h"
/*---------------------------------------------����--------------------------------------*/


/*--------------------------------------------�궨��-------------------------------------*/


/*---------------------------------------------����--------------------------------------*/
//6�� 8*8��������־ͼ��
const u8 ScrollSign[6][8]=
{
		{0x00,0x00,0x10,0x38,0x7C,0xFE,0x00,0x00},		//��ͼ��  
		{0x00,0x00,0xFE,0x7C,0x38,0x10,0x00,0x00},		//��ͼ��
		{0x04,0x0C,0x1C,0x3C,0x1C,0x0C,0x04,0x00}, 		//��ͼ��
		{0x20,0x30,0x38,0x3C,0x38,0x30,0x20,0x00},		//��ͼ��
		{0x00,0xFE,0x00,0xFE,0x00,0xFE,0x00,0x00},		//��ֱͼ��
		{0x00,0x54,0x54,0x54,0x54,0x54,0x54,0x54},		//ˮƽͼ��
}; 


/*---------------------------------------------����--------------------------------------*/


/*---------------------------------------------����--------------------------------------*/


/******************************************************************************************
*����				:	����һ��������
*�������		: x0,y0,xSize,ySize������ͳߴ�
							type:[bit7]:����,0,ˮƽ;1,��ֱ;
							[bit6:2]:����;
							[bit1:0]:��ť����,0,��׼������;1,û�ж˰�ť�Ĺ�����;2,3,����.
*���ز���		: ����һ��ScrollBar��ָ��
******************************************************************************************/

static Scrollbar * ScrollbarCreat(uint x0,uint y0,uint xSize,uint ySize,u8 type)
{
	Scrollbar *ScrollbarPointer;
	
	
	ScrollbarPointer=(Scrollbar *)malloc(sizeof(Scrollbar));
	if(ScrollbarPointer==NULL)
		return NULL;
	memset((u8*)ScrollbarPointer,0,sizeof(Scrollbar));//��scrollbar_crt��ֵȫ������Ϊ0
	
	ScrollbarPointer->x0=x0;
	ScrollbarPointer->y0=y0;
	ScrollbarPointer->x1=xSize;
	ScrollbarPointer->y1=ySize;
	ScrollbarPointer->Type=type;
	ScrollbarPointer->State=0;
	ScrollbarPointer->ID=0;
	ScrollbarPointer->TotalItems=0;
	ScrollbarPointer->PerpageItems=0;
	ScrollbarPointer->TopItems=0;
	ScrollbarPointer->scrollbarLength=0;
	
	ScrollbarPointer->BackColor=ScrollbarBackColor;
	ScrollbarPointer->ButtonColor=ScrollbarButtonColor;
	ScrollbarPointer->FrameColor=ScrollbarFrameColor;
	
	return ScrollbarPointer;
}


/***********************************************************************************
*����   ��ɾ��������
*
***********************************************************************************/
static void ScrollBarDelete(Scrollbar *ScrollbarDelete)
{
		free(ScrollbarDelete);
}

/*******************************************************************************************************
*����			��������
*�������	��x,y������
						Signx�����ű��
*
*******************************************************************************************************/
static void ScrollbarDrawSign(u16 x,u16 y,u16 SignColor,u8 Signx)
{
	u8 i,j;
	u8 *signbuf;
	u8 temp;	  
	signbuf=(u8*)ScrollSign[Signx];//�õ�ͼ����������
	for(i=0;i<8;i++)
	{
		temp=signbuf[i];		  
		for(j=0;j<8;j++)
		{
			if(temp&0x80)
				System.Device.Lcd.LcdDrawPoint(x+j,y+i,SignColor);		
			temp<<=1;		  
		}	
	}
}
/******************************************************************************************************************
*
*	u16 x;
	u16 y;
	u16 xSize;
	u16 ySize;
	u16 BackColor;//��ť����ɫ
	u16 FrameColor;//�߿���ɫ
	u16 SignColor;//������ɫ
	u16 Signx;//���ű�� 0,��;1,��;2,��;3,��;4,��ֱ;5,ˮƽ;
******************************************************************************************************************/
static void ScrollbarDrawButton(	u16 x,u16 y,u16 xSize,u16 ySize,u16 BackColor,u16 FrameColor,u16 SignColor,u16 Signx)
{
	//�ڲ���ɫ���
	GUI_SetColor(BackColor);
	GUI_FillRect(x,y,xSize,ySize);
	
	//���߿�
	GUI_SetColor(FrameColor);
	GUI_DrawRect(x,y,xSize,ySize);
	
	if((xSize>=8)&&(ySize>=8))//�ߴ��㹻����ť
	{
			ScrollbarDrawSign(x+xSize/2-4,y+ySize/2-4,SignColor,Signx);
	}
}


static void ScrollBarDraw(Scrollbar * ScrollbarPointer)
{
	u32 ScrollBarLength=0;//���������Թ����Ŀռ䳤��
	
	
	if(ScrollbarPointer==NULL)//��Ч���˳�
		return;
	GUI_SetColor(ScrollbarPointer->BackColor);
	GUI_FillRect(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollbarPointer->x1,ScrollbarPointer->y1);
	GUI_SetColor(ScrollbarPointer->FrameColor);
	GUI_DrawRect(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollbarPointer->x1,ScrollbarPointer->y1);
	
	if(0==(0x03&ScrollbarPointer->Type))//�Ǳ�׼������
	{
			if(0==(0x80&ScrollbarPointer->Type))//ˮƽ������
			{
					ScrollbarDrawButton(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollBarMinWidth,ScrollbarPointer->y1,
															ScrollbarPointer->ButtonColor,ScrollbarPointer->FrameColor,ScrollbarSignLooseColor,ScrollbarsignLeft);
					ScrollbarDrawButton(ScrollbarPointer->x0+ScrollbarPointer->x0-ScrollBarMinWidth,ScrollbarPointer->y0,
															ScrollBarMinWidth,ScrollbarPointer->y1,ScrollbarPointer->ButtonColor,ScrollbarPointer->FrameColor,ScrollbarSignLooseColor,ScrollbarsignRight);
					ScrollBarLength=ScrollbarPointer->x1-2*ScrollBarMinWidth;//�ɹ� �����������Ŀռ䷶Χ
			}else//��ֱ������
			{
					
				//ScrollbarDrawButton(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollBarMinWidth,
			}
	}
}



