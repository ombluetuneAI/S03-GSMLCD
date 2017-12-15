/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "Device_Voice.h"
#include "system.h"
#include "Device_IIC.h"

extern SystemStruct System;
/*---------------------------------------------����--------------------------------------*/
//����ͨ��ѡͨ����
#define BothChannelValue			0x0000	//��ͨ��
#define OneChannelValue				0x4000	//��һͨ��
#define TwoChannelValue				0xc000	//�ڶ�ͨ��

//D9/D10����λ
#define Control_D9D10				0x0060
/*--------------------------------------------�궨��-------------------------------------*/


/*---------------------------------------------����--------------------------------------*/
//����������ֵ��(�ݼ�˳��,��datasheet������)
u16 Volume_L[] = {0x0003,0x0001,0x0002,0x0000};
	/*D7~D8��ֵ:  0dB,   -1dB,  -2dB,  -3dB	*/

u16 Volume_H[] = 
{	
    /*D2~D6����*/
    0x0015,0x0005,0x0019,0x0009,	
    /*	  0db,  -4db,  -8db,  -12db	*/
    0x0011,0x0001,0x001e,0x000e,	
    /*   -16db, -20db, -24db, -28db	*/
    0x0016,0x0006,0x001a,0x000a,	
    /*   -32db, -36db, -40db, -44db	*/
    0x0012,0x0002,0x001c,0x000c,	
    /*   -48db, -52db, -56db, -60db	*/
    0x0014,0x0004,0x0018,0x0008,	
    /*   -64db, -68db, -72db, -76db	*/
    0x0010,0x0000
    /*   -80db, -84db	*/
};	

/*---------------------------------------------����--------------------------------------*/



/*---------------------------------------------����--------------------------------------*/
static void SirenDi(u8 count);
//static u8 IsLoudSpenkerClosed = 0;
void CloseLoudSpeaker(void)
{
  CloseSpeaker();
}
//mode:1~4(12/16/24/27.5dB)
void AW8377SetMode(u8 mode)
{
  while(mode--)
  {
    VoiceEN = 0;
    DelayUs(5);
    VoiceEN = 1;
    DelayUs(5);
  }
}

//void AW8733Init(void)
//{
//    VoiceEN=0;
//    DelayUs(5);
//    VoiceEN=1;
//    DelayUs(5);

//    VoiceEN=0;
//    DelayUs(5);
//    VoiceEN=1;
//    DelayUs(5);
//    
//    VoiceEN=0;
//}
void SendThreeLines(u8  addr) 
{
	u8 i;
//  if(!IsLoudSpenkerOpened)
//  {
//    AW8377SetMode(4);
//  }
//  else
//  {
//    OpenSpeaker();
//  }
	WTcs=0;
	DelayMs(5);
	for(i=0;i<8;i++)
	{
		WTclk=0;
		if(addr & 1)WTdata=1;
		else WTdata=0;
		addr>>=1;
		DelayUs(150); /* 150us */
		WTclk=1;
		DelayUs(150);
	}
	WTcs=1;
//  System.Device.Timer.Start(LoudSpeaker,TimerSystickHandle,5000,CloseLoudSpeaker);
}

void WT5880DSetVol(u8 vol)
{
  SendThreeLines(WT5880DVolume+vol);
}
/*----------------------------------------------------------------------------------------------
--����      :     �˿ڳ�ʼ�������ע��
--����      :     ��
--����ֵ    :     ��
----------------------------------------------------------------------------------------------*/
void VoicePortInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE,&GPIO_InitStructure); 
//	RecordData=1;
    
	//����ICʹ�ܶ�
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure); 
  CloseSpeaker();
  DelayUs(500);

	//���Ŷ˿�
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
	//WT588D
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	WTclk=1;
	WTcs=1;
	WTdata=1;
  
//  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//  RecordData = 1;
//  PlayData = 1;
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	

	CD4053Sa=0;
	CD4053Sb=0;
	CD4053Sc=0;
  
  AW8377SetMode(3);
