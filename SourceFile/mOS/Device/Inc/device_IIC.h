#ifndef __DEVICE_IIC_H
#define __DEVICE_IIC_H
#include "sys.h"
	  

 
//IO方向设置
#define SDAIN1()  {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8;}
#define SDAOUT1() {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=3;}

#define SDAIN2()  {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=8<<8;}
#define SDAOUT2() {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=3<<8;}

//IO操作函数	 
#define IICSCL1    PAout(9) //SCL
#define IICSDA1    PAout(8) //SDA	 
#define READSDA1   PAin(8)  //输入SDA 

#define IICSCL2    PCout(11)
#define IICSDA2    PCout(10)
#define READSDA2   PCin(10)

//IIC所有操作函数
void IICInit(void);                //初始化IIC的IO口				 
void IICStart(u8 uumber);				//发送IIC开始信号
void IICStop(u8 uumber);	  			//发送IIC停止信号
void IICSendByte(u8 txd,u8 uumber);			//IIC发送一个字节
u8 IICReadByte(u8 ack,u8 uumber);//IIC读取一个字节
u8 IICWaitAck(u8 uumber); 				//IIC等待ACK信号
void IICAck(u8 uumber);					//IIC发送ACK信号
void IICNAck(u8 uumber);				//IIC不发送ACK信号
	  
#endif


