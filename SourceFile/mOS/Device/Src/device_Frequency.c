#include "device_Frequency.h"



u8  TIM5CH1CaptureState=0;	//输入捕获状态		    				
u32	TIM5CH1CaptureVal;	//输入捕获值
u16 ReadValue1,ReadValue2; 

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //使能GPIOA时钟

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	GPIO_SetBits(GPIOA,GPIO_Pin_0);

	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_CC1|TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
   
}


//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 
 	if((TIM5CH1CaptureState&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			if(TIM5CH1CaptureState&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1CaptureState&0X3F)==0X3f)//高电平太长了
				{
					TIM5CH1CaptureState=0X80;//标记成功捕获了一次
					TIM5CH1CaptureVal=0;
				}
				else 
					TIM5CH1CaptureState++;
			}
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM5CH1CaptureState&0X40)		//捕第二次获到上升沿
			{
				ReadValue2 = TIM_GetCapture1(TIM5);
				if ((ReadValue2 > ReadValue1)&&(TIM5CH1CaptureState==0x40))  //无溢出
				{
				  TIM5CH1CaptureVal = (ReadValue2 - ReadValue1); 
				}
				else  //有溢出
				{
				  TIM5CH1CaptureVal =  ((0xFFFF - ReadValue1) +ReadValue2);
				}
				TIM5CH1CaptureState|=0X80;		//标记成功捕获到一次上升沿
			}
			else  								//第一次捕获上升沿
			{
				ReadValue1=TIM_GetCapture1(TIM5);
				//TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1CaptureVal=0;
				TIM5CH1CaptureState = 0X40;		//标记捕获到了上升沿
			}
		}
 	} 
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
 
}


#define TempLen 10
u32 Freq[TempLen];        //频率值缓存数组
u32 Freq_Sum=0;      //
u32 Overflow_ptr=0;  //溢出计数值
u8 Freq_ptr1=0;      //滤波计数值1
u8 Freq_ptr2=0;      //滤波计数值2
float Freq_value=0;  //频率浮点值

/**************************************************************************************************
*
*描述       ：频率采样滤波处理
*输入参数   ：无
*输出参数   ：无
*
**************************************************************************************************/

float FreqSample(void)
{
	u32 dTemp=0; 
	
	u8 i;
	//捕获了两次高电平
	if(TIM5CH1CaptureState&0X80)
	{
		//滤波处理
		Freq_ptr1++;
		Freq_ptr2++;
		if(Freq_ptr1>=TempLen)
		Freq_ptr1=0;    
		dTemp = TIM5CH1CaptureState&0X3F;
		dTemp *= 65536;//溢出时间总和  
		if(dTemp<=65536)  //溢出次数小于2
			dTemp = TIM5CH1CaptureVal;
		else 
			dTemp = dTemp-65536+TIM5CH1CaptureVal;
		 
		Freq[Freq_ptr1] = dTemp;
		dTemp=0;

		if(Freq_ptr2>=TempLen)
		{
			for(i=0;i<TempLen;i++)
				dTemp += Freq[i];     
			Freq_value = 1000000.0/dTemp;
			Freq_ptr2=TempLen;
		}
		TIM5CH1CaptureState=0;
		Overflow_ptr=0;
	}
	else //未捕获等待清零
	{
		Overflow_ptr++;
		if(Overflow_ptr>20)
		{
			 Freq_value=0;
			 Overflow_ptr=0;
		}		
	}
	return Freq_value;
 
}


