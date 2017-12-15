


#include "device_adc.h"
#include "system.h"
#include "App.h"

static float NullValue;

static float * RegisterPointerBlock[4] =
{
    &NullValue, &NullValue, &NullValue, &NullValue
};


/*******************************************************************************
* ������   : Register
* ����        : ��Ӧ�ò����ָ��ע�ᵽ��Ӧ��ͨ�������У�ʵ�ֵײ������򶥲㴫��
* �������  : adcChannel: ADCͨ����
*           : dataPoint: Ӧ�ò����ָ��
* ���ز���  : ��
*******************************************************************************/
static void Register(AdcChannelEnum adcChannel, float * dataPointer)
{
    RegisterPointerBlock[adcChannel] = dataPointer;
}


/*******************************************************************************
* ������   : AdcSystick10000Routine
* ����        : ϵͳʱ��10000/S����0.1mS�������һ�����У���ȡADCֵ��App�����
* �������  : ��
* ���ز���  : ��

*******************************************************************************/

u16 GetAdcValue(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1 , ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
  return ADC_GetConversionValue(ADC1);
}

void AdcSystick100Routine(void)
{
  static u8 AdcStep = 0;
	static float tmpValue = 0;
	tmpValue += GetAdcValue(10);
	AdcStep++;
	if(AdcStep==5)
	{
		*RegisterPointerBlock[0] = tmpValue*(3.3)*2/(4096*5);
		AdcStep = 0;
		tmpValue = 0;
	}
	
    
//	ADC_RegularChannelConfig(ADC1,Counter + 10, 1 , ADC_SampleTime_239Cycles5);
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
//  *RegisterPointerBlock[Counter] = (float)ADC_GetConversionValue(ADC1)*(3.3/4096)/0.5;
//	
//	if (++Counter > 1) Counter = 0;
}

void InitAdc(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 10;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);

    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);

    while(ADC_GetCalibrationStatus(ADC1));

    //ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    System.Device.Adc.Register = Register;
		
		System.Device.Systick.Register(Systick100,AdcSystick100Routine);
}




