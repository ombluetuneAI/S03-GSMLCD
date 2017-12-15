#include "device_GSM.h"
#include "system.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "App.h"
///////////////////////////////////////////////////////////////////
//ά����־(V0.1/2017.5.5)
//1.�Ż� GsmGPIOInit ��������ʼ��ʱ������IO����Ϊ�ߵ�ƽ
//2.�Ż� GsmPowe ���������������Ż�������ʱ�����3s
//3.�Ż� Simcom800CInit ����
//

#define GsmPoweOn()			GsmPowe()
#define GsmPowerOff()		GsmPowe()


#define GsmTxdBufferSize	100
#define GsmRxdBufferSize  100
		


__align(8) u8 GsmTxdBuffer[GsmTxdBufferSize]; 
__align(8) u8 GsmRxdBuffer[GsmRxdBufferSize];

#define StateTotal		15	//Ҫɨ���״̬
//#define GsmTimeID		2

const char GsmStateTable[StateTotal][20]=
{
	"+CLIP:",//������               	0
	"+DTMF: ",//DTMF����             	1
	"+CSQ:",//�ź�����                2
  "+CPIN: READY",//��⵽SIM��      3
	"+CMT:",//����Ϣ                  4
  "MO CONNECTED",//��ͨ�绰         5
  "NO CARRIER",//����ͨ��   		    6
  "+CMGS:",//���ŷ��ͳɹ�        	  7
  "+COPS: 0,0,",//��Ӫ������        8
	"BUSY"	,	//					            9
	"NO ANSWER",//��Ӧ��              10
  "+CPIN: NOT READY",           //  11
  "+CPIN: NOT INSERTED",        //  12
  "Call Ready",                 //  13
  "SMS Ready",                  //  14
  
};
//�޸���־��
/*
2017-0406����ԭ����SIM5320 ����ΪSIM800C
��Ҫ�޸ĵĺ�����1��IO ��ʼ���� ��GsmGPIOInit() ��2����ʼ��������GsmInit()
*/


