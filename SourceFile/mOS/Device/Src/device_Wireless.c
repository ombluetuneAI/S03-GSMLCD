
/*----------------------------头文件-----------------------------*/

#include "device_Wireless.h"
#include "App.h"
/*----------------------------------常数-------------------------------------*/


//---------------------------------------------
//脉宽时间宏

#define PulseMax	 				200  //最大宽脉宽上限
#define NarrowPulseMax  			70 //窄脉冲最大上限值
#define NarrowPulseMin	 			8  //窄脉冲最小下限值

#define SyncPulseMax	 			2000  //同步低电平最大上限
#define SyncPulseMin  				500   //同步低电平最小下限

#define SyncMultiplesMax 			36	   //同步码最大上限倍数
#define SyncMultiplesMin 			29    //同步码最小下限倍数
#define EV1527						0xFE
//----------------------------------------------


/*------------------宏定义----------------------*/

#define delay10us()  DelayUs(8);//这里测试实际时间为11us


/*-------------------变量----------------------*/

static u16  rxTimes=0;
static u8   rxTimesFlag=0;
static  u8  eV1527[3];

uint16_t signalLow=0;//同步码低电平时间记录
uint16_t signalHigh=0;//同步码高电平时间记录
/*-------------------类型----------------------*/




/*-------------------------函数---------------------------*/
static u32 WirelessDecode(void);
static void WirelessSendPacket(uint8_t *SendBufferPointer);
extern void OpenRF(void);
void InitTime6(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	//TIM_Cmd(TIM7, DISABLE);  //使能TIMx	
	TIM_Cmd(TIM7, ENABLE);	
}
void WirelessDecodeInterrupt(void)
{
	if(WirelessPort)
	{
		signalHigh++;
		if((signalLow>500)&&(signalLow<2000))
		{
			if(((signalHigh*29)<signalLow)&&((signalHigh*32)>signalLow))
			{
        RTT_printf(0,"head  ");
				if(WirelessDecode())
				{
					TIM_Cmd(TIM7, DISABLE);
					System.Device.Timer.Start(RFOpenTimeID,TimerMessageHandle,1000,OpenRF);

					signalLow=0;
					signalHigh=0;
				}
			}else if(signalLow)
			{
				signalLow=0;
				signalHigh=0;
			}
		}
		else
		{
			if(signalLow)
			{
				signalLow=0;
				signalHigh=0;
			}
		}
	}
	else if(!WirelessPort)
	{
		if((signalHigh>8)&&(signalHigh<70))
			signalLow++;
		else
			signalHigh=0;
	}
}

void TIM7_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
  {
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIMx更新中断标志 
		WirelessDecodeInterrupt();
		//SimcomReset=~SimcomReset;
  }
        
}
/********************************************************************************************


*********************************************************************************************/
void WirelessPortInit(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    /*无线信号输入引脚*/
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//悬浮输入
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    WirelessDataSendPort=0;
	InitTime6(800,0);
	
	
	System.Device.Wireless.WirelessDecode = WirelessDecode;
	System.Device.Wireless.WirelessSend= WirelessSendPacket;
}
/***********************************************************************************************
*
*
*
*
*
************************************************************************************************/

