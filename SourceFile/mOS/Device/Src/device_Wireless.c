
/*----------------------------ͷ�ļ�-----------------------------*/

#include "device_Wireless.h"
#include "App.h"
/*----------------------------------����-------------------------------------*/


//---------------------------------------------
//����ʱ���

#define PulseMax	 				200  //������������
#define NarrowPulseMax  			70 //խ�����������ֵ
#define NarrowPulseMin	 			8  //խ������С����ֵ

#define SyncPulseMax	 			2000  //ͬ���͵�ƽ�������
#define SyncPulseMin  				500   //ͬ���͵�ƽ��С����

#define SyncMultiplesMax 			36	   //ͬ����������ޱ���
#define SyncMultiplesMin 			29    //ͬ������С���ޱ���
#define EV1527						0xFE
//----------------------------------------------


/*------------------�궨��----------------------*/

#define delay10us()  DelayUs(8);//�������ʵ��ʱ��Ϊ11us


/*-------------------����----------------------*/

static u16  rxTimes=0;
static u8   rxTimesFlag=0;
static  u8  eV1527[3];

uint16_t signalLow=0;//ͬ����͵�ƽʱ���¼
uint16_t signalHigh=0;//ͬ����ߵ�ƽʱ���¼
/*-------------------����----------------------*/




/*-------------------------����---------------------------*/
static u32 WirelessDecode(void);
static void WirelessSendPacket(uint8_t *SendBufferPointer);
extern void OpenRF(void);
void InitTime6(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	//TIM_Cmd(TIM7, DISABLE);  //ʹ��TIMx	
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
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
  {
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
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

    /*�����ź���������*/
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
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
*���������߽������
*����: ��
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
		/*----------------------------��ʼ����-------------------------*/
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
					//������   խ����Ϊ�������4�� ʵ�ʲ���Ϊ3��
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
/*---------------------�����ڶ��ε�ͬ����-------------------------*/			
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
/*------------------------------------------�ȶԽ���������������ɹ�----------------------------------------------*/
		if((pt2262Res[0][0]==pt2262Res[1][0])&&(pt2262Res[0][1]==pt2262Res[1][1])&&(pt2262Res[0][2]==pt2262Res[1][2]))
		{
			for(i=0;i<3;i++)  //�ж�2262��1527
			{
				for(u=0;u<4;u++)
				{
					if(((pt2262Res[0][i]>>(u*2)) & 3)==2) 
					{
						i=EV1527;
					}
				}  //��10��Ϊ1527
				if(i==EV1527) break;
			}
			if(EV1527==i)  //1527
			{
				keyVal=pt2262Res[1][2] & 0x0f;//�ֳ�1527�İ���ֵ
				pt2262Res[0][2]=keyVal; //�ֳ�1527�ĺ�4λ��ַ
				//decodeType=1;         //Ϊ0��2262��1��1527
			}
			else//2262
			{
				keyVal=0;
				for(i=0;i<4;i++)
				{
					if(((pt2262Res[0][2]>>(i*2))&3)==3) 
						keyVal|=1<<i;
				}   //�����2262�İ�������                                  
				//decodeRes[0][2]=00; //2262�޺�4λ��ַ,ȫΪ0
				//decodeType=0;//Ϊ0��2262��1��1527
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

//=============================================���߱���=================================================//

/***********************************************************************************************
*
*���������ݱ���
*������HighLevel:�ߵ�ƽʱ��
*       LowLevel: �͵�ƽʱ��
*
************************************************************************************************/
static void WirelessSendBit(uint8_t HighLevel,uint8_t LowLevel)
{
    uint32_t signal;
    uint16_t hightTime=200; //�ߵ�ƽ����ʱ��
	
    signal=hightTime*HighLevel; //�ߵ�ƽʱ��
    WirelessDataSendPort=1;
    DelayUs(signal);

    signal=hightTime*LowLevel;//�͵�ƽʱ��
	WirelessDataSendPort=0;
	DelayUs(signal);
}

/***********************************************************************************************
*
*����: ����һ�����ݰ�
*����: SendBufferPointer: Ҫ���͵�����ָ��
*
************************************************************************************************/
static void WirelessSendPacket(u8 *SendBufferPointer)
{
    uint8_t groupsCount=0;//�������ݵ�����
    uint8_t byteCount=0;//һ�����ݵ��ֽڸ���
    uint8_t bit=0;
    uint8_t getBitResults=0;
    uint8_t GetBit[] = {0X01,0X02,0X04,0X08,0X10,0X20,0X40,0X80};
    
    
    EnterCritical();
    for(groupsCount=0;groupsCount<30;groupsCount++)//Ҫ����5������
    {
        WirelessDataSendPort=1;
        WirelessSendBit(1,30);//ͬ����
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