//������ʼ
void send(u8 *str,u8 len);
void GsmSimCheck(void);
u8 GsmATSend(u8 *strPointer,u8 *ackPointer,u16 waitTime);
extern void test3(u8 state,u8 zone);
u8 CheckContactID(void)
{
	u8 *strPointer;
	static u8 testCID=0;
	if(App.Data.CidState.CidStart)
	{
		if(!testCID)
		{
			if(App.Data.CidState.CidDialSend)
			{
				App.Data.CidState.CidDialSend=0;
				App.Data.CidState.CidEnd=1;
				App.Data.CidState.CidStart=0;
				App.Data.OriginateCalltimes=0;
				App.Data.OriginateCount=0;
				DelayMs(100);
				return 1;
			}				
			strPointer=(u8*)strstr((const char*)GsmRxdBuffer,"1400");
			if(strPointer!=NULL) 
				testCID++;
		}
		else if(1==testCID)
		{
			strPointer=(u8*)strstr((const char*)GsmRxdBuffer,"2300");
			if(strPointer!=NULL)
			{
				testCID=0;
				test3(App.Data.SystemMode,App.Data.AlarmZone);
				App.Data.CidState.CidDialSend=1;
				return 1;
			}
		}
	}
	App.Data.GsmState=0;
	return 0;
}
/******************************************************************************
*����		:	�ȴ����ڽ��ճ�ʱʱ�䣬�����ʱ����Ϊ�������
				��������Ϣ��Logic�н��д���
*�������	��	��
*�������	��	��
******************************************************************************/
static void GsmRxdTime(void)
{
	u8 *strPointer;
	u8	temp=0;
	u8	len=0;
	u8 i=StateTotal;
    
  strPointer=NULL;
	App.Data.GsmState|=1<<15;	
	while(i--)
	{
		strPointer=(u8*)strstr((const char*)GsmRxdBuffer,GsmStateTable[i]);
		if(strPointer!=NULL)
		{
			len=strlen(GsmStateTable[i]);
			strPointer+=len;
			switch(i)
			{
				case 0://������
					App.Data.CallRing=1;
					PostMessage(MessageGsmType,GsmRingMessage);
          RTT_printf(0,"PostMessage:Ring\r\n");
//					CloseSpeaker();
					return;
				case 1://DTMF����
//					if(!CheckContactID())
//					{
//						temp=*strPointer++;
//						PostMessage(MessageGsmType,MessageCallTDMF|temp);
//            RTT_printf(0,"PostMessage:DTMF\r\n");
//					}
          
						temp=*strPointer;
						PostMessage(MessageCallTDMF,temp);
            RTT_printf(0,"PostMessage:DTMF:%#x\r\n",temp);
					return;
				case 2://�ź���
					App.Data.GsmSMSInfo=strPointer;
          PostMessage(MessageGsmType,GsmCSQMessage);
          RTT_printf(0,"PostMessage:CSQ\r\n");
					return;
//				case 3://׼������ ��鵽SIM��
//					App.Data.GsmSimCardState=1;
//          App.Data.OperatorUpdate = 1;
//          PostMessage(MessageGsmType,GsmReadyMessage);
////					PostMessage(MessageGsmType,GsmNormalMessage);
//					return;
        case 4://�յ�����Ϣ �Ѷ�ȡ
					App.Data.GsmSMSInfo=strPointer;
					PostMessage(MessageGsmType,GsmSMSRead);
        RTT_printf(0,"PostMessage:SMSRead\r\n");
					return;
        case 5://��ͨ�绰
          PostMessage(MessageGsmType,GsmMessageCONN);
          RTT_printf(0,"PostMessage:MO CONNECTED\r\n");
          return;
                
        case 6://����ͨ��
//          CloseSpeaker();
          App.Data.CallRing=0;
          App.Data.DtmfCount=0;
          App.Data.GsmSMSInfo=strPointer;
          PostMessage(MessageGsmType,GsmMessageCEND);
        RTT_printf(0,"PostMessage:HungUp\r\n");
          return;
        case 7://���ŷ��ͳɹ�
          PostMessage(MessageGsmType,GsmSMSSendOK);
        RTT_printf(0,"PostMessage:SMSSendOK\r\n");
          return;
        case 8://��Ӫ������
          App.Data.GsmSMSInfo=strPointer;
//          App.Data.OperatorState=1;
          App.Data.GsmState=0;
          PostMessage(MessageGsmType,GsmMessageOperator);
        RTT_printf(0,"PostMessage:SIMName\r\n");
          return;
				case 9:
//					CloseSpeaker();
					PostMessage(MessageGsmType,GsmBusy);
        RTT_printf(0,"PostMessage:Busy\r\n");
					return;
				case 10:
					PostMessage(MessageGsmType,GsmBusy);
        RTT_printf(0,"PostMessage:Busy\r\n");
					return;
        case 11:
        case 12:
          App.Data.GsmSimCardState = 0;
          App.Data.OperatorUpdate = 1;
          PostMessage(MessageGsmType,GSMSimRemoved);
        RTT_printf(0,"PostMessage:SimRemoved\r\n");
        return;
        case 3:
        case 13:
        case 14://��⵽SIM��
          App.Data.OperatorUpdate = 1;
          App.Data.GsmSimCardState=1;
					PostMessage(MessageGsmType,GsmReadyMessage);
          RTT_printf(0,"PostMessage:SimReady\r\n");
        return;
			}
		}
	}
	PostMessage(MessageGsmType,GsmNotMessage);
}

/******************************************************************************************************
*����		��	��1���ַ�ת��Ϊ16��������
*�������	��	chr���ַ�,0~9/A~F/a~F
*�������	��	chr��Ӧ��16������ֵ
******************************************************************************************************/
u8 Char2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}

/*******************************************************************************************************
*����		��	��1��16��������ת��Ϊ�ַ�
*�������	��	hex,16��������,0~15;
*�������	��	ת������ַ�
*******************************************************************************************************/
u8 Hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
/*******************************************************************************************************
*����		��DMA�ĸ�ͨ�����ã�����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
			  �Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
*�������	��DMAChannelx,DMAͨ��
			  cpar,�����ַ
			  cmar,�洢����ַ
			  cndtr,���ݴ����� 
*�������	����
*******************************************************************************************************/

void GsmDMASendConfig(void)
{
	DMA_InitTypeDef DMA_InitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
  DMA_DeInit(DMA1_Channel7);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)GsmTxdBuffer;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
	DMA_DeInit(DMA1_Channel6);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)GsmRxdBuffer;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴����赽�ڴ�
	DMA_InitStructure.DMA_BufferSize =GsmRxdBufferSize;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
	//DMA_ClearFlag(DMA1_FLAG_TC6);
 	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);//��������ж�
 	DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE);//��������ж�
	DMA_ITConfig(DMA1_Channel6, DMA1_IT_GL6, ENABLE);//
	/* Enable USART2 DMA RX request */
	
	DMA_Cmd(DMA1_Channel6, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =  1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

} 
/*******************************************************************************************************
*����		������һ��DMA����
*�������	��DMAChannelx,DMAͨ��
			  length,���ݳ���
*�������	����
*******************************************************************************************************/

