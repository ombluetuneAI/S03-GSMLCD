#include "Device_Call.h"
#include "App.h"

#define LedTextPort		PCout(9)


extern float Freq_value;  //频率浮点值

extern float FreqSample(void);


/************************************************************************************************************
*
*描述     :初始化HT9200 GPIO
*
************************************************************************************************************/
static void InitHT9200(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	HT9200Data=1;
	HT9200Cs=1;
	HT9200CLK=1;
}
/********************************************
*
*描述		：发送一个DTMF字
*输入参数	：number,发送的字符
*输出参数	：无
*
********************************************/
static void Send5Bit(u8 number)
{
	u8 i;
	for(i=0;i<5;i++)
	{
		if((number>>i)&0x01)
			HT9200Data=1;
		else
			HT9200Data=0;
		HT9200CLK=0;
		DelayUs(10);
		HT9200CLK=1;
		DelayUs(10);
	}
}
/****************************************************************************************
*
*描述       ：进行一次完整的拨号
*输入参数	：pointer,字符串指针
*输出参数	：无
*
****************************************************************************************/
void HT9200Call(u8 *pointer)
{
	u8 *memPointer;
	u8 *str;
	u8 temp =strlen((const char *)pointer)+1;
	
	memPointer=(u8*)malloc(temp);
	if(memPointer==NULL)
		return;
	memcpy(memPointer,pointer,temp);
	str=memPointer;
    
	HT9200Cs=0;
	HT9200CLK=1;
	DelayMs(300);//这里延时是为了让9200开始进入稳定的工作状态
	while(*str!='\0')
	{
		HT9200Cs=0;
		HT9200CLK=1;
		DelayMs(50);
		if((*str>0x30)&&(*str<0x3A))
			temp=*str-0x30;
		else if(*str=='*')
			temp=0x0B;
		else if(*str=='#')
			temp=0x0C;
		else if(*str=='0')
			temp=0x0A;
		Send5Bit(temp);
		DelayMs(50);
		HT9200Cs=1;
		str++;
		
	}
	//停止位
	DelayMs(300);
	HT9200Cs=0;
	HT9200CLK=1;
	DelayMs(50);
	Send5Bit(0x1F);
	DelayMs(80);
	HT9200Cs=1;
	DelayMs(300);
	free(memPointer);//释放
	str=null;
	//CallConnect=1;//挂断
}
/****************************************************************************************
*
*描述       ：HT9170 GPIO 初始化
*输入参数	：无
*输出参数	：无
*
****************************************************************************************/
void InitHT9170(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);   
	//D0
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);  

	//DV
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);  
	

    DelayMs(500);
	
}
/****************************************************************************************
*
*描述       ：对收到的DTMF 进行解码
*输入参数	：无
*输出参数	：返回DTMF值
*
****************************************************************************************/
static u8 DTMFScan(void)
{
	u8 temp=0xFF;
	
	if(HT9170D3)
		temp=1<<3;
	else if(!HT9170D3)
		temp=0;
	if(HT9170D2)
		temp|=1<<2;
	else if(!HT9170D2)
		temp|=0;
	if(HT9170D1)
		temp|=1<<1;
	else if(!HT9170D1)
		temp|=0;
	if(HT9170D0)
		temp|=1;
	else if(!HT9170D0)
		temp|=0;
	
	switch(temp)
	{
		case 0x01: temp='1';break;
		case 0x02: temp='2';break;
		case 0x03: temp='3';break;
		case 0x04: temp='4';break;
		case 0x05: temp='5';break;
		case 0x06: temp='6';break;
		case 0x07: temp='7';break;
		case 0x08: temp='8';break;
		case 0x09: temp='9';break;
		case 0x0A: temp='0';break;
		case 0x0B: temp='*';break;
		case 0x0C: temp='#';break;
		case 0x0D: temp='A';break;
		case 0x0E: temp='B';break;
		case 0x0F: temp='C';break;
		case 0x00: temp='D';break;
	}
	return temp;
}

/**********************************************************************************
*
*描述		：对DTMF解码
*输入参数	：无
*输出参数	：返回0xFF则是没有DTMF信号，返回其他则是DTMF信号值
*
**********************************************************************************/
u16 DTMF9270Decode(void)
{ 	
	u16 temp=0xFF;
//    
//	if((Ht9170DV==1)&&(App.Data.CallRingEnable==0))
//	{
//        DelayMs(1);
//        if(Ht9170DV)
//        {
//            while(Ht9170DV)
//                temp=DTMFScan();
//        }
//    }
	return temp;
}
/***********************************************************************************************
*
*描述         ：振铃检测初始化
*输入参数     ：无
*输出参数     ：无
*
***********************************************************************************************/
void CallRingInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	TIM_ITRxExternalClockConfig(TIM2,TIM_TS_ETRF); 
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);  
	TIM_SetCounter(TIM2, 0);   
	TIM_Cmd(TIM2, ENABLE); 	
	
}

/******************************************************************************
*
*描述       :检测振铃     
*输入参数   :无
*输出参数   :无
*
******************************************************************************/
void CallRingSystick1000Routine(void)
{
	u8 res=0;

    res=(u16)FreqSample();
    if((App.Data.Calltime5s)&&(CallConnect==0))
    {
        if(((res<=28)&&(res>=22))&&(CallConnect==0))
        {
            App.Data.FrequencyTotal+=res;
            App.Data.FrequencyCount++;
            if(App.Data.FrequencyCount==8)
            {
                App.Data.FrequencyTotal/=App.Data.FrequencyCount;//取8次平均值
                App.Data.FrequencyCount=0;
                if((App.Data.FrequencyTotal<=28)&&(App.Data.FrequencyTotal>=23))
                {
                    App.Data.FrequencyTotal=0;
                    PostMessage(MessageCallRing,0);
                    App.Data.Calltime5s=0;
                    TIM_Cmd(TIM4, ENABLE); //开始计时
                }
            }
        }
    }

}
/*****************************************************************************
*
*描述		：DTMF检测，当检测到不是空值（0xFF）就给系统发送信息
*
*****************************************************************************/
void DTMFSystick1000Routine(void)
{
	u16 res=0xFF;
    res=DTMF9270Decode();	
	if(res!=0xFF)
		PostMessage(MessageCallTDMF,res);
}

void TIM4Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //

	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM4, DISABLE);					 
}

void TIM4_IRQHandler(void)   
{
    static u16 count=0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );   
        if(count==80)
        {
            App.Data.Calltime5s=1;
            count=0;
            TIM_Cmd(TIM4, DISABLE); 
        }
        else
            count++; 	
    }
        
}
/******************************************************************************
*
*描述		：电话机GPIO 初始化
*输入参数	：无
*罝出参数	：无
*
******************************************************************************/
void InitCall(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOA,ENABLE);	
	
	
	//PA12，断线检测    PA5，电话接通
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD,&GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	
	CallConnect=0;
	
	//振铃,上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    
	
  TIM4Init(499,7199);
	InitHT9170();
	InitHT9200();
	CallRingInit();
	System.Device.Telephone.Call=HT9200Call;
		
}

