#ifndef __DEVICE_IIC_H
#define __DEVICE_IIC_H
#include "sys.h"
	  

 
//IO��������
#define SDAIN1()  {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8;}
#define SDAOUT1() {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=3;}

#define SDAIN2()  {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=8<<8;}
#define SDAOUT2() {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=3<<8;}

//IO��������	 
#define IICSCL1    PAout(9) //SCL
#define IICSDA1    PAout(8) //SDA	 
#define READSDA1   PAin(8)  //����SDA 

#define IICSCL2    PCout(11)
#define IICSDA2    PCout(10)
#define READSDA2   PCin(10)

//IIC���в�������
void IICInit(void);                //��ʼ��IIC��IO��				 
void IICStart(u8 uumber);				//����IIC��ʼ�ź�
void IICStop(u8 uumber);	  			//����IICֹͣ�ź�
void IICSendByte(u8 txd,u8 uumber);			//IIC����һ���ֽ�
u8 IICReadByte(u8 ack,u8 uumber);//IIC��ȡһ���ֽ�
u8 IICWaitAck(u8 uumber); 				//IIC�ȴ�ACK�ź�
void IICAck(u8 uumber);					//IIC����ACK�ź�
void IICNAck(u8 uumber);				//IIC������ACK�ź�
	  
#endif