void GsmDMAEnable(DMA_Channel_TypeDef* DMA_CHx,u16 length)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��  
	DMA_ClearFlag(DMA1_FLAG_TC7);   
	DMA_SetCurrDataCounter(DMA1_Channel7,length);//DMAͨ����DMA����Ĵ�С
	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	


/**********************************************************************************************
*����		��	������ת����ͨ��DMA���ͳ�ȥ������2,printf ����ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
*�������	��	
*�������	��	
**********************************************************************************************/
void StrinPrintf(char* fmt,...)  
{  
	va_list ap;
	memset(GsmRxdBuffer,0,128);
  DisableIrq();
	va_start(ap,fmt);
	vsnprintf((char*)GsmTxdBuffer,128,fmt,ap);
	va_end(ap);
  EnableIrq();
    //send((u8*)GsmTxdBuffer,strlen((char*)GsmTxdBuffer));
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���1��DMA���� 
	GsmDMAEnable(DMA1_Channel7,strlen((const char*)GsmTxdBuffer)); 	//ͨ��dma���ͳ�ȥ
	while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
	USART_DMACmd(USART2,USART_DMAReq_Tx,DISABLE); //��ֹ����1��DMA���� 
	DelayMs(100);	
}

/***********************************************************************************
*����		��	GSMģ���GPIO��ʼ��
*�������	��	��
*�������	��	��
************************************************************************************/

static void	GsmGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    //��������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
	GsmPower=0;
}

/***********************************************************************************
*����		��	GSMģ���Դ����(�ߵ�ƽ���ֱ���Խ��1��)
*�������	��	��
*�������	��	��
************************************************************************************/
void GsmPowe(void)
{
	GsmPower=1;
	DelayMs(1200);
	GsmPower=0;
	DelayMs(2200);
}
/***********************************************************************************
*����		:	���������,�����յ���Ӧ��
*�������	��	str:�ڴ���Ӧ����
*�������	:	0,û�еõ��ڴ���Ӧ����
			    ����,�ڴ�Ӧ������λ��(str��λ��)
***********************************************************************************/
u8* GsmStringCheckString(u8 *str)
{
	char *strx=0;
	if(App.Data.GsmState&0X8000)		//���յ�һ��������
	{ 
		GsmRxdBuffer[App.Data.GsmState&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)GsmRxdBuffer,(const char*)str);
	} 
	return (u8*)strx;
}
/***********************************************************************************
*
*����		:   AT ָ���
*�������	�� 	strPointer��Ҫ���͵�ָ���ַ���
                ackPointer�����ص��ַ���
*�������	�� 0�����ͳɹ�
             1���յ���Ԥ�ڽ��
             2����ʱδ�յ��κν��
************************************************************************************/
u8 GsmATSend(u8 *strPointer,u8 *ackPointer,u16 waitTime)
{
  u8 res = 0;
	App.Data.GsmState=0;
//	App.Data.GsmRxdCount=0;
  memset(GsmRxdBuffer,0x00,100);
	if((u32)strPointer<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
		while((USART2->SR&0X40)==0);//һ��Ҫ���ϣ������һ���ֽڻᶪʧ
		USART2->DR=(u32)strPointer;
	}else StrinPrintf("%s\r\n",strPointer);//��������
	if(ackPointer&&waitTime)
	{
		while(--waitTime)
		{
			DelayMs(5);
			if(App.Data.GsmState&0x8000)
			{
				if(GsmStringCheckString(ackPointer))res = 0;
        else res = 1;
        break;
			}
		}
		if(waitTime==0)  res = 2;
	}
	App.Data.GsmState=0;//��ɽ��գ����ȴ��´ν���
	return res;
	
}

