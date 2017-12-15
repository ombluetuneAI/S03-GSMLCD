#ifndef __DEVICE_CALL_
#define	__DEVICE_CALL_

#include "stm32F10x.h"
#include "sys.h"
#include "system.h"

#define HT9200Cs		PCout(1)
#define HT9200Data		PCout(2)
#define HT9200CLK		PCout(3)

#define CallConnect		PBout(0)
#define LineCall		PCin(5)

#define Ht9170DV		PCin(4)//(GPIOB,GPIO_Pin_1)
#define HT9170D0		PAin(4)//	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define HT9170D1		PAin(5)//	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)
#define HT9170D2		PAin(6)//	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define HT9170D3		PAin(7)

#define	HT9170DWDN		PAout(4)
			
#define CallRinging			PAin(0)	



typedef enum
{
	DTMFInit      		=0, //������ʼֵ
	DTMFShake     		=0x100, //��������
	DTMFPress     		=0x200, //����״̬
}DTMFEnum;

typedef enum
{
	CallBreakMessage		= 0x100,//����
	CallRingMessage			= 0x200,//����
	CallConnectMessage		= 0x300,//ȥ��
	CallDTMFMessage			= 0x400,//DTMF
}CallEnum;

#endif