uint8_t pt2262Res[2][3];
/******************************************************************************
*
*描述：无线解码程序
*参数: 无
*
******************************************************************************/
static u32 WirelessDecode(void)
{	
	uint8_t rxCount=0;
	uint8_t byte=0;
	uint8_t bit=0;
	u8 keyVal,i,u;
	
	uint16_t highTime=0;
	uint16_t lowTime=0;
	
	uint8_t res;
	
	uint32_t receiveData=0;

    //signalLow=0;
    //signalHigh=0;
	
	/*while(WirelessPort)
	{
		delay10us();
		signalHigh++;
	}
	if((signalHigh>NarrowPulseMax)||(signalHigh<NarrowPulseMin))
		return 0;
	while(!WirelessPort)
	{
		delay10us();
		signalLow++;
	}
	if((signalLow>SyncPulseMax)||(signalLow<SyncPulseMin))//
		return 0;
	
	if(((signalHigh*SyncMultiplesMin)<signalLow)&&((signalHigh*SyncMultiplesMax)>signalLow))*/
	{
		/*----------------------------开始解码-------------------------*/
		for(rxCount=0;rxCount<2;rxCount++)
		{
			for(byte=0;byte<3;byte++)
			{
				for(bit=0;bit<8;bit++)
				{
					highTime=0;
					lowTime=0;
					
					while(WirelessPort)
					{
						delay10us();
						highTime++;
					}
					if((highTime>PulseMax)||(highTime<NarrowPulseMin))
					{
						return 0;
					}
					//宽脉冲   窄脉冲为宽脉冲的4倍 实际测试为3倍
					if(((signalHigh*2)<highTime)&&(highTime<signalHigh*4))
					{
						res=1;
					}
					else
					{
						res=0;
						
					}
					pt2262Res[rxCount][byte]<<=1;
					pt2262Res[rxCount][byte]|=res;
					while(!WirelessPort)
					{
						delay10us();
						lowTime++;
					}
					if((lowTime>PulseMax)||(lowTime<NarrowPulseMin))
						return 0;					
				}
			}
			highTime=0;
			signalLow=0;			
/*---------------------跳过第二次的同步码-------------------------*/			
			while(WirelessPort)
			{
				delay10us();
				highTime++;
			}
			while(!WirelessPort)
			{
				delay10us();
				signalLow++;
			}
			//if((signalLow<signalHigh*SyncMultiplesMin)||(signalLow>signalHigh*SyncMultiplesMax))
				//return 0;
		}
/*------------------------------------------比对结果，如果相等则解码成功----------------------------------------------*/
		if((pt2262Res[0][0]==pt2262Res[1][0])&&(pt2262Res[0][1]==pt2262Res[1][1])&&(pt2262Res[0][2]==pt2262Res[1][2]))
		{
			for(i=0;i<3;i++)  //判定2262与1527
			{
				for(u=0;u<4;u++)
				{
					if(((pt2262Res[0][i]>>(u*2)) & 3)==2) 
					{
						i=EV1527;
					}
				}  //有10则为1527
				if(i==EV1527) break;
			}
			if(EV1527==i)  //1527
			{
				keyVal=pt2262Res[1][2] & 0x0f;//分出1527的按键值
				pt2262Res[0][2]=keyVal; //分出1527的后4位地址
				//decodeType=1;         //为0是2262，1是1527
			}
			else//2262
			{
				keyVal=0;
				for(i=0;i<4;i++)
				{
					if(((pt2262Res[0][2]>>(i*2))&3)==3) 
						keyVal|=1<<i;
				}   //计算出2262的按键数据                                  
				//decodeRes[0][2]=00; //2262无后4位地址,全为0
				//decodeType=0;//为0是2262，1是1527
			}
//             if((eV1527[0]==pt2262Res[0][0])&&(eV1527[1]==pt2262Res[0][1])&&(eV1527[2]==pt2262Res[0][2]))
//                 return 0;
            receiveData=eV1527[0]=pt2262Res[0][0];
            receiveData<<=8;
            receiveData|=eV1527[1]=pt2262Res[0][1];
            receiveData<<=8;
            receiveData|=eV1527[2]=pt2262Res[0][2];
            RTT_printf(0,"Rmt:%#x\r\n",receiveData);
            PostMessage(MessageWireless, receiveData); 
            rxTimesFlag=1;          
		}

	}

	signalLow=0;
	signalHigh=0;
	return receiveData;
}

//=============================================无线编码=================================================//

/***********************************************************************************************
*
*描述：数据编码
*参数：HighLevel:高电平时间
*       LowLevel: 低电平时间
*
************************************************************************************************/
static void WirelessSendBit(uint8_t HighLevel,uint8_t LowLevel)
{
    uint32_t signal;
    uint16_t hightTime=200; //高电平基本时间
	
    signal=hightTime*HighLevel; //高电平时间
    WirelessDataSendPort=1;
    DelayUs(signal);

    signal=hightTime*LowLevel;//低电平时间
	WirelessDataSendPort=0;
	DelayUs(signal);
}

/***********************************************************************************************
*
*描述: 发送一个数据包
*参数: SendBufferPointer: 要发送的数据指针
*
************************************************************************************************/
static void WirelessSendPacket(u8 *SendBufferPointer)
{
    uint8_t groupsCount=0;//发送数据的组数
    uint8_t byteCount=0;//一组数据的字节个数
    uint8_t bit=0;
    uint8_t getBitResults=0;
    uint8_t GetBit[] = {0X01,0X02,0X04,0X08,0X10,0X20,0X40,0X80};
    
    
    EnterCritical();
    for(groupsCount=0;groupsCount<30;groupsCount++)//要发送5组数据
    {
        WirelessDataSendPort=1;
        WirelessSendBit(1,30);//同步码
        for(byteCount=0;byteCount<3;byteCount++)
        {
            for(bit=8;bit>0;bit--)
            {
                getBitResults=SendBufferPointer[byteCount] & GetBit[bit-1];;
                if(getBitResults==0)
                {
                    WirelessSendBit(1,3);
                }
                else
                {
                    WirelessSendBit(3,1);
                }
            }
        }
        WirelessDataSendPort=0;
    }
    ExitCritical();
}
void SendWirelessData(void)
{
	u8 data[3];
	data[0]=Ushort0(App.Data.SystemID);
	data[1]=Ushort1(App.Data.SystemID);
	if(App.Data.SystemMode==SystemArmMode)
		data[2]=SendArmData;
	else if(App.Data.SystemMode==SystemDisarmMode)
		data[2]=SendDisarmData;
	else if(App.Data.SystemMode==SystemAlarmMode)
		data[2]=SendAlarmData;
	WirelessSendPacket(data);
}
/***********************************************************************************


************************************************************************************/
void WirelessSystick100Routine(void)
{
	u32 receiveData=0;
	static u16 rxCount=0,timeCount=0;

	if(rxCount>=1)
		if(1500<=++timeCount)
		{
      timeCount=0;
      rxCount=0;
		}
    if(rxTimesFlag)
    {
      rxTimes++;
      if(rxTimes>=3000)
      {
        memset(eV1527,0,3);
        rxTimesFlag=0;
        rxTimes=0;
      }
    }
	if((App.Data.UseWireless)&&(rxCount<1))
	{
		receiveData=WirelessDecode();
		if(0!=receiveData)
    {
			rxCount++;
    }
	}
}





