#ifndef __DEVICE_LCD_H
#define __DEVICE_LCD_H
/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "stm32f10x.h"
#include "sys.h"
/*---------------------------------------------����--------------------------------------*/
//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111110=0X3E	
#define LCDBASE   (uint32_t) (0x60000000 |0x01FFFE )
#define LCDAdd       ((LCDTypedef *) LCDBASE)

#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* ��8λR,G,Bת��Ϊ 16λRGB565��ʽ */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)

typedef struct
{
	uint16_t x0;
	uint16_t y0;
	uint32_t *Add;
		
}Bitmap;

//��ɫ
#define Red		0xf800
#define Green	0x07e0
#define Blue	0x001f
#define Black	0x0000

#define HalfBlack	0x7bef
#define White	0xffff
#define Yellow	0xffe0


#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //
#define BRRED 			 0XFC07 //
#define GRAY  			 0X8430 //


#define DARKBLUE      	 0X01CF	//
#define LIGHTBLUE      	 0X7D7C	// 
#define GRAYBLUE       	 0X5458 //

 
#define LIGHTGREEN     	 0X841F //
//#define LIGHTGRAY        0XEF5B //
#define LGRAY 			 0XC618 //

#define LGRAYBLUE        0XA651 //
#define LBBLUE           0X2B12 //
/*--------------------------------------------�궨��-------------------------------------*/

//-----------------LCD�˿ڶ���---------------- 
#define	LcdLED PBout(11) //LCD����    		 PB0 	  

#define LcdLedOn()		LcdLED=0
#define LcdLedOff()		LcdLED=1

#define LcdReset		PBout(10)


#define LCDx_Init()		LCDInit();
/*---------------------------------------------����--------------------------------------*/
typedef struct Headcolor
{
    unsigned char scan;
    unsigned char gray;
    unsigned short w;
    unsigned short h;
    unsigned char is565;
    unsigned char rgb;
}Headcolor;
typedef enum
{
	L2RU2D  =0 ,//������,���ϵ���
	L2RD2U  =1 ,//������,���µ���
	R2LU2D  =2 ,//���ҵ���,���ϵ���
	R2LD2U  =3 ,//���ҵ���,���µ���

	U2DL2R  =4 ,//���ϵ���,������
	U2DR2L  =5 ,//���ϵ���,���ҵ���
	D2UL2R  =6 ,//���µ���,������
	D2UR2L  =7 ,//���µ���,���ҵ���	
}ScanDriectionEnum;

typedef struct
{
    uint16_t LCDRegister;
    uint16_t LCDRAMData;
      
}LCDTypedef;

typedef enum
{
	Vertical=0,
	Horizontal=1,
}DirectionEnum;
//LCD��Ҫ������
typedef struct  
{										    
	u16 Width;			//LCD ���
	u16 Height;			//LCD �߶�
	u16 ID;				//LCD ID
	u8  Direction;			//���������������ƣ�0��������1��������	
	u16	WritRamCommand;		//��ʼдgramָ��
	u16 SetCommandX;		//����x����ָ��
	u16 SetCommandY;		//����y����ָ��	 
}LcdStruct; 
/*---------------------------------------------����--------------------------------------*/



void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);

/*---------------------------------------------����--------------------------------------*/

void LCDSetCursor(uint16_t Xpos,uint16_t Ypos);
void LCDWriteRAMPrepare(void);	//��ʼд��GRAM

void LCDInit(void);

void LCDClear(uint16_t color);
uint16_t LCDReadRegister(uint16_t RegisterAdd);
void LCDDrawPoint(uint16_t X,uint16_t Y,uint16_t Color);
void LCDDrawRectangle(uint16_t XSatrt, uint16_t YStart, uint16_t XEnd, uint16_t YEnd,uint16_t Color);
uint16_t LCDReadPointRGB(uint16_t X,uint16_t Y);
void LCDDrawHorizontalLine(uint16_t X,uint16_t Y,uint16_t Length,uint16_t Color);
void LCDDrawVerticalLine(uint16_t X,uint16_t Y,uint16_t High,uint16_t Color);

void LcdFillArea(u16 x0,u16 y0,u16 x1,u16 y1,u16 color);
void LCDFillAlpha(u16 x,u16 y,u16 width,u16 height,u16 color);
  
void ImageDraw(u16 x,u16 y,u8 *color);
void ImageDrawAlpah(u16 x,u16 y,u8 *color);
void ImageLocalDraw(u16 x,u16 y,u16 wide,u16 high,u8 *pointer);
#endif

