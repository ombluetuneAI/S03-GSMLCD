/*----------------------------------------------------------------------------------------

描述        :       TFT的驱动IC : ST7775 
光标设定    :  
                    * x
                    *
                    *
                    *
                  0 * * * * * * * y
            

-----------------------------------------------------------------------------------------*/

/*--------------------------------------------头文件-------------------------------------*/
#include "device_Lcd.h"
#include "system.h"

#include "device_SpiFlash.h"
/*---------------------------------------------常数--------------------------------------*/
#define LcdID	0x9341

/*--------------------------------------------宏定义-------------------------------------*/

/*---------------------------------------------变量--------------------------------------*/
LcdStruct LcdDevice;

/*---------------------------------------------类型--------------------------------------*/



/*---------------------------------------------函数--------------------------------------*/
static void LCDDrawingBitmap(Bitmap *BitmapPointer);
//static void LCDColorFill(uint16_t XStart,uint16_t YStart,uint16_t XEnd,uint16_t Yend,uint16_t Color);


/*-----------------------------------------------------------------------------------------------
--描述      :     写寄存器
--参数      :     RegisterData; 寄存器值
--返回值    :     无
-----------------------------------------------------------------------------------------------*/
static void LCDWriteReg(uint16_t RegisterData)
{
    LCDAdd->LCDRegister=RegisterData;
}

/*-----------------------------------------------------------------------------------------------
--描述      :     写寄存器数据
--参数      :     RAMData; 写入的值
--返回值    :     无
-----------------------------------------------------------------------------------------------*/
void LCDWriteData(uint16_t RAMData)
{
    LCDAdd->LCDRAMData=RAMData;
}
/*************************************************************************************************


*************************************************************************************************/
uint16_t LCDReadData(void)
{
    return LCDAdd->LCDRAMData;
}
/*-----------------------------------------------------------------------------------------------
--描述      :     写寄存器
--参数      :     RegisterAdd : 寄存器地址
                  RegisterValue : 写入寄存器的值
--返回值    :     无
-----------------------------------------------------------------------------------------------*/
static void LCDWriteRegister(uint16_t RegisterAdd,uint16_t RegisterValue)
{
    LCDAdd->LCDRegister=RegisterAdd;
    LCDAdd->LCDRAMData=RegisterValue;
}
/*-----------------------------------------------------------------------------------------------
--描述      :     读寄存器
--参数      :     RegisterAdd : 寄存器地址
--返回值    :     读到的数据
-----------------------------------------------------------------------------------------------*/
uint16_t LCDReadRegister(uint16_t RegisterAdd)
{
    LCDWriteReg(RegisterAdd);
    DelayUs(5);	
    return LCDReadData();
}

/****************************************************************************************************


****************************************************************************************************/
void LCDWriteRAMPrepare(void)	//开始写入GRAM
{
    LCDAdd->LCDRegister=LcdDevice.WritRamCommand;
}

/***********************************************************************************************/

static u16 RGB888toGRB565(u32 color)
{
	u8 R,G,B;
// 	B=(color>>3)&0x00001F;
// 	B|=((color>>10)&0x00001F)<<5;
// 	
// 	R=((color>>13)&0x000007)|(((color>>18)&0x000001)<<3);
// 	R|=((color>>20)&0x00000F)<<4;
// 	R<<=8;
// 	G=((color>>3)&0x000001F)|(((color>>10)&0x000007)<<5);
// 	resColor=R|G;
	R=color>>16;
	G=color>>8;
	B=color;
	return (RGB(R,G,B));
	
}


/*----------------------------------------------------------------------------------------------
--描述      :     TFT屏幕用的IO口进行初始化
--参数      :     无
--返回值    :     无
----------------------------------------------------------------------------------------------*/
static void LCDGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|
						   RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|
						   RCC_APB2Periph_AFIO,ENABLE);//使能PORTB,D,E,G以及AFIO复用功能时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;				 //PB0 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	LcdLedOff();
	
	
    //GPIOD 复用推挽输出
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0    |     GPIO_Pin_1  |     GPIO_Pin_4  |     \
                              GPIO_Pin_5    |     GPIO_Pin_8  |     GPIO_Pin_9  |     \
                              GPIO_Pin_10   |     GPIO_Pin_14 |     GPIO_Pin_15 |     \
                              GPIO_Pin_7    |     GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD,&GPIO_InitStructure);

    //GPIOE 复用推挽输出
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7  |     GPIO_Pin_8  |     GPIO_Pin_9  |     GPIO_Pin_10 |     \
                              GPIO_Pin_10   |     GPIO_Pin_11 |     GPIO_Pin_12 |     \
                              GPIO_Pin_13   |     GPIO_Pin_14 |     GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIO_InitStructure);    
}
/*----------------------------------------------------------------------------------------------
--描述      :     TFT屏幕用的复位
--参数      :     无
--返回值    :     无
----------------------------------------------------------------------------------------------*/
static void LCDReset(void)
{	
  LcdReset=1;
  DelayMs(100);
  LcdReset=0;
  DelayMs(100);
	LcdReset=1;
}

