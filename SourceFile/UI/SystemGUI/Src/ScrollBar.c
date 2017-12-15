



/*--------------------------------------------头文件-------------------------------------*/
#include "ScrollBar_.h"
/*---------------------------------------------常数--------------------------------------*/


/*--------------------------------------------宏定义-------------------------------------*/


/*---------------------------------------------变量--------------------------------------*/
//6个 8*8的两个标志图案
const u8 ScrollSign[6][8]=
{
		{0x00,0x00,0x10,0x38,0x7C,0xFE,0x00,0x00},		//上图标  
		{0x00,0x00,0xFE,0x7C,0x38,0x10,0x00,0x00},		//下图标
		{0x04,0x0C,0x1C,0x3C,0x1C,0x0C,0x04,0x00}, 		//左图标
		{0x20,0x30,0x38,0x3C,0x38,0x30,0x20,0x00},		//右图标
		{0x00,0xFE,0x00,0xFE,0x00,0xFE,0x00,0x00},		//垂直图标
		{0x00,0x54,0x54,0x54,0x54,0x54,0x54,0x54},		//水平图标
}; 


/*---------------------------------------------类型--------------------------------------*/


/*---------------------------------------------函数--------------------------------------*/


/******************************************************************************************
*描述				:	创建一个滚动条
*输入参数		: x0,y0,xSize,ySize：坐标和尺寸
							type:[bit7]:方向,0,水平;1,垂直;
							[bit6:2]:保留;
							[bit1:0]:按钮类型,0,标准滚动条;1,没有端按钮的滚动条;2,3,保留.
*返回参数		: 返回一个ScrollBar的指针
******************************************************************************************/

static Scrollbar * ScrollbarCreat(uint x0,uint y0,uint xSize,uint ySize,u8 type)
{
	Scrollbar *ScrollbarPointer;
	
	
	ScrollbarPointer=(Scrollbar *)malloc(sizeof(Scrollbar));
	if(ScrollbarPointer==NULL)
		return NULL;
	memset((u8*)ScrollbarPointer,0,sizeof(Scrollbar));//将scrollbar_crt的值全部设置为0
	
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
*描述   ：删除滚动条
*
***********************************************************************************/
static void ScrollBarDelete(Scrollbar *ScrollbarDelete)
{
		free(ScrollbarDelete);
}

/*******************************************************************************************************
*描述			：画符号
*输入参数	：x,y，坐标
						Signx，符号编号
*
*******************************************************************************************************/
static void ScrollbarDrawSign(u16 x,u16 y,u16 SignColor,u8 Signx)
{
	u8 i,j;
	u8 *signbuf;
	u8 temp;	  
	signbuf=(u8*)ScrollSign[Signx];//得到图标像素阵列
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
	u16 BackColor;//按钮背景色
	u16 FrameColor;//边框颜色
	u16 SignColor;//符号颜色
	u16 Signx;//符号编号 0,上;1,下;2,左;3,右;4,垂直;5,水平;
******************************************************************************************************************/
static void ScrollbarDrawButton(	u16 x,u16 y,u16 xSize,u16 ySize,u16 BackColor,u16 FrameColor,u16 SignColor,u16 Signx)
{
	//内部颜色填充
	GUI_SetColor(BackColor);
	GUI_FillRect(x,y,xSize,ySize);
	
	//画边框
	GUI_SetColor(FrameColor);
	GUI_DrawRect(x,y,xSize,ySize);
	
	if((xSize>=8)&&(ySize>=8))//尺寸足够画按钮
	{
			ScrollbarDrawSign(x+xSize/2-4,y+ySize/2-4,SignColor,Signx);
	}
}


static void ScrollBarDraw(Scrollbar * ScrollbarPointer)
{
	u32 ScrollBarLength=0;//滚动条可以滚动的空间长度
	
	
	if(ScrollbarPointer==NULL)//无效的退出
		return;
	GUI_SetColor(ScrollbarPointer->BackColor);
	GUI_FillRect(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollbarPointer->x1,ScrollbarPointer->y1);
	GUI_SetColor(ScrollbarPointer->FrameColor);
	GUI_DrawRect(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollbarPointer->x1,ScrollbarPointer->y1);
	
	if(0==(0x03&ScrollbarPointer->Type))//是标准滚动条
	{
			if(0==(0x80&ScrollbarPointer->Type))//水平滚动条
			{
					ScrollbarDrawButton(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollBarMinWidth,ScrollbarPointer->y1,
															ScrollbarPointer->ButtonColor,ScrollbarPointer->FrameColor,ScrollbarSignLooseColor,ScrollbarsignLeft);
					ScrollbarDrawButton(ScrollbarPointer->x0+ScrollbarPointer->x0-ScrollBarMinWidth,ScrollbarPointer->y0,
															ScrollBarMinWidth,ScrollbarPointer->y1,ScrollbarPointer->ButtonColor,ScrollbarPointer->FrameColor,ScrollbarSignLooseColor,ScrollbarsignRight);
					ScrollBarLength=ScrollbarPointer->x1-2*ScrollBarMinWidth;//可供 滚动条滚动的空间范围
			}else//垂直滚动条
			{
					
				//ScrollbarDrawButton(ScrollbarPointer->x0,ScrollbarPointer->y0,ScrollBarMinWidth,
			}
	}
}



