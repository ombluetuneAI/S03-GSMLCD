/*--------------------------------------------头文件-------------------------------------*/
#include "Device_RTC.h"
#include "system.h"
#include "App.h"
/*---------------------------------------------常数--------------------------------------*/

/*--------------------------------------------宏定义-------------------------------------*/
#define RTCInitRecord	0x6107	//初始化后写入该值表示RTC初始化过了(该值可随意写 <0xffff)

/*---------------------------------------------变量--------------------------------------*/
CalendarStruct Calendar;

//月份数据表
u8 const MonthCorrectionTable[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  

//平年的月份日期表
const u8 MonthTable[12]={31,28,31,30,31,30,31,31,30,31,30,31};
/*---------------------------------------------类型--------------------------------------*/



/*---------------------------------------------函数--------------------------------------*/

/*****************************************************************************************************************************
*描述		：	判断是否是闰年函数
				月份   1  2  3  4  5  6  7  8  9  10 11 12
				闰年   31 29 31 30 31 30 31 31 30 31 30 31
				非闰年 31 28 31 30 31 30 31 31 30 31 30 31
*输入参数	:	year,年份
*输出参数	:	该年份是不是闰年：
				1,是.0,不是

*****************************************************************************************************************************/
u8 LeapYear(u16 year)
{
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
				return 1;//如果以00结尾,还要能被400整除 	   
			else 
				return 0;   
		}else 
			return 1;   
	}
	else 
		return 0;	
}

/******************************************************************************************************************************
*描述		：	设置时钟，把输入的时钟转换为秒钟，以1970年1月1日为基准，1970~2099年为合法年份
*输入参数	：	timeSet,时间列表
*输出参数	：	0,成功;
				其他:错误代码.

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
			secCount+=31622400;//闰年的秒钟数
		}
		else 
		{
			secCount+=31536000;	//平年的秒钟数
		}
	}
	timeSet->Month-=1;
	for(t=0;t<timeSet->Month;t++)//把前面月份的秒钟数相加
	{
		secCount+=(u32)MonthTable[t]*86400;//月份秒钟数相加
		if(LeapYear(timeSet->Year)&&t==1)
			secCount+=86400;			//闰年2月份增加一天的秒钟数	 		
	}
	secCount+=(u32)(timeSet->Day-1)*86400;//把前面日期的秒钟数相加 
	secCount+=(u32)timeSet->Hour*3600;//小时秒钟数
	secCount+=(u32)timeSet->Min*60;	 //分钟秒钟数
	secCount+=timeSet->Sec;//最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(secCount);	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	System.Device.DateSet.RTCDateSet=RTCTimeSet;
	return 0;	 
	
}
/*******************************************************************************************************************************
*描述		：	RCT中断分组设置
*输入参数	：	无
*输出参数	：	无
*******************************************************************************************************************************/
static void RTCNVICConfig(void)
{	
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}
/*******************************************************************************************************************************
*描述		：	获得现在是星期几，输入公历日期得到星期(只允许1901-2099年)
*输入参数	：	公历年月日 
*输出参数	：	星期号	

*******************************************************************************************************************************/
																					 
static u8 RTCGetWeek(CalendarStruct *date)
{	
	u16 temp;
	u8 yearH,yearL;
	
	yearH=date->Year/100;	
	yearL=date->Year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp=yearL+yearL/4;
	temp=temp%7; 
	temp=temp+date->Date+MonthCorrectionTable[date->Month-1];
	if (yearL%4==0&&date->Month<3)temp--;
	return(temp%7);
}			  


/**************************************************************************************************
*描述		：	得到当前的时间
*输入参数	：	无
*输出参数	：	0,成功;其他:错误代码.
**************************************************************************************************/
u8 RTCGetTime(void)
{
	static u16 daycnt=0;
	u32 timeCount=0; 
	u32 numberDays=0;
	u16 temp=0;	  
    timeCount=RTC_GetCounter();	 
 	numberDays=timeCount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=numberDays)//超过一天了
	{	  
		daycnt=numberDays;
		temp=1970;			//从1970年开始
		while(numberDays>=365)
		{				 
			if(LeapYear(temp))//是闰年
			{
				if(numberDays>=366)
					numberDays-=366;//闰年的秒钟数
				else 
				{
					temp++;
					break;
				}  
			}
			else 
				numberDays-=365;	  //平年 
			temp++;  
		}   
		Calendar.Year=temp;//得到年份
		temp=0;
		while(numberDays>=28)//超过了一个月
		{
			if(LeapYear(Calendar.Year)&&temp==1)//当年是不是闰年/2月份
			{
				if(numberDays>=29)
					numberDays-=29;//闰年的秒钟数
				else 
					break; 
			}
			else 
			{
				if(numberDays>=MonthTable[temp])
					numberDays-=MonthTable[temp];//平年
				else 
					break;
			}
			temp++;  
		}
		Calendar.Month=temp+1;	//得到月份
		Calendar.Date=numberDays+1;  	//得到日期 
	}
	numberDays=timeCount%86400;		//得到秒钟数   	   
	Calendar.Hour=numberDays/3600; 	//小时
	Calendar.Min=(numberDays%3600)/60; 	//分钟	
	Calendar.Sec=(numberDays%3600)%60; 	//秒钟
	Calendar.Week=RTCGetWeek(&Calendar);//获取星期   
	
	return 0;
}	
/*******************************************************************************************************************************
*描述		：	RTC时钟的初始化
*输入参数	：	无
*输出参数	：	返回0，初始化成功  其他值初始化失败
*******************************************************************************************************************************/
u8 InitRTC(void)
{
	//检查是不是第一次配置时钟
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
	if (BKP_ReadBackupRegister(BKP_DR1) != RTCInitRecord)//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	 			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
		BKP_DeInit();					//复位备份区域 	
		RCC_LSEConfig(RCC_LSE_ON);		//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			DelayMs(10);
            if(temp>=250)	return 1;//初始化时钟失败,晶振有问题	
		}    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();	//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTCTimeSet(&timeSet);  //设置时间	
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, RTCInitRecord);	//向指定的后备寄存器中写入用户程序数据
	}
	else//系统继续计时
	{
		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	}
	RTCNVICConfig();//RCT中断分组设置		    				     
	RTCGetTime();//更新时间	
	
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
*描述		：	RTC时钟中断，每秒触发一次 

**************************************************************************************/
void RTC_IRQHandler(void)
{	
	static u8 min=0xFF;	
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{							
		RTCGetTime();//更新时间 
    PostMessage(MessageDateType,Calendar.Sec);    
		if(min!=Calendar.Min)
			min=Calendar.Min;
		AutoArmOrDisarm();		
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	   
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}

void DateSystick1000Routine(void)
{
	if(Calendar.Sec!=App.Data.Sec)
	{
		PostMessage(MessageDateType,Calendar.Sec);
	}
}