//	LineDataToWTR050(0xE7);
  System.Device.Voice.Siren=SirenDi;	
}
void LineDataToWTR050( u8 data) 
{ 
	u8 temp; 
	u8 resData;
	u8 i;
	temp = data; 
	
	PlayData = 0; 
	DelayMs(5); //5ms 
	resData = temp&0X01; 
	
	for(i=0;i<8;i++) 
	{ 
		if(resData == 1) 
		{ 
			PlayData = 1; 
			DelayUs(1200); 
			PlayData = 0; 
			DelayUs(400);  
		} 
		else 
		{ 
			PlayData = 1; 
			DelayUs(400); // 400us
			PlayData = 0; 
			DelayUs(1200); //1.2ms
		} 
		temp = temp>>1; 
		resData = temp&0X01; 
	} 
	PlayData = 1; 
}
/*******************************************************************************
*
*����      : ����������������m62429
*�������  : data:����������(ʮ����)0 <= Vol_dat <= 84;���ֵ:0dB��Сֵ:-84dB
*			 sum,0ѡ������ͨ����1ѡ��ͨ��1���ߣ�2ѡ��ͨ��2����
*�������  : ��
*
*******************************************************************************/

void M62429WriteByte(u8 data,u8 sum)			
{
//	static u8 sendBit;
//	static u16 channelValue;
//	static u16 volumeTemp1,volumeTemp2;
//	static u16 M62429VolumeData;

//	switch(sum)	
//	{
//		case 0:
//			channelValue = BothChannelValue;
//			break;

//		case 1: 
//			channelValue = OneChannelValue;
//			break;

//		case 2: 
//			channelValue = TwoChannelValue;
//			break;

//		default:
//			channelValue = BothChannelValue;
//			break;
//	}

//	volumeTemp1 = Volume_H[data / 4];	//D2~D6
//	volumeTemp2 = Volume_L[data % 4];	//D7~D8

//	/*�����������ֽ�  |  D2 ~ D6����λ | D7~D8����λ | ͨ�����ÿ���  | D9��D10����λ*/
//	M62429VolumeData = (volumeTemp1 << 9) + (volumeTemp2 << 7) + channelValue + Control_D9D10;

//	for(sendBit = 0;sendBit < 11;sendBit++)
//	{
//		M62429Data=0;	//�������ݴ�������
//		DelayUs(200);
//		M62429Clk=0;
//		DelayUs(200);
//		if((M62429VolumeData & 0x8000) == 0x8000)	
//            M62429Data=1;
//		else	
//            M62429Data=0;
//		DelayUs(100);
//		M62429Clk=1;	//send the bit
//		DelayUs(200);
//		M62429VolumeData <<= 1;
//	}
//	/*	���ݷ�����ɣ��������ݽӿ�	*/
//	M62429Data=1;
//	DelayUs(200);
//	M62429Clk=0;	//�½�������
//	DelayUs(200);
//	M62429Clk=1;
}
/*******************************************************************************************

*******************************************************************************************/
static void SirenDi(u8 count)
{
    u8 i;
    for(i=count;i>0;i--)
    {
        OpenSiren();
        DelayMs(60);
        CloseSiren();
        DelayMs(60);
    }
//    CloseSpeaker();
}


void VoiceSendByte(void)
{
    //OpenSpeaker();
    IICStart(2);
    IICSendByte(0xA0,2);
    IICWaitAck(2);
    
    DelayUs(100);
    IICSendByte(0x08,2);
    IICWaitAck(2);
    DelayUs(100);
    
    IICStop(2);
    DelayUs(100);
}
void VoiceVol(void)
{
    IICStart(2);
    IICSendByte(0xA0,2);
    IICWaitAck(2);

    DelayUs(100); 
    IICSendByte(0x0E,2);
    IICWaitAck(2);
    DelayUs(100);

    IICStop(2);
    DelayUs(100);   
}















