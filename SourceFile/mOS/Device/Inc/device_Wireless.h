#ifndef __DEVICE_WRIELESS_H
#define __DEVICE_WRIELESS_H


#include "stm32f10x.h"
#include "sys.h"
#include "system.h"





#define WirelessPort            PCin(8)// RF������������
#define WirelessDataSendPort    PCout(6)//���ݷ��Ͷ˿�
#define LED1  					PBout(14)
//////////////////////////////////////////////

		        	  
// 	uint16_t signalLow=0;//ͬ����͵�ƽʱ���¼
// 	uint16_t signalHigh=0;//ͬ����ߵ�ƽʱ���¼
// 	
// 	uint8_t rxCount=0;//���մ���
// 	uint8_t byte=0;//�ֽڸ���
// 	uint8_t bit=0;//λ
// 	
// 	uint16_t highTime=0;//�ߵ�ƽʱ���¼
// 	uint16_t lowTime=0;//�͵�ƽʱ���¼
// 	
// 	uint8_t res;//�ߵ������¼
// 	uint8_t pt2262Res[2][3];
// 	
// 	uint32_t receiveData=0;
// /*-------------------------���ͬ����------------------------*/	
// 	
// 	//��������ʱ��
// 	while(WirelessPort)
// 	{
// 		delay10us();//10us��ʱ
// 		signalHigh++;//�ۼ���ʱ
// 	}
// 	//����յ��ĸ������ǲ����ڷ�Χ��
// 	if((signalHigh>NarrowPulseMax)||(signalHigh<NarrowPulseMin))//
// 	{
// 		return 0;
// 	}
// 	//������ʱ��
// 	while(!WirelessPort)
// 	{
// 		delay10us();//10us��ʱ
// 		signalLow++;//��ʱ�ۼ�(signalLow*10us=����ʱ��)
// 	}
// 	//����յ��ĵ������ǲ�����ͬ���뷶Χ�� ���˳�
// 	if((signalLow>SyncPulseMax)||(signalLow<SyncPulseMin))//
// 	{
// 		return 0;
// 	}
// 	//����ǲ���ͬ����  խ�����32��  �������ſ�26��38֮��
// 	
// 	if(((signalHigh*SyncMultiplesMin)<signalLow)&&((signalHigh*SyncMultiplesMax)>signalLow))
// 	{
// 		/*----------------------------��ʼ����-------------------------*/
// 		for(rxCount=0;rxCount<2;rxCount++)//�����������Աȣ���������һ��˵������ɹ�
// 		{
// 			for(byte=0;byte<3;byte++)//һ��24����Ч��ƽ(3*8=24)
// 			{
// 				for(bit=0;bit<8;bit++)
// 				{
// 					highTime=0;//ʱ������
// 					lowTime=0;
// 					
// 					//������
// 					while(WirelessPort)
// 					{
// 						delay10us();
// 						highTime++;//�ۼ�ʱ��
// 					}
// 					//�Ƿ������˳�
// 					if((highTime>PulseMax)||(highTime<NarrowPulseMin))
// 					{
// 						return 0;
// 					}
// 					//������   խ����Ϊ�������4�� ʵ�ʲ���Ϊ3��
// 					if(((signalHigh*2)<highTime)&&(highTime<signalHigh*4))
// 					{
// 						res=1;
// 					}
// 					//խ����
// 					else
// 					{
// 						res=0;
// 						
// 					}
// 					//�ϲ��õ��Ķ�����
// 					pt2262Res[rxCount][byte]<<=1;
// 					pt2262Res[rxCount][byte]|=res;
// 					
// 					//����������
// 					while(!WirelessPort)
// 					{
// 						delay10us();
// 						lowTime++;
// 					}
// 					//�Ƿ�����
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
// /*---------------------�����ڶ��ε�ͬ����-------------------------*/
// 			
// 			
// 			//����ͬ���������
// 			while(WirelessPort)
// 			{
// 				delay10us();
// 				highTime++;
// 			}
// 			//����ͬ���������
// 			while(!WirelessPort)
// 			{
// 				delay10us();
// 				signalLow++;
// 			}
// 			//ͬ��������� (խ�����32��)
// 			if((signalLow<signalHigh*SyncMultiplesMin)||(signalLow>signalHigh*SyncMultiplesMax))//�ǲ������� 
// 			{
// 				return 0;//����������˳�
// 			}
// 		}
// /*------------------------------------------�ȶԽ���������������ɹ�----------------------------------------------*/
// 		if((pt2262Res[0][0]==pt2262Res[1][0])&&(pt2262Res[0][1]==pt2262Res[1][1])&&(pt2262Res[0][2]==pt2262Res[1][2]))
// 		{
// 			receiveData=pt2262Res[0][0];
// 			receiveData<<=8;
// 			receiveData|=pt2262Res[0][1];
// 			receiveData<<=8;
// 			receiveData|=pt2262Res[0][2];
// 			System.Device.KeyLed.KeyLedConfig(1);	//��LED
// 			System.Device.Timer.Start(KeyLedID,TimerSystickHandle,100,System.Device.KeyLed.KeyCloseLed);
// 			System.Device.Voice.VoiceConfig(VoicePlayNumber,(uint8_t*)"016",1);	
// 		}

// 	}

// 	signalLow=0;
// 	signalHigh=0;
// 	return receiveData;




#endif