/*******************************************************************************
* ����	    : �����жϴ�����
*******************************************************************************/
void USART2_IRQHandler(void) 
{
  byte 	data;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		data=USART2->DR;
    if((App.Data.GsmState&(1<<15))==0)
    {
      if(App.Data.GsmState<GsmRxdBufferSize)
      {
        TIM3->CNT = 0;//��������� 
				if(App.Data.GsmState==0) 				
				{
//					System.Device.Timer.Start(GsmTimer,TimerMessageHandle,70,GsmRxdTime);
//        RTT_printf(0,"USART2_IRQHandler:GsmRxdTime\r\n");
          TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	
				}
				GsmRxdBuffer[App.Data.GsmState++]=data; 
			}else 
			{
				App.Data.GsmState|=1<<15;
			}
    }
  } 
}
//void USART2_IRQHandler(void) 
//{
//    byte 	data;
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
//	{
//		data=USART2->DR;
//		if(App.Data.GsmState<GsmRxdBufferSize)
//		{
//			if(App.Data.GsmState==0)
//      {
//				System.Device.Timer.Start(GsmTimer,TimerMessageHandle,70,GsmRxdTime);
////        RTT_printf(0,"USART2_IRQHandler:GsmRxdTime\r\n");
//        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	
//      }
//			GsmRxdBuffer[App.Data.GsmState++]=data;
//		}
//		else
//		{
//			App.Data.GsmState|=1<<15;
//		}
//		//App.Data.GsmRxdCount=App.Data.GsmState;//��¼��ǰ�յ����ֽ���
//    } 
//} 
void DMA1_Channel6_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC6);
	DMA_ClearITPendingBit(DMA1_IT_TE6);
	DMA_Cmd(DMA1_Channel6, DISABLE);
	DMA1_Channel6->CNDTR = 100;
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

/********************************************************************************************

*����		��	���Ͷ���
*�������	��	number,�绰����
				text,Ҫ���͵�����
*�������	��	0�����ͳɹ�������ֵ��ʾʧ��

********************************************************************************************/
void delay(u32 times)
{
    u16 i;
    while(times--)
    {
        for(i=0;i<1000;i++);
    }
}
/********************************************************************************************

*����		��	������
*�������	��	number,����;text,�ַ���
*�������	��	0��

*********************************************************************************************/

static void GsmSendSMS(u8 *number,u8 *text)
{
	u8 numberB[120];
	memset(numberB,0,120);
	sprintf((char*)numberB,"AT+CMGS=\"%s\"",number);
	if(GsmATSend(numberB,">",500)==0)
	{
		StrinPrintf("%s",text);
    DelayMs(100);
		GsmATSend((u8*)0x1A,"+CMGS:",10);
	}	
}

/********************************************************************************************

*����		��	���ٷ��Ͷ���
*�������	��	nPointer,����;textPointer,����
*�������	��	��

*********************************************************************************************/
/*static void Simcom5320Quickly(u8 *nPointer,u8 *textPointer)
{
    u8 numberBuffer[128];
    
    memset(numberBuffer,0,128);
    sprintf((char*)numberBuffer,"AT+CMGSO=\"%s\",\"%s\"",nPointer,textPointer);
    StrinPrintf("%s\r\n",numberBuffer);
}*/
/********************************************************************************************

*����		��	����绰
*�������	��	phoneNumber,Ҫ����ĵ绰
*�������	��	0��

*********************************************************************************************/
u8 GsmCall(const char *PhoneNumber)
{
	u8 res=0xff;
  App.Data.GsmState=0;
  StrinPrintf("ATD%s;\r\n",PhoneNumber);
  res=0;

	return res;
}

/**************************************************************************************************

*����		��	�ô���ֱ�ӷ�������
*�������	��	str,Ҫ���͵��ַ���
				len,�����ַ����ĳ���
*�������	��	��

**************************************************************************************************/
void send(u8 *str,u8 len)
{
	while(len--)
	{
		USART_GetFlagStatus(USART2, USART_FLAG_TC);//һ��Ҫ���ϣ������һ���ֽڻᶪʧ
		USART_ClearFlag(USART2,USART_FLAG_TC);
		USART_SendData(USART2, *str++);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
	}
}
/**************************************************************************************************
*
*����       ��IMEI����
*�������   ����
*�������   ����
*
**************************************************************************************************/
void IMEIParse(void)
{
  u8 i; 
	for(i=0;i<15;i++)
  {
		App.Data.IMEINum[i]=GsmRxdBuffer[i];   
  }    
}
/******************************************************************
*
*����		��	GSMģ��ĳ�ʼ��
*�������	��	��
*�������	��	��
*
******************************************************************/
void Simcom800CInit(void)
{
    u8 *strPointer;
    GsmATSend("AT+IPR=115200","OK",2);      //���ò�����  
    GsmATSend("AT+CLIP=1","OK",10);        //����������ʾ
    GsmATSend("AT+MORING=1","OK",10);      //��ʾȥ��ĺ���״̬
//    GsmATSend("AT+CPIN?","OK",10);	
    GsmATSend("AT+CGREG?","OK",10);      //GPRS����ע��״̬
    GsmATSend("AT+COPS?","OK",10);         //ѡ����Ӫ�� 
    GsmATSend("AT+CSQ?","OK",10);
//    IMEIParse();
    GsmATSend("AT+CIPMODE=0","OK",10);     //ѡ��TCPIPӦ��ģʽΪ��͸��ģʽ
    GsmATSend("AT+CENG=1","OK",10);        //��������ģʽ
    GsmATSend("AT*CELLLOCK=0","OK",10);    //����������С��ARFCN�б�
                   //�����û���ǰ������
    GsmATSend("AT+CNMI=2,2,0,0,0","OK",10);//����Ϣ��ʾ
    GsmATSend("AT+CMGF=1","OK",200);        //ѡ�����Ϣ��ʽΪ�ı���ʽ
    GsmATSend("AT+CSCS=\"UCS2\"","OK",10); //ѡ��TE�ַ���
//    DelayMs(50);
    GsmATSend("AT+CSMP=17,168,0,8","OK",10);//���ö���Ϣ�ı�ģʽ����
    GsmATSend("AT+DDET=1,0,1,1","OK",10);    //DTMF������
    GsmATSend("AT+CMIC=0,15","OK",10);       //����mic����
    GsmATSend("ATS0=1","OK",10);             //�����Զ������������
    GsmATSend("AT+CALM=1","OK",10);            //�������羲��
    GsmATSend("AT&W","OK",2);
    GsmATSend("AT+CSQ?","OK",10);
    
}