/*----------------------------------------------------------------------------------------------
--描述      :     TFT屏幕用的FSMC初始化
--参数      :     无
--返回值    :     无
----------------------------------------------------------------------------------------------*/
static void LCDFSMCInit(void)
{
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  ReadWriteTiming; 
    FSMC_NORSRAMTimingInitTypeDef  WriteTiming;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);

    ReadWriteTiming.FSMC_AddressSetupTime=0x01;//地址建立时间
    ReadWriteTiming.FSMC_AddressHoldTime=0;//地址保持时间
    ReadWriteTiming.FSMC_DataSetupTime=0x0f;//数据保存时间为16个HCLK,LCD的数据不能太快了
    ReadWriteTiming.FSMC_BusTurnAroundDuration=0x00;
    ReadWriteTiming.FSMC_CLKDivision=0x00;
    ReadWriteTiming.FSMC_DataLatency=0x00;
    ReadWriteTiming.FSMC_AccessMode=FSMC_AccessMode_A;

    WriteTiming.FSMC_AddressSetupTime = 0x00;	 
    WriteTiming.FSMC_AddressHoldTime = 0x00;	 	
    WriteTiming.FSMC_DataSetupTime = 0x03;			
    WriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    WriteTiming.FSMC_CLKDivision = 0x00;
    WriteTiming.FSMC_DataLatency = 0x00;
    WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	


    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  这里我们使用NE1 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &ReadWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &WriteTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // 使能BANK1 
		
}




     
static void LcdScanDirection(u8 Direction)
{
	u16 regval=0;
	u16 dirreg=0; 
	switch(Direction)//方向转换
	{
		case 0:Direction=6;break;
		case 1:Direction=7;break;
		case 2:Direction=4;break;
		case 3:Direction=5;break;
		case 4:Direction=1;break;
		case 5:Direction=0;break;
		case 6:Direction=3;break;
		case 7:Direction=2;break;	     
	}
	switch(Direction)
		{
			case L2RU2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2RD2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2LU2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2LD2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2DL2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2DR2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2UL2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2UR2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
	dirreg=0X36;
	regval|=0X08;//5310/5510不需要BGR 
	LCDWriteRegister(dirreg,regval);
	LCDWriteReg(LcdDevice.SetCommandX); 
	LCDWriteData(0);
	LCDWriteData(0);
	LCDWriteData((LcdDevice.Width-1)>>8);
	LCDWriteData((LcdDevice.Width-1)&0XFF);
	LCDWriteReg(LcdDevice.SetCommandY); 
	LCDWriteData(0);
	LCDWriteData(0);
	LCDWriteData((LcdDevice.Height-1)>>8);
	LCDWriteData((LcdDevice.Height-1)&0XFF);  
}
/****************************************************************************************
*描述         	：   设定显示方向
*输入参数		：   无	
*输出参数		:    无

****************************************************************************************/
static void LcdDisplayDirection(DirectionEnum Direction)
{
	if(Direction==Vertical)
	{
		LcdDevice.Direction=Vertical;
		LcdDevice.Width=240;
		LcdDevice.Height=320;
		
		LcdDevice.WritRamCommand=0X2C;
		LcdDevice.SetCommandX=0X2A;
		LcdDevice.SetCommandY=0X2B;  
	}
	else
	{
		LcdDevice.Direction=Horizontal;
		LcdDevice.Width=320;
		LcdDevice.Height=240;
		
		LcdDevice.WritRamCommand=0X2C;
		LcdDevice.SetCommandX=0X2A;
		LcdDevice.SetCommandY=0X2B;  		
	}
	LcdScanDirection(R2LD2U);
}

// static void LCDAddSet(void)
// {
//     LCDWriteRegister(0x36,0x00AF);
//     LCDWriteRegister(0x37,0x0000);
//     LCDWriteRegister(0x38,0x00DB);
//     LCDWriteRegister(0x39,0x0000);
//     LCDWriteRegister(0x20,0x0000);
//     LCDWriteRegister(0x21,0x0000);
//     LCDWriteReg(0x0022);
// }

 void LCDSetCursor(uint16_t Xpos,uint16_t Ypos)
{

	LCDWriteReg(LcdDevice.SetCommandX); 
	LCDWriteData(Xpos>>8); 
	LCDWriteData(Xpos&0XFF);	 
	LCDWriteReg(LcdDevice.SetCommandY); 
	LCDWriteData(Ypos>>8); 
	LCDWriteData(Ypos&0XFF);	
}
/*----------------------------------------------------------------------------------------------
--描述      :     读出的数据为GBR格式，而我们写入的时候为RGB格式。
--参数      :     Color :  要转换的颜色
--返回值    :     返回转换后的色值
----------------------------------------------------------------------------------------------*/
u16 LCDBGRtoRGB(u16 Color)
{
    uint16_t  R,G,B,RGB;   
    B=(Color>>0)&0x1f;
    G=(Color>>5)&0x3f;
    R=(Color>>11)&0x1f;	 
    RGB=(B<<11)+(G<<5)+(R<<0);		 
    return(RGB);
} 
/*----------------------------------------------------------------------------------------------
--描述      :     读点的颜色
--参数      :     x,y :指定的坐标
--返回值    :     返回该点上的色值
----------------------------------------------------------------------------------------------*/
uint16_t LCDReadPointRGB(uint16_t X,uint16_t Y)
{
  uint32_t r,g,b;
  LCDSetCursor(X,Y);
  LCDWriteReg(0x2E);
	if(LCDAdd->LCDRAMData) 
  {
    r=0;
  }
  r=LCDAdd->LCDRAMData;
	b=LCDAdd->LCDRAMData;
	g=r&0XFE;//对于9341/5310,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8;
  return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
}
/*************************************************************************************************


*************************************************************************************************/
void LcdSetWindow(u16 sx,u16 sy,u16 width,u16 height)
{   
	width=sx+width-1;
	height=sy+height-1;
	if(LcdDevice.ID==0X9341)//6804横屏不支持
	{
		LCDWriteReg(LcdDevice.SetCommandX); 
		LCDWriteData(sx>>8); 
		LCDWriteData(sx&0XFF);	 
		LCDWriteData(width>>8); 
		LCDWriteData(width&0XFF);  
		LCDWriteReg(LcdDevice.SetCommandY); 
		LCDWriteData(sy>>8); 
		LCDWriteData(sy&0XFF); 
		LCDWriteData(height>>8); 
		LCDWriteData(height&0XFF); 
	}
}
/*----------------------------------------------------------------------------------------------
--描述      :     画点
--参数      :     Color : 要输入的颜色
--返回值    :     无
----------------------------------------------------------------------------------------------*/

void LCDDrawPoint(uint16_t X,uint16_t Y,uint16_t Color)
{ 	
	LCDSetCursor(X,Y);      				//设置光标位置 
	LCDWriteRAMPrepare();     			//开始写入GRAM	  
	LCDWriteData(Color);	//设置光标位置 	
}
u16 ImageGetColor(u8 mode,u8 *str)
{
    u16 color;
    if(mode)
    {
        color=((u16)*str++)<<8;
        color|=*str;
    }else
    {
        color=*str++;
        color|=((u16)*str)<<8;
    }
    return color;   
}
/*----------------------------------------------------------------------------------------------
--描述      :     TFT清屏函数
--参数      :     Color : 要输入的颜色
--返回值    :     无
----------------------------------------------------------------------------------------------*/
void LCDClear(uint16_t Color)
{
	u32 index=0;      
	u32 totalpoint=LcdDevice.Width;
	totalpoint*=LcdDevice.Height; 			//得到总点数
	LCDSetCursor(0x00,0x0000);	//设置光标位置 
	LCDWriteRAMPrepare();     		//开始写入GRAM	 	  
	for(index=totalpoint;index>0;index--)
	{
		LCDAdd->LCDRAMData=Color;
	}
}

u16 piclib_alpha_blend(u16 src,u16 dst,u8 alpha)
{
	u32 src2;
	u32 dst2;	 
	//Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
	src2=((src<<16)|src)&0x07E0F81F;
	dst2=((dst<<16)|dst)&0x07E0F81F;   
	//Perform blending R:G:B with alpha in range 0..32
	//Note that the reason that alpha may not exceed 32 is that there are only
	//5bits of space between each R:G:B value, any higher value will overflow
	//into the next component and deliver ugly result.
	dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
	return (dst2>>16)|dst2;  
}

/*********************************************************************************************

描述        ：显示BMP图片
输入参数    ：x,y ;坐标 ;color, 图片指针
输出参数    ：无
备注        ：图片的扫瞄主向是水平方向，高位在前 带图片头信息
              其中头信息占了8字节，
              [0],扫描模式,
              [1],灰度值，也就是颜色深度
              [2],[3];图片的宽度，
              [4],[5],图片的高度
              [6],在4096色模式下为0表示使用[16bits(WORD)]格式，
              		此时图像数据中每个WORD表示一个象素；为1表示使用[12bits(连续字节流)]格式，
              		此时连续排列的每12Bits代表一个象素。
					在16位彩色模式下为0表示R G B颜色分量所占用的位数都为5Bits，
					为1表示R G B颜色分量所占用的位数分别为5Bits,6Bits,5Bits。
					在18位彩色模式下为0表示"6Bits in Low Byte"，为1表示"6Bits in High Byte"。
					在24位彩色和32位彩色模式下is565无效。
				[7],描述R G B颜色分量的排列顺序，rgb中每2Bits表示一种颜色分量，
					[00]表示空白，[01]表示Red，[10]表示Green，[11]表示Blue。
***********************************************************************************************/

void ImageDraw(u16 x,u16 y,u8 *color)
{
	u16 width,height;//??
	u8  grayLevel;//????

	u16 colorTemp;
	u32 index=0;     
	u32 totalpoint;

	grayLevel=color[1];

	width=color[2];
	width<<=8;
	width|=color[3];

	height=color[4];
	height<<=8;
	height|=color[5];

	totalpoint=width*height;

	color+=8;

	switch(grayLevel)
	{
   case 16:
		LCDSetCursor(x,y);	//?????? 
		LcdSetWindow(x,y,width,height);
    LCDWriteRAMPrepare(); //????GRAM
		for(index=totalpoint;index>0;index--)
		{
			colorTemp=((u16)*color++)<<8;
      colorTemp|=*color++;
      if(colorTemp)
      {
        LCDAdd->LCDRAMData = colorTemp;
      }
		}
		LcdSetWindow(0,0,320,240);
		break;
   default:
     break;
	}
}
void ImageDrawAlpah(u16 x,u16 y,u8 *color)
{
	u16 width,height;//??
	u8  grayLevel;//????
	u8  mode;//RGB/BGR
  
	u16 colorTemp;
	u8 alphabend;   
	u16 xSize,ySize;

	grayLevel=color[1];

	width=color[2];
	width<<=8;
	width|=color[3];

	height=color[4];
	height<<=8;
	height|=color[5];


	mode = color[7];
	color+=8;

	switch(grayLevel)
	{
     case 16:
        for(ySize=y;ySize<(y+height);ySize++)
				{
					for(xSize=x;xSize<(x+width);xSize++)
					{
						colorTemp=((u16)*color++)<<8;
            colorTemp|=*color++;
						if(colorTemp)
						{
              if(colorTemp==WHITE)
              {
                colorTemp = WHITE - 1;
              }
							LCDAdd->LCDRegister=LcdDevice.SetCommandX;
							LCDAdd->LCDRAMData=xSize>>8;
							LCDAdd->LCDRAMData=xSize&0XFF;
							LCDAdd->LCDRegister=LcdDevice.SetCommandY;
							LCDAdd->LCDRAMData=ySize>>8;
							LCDAdd->LCDRAMData=ySize&0XFF;
							LCDAdd->LCDRegister=LcdDevice.WritRamCommand; 
							LCDAdd->LCDRAMData=colorTemp;
						}
					}	
				}
		break;
	case 32:
		switch(mode)
		{
		case 0x6C://RGB
			{
				for(ySize=y;ySize<(y+height);ySize++)
				{
					for(xSize=x;xSize<(x+width);xSize++)
					{
						colorTemp=(*color++)>>3;		   		 	//B
						colorTemp+=((u16)(*color++)<<3)&0X07E0;	//G
						colorTemp+=(((u16)*color++)<<8)&0XF800;	//R
						alphabend=*color++;					//ALPHA??
						if(alphabend==0)//???????????
						{
							if(colorTemp)
							{
								LCDAdd->LCDRegister=LcdDevice.SetCommandX;
								LCDAdd->LCDRAMData=xSize>>8;
								LCDAdd->LCDRAMData=xSize&0XFF;
								LCDAdd->LCDRegister=LcdDevice.SetCommandY;
								LCDAdd->LCDRAMData=ySize>>8;
								LCDAdd->LCDRAMData=ySize&0XFF;
								LCDAdd->LCDRegister=LcdDevice.WritRamCommand; 
								LCDAdd->LCDRAMData=colorTemp;
							}
						}
					}	
				}
			}
			break;
		case 0xE4:
			{
				for(ySize=y;ySize<(y+height);ySize++)
				{
					for(xSize=x;xSize<(x+width);xSize++)
					{
						colorTemp=(*color++)>>3;		   		 	//B
						colorTemp+=((u16)(*color++)<<3)&0X07E0;	//G
						colorTemp+=(((u16)*color++)<<8)&0XF800;	//R
						alphabend=*color++;					//ALPHA??
						if(alphabend==0)//???????????
						{
							if(colorTemp)
							{
								LCDAdd->LCDRegister=LcdDevice.SetCommandX;
								LCDAdd->LCDRAMData=xSize>>8;
								LCDAdd->LCDRAMData=xSize&0XFF;
								LCDAdd->LCDRegister=LcdDevice.SetCommandY;
								LCDAdd->LCDRAMData=ySize>>8;
								LCDAdd->LCDRAMData=ySize&0XFF;
								LCDAdd->LCDRegister=LcdDevice.WritRamCommand; 
								LCDAdd->LCDRAMData=colorTemp;
							}
						}
					}	
				}
			}
			break;
		default: 
			break;
		}
		break;
	}
}
void ImageLocalDraw(u16 x,u16 y,u16 wide,u16 high,u8 *pointer)
{
    u32 totalpoint;
    u16 colorTemp;
    u16 xCount=0;
    u16 i;
    
    totalpoint=wide*high;
    pointer+=8;
    pointer+=((y-30-1)*320+x)*2;
    LcdSetWindow(x,y,wide,high);
    LCDWriteRAMPrepare();
    for(i=totalpoint;i>0;i--)
    {
        colorTemp=ImageGetColor(1,pointer); 
        LCDAdd->LCDRAMData=colorTemp;
        pointer+=2;
        xCount++;
        if(xCount==wide)
        {
            xCount=0;
            pointer+=((320-(x+wide))+x)*2;
        }
    }
    LcdSetWindow(0,0,320,240);
}
/*----------------------------------------------------------------------------------------------
--描述      :     画任意直线 (此算法出自计算机图形学)
--参数      :     
--返回值    :     无
----------------------------------------------------------------------------------------------*/
void LCDDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)  
{  
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else 
	{incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCDDrawPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

void LCDDrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint16_t Color)
{
	u16 x1,y1;
	//Color=LCDBGRtoRGB(Color);
	x1=x+width;
	y1=y+height;
  LCDDrawLine(x,y,x1,y,Color);
  LCDDrawLine(x,y,x,y1,Color);
	LCDDrawLine(x,y1,x1,y1,Color);
  LCDDrawLine(x1,y,x1,y1,Color);
}

/*----------------------------------------------------------------------------------------------
--描述      :     填充一整块颜色
--参数      :     
--返回值    :     无
----------------------------------------------------------------------------------------------*/
// static void LCDColorFill(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color)
// {  
//     uint16_t xPos,yPos;
// 	color=LCDBGRtoRGB(color);
//     for(xPos=x;xPos<x+width+1;xPos++)
//     {    				   			 
//     	for(yPos=y;yPos<y+height+1;yPos++)
//         {     
//               LCDDrawPoint(xPos,yPos,color); 
//         }
//     }
// 	  
// }
/*----------------------------------------------------------------------------------------------
--描述      :     画一条横线
--参数      :     x,y,坐标
                  length,线长度
                  color,颜色
--返回值    :     无
----------------------------------------------------------------------------------------------*/
void LCDDrawHorizontalLine(uint16_t X,uint16_t Y,uint16_t Length,uint16_t Color)
{
    uint16_t Xend;
    Xend=X+Length;
    LCDDrawLine(X,Y,Xend,Y,Color);
}
/*----------------------------------------------------------------------------------------------
--描述      :     画一条垂直线
--参数      :     x,y,坐标
                  length,线长度
                  color,颜色
--返回值    :     无
----------------------------------------------------------------------------------------------*/
void LCDDrawVerticalLine(uint16_t X,uint16_t Y,uint16_t High,uint16_t Color)
{
    uint16_t Yend;
    Yend=Y+High;
    LCDDrawLine(X,Y,X,Yend,Color);
}
	



/*******************************************************************************
* Function Name  : GetGBKCode
* Description    : 取GBK内码 数据
* Input          : - *c: 输入的GBK内码，如'我' 
* Output         : - *pBuffer: 存放数据的指针
* Return         : None
* Attention		 : 输入一个GBK内码，取得它的32Byte显示代码并将其存放到一个32byte的显示缓冲pBuffer[]中
*******************************************************************************/
void GetGBKCode(unsigned char* pBuffer,unsigned char * c)
{ 
   unsigned char High8bit,Low8bit; 
   High8bit=*c;     /* 取高8位数据 */
   Low8bit=*(c+1);  /* 取低8位数据 */

   SPI_FLASH_BufferRead(pBuffer,((High8bit-0xb0)*94+Low8bit-0xa1)*64,64);
	// printf("\r\n 从W25x16 SPI FLASH读出的数据是: %s",pBuffer);

}

/******************************************************************************
* Function Name  : PutChinese
* Description    : 将Lcd屏上任意位置显示一个中文字
* Input          : - Xpos: 水平坐标 
*                  - Ypos: 垂直坐标  
*				   - str: 显示的中文字
*				   - Color: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChinese(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t Color,uint16_t bkColor)
{
  uint8_t i=0,j=0,k=0;
  uint8_t buffer[64],mbuffer[32];
  uint16_t tmp_char=0;
	
 
		GetGBKCode(buffer,str);  /* 取字模数据 */
		
		for(k=0;k<32;k++)
		{
			mbuffer[k]=buffer[2*k];
			mbuffer[k]=((mbuffer[k]&0x0f)<<4);
			mbuffer[k]|=(buffer[2*k+1]&0x0f);
		}
	/*				这样也可以
	//	
	//		for(k=0; k<sizeof(mbuffer)/sizeof(mbuffer[0]); k++)//
	//		{
	//			tmp1 =  buffer[2*k];
	//			tmp2 =  buffer[2*k+1];			
	//			tmp  = (tmp1 << 4) | (tmp2&0x0f);
	//			mbuffer[k] = tmp;
	//    }

	*/
		for (i=0;i<16;i++)
		{
			tmp_char=mbuffer[i*2];
			tmp_char=(tmp_char<<8);
			tmp_char|=mbuffer[2*i+1];
			
			for (j=0;j<16;j++)
			{
				if ( (tmp_char >> (15-j)) && 0x01 == 0x01)
					{
						LCDDrawPoint(Xpos+i,Ypos+j,Color);  /* 字符颜色 */
					}
					else
					{
						LCDDrawPoint(Xpos+i,Ypos+j,Color);  /* 字符颜色 */
					}
			}
		}				
}
/******************************************************************************
* Function Name  : GUI_Chinese
* Description    : 在指定座标显示字符串
* Input          : - Xpos: 行座标
*                  - Ypos: 列座标 
*				   - str: 字符串
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
  do
  {
    PutChinese(Xpos,Ypos,str++,Color,bkColor);
	str++;
   if (Ypos<176)
    {
      Ypos+=16;
    }
    else if (Xpos<220)
    {
      Ypos=0;
      Xpos+=16;
    }
    else
    {
      Xpos=0;
      Ypos=0;
    }       
  }
  while(*str!=0);
}  

static void LCDDrawingBitmap(Bitmap *BitmapPointer)
{
		uint8_t y,x;
		uint8_t buffer[8];
		uint8_t G1,B1,R1,A,G0,R0,B0;
		uint32_t usNewRGB;
		uint16_t usOldRGB;
		uint32_t add=54;
		uint8_t size[6];
		uint16_t xSize,ySize;
	
		SPI_FLASH_BufferRead(buffer,*BitmapPointer->Add,3);
		if(('B'!=buffer[0])&&('M'!=buffer[1])&&('P'!=buffer[2])) return;
	//位图在数组中的尺寸数据
		SPI_FLASH_BufferRead(size,*BitmapPointer->Add+18,6);
		

		xSize=size[1];
		xSize=(xSize<<8)|size[0];
		
		ySize=size[5];
		ySize=(ySize<<8)|size[4];
	
		add|=*BitmapPointer->Add;
		for(y=ySize;y>0;y--)
		{
			for(x=0;x<xSize;x++)
			{
				SPI_FLASH_BufferRead(buffer,add,4);				
				
				B1 = buffer[0];
				G1 = buffer[1];
				R1 = buffer[2];
				A =  buffer[3];	/* Alpha 值(透明度)，0-255, 0表示透明，1表示不透明, 中间值表示透明度 */

				if (A == 0x00)	/* 需要透明,显示背景 */
				{
					;	/* 不用刷新背景 */
				}
				else if (A == 0xFF)	/* 完全不透明， 显示新像素 */
				{
					usNewRGB = RGB(R1, G1, B1);

					LCDDrawPoint(BitmapPointer->x0+x,BitmapPointer->y0+y, usNewRGB);
	// 				LCDSetCursor(220-1-(BitmapPointer->x0+x),BitmapPointer->y0+y);
	// 				LCDWriteRAMPrepare();
	// 				LCDWriteData(usNewRGB); 
	// 				LCDWriteRAMPrepare();				
				}
				else 	/* 半透明 */
				{
					/* 计算公式： 实际显示颜色 = 前景颜色 * Alpha / 255 + 背景颜色 * (255-Alpha) / 255 */
					usOldRGB = LCDReadPointRGB(BitmapPointer->x0+x,BitmapPointer->y0+y);
	// 				LCDSetCursor(220-1-(BitmapPointer->x0+x),BitmapPointer->y0+y);
	// 				LCDWriteRAMPrepare();
	// 				usOldRGB=LCDReadData();
	// 				usOldRGB=LCDBGRtoRGB(usOldRGB);
					//usOldRGB = 0xFFFF;
					R0 = RGB565_R(usOldRGB);
					G0 = RGB565_G(usOldRGB);
					B0 = RGB565_B(usOldRGB);

					R1 = (R1 * A) / 255 + R0 * (255 - A) / 255;
					G1 = (G1 * A) / 255 + G0 * (255 - A) / 255;
					B1 = (B1 * A) / 255 + B0 * (255 - A) / 255;
					usNewRGB = RGB(R1, G1, B1);

					LCDDrawPoint(BitmapPointer->x0+x,BitmapPointer->y0+y, usNewRGB);
	// 				LCDSetCursor(220-1-(BitmapPointer->x0+x),BitmapPointer->y0+y);
	// 				LCDWriteRAMPrepare();
	// 				LCDWriteData(usNewRGB); 
	// 				LCDWriteRAMPrepare();
				}
 				add+=4;
			}
		}

}
void LcdFillArea(u16 x0,u16 y0,u16 x1,u16 y1,u16 color)
{          
	u32 index;
	u32	totalPoint=0;
	totalPoint=(x1-x0+1)*(y1-y0+1);	
		
	LCDSetCursor(x0,y0);
	LcdSetWindow(x0,y0,x1-x0+1,y1-y0+1);
	LCDWriteRAMPrepare();     			//????GRAM	 
	for(index=0;index<totalPoint;index++)
	{
		LCDAdd->LCDRAMData=color;	//???? 	
	}	 
	LcdSetWindow(0,0,320,240);
}
void LCDFillAlpha(u16 x,u16 y,u16 width,u16 height,u16 color)
{
  u16 xSize,ySize;
  u16 tmpColor;
  u16 alphaColor;
  alphaColor = color;
  for(ySize=y;ySize<height+y;ySize++)
  {
    for(xSize=x;xSize<width+x;xSize++)
    {
      tmpColor = LCDReadPointRGB(xSize,ySize);
      if(tmpColor!=WHITE)
      {
        alphaColor = piclib_alpha_blend(tmpColor,alphaColor,10);
        LCDDrawPoint(xSize,ySize,alphaColor);
      }
      else
      {
        LCDDrawPoint(xSize,ySize,color);
      }
    }
  }
}
/*----------------------------------------------------------------------------------------------
--描述      :     TFT屏初始化
--参数      :     无
--返回值    :     无
----------------------------------------------------------------------------------------------*/
void LCDInit(void)
{

  LCDGPIOInit();

  LCDFSMCInit();
	DelayMs(50);
	LCDReset();
	LCDWriteRegister(0x0000,0x0001);
	DelayMs(50);
	
	LcdDevice.ID=LCDReadRegister(0x0000);	
	LCDWriteReg(0XD3);				   
	LCDReadData(); 				//dummy read 	
	LCDReadData();  	    	//读到0X00
  LcdDevice.ID=LCDReadData();  //读取93
	LcdDevice.ID<<=8;
	LcdDevice.ID|=LCDReadData();
	if(LcdDevice.ID==LcdID)
	{
		LCDWriteReg(0xCF);  
		LCDWriteData(0x00); 
		LCDWriteData(0xC1); 
		LCDWriteData(0X30); 
		LCDWriteReg(0xED);  
		LCDWriteData(0x64); 
		LCDWriteData(0x03); 
		LCDWriteData(0X12); 
		LCDWriteData(0X81); 
		LCDWriteReg(0xE8);  
		LCDWriteData(0x85); 
		LCDWriteData(0x10); 
		LCDWriteData(0x7A); 
		LCDWriteReg(0xCB);  
		LCDWriteData(0x39); 
		LCDWriteData(0x2C); 
		LCDWriteData(0x00); 
		LCDWriteData(0x34); 
		LCDWriteData(0x02); 
		LCDWriteReg(0xF7);  
		LCDWriteData(0x20); 
		LCDWriteReg(0xEA);  
		LCDWriteData(0x00); 
		LCDWriteData(0x00); 
		LCDWriteReg(0xC0);    //Power control 
		LCDWriteData(0x1B);   //VRH[5:0] 
		LCDWriteReg(0xC1);    //Power control 
		LCDWriteData(0x01);   //SAP[2:0];BT[3:0] 
		LCDWriteReg(0xC5);    //VCM control 
		LCDWriteData(0x30); 	 //3F
		LCDWriteData(0x30); 	 //3C
		LCDWriteReg(0xC7);    //VCM control2 
		LCDWriteData(0XB7); 
		LCDWriteReg(0x36);    // Memory Access Control 
		LCDWriteData(0x48); 
		LCDWriteReg(0x3A);   
		LCDWriteData(0x55); 
		LCDWriteReg(0xB1);   
		LCDWriteData(0x00);   
		LCDWriteData(0x1A); 
		LCDWriteReg(0xB6);    // Display Function Control 
		LCDWriteData(0x0A); 
		LCDWriteData(0xA2); 
		LCDWriteReg(0xF2);    // 3Gamma Function Disable 
		LCDWriteData(0x00); 
		LCDWriteReg(0x26);    //Gamma curve selected 
		LCDWriteData(0x01); 
		LCDWriteReg(0xE0);    //Set Gamma 
		LCDWriteData(0x0F); 
		LCDWriteData(0x2A); 
		LCDWriteData(0x28); 
		LCDWriteData(0x08); 
		LCDWriteData(0x0E); 
		LCDWriteData(0x08); 
		LCDWriteData(0x54); 
		LCDWriteData(0XA9); 
		LCDWriteData(0x43); 
		LCDWriteData(0x0A); 
		LCDWriteData(0x0F); 
		LCDWriteData(0x00); 
		LCDWriteData(0x00); 
		LCDWriteData(0x00); 
		LCDWriteData(0x00); 		 
		LCDWriteReg(0XE1);    //Set Gamma 
		LCDWriteData(0x00); 
		LCDWriteData(0x15); 
		LCDWriteData(0x17); 
		LCDWriteData(0x07); 
		LCDWriteData(0x11); 
		LCDWriteData(0x06); 
		LCDWriteData(0x2B); 
		LCDWriteData(0x56); 
		LCDWriteData(0x3C); 
		LCDWriteData(0x05); 
		LCDWriteData(0x10); 
		LCDWriteData(0x0F); 
		LCDWriteData(0x3F); 
		LCDWriteData(0x3F); 
		LCDWriteData(0x0F); 
		LCDWriteReg(0x2B); 
		LCDWriteData(0x00);
		LCDWriteData(0x00);
		LCDWriteData(0x01);
		LCDWriteData(0x3f);
		LCDWriteReg(0x2A); 
		LCDWriteData(0x00);
		LCDWriteData(0x00);
		LCDWriteData(0x00);
		LCDWriteData(0xef);	 
		LCDWriteReg(0x11); //Exit Sleep
		DelayMs(120);
		LCDWriteReg(0x29); //display on	
	}
	LcdDisplayDirection(Horizontal);	
	//DelayMs(50);
	System.Device.Lcd.LcdClear=LCDClear;
	System.Device.Lcd.LcdClear(0xFFFF);
	System.Device.Lcd.LCDColorFill=LcdFillArea;
	System.Device.Lcd.LCDDrawingBitmap=LCDDrawingBitmap;
	System.Device.Lcd.RGB888toGRB565=RGB888toGRB565;
	System.Device.Lcd.LcdReadPoint=LCDReadPointRGB;
	System.Device.Lcd.LcdDrawPoint=LCDDrawPoint;
	System.Device.Lcd.ImageDraw=ImageDraw;
	System.Device.Lcd.ImageLocalDraw=ImageLocalDraw;
	//LCDDrawPoint(10,10,Blue);
	//LcdClear(GRED);
	//LcdFillArea(250,20,300,80,Yellow);
	//LcdFillArea(200,20,230,80,Green);
	//LCDDrawLine(50,100,320,100,Blue);

}

