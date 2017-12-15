/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "Device_RTC.h"
#include "system.h"
#include "App.h"
/*---------------------------------------------����--------------------------------------*/

/*--------------------------------------------�궨��-------------------------------------*/
#define RTCInitRecord	0x6107	//��ʼ����д���ֵ��ʾRTC��ʼ������(��ֵ������д <0xffff)

/*---------------------------------------------����--------------------------------------*/
CalendarStruct Calendar;

//�·����ݱ�
u8 const MonthCorrectionTable[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  

//ƽ����·����ڱ�
const u8 MonthTable[12]={31,28,31,30,31,30,31,31,30,31,30,31};
/*---------------------------------------------����--------------------------------------*/



/*---------------------------------------------����--------------------------------------*/

/*****************************************************************************************************************************
*����		��	�ж��Ƿ������꺯��
				�·�   1  2  3  4  5  6  7  8  9  10 11 12
				����   31 29 31 30 31 30 31 31 30 31 30 31
				������ 31 28 31 30 31 30 31 31 30 31 30 31
*�������	:	year,���
*�������	:	������ǲ������꣺
				1,��.0,����

*****************************************************************************************************************************/
u8 LeapYear(u16 year)
{
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
				return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else 
				return 0;   
		}else 
			return 1;   
	}
	else 
		return 0;	
}

/******************************************************************************************************************************
*����		��	����ʱ�ӣ��������ʱ��ת��Ϊ���ӣ���1970��1��1��Ϊ��׼��1970~2099��Ϊ�Ϸ����
*�������	��	timeSet,ʱ���б�
*�������	��	0,�ɹ�;
				����:�������.

******************************************************************************************************************************/
u8 RTCTimeSet(TimeSetStruct *timeSet)
{
	u16 t;
	u32 secCount=0;
	
	if((timeSet->Year<1970)||(timeSet->Year>2099)) return 1;
	for(t=1970;t<timeSet->Year;t++)
	{
		if(LeapYear(t))
		{
			secCount+=31622400;//�����������
		}
		else 
		{
			secCount+=31536000;	//ƽ���������
		}
	}
	timeSet->Month-=1;
	for(t=0;t<timeSet->Month;t++)//��ǰ���·ݵ����������
	{
		secCount+=(u32)MonthTable[t]*86400;//�·����������
		if(LeapYear(timeSet->Year)&&t==1)
			secCount+=86400;			//����2�·�����һ���������	 		
	}
	secCount+=(u32)(timeSet->Day-1)*86400;//��ǰ�����ڵ���������� 
	secCount+=(u32)timeSet->Hour*3600;//Сʱ������
	secCount+=(u32)timeSet->Min*60;	 //����������
	secCount+=timeSet->Sec;//�������Ӽ���ȥ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(secCount);	//����RTC��������ֵ

	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	System.Device.DateSet.RTCDateSet=RTCTimeSet;
	return 0;	 
	
}
/*******************************************************************************************************************************
*����		��	RCT�жϷ�������
*�������	��	��
*�������	��	��
*******************************************************************************************************************************/
static void RTCNVICConfig(void)
{	
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}
/*******************************************************************************************************************************
*����		��	������������ڼ������빫�����ڵõ�����(ֻ����1901-2099��)
*�������	��	���������� 
*�������	��	���ں�	

*******************************************************************************************************************************/
																					 
static u8 RTCGetWeek(CalendarStruct *date)
{	
	u16 temp;
	u8 yearH,yearL;
	
	yearH=date->Year/100;	
	yearL=date->Year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp=yearL+yearL/4;
	temp=temp%7; 
	temp=temp+date->Date+MonthCorrectionTable[date->Month-1];
	if (yearL%4==0&&date->Month<3)temp--;
	return(temp%7);
}			  


