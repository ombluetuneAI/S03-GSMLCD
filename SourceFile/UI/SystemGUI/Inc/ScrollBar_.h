

#ifndef __SCROLLBAR_H
#define __SCROLLBAR_H

/*--------------------------------------------头文件-------------------------------------*/
#include "stm32F10x.h"
#include "sys.h"
#include "System.h"



//滚动条尺寸设定
#define ScrollBarMinWidth		20			//两端按钮的长度(即宽度)
#define ScrollBarMinHigh		10			//最小的厚度(即高度)

#define ScrollbarSignLooseColor 	0X0000	    //符号松开颜色为黑色


//图标编号
#define ScrollbarsignUP	 		0X00	    //上图标
#define ScrollbarsignDown 	0X01	    //下图标
#define ScrollbarsignLeft 	0X02	    //左图标
#define ScrollbarsignRight 	0X03	    //右图标
#define ScrollbarsignHOR	 	0X04	    //水平图标
#define ScrollbarsignVER 		0X05	    //垂直图标



#define ScrollbarHorizontal		0X00		//水平scrollbar
#define ScrollbarVertical			0X80		//垂直scrollbar

//滚动条默认颜色 
#define ScrollbarBackColor				0XFFFF		//填充色A颜色
#define ScrollbarButtonColor			0X2DDA //0X031F		//填充色B颜色
#define ScrollbarFrameColor				0X4A49		//边框颜色


typedef struct
{
	u16 x;
	u16 y;
	u16 xSize;
	u16 ySize;
	u16 BackColor;//按钮背景色
	u16 FrameColor;//边框颜色
	u16 SignColor;//符号颜色
	u16 Signx;//符号编号 0,上;1,下;2,左;3,右;4,垂直;5,水平;
}ScrollbarButton;

typedef struct
{
	//坐标尺寸
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
	
	uint8_t ID;
	uint8_t State;//按钮状态 bit0:左端/顶部按钮;bit1:中间按钮;bit2:右端/底部按钮.	 
	uint8_t Type;//类型标记字节 [bit7]:方向,0,水平;1,垂直;bit[6:2]:保留;[bit1:0]:按钮类型,0,标准滚动条;1,没有端按钮的滚动条;2,3,保留.
	uint16_t TotalItems;//总条目
	uint16_t PerpageItems;//每页显示的条目
	uint16_t TopItems;//最顶端的条目
	uint16_t scrollbarLength;//滚动长长度
	
	uint16_t BackColor;
	uint16_t ButtonColor;   //按钮颜色
	uint16_t FrameColor;//边框颜色
	
}Scrollbar;

/*---------------------------------------------函数--------------------------------------*/


#endif

