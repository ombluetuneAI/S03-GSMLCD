#include "delay.h"
#include "sys.h"

//-------------------------------------------------------------------------------------//	 


#define MainClock                   72000000
#define DelayUsTick MainClock / 9000000

#define DelayMsTick MainClock / 9000
/*----------------------------------------------------------------------------------------

--����      ����ʼ���ӳٺ�������ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ��ģ�SYSTICK��
              ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
--����      ����
--����ֵ	: ��

----------------------------------------------------------------------------------------*/
void DelayInit()	 
{

	SysTick_Config(SystemCoreClock / 10000);	//ѡ���ⲿʱ��  HCLK/8

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




























