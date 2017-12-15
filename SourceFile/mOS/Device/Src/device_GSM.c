#include "device_GSM.h"
#include "system.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "App.h"
///////////////////////////////////////////////////////////////////
//维护日志(V0.1/2017.5.5)
//1.优化 GsmGPIOInit 函数，初始化时，开机IO口置为高电平
//2.优化 GsmPowe 函数，开机过程优化，开机时间减少3s
//3.优化 Simcom800CInit 函数
//

#define GsmPoweOn()			GsmPowe()
#define GsmPowerOff()		GsmPowe()


#define GsmTxdBufferSize	100
#define GsmRxdBufferSize  100
		


__align(8) u8 GsmTxdBuffer[GsmTxdBufferSize]; 
__align(8) u8 GsmRxdBuffer[GsmRxdBufferSize];

#define StateTotal		15	//要扫描的状态
//#define GsmTimeID		2

const char GsmStateTable[StateTotal][20]=
{
	"+CLIP:",//有来电               	0
	"+DTMF: ",//DTMF解码             	1
	"+CSQ:",//信号质量                2
  "+CPIN: READY",//检测到SIM卡      3
	"+CMT:",//新消息                  4
  "MO CONNECTED",//接通电话         5
  "NO CARRIER",//结束通话   		    6
  "+CMGS:",//短信发送成功        	  7
  "+COPS: 0,0,",//运营商名字        8
	"BUSY"	,	//					            9
	"NO ANSWER",//无应答              10
  "+CPIN: NOT READY",           //  11
  "+CPIN: NOT INSERTED",        //  12
  "Call Ready",                 //  13
  "SMS Ready",                  //  14
  
};
//修改日志：
/*
2017-0406，将原来的SIM5320 更换为SIM800C
所要修改的函数：1，IO 初始函数 ，GsmGPIOInit() ，2，初始化函数，GsmInit()
*/


//函数开始
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
*描述		:	等待串口接收超时时间，如果超时则标记为接收完成
				并发送消息到Logic中进行处理
*输入参数	：	无
*输出参数	：	无
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
				case 0://有来电
					App.Data.CallRing=1;
					PostMessage(MessageGsmType,GsmRingMessage);
          RTT_printf(0,"PostMessage:Ring\r\n");
//					CloseSpeaker();
					return;
				case 1://DTMF解码
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
				case 2://信号量
					App.Data.GsmSMSInfo=strPointer;
          PostMessage(MessageGsmType,GsmCSQMessage);
          RTT_printf(0,"PostMessage:CSQ\r\n");
					return;
//				case 3://准备就绪 检查到SIM卡
//					App.Data.GsmSimCardState=1;
//          App.Data.OperatorUpdate = 1;
//          PostMessage(MessageGsmType,GsmReadyMessage);
////					PostMessage(MessageGsmType,GsmNormalMessage);
//					return;
        case 4://收到新消息 已读取
					App.Data.GsmSMSInfo=strPointer;
					PostMessage(MessageGsmType,GsmSMSRead);
        RTT_printf(0,"PostMessage:SMSRead\r\n");
					return;
        case 5://接通电话
          PostMessage(MessageGsmType,GsmMessageCONN);
          RTT_printf(0,"PostMessage:MO CONNECTED\r\n");
          return;
                
        case 6://结束通话
//          CloseSpeaker();
          App.Data.CallRing=0;
          App.Data.DtmfCount=0;
          App.Data.GsmSMSInfo=strPointer;
          PostMessage(MessageGsmType,GsmMessageCEND);
        RTT_printf(0,"PostMessage:HungUp\r\n");
          return;
        case 7://短信发送成功
          PostMessage(MessageGsmType,GsmSMSSendOK);
        RTT_printf(0,"PostMessage:SMSSendOK\r\n");
          return;
        case 8://运营商名字
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
        case 14://检测到SIM卡
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
*描述		：	将1个字符转换为16进制数字
*输入参数	：	chr，字符,0~9/A~F/a~F
*输出参数	：	chr对应的16进制数值
******************************************************************************************************/
u8 Char2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}

