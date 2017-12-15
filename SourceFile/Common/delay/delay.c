#include "delay.h"
#include "sys.h"

//-------------------------------------------------------------------------------------//	 


#define MainClock                   72000000
#define DelayUsTick MainClock / 9000000

#define DelayMsTick MainClock / 9000
/*----------------------------------------------------------------------------------------

--描述      ：初始化延迟函数，当使用ucos的时候,此函数会初始化ucos的时钟节拍，SYSTICK的
              时钟固定为HCLK时钟的1/8
--参数      ：无
--返回值	: 无

----------------------------------------------------------------------------------------*/
void DelayInit()	 
{

	SysTick_Config(SystemCoreClock / 10000);	//选择外部时钟  HCLK/8

}								    



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




























