/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "device_LED.h"
#include "system.h"
#include "device_Key.h"
/*---------------------------------------------����--------------------------------------*/

/*--------------------------------------------�궨��-------------------------------------*/


/*---------------------------------------------����--------------------------------------*/


/*---------------------------------------------����--------------------------------------*/



/*---------------------------------------------����--------------------------------------*/

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
* ����	    : �ر�����LED��
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
--����      :     LED�˿ڳ�ʼ��
--����      :     ��
--����ֵ    :     ��
----------------------------------------------------------------------------------------------*/
void LEDInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//�������	
	
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    LEDKeyOn();
	System.Device.KeyLed.KeyLedConfig= LEDConfig;
	System.Device.KeyLed.KeyCloseLed=KeyLEDClose;
	
}

