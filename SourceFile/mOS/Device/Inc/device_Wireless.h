#ifndef __DEVICE_WRIELESS_H
#define __DEVICE_WRIELESS_H


#include "stm32f10x.h"
#include "sys.h"
#include "system.h"





#define WirelessPort            PCin(8)// RF接收数据输入
#define WirelessDataSendPort    PCout(6)//数据发送端口
#define LED1  					PBout(14)
//////////////////////////////////////////////

		        	  
// 	uint16_t signalLow=0;//同步码低电平时间记录
// 	uint16_t signalHigh=0;//同步码高电平时间记录
// 	
// 	uint8_t rxCount=0;//接收次数
// 	uint8_t byte=0;//字节个数
// 	uint8_t bit=0;//位
// 	
// 	uint16_t highTime=0;//高电平时间记录
// 	uint16_t lowTime=0;//低电平时间记录
// 	
// 	uint8_t res;//高低脉冲记录
// 	uint8_t pt2262Res[2][3];
// 	
// 	uint32_t receiveData=0;
// /*-------------------------检测同步码------------------------*/	
// 	
// 	//检查高脉冲时间
// 	while(WirelessPort)
// 	{
// 		delay10us();//10us延时
// 		signalHigh++;//累计延时
// 	}
// 	//检查收到的高脉冲是不是在范围内
// 	if((signalHigh>NarrowPulseMax)||(signalHigh<NarrowPulseMin))//
// 	{
// 		return 0;
// 	}
// 	//低脉冲时间
// 	while(!WirelessPort)
// 	{
// 		delay10us();//10us延时
// 		signalLow++;//计时累计(signalLow*10us=脉宽时间)
// 	}
// 	//检查收到的低脉冲是不是在同步码范围内 则退出
// 	if((signalLow>SyncPulseMax)||(signalLow<SyncPulseMin))//
// 	{
// 		return 0;
// 	}
// 	//检查是不是同步码  窄脉冲的32倍  因有误差放宽到26～38之间
// 	
// 	if(((signalHigh*SyncMultiplesMin)<signalLow)&&((signalHigh*SyncMultiplesMax)>signalLow))
// 	{
// 		/*----------------------------开始解码-------------------------*/
// 		for(rxCount=0;rxCount<2;rxCount++)//接收两次作对比，如果结果是一样说明解码成功
// 		{
// 			for(byte=0;byte<3;byte++)//一共24个有效电平(3*8=24)
// 			{
// 				for(bit=0;bit<8;bit++)
// 				{
// 					highTime=0;//时间清零
// 					lowTime=0;
// 					
// 					//高脉冲
// 					while(WirelessPort)
// 					{
// 						delay10us();
// 						highTime++;//累计时间
// 					}
// 					//非法脉冲退出
// 					if((highTime>PulseMax)||(highTime<NarrowPulseMin))
// 					{
// 						return 0;
// 					}
// 					//宽脉冲   窄脉冲为宽脉冲的4倍 实际测试为3倍
// 					if(((signalHigh*2)<highTime)&&(highTime<signalHigh*4))
// 					{
// 						res=1;
// 					}
// 					//窄脉冲
// 					else
// 					{
// 						res=0;
// 						
// 					}
// 					//合并得到的二进制
// 					pt2262Res[rxCount][byte]<<=1;
// 					pt2262Res[rxCount][byte]|=res;
// 					
// 					//跳过低脉冲
// 					while(!WirelessPort)
// 					{
// 						delay10us();
// 						lowTime++;
// 					}
// 					//非法脉冲
// 					if((lowTime>PulseMax)||(lowTime<NarrowPulseMin))
// 					{
// 						return 0;
// 					}					
// 				}
// 			}
// 			highTime=0;
// 			signalLow=0;
// 			
// 			
// /*---------------------跳过第二次的同步码-------------------------*/
// 			
// 			
// 			//跳过同步码高脉冲
// 			while(WirelessPort)
// 			{
// 				delay10us();
// 				highTime++;
// 			}
// 			//跳过同步码低脉冲
// 			while(!WirelessPort)
// 			{
// 				delay10us();
// 				signalLow++;
// 			}
// 			//同步码低脉宽 (窄脉宽的32倍)
// 			if((signalLow<signalHigh*SyncMultiplesMin)||(signalLow>signalHigh*SyncMultiplesMax))//是不是乱码 
// 			{
// 				return 0;//如果乱码则退出
// 			}
// 		}
// /*------------------------------------------比对结果，如果相等则解码成功----------------------------------------------*/
// 		if((pt2262Res[0][0]==pt2262Res[1][0])&&(pt2262Res[0][1]==pt2262Res[1][1])&&(pt2262Res[0][2]==pt2262Res[1][2]))
// 		{
// 			receiveData=pt2262Res[0][0];
// 			receiveData<<=8;
// 			receiveData|=pt2262Res[0][1];
// 			receiveData<<=8;
// 			receiveData|=pt2262Res[0][2];
// 			System.Device.KeyLed.KeyLedConfig(1);	//打开LED
// 			System.Device.Timer.Start(KeyLedID,TimerSystickHandle,100,System.Device.KeyLed.KeyCloseLed);
// 			System.Device.Voice.VoiceConfig(VoicePlayNumber,(uint8_t*)"016",1);	
// 		}

// 	}

// 	signalLow=0;
// 	signalHigh=0;
// 	return receiveData;




#endif