/*******************************************************************************************************
*描述		：	将1个16进制数字转换为字符
*输入参数	：	hex,16进制数字,0~15;
*输出参数	：	转换后的字符
*******************************************************************************************************/
u8 Hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
/*******************************************************************************************************
*描述		：DMA的各通道配置，这里的传输形式是固定的,这点要根据不同的情况来修改
			  从存储器->外设模式/8位数据宽度/存储器增量模式
*输入参数	：DMAChannelx,DMA通道
			  cpar,外设地址
			  cmar,存储器地址
			  cndtr,数据传输量 
*输出参数	：无
*******************************************************************************************************/

void GsmDMASendConfig(void)
{
	DMA_InitTypeDef DMA_InitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  DMA_DeInit(DMA1_Channel7);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)GsmTxdBuffer;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 0;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_DeInit(DMA1_Channel6);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)GsmRxdBuffer;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设到内存
	DMA_InitStructure.DMA_BufferSize =GsmRxdBufferSize;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	//DMA_ClearFlag(DMA1_FLAG_TC6);
 	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);//传输完成中断
 	DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE);//传输错误中断
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
*描述		：开启一次DMA传输
*输入参数	：DMAChannelx,DMA通道
			  length,数据长度
*输出参数	：无
*******************************************************************************************************/

void GsmDMAEnable(DMA_Channel_TypeDef* DMA_CHx,u16 length)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道  
	DMA_ClearFlag(DMA1_FLAG_TC7);   
	DMA_SetCurrDataCounter(DMA1_Channel7,length);//DMA通道的DMA缓存的大小
	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	


/**********************************************************************************************
*描述		：	将命中转换并通过DMA发送出去，串口2,printf 函数确保一次发送数据不超过USART2_MAX_SEND_LEN字节
*输入参数	：	
*输出参数	：	
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
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送 
	GsmDMAEnable(DMA1_Channel7,strlen((const char*)GsmTxdBuffer)); 	//通过dma发送出去
	while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
	USART_DMACmd(USART2,USART_DMAReq_Tx,DISABLE); //禁止串口1的DMA发送 
	DelayMs(100);	
}

/***********************************************************************************
*描述		：	GSM模块的GPIO初始化
*输入参数	：	无
*输出参数	：	无
************************************************************************************/

static void	GsmGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    //开机引脚
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
	GsmPower=0;
}

/***********************************************************************************
*描述		：	GSM模块电源控制(高电平保持必须越过1秒)
*输入参数	：	无
*输出参数	：	无
************************************************************************************/
void GsmPowe(void)
{
	GsmPower=1;
	DelayMs(1200);
	GsmPower=0;
	DelayMs(2200);
}
/***********************************************************************************
*描述		:	发送命令后,检测接收到的应答
*输入参数	：	str:期待的应答结果
*输出参数	:	0,没有得到期待的应答结果
			    其他,期待应答结果的位置(str的位置)
***********************************************************************************/
u8* GsmStringCheckString(u8 *str)
{
	char *strx=0;
	if(App.Data.GsmState&0X8000)		//接收到一次数据了
	{ 
		GsmRxdBuffer[App.Data.GsmState&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)GsmRxdBuffer,(const char*)str);
	} 
	return (u8*)strx;
}
/***********************************************************************************
*
*描述		:   AT 指令发送
*输入参数	： 	strPointer，要发送的指令字符串
                ackPointer，返回的字符串
*输出参数	： 0、发送成功
             1、收到非预期结果
             2、超时未收到任何结果
************************************************************************************/
u8 GsmATSend(u8 *strPointer,u8 *ackPointer,u16 waitTime)
{
  u8 res = 0;
	App.Data.GsmState=0;
//	App.Data.GsmRxdCount=0;
  memset(GsmRxdBuffer,0x00,100);
	if((u32)strPointer<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		while((USART2->SR&0X40)==0);//一定要加上，否则第一个字节会丢失
		USART2->DR=(u32)strPointer;
	}else StrinPrintf("%s\r\n",strPointer);//发送命令
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
	App.Data.GsmState=0;//完成接收，并等待下次接收
	return res;
	
}

