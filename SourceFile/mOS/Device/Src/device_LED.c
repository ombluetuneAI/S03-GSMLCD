/*--------------------------------------------头文件-------------------------------------*/
#include "device_LED.h"
#include "system.h"
#include "device_Key.h"
/*---------------------------------------------常数--------------------------------------*/

/*--------------------------------------------宏定义-------------------------------------*/


/*---------------------------------------------变量--------------------------------------*/


/*---------------------------------------------类型--------------------------------------*/



/*---------------------------------------------函数--------------------------------------*/

static void LEDConfig(u8 state)
{
	if(state)
	{
		LEDKeyOn();
		LcdLedOn();
	}
	else
	{
		LEDKeyOff();
		LcdLedOff();
	}
}

/*******************************************************************************
* 描述	    : 关闭所有LED灯
*******************************************************************************/
static void KeyLEDClose(void)
{
	LEDKeyOff();
	MicPowerCtrl=1;
    LcdLedOff();
}
void LcdLedClose(void)
{
    LcdLedOff();
}
/*----------------------------------------------------------------------------------------------
--描述      :     LED端口初始化
--参数      :     无
--返回值    :     无
----------------------------------------------------------------------------------------------*/
void LEDInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//上拉输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输出	
	
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    LEDKeyOn();
	System.Device.KeyLed.KeyLedConfig= LEDConfig;
	System.Device.KeyLed.KeyCloseLed=KeyLEDClose;
	
}