/********************************************************************************
*
*����		��	ͨ�ö�ʱ��3�жϳ�ʼ��,����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
*�������	��	arr���Զ���װֵ;psc��ʱ��Ԥ��Ƶ��
*�������	��	��
*
*********************************************************************************/
void InitTime3(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx					 
}
/*******************************************************************************************************
*
*����		�� ��ʱ��3�жϷ������
*
*
******************************************************************************************************/
void TIM3_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 
        App.Data.GsmState|=1<<15;
        TIM_Cmd(TIM3, DISABLE); 
        GsmRxdTime();
    }
        
}

/*******************************************************************
*����		��Ӣ�Ķ��Ÿ�ʽת��
*�������	: content,��������
			  resPointer,ת���������
*�������	����
*******************************************************************/
void EnglishConvert(u8 *content,u8 *resPointer)
{
	u8 i,len;
	u8 temp[2];
	
	len=strlen((const char*)content);
	for(i=0;i<len;i++)
	{
		*resPointer++=0x30;
		*resPointer++=0x30;		
		sprintf((char*)temp,"%X",*content);

		*resPointer++=temp[0];
		*resPointer++=temp[1];
		content++;
	}
}
void Sim800SystickRoutine(void)
{
	static u32 time;
	
	if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)	
	{
		if(!App.Data.CidState.CidStart)
		{
			if(++time==1000)time=0;
      if(App.Data.GsmSimCardState)
      {
          switch(time)
          {
          case 100: 
            if(App.Data.GsmSimCardState)
              PostMessage(MessageGsmType,GsmSingQuery);
            break;
          case 200:
            break;
          case 300:
            time = 0;
            PostMessage(MessageGsmType,GsmOperatorQuery);
            break;
          }
        }
       else//SIM���Ƴ�
       {
          switch(time)
          {
            case 100:
            case 300:
            case 500:
            case 700:
              PostMessage(MessageGsmType,GsmSimCardQuery);
              break;
            case 800:
              PostMessage(MessageGsmType,GsmDiscovery);
              break;
            case 900:
              PostMessage(MessageGsmType,GSMSimRemoved);
              break;
          } 
        }
		}
  }
}
/***********************************************************************************
*����		��	GSMģ��ĳ�ʼ��
*�������	��	��
*�������	��	��
************************************************************************************/
void GsmInit(void)
{	
	GsmGPIOInit();
  InitTime3(499,7199);
	GsmDMASendConfig();
  GsmPoweOn();
	GsmATSend("AT","OK",50);
	GsmATSend("AT","OK",50);
	Simcom800CInit();
  
  GsmATSend("AT+GSN","OK",100);           //��ѯIMEI
  memcpy(App.Data.IMEINum,&GsmRxdBuffer[9],15);
  
  if(!GsmATSend("AT+CPIN?","OK",100))
  {
    App.Data.OperatorUpdate = 1;
    App.Data.GsmSimCardState = 2;
    RTT_printf(0,"AT + CPIN READY\r\n");
  }
  else
  {
    App.Data.OperatorUpdate = 1;
    App.Data.GsmSimCardState = 0;
    RTT_printf(0,"AT+CPIN NOT READY :No Sim\r\n");
  }
	System.Device.Gsm.GsmATSend=GsmATSend;
	System.Device.Gsm.GsmDMAStrin=StrinPrintf;
	System.Device.Gsm.GsmSendSMS=GsmSendSMS;
	System.Device.Gsm.GsmCall=GsmCall;	
}