/*******************************************************************************
* 描述	    : 串口中断处理函数
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
        TIM3->CNT = 0;//计数器清空 
				if(App.Data.GsmState==0) 				
				{
//					System.Device.Timer.Start(GsmTimer,TimerMessageHandle,70,GsmRxdTime);
//        RTT_printf(0,"USART2_IRQHandler:GsmRxdTime\r\n");
          TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
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
//        TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
//      }
//			GsmRxdBuffer[App.Data.GsmState++]=data;
//		}
//		else
//		{
//			App.Data.GsmState|=1<<15;
//		}
//		//App.Data.GsmRxdCount=App.Data.GsmState;//记录当前收到的字节数
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

*描述		：	发送短信
*输入参数	：	number,电话号码
				text,要发送的内容
*输出参数	：	0，发送成功，其他值表示失败

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

*描述		：	发短信
*输入参数	：	number,号码;text,字符串
*输出参数	：	0，

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

*描述		：	快速发送短信
*输入参数	：	nPointer,号码;textPointer,内容
*输出参数	：	无

*********************************************************************************************/
/*static void Simcom5320Quickly(u8 *nPointer,u8 *textPointer)
{
    u8 numberBuffer[128];
    
    memset(numberBuffer,0,128);
    sprintf((char*)numberBuffer,"AT+CMGSO=\"%s\",\"%s\"",nPointer,textPointer);
    StrinPrintf("%s\r\n",numberBuffer);
}*/
/********************************************************************************************

*描述		：	拨打电话
*输入参数	：	phoneNumber,要拨打的电话
*输出参数	：	0，

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

*描述		：	用串口直接发送数据
*输入参数	：	str,要发送的字符串
				len,发送字符串的长度
*输出参数	：	无

**************************************************************************************************/
void send(u8 *str,u8 len)
{
	while(len--)
	{
		USART_GetFlagStatus(USART2, USART_FLAG_TC);//一定要加上，否则第一个字节会丢失
		USART_ClearFlag(USART2,USART_FLAG_TC);
		USART_SendData(USART2, *str++);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
	}
}
/**************************************************************************************************
*
*描述       ：IMEI解析
*输入参数   ：无
*输出参数   ：无
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
*描述		：	GSM模块的初始化
*输入参数	：	无
*输出参数	：	无
*
******************************************************************/
void Simcom800CInit(void)
{
    u8 *strPointer;
    GsmATSend("AT+IPR=115200","OK",2);      //设置波特率  
    GsmATSend("AT+CLIP=1","OK",10);        //设置来电显示
    GsmATSend("AT+MORING=1","OK",10);      //显示去电的呼叫状态
//    GsmATSend("AT+CPIN?","OK",10);	
    GsmATSend("AT+CGREG?","OK",10);      //GPRS网络注册状态
    GsmATSend("AT+COPS?","OK",10);         //选择运营商 
    GsmATSend("AT+CSQ?","OK",10);
//    IMEIParse();
    GsmATSend("AT+CIPMODE=0","OK",10);     //选择TCPIP应用模式为非透明模式
    GsmATSend("AT+CENG=1","OK",10);        //开启工程模式
    GsmATSend("AT*CELLLOCK=0","OK",10);    //设置需锁定小区ARFCN列表
                   //保存用户当前的配置
    GsmATSend("AT+CNMI=2,2,0,0,0","OK",10);//新消息提示
    GsmATSend("AT+CMGF=1","OK",200);        //选择短消息格式为文本格式
    GsmATSend("AT+CSCS=\"UCS2\"","OK",10); //选择TE字符集
//    DelayMs(50);
    GsmATSend("AT+CSMP=17,168,0,8","OK",10);//设置短消息文本模式参数
    GsmATSend("AT+DDET=1,0,1,1","OK",10);    //DTMF检测控制
    GsmATSend("AT+CMIC=0,15","OK",10);       //设置mic增益
    GsmATSend("ATS0=1","OK",10);             //设置自动接听响铃次数
    GsmATSend("AT+CALM=1","OK",10);            //设置来电静音
    GsmATSend("AT&W","OK",2);
    GsmATSend("AT+CSQ?","OK",10);
    
}

/********************************************************************************
*
*描述		：	通用定时器3中断初始化,这里时钟选择为APB1的2倍，而APB1为36M
*输入参数	：	arr：自动重装值;psc：时钟预分频数
*输出参数	：	无
*
*********************************************************************************/
void InitTime3(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, DISABLE);  //使能TIMx					 
}
/*******************************************************************************************************
*
*描述		： 定时器3中断服务程序
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
*描述		：英文短信格式转换
*输入参数	: content,短信内容
			  resPointer,转换后的内容
*输出参数	：无
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
       else//SIM卡移除
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
*描述		：	GSM模块的初始化
*输入参数	：	无
*输出参数	：	无
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
  
  GsmATSend("AT+GSN","OK",100);           //查询IMEI
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




