/*
********************************************************************************

********************************************************************************
*文件名   : system.c
*作用     : 系统应用文件
********************************************************************************
*/

#include "system.h"

uint Empty;
void Dummy(void) {};

ushort CriticalNesting = 0;

uint RomBase;
uint RamBase;
DataStruct * AppDataPointer;

static const byte HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define DelayUsTick MainClock / 9000000

#define DelayMsTick MainClock / 9000

void Delay(int times)
{
	while(times--);
}

void DelayUs(int times)
{
	while(times--)
		Delay(DelayUsTick);
}

void DelayMs(int times)
{
	while(times--)
		Delay(DelayMsTick);
}


byte HexToAscii(byte hex)
{
	Assert(hex < 16);

    if (hex < 16) 
        return HextoAscii[hex];
    else 
         return 0x30;
}




/*******************************************************************************
* 函数名	: PostMessage
* 描述	    : 向LogicTask发送消息
* 输入参数  : messageType: 消息类型
*             data: 消息值
* 返回参数  : 无
*******************************************************************************/
void PostMessage(MessageEnum message, uint data)
{
		RTT_printf(0,"PostMessage : type:%d\r\n",message);
    Byte3(data) = message;
    System.OS.PostMessageQueue(data);	
}


extern void SystemAllInit(void);


static void Init(void)
{
  DisableIrq();
    
  RomBase = (uint)Init & 0xFF000000;
  RamBase = (uint)(&RamBase) & 0xFF000000;
    
	InitOs();
	InitGui();
	SystemAllInit();
}

SystemStruct System = 
{
	Init,
};


