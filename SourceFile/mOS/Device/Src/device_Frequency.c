#include "device_Frequency.h"



u8  TIM5CH1CaptureState=0;	//���벶��״̬		    				
u32	TIM5CH1CaptureVal;	//���벶��ֵ
u16 ReadValue1,ReadValue2; 

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //ʹ��GPIOAʱ��

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	GPIO_SetBits(GPIOA,GPIO_Pin_0);

	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_CC1|TIM_IT_Update,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
   
}


//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 
 	if((TIM5CH1CaptureState&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		{
			if(TIM5CH1CaptureState&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1CaptureState&0X3F)==0X3f)//�ߵ�ƽ̫����
				{
					TIM5CH1CaptureState=0X80;//��ǳɹ�������һ��
					TIM5CH1CaptureVal=0;
				}
				else 
					TIM5CH1CaptureState++;
			}
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM5CH1CaptureState&0X40)		//���ڶ��λ�������
			{
				ReadValue2 = TIM_GetCapture1(TIM5);
				if ((ReadValue2 > ReadValue1)&&(TIM5CH1CaptureState==0x40))  //�����
				{
				  TIM5CH1CaptureVal = (ReadValue2 - ReadValue1); 
				}
				else  //�����
				{
				  TIM5CH1CaptureVal =  ((0xFFFF - ReadValue1) +ReadValue2);
				}
				TIM5CH1CaptureState|=0X80;		//��ǳɹ�����һ��������
			}
			else  								//��һ�β���������
			{
				ReadValue1=TIM_GetCapture1(TIM5);
				//TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1CaptureVal=0;
				TIM5CH1CaptureState = 0X40;		//��ǲ�����������
			}
		}
 	} 
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
 
}


#define TempLen 10
u32 Freq[TempLen];        //Ƶ��ֵ��������
u32 Freq_Sum=0;      //
u32 Overflow_ptr=0;  //�������ֵ
u8 Freq_ptr1=0;      //�˲�����ֵ1
u8 Freq_ptr2=0;      //�˲�����ֵ2
float Freq_value=0;  //Ƶ�ʸ���ֵ

/**************************************************************************************************
*
*����       ��Ƶ�ʲ����˲�����
*�������   ����
*�������   ����
*
**************************************************************************************************/

float FreqSample(void)
{
	u32 dTemp=0; 
	
	u8 i;
	//���������θߵ�ƽ
	if(TIM5CH1CaptureState&0X80)
	{
		//�˲�����
		Freq_ptr1++;
		Freq_ptr2++;
		if(Freq_ptr1>=TempLen)
		Freq_ptr1=0;    
		dTemp = TIM5CH1CaptureState&0X3F;
		dTemp *= 65536;//���ʱ���ܺ�  
		if(dTemp<=65536)  //�������С��2
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
	else //δ����ȴ�����
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