/**************************************************************************************************
*����		��	�õ���ǰ��ʱ��
*�������	��	��
*�������	��	0,�ɹ�;����:�������.
**************************************************************************************************/
u8 RTCGetTime(void)
{
	static u16 daycnt=0;
	u32 timeCount=0; 
	u32 numberDays=0;
	u16 temp=0;	  
    timeCount=RTC_GetCounter();	 
 	numberDays=timeCount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=numberDays)//����һ����
	{	  
		daycnt=numberDays;
		temp=1970;			//��1970�꿪ʼ
		while(numberDays>=365)
		{				 
			if(LeapYear(temp))//������
			{
				if(numberDays>=366)
					numberDays-=366;//�����������
				else 
				{
					temp++;
					break;
				}  
			}
			else 
				numberDays-=365;	  //ƽ�� 
			temp++;  
		}   
		Calendar.Year=temp;//�õ����
		temp=0;
		while(numberDays>=28)//������һ����
		{
			if(LeapYear(Calendar.Year)&&temp==1)//�����ǲ�������/2�·�
			{
				if(numberDays>=29)
					numberDays-=29;//�����������
				else 
					break; 
			}
			else 
			{
				if(numberDays>=MonthTable[temp])
					numberDays-=MonthTable[temp];//ƽ��
				else 
					break;
			}
			temp++;  
		}
		Calendar.Month=temp+1;	//�õ��·�
		Calendar.Date=numberDays+1;  	//�õ����� 
	}
	numberDays=timeCount%86400;		//�õ�������   	   
	Calendar.Hour=numberDays/3600; 	//Сʱ
	Calendar.Min=(numberDays%3600)/60; 	//����	
	Calendar.Sec=(numberDays%3600)%60; 	//����
	Calendar.Week=RTCGetWeek(&Calendar);//��ȡ����   
	
	return 0;
}	
/*******************************************************************************************************************************
*����		��	RTCʱ�ӵĳ�ʼ��
*�������	��	��
*�������	��	����0����ʼ���ɹ�  ����ֵ��ʼ��ʧ��
*******************************************************************************************************************************/
u8 InitRTC(void)
{
	//����ǲ��ǵ�һ������ʱ��
	u8 temp=0;
  u16 tmp;
	TimeSetStruct timeSet;
	
	timeSet.Year=2017;
	timeSet.Month=06;
	timeSet.Day=01;
	timeSet.Hour=10;
	timeSet.Min=00;
	timeSet.Sec=00;
	tmp = BKP_ReadBackupRegister(BKP_DR1);
	if (BKP_ReadBackupRegister(BKP_DR1) != RTCInitRecord)//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{	 			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
		PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
		BKP_DeInit();					//��λ�������� 	
		RCC_LSEConfig(RCC_LSE_ON);		//�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			DelayMs(10);
            if(temp>=250)	return 1;//��ʼ��ʱ��ʧ��,����������	
		}    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();	//�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������	
		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTCTimeSet(&timeSet);  //����ʱ��	
		RTC_ExitConfigMode(); //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, RTCInitRecord);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	else//ϵͳ������ʱ
	{
		RTC_WaitForSynchro();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}
	RTCNVICConfig();//RCT�жϷ�������		    				     
	RTCGetTime();//����ʱ��	
	
	System.Device.DateSet.RTCDateSet=RTCTimeSet;
	System.Device.DateSet.RTCGetTime=RTCGetTime;
	return 0; //ok	
}
extern void AutoArmOrDisarm(void);
/*************************************************************************************

*************************************************************************************/
/*void RtcSystick100Routine(void) 
{
	static u8 min=0xFF;
	
	RTCGetTime();
	if(min!=Calendar.Min)
		min=Calendar.Min;
	AutoArmOrDisarm();
	//PostMessage(MessageTimerType,res); 
}*/
/**************************************************************************************
*����		��	RTCʱ���жϣ�ÿ�봥��һ�� 

**************************************************************************************/
void RTC_IRQHandler(void)
{	
	static u8 min=0xFF;	
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
	{							
		RTCGetTime();//����ʱ�� 
    PostMessage(MessageDateType,Calendar.Sec);    
		if(min!=Calendar.Min)
			min=Calendar.Min;
		AutoArmOrDisarm();		
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	  	   
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//�������ж�
	RTC_WaitForLastTask();	  	    						 	   	 
}

void DateSystick1000Routine(void)
{
	if(Calendar.Sec!=App.Data.Sec)
	{
		PostMessage(MessageDateType,Calendar.Sec);
	}
}

