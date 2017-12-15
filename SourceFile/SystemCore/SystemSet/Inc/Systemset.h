#ifndef __SYSTEMSET_H
#define __SYSTEMSET_H

#include "stm32F10x.h"
#include "sys.h"
#include "System.h"

#define ArmState			1
#define DisarmState			2
#define StayState			3
#define	Doorbellstate		4

typedef enum
{
	SystemArm			=1,//布防
	SysteHome			=2,//在家布防
	SystemDisarm		=0,//撤防
	SystemMonitor		=3,//回拨监听
	SystemQuery			=6,//系统查询
	
	SystemPassWord		=24,//密码	
	SystemDate			=20,//时间设定	
	SystemDelay			=17,//延时
	SystemSiren			=22,//鸣警时间设定
	
	SystemVolume		=21,//系统音量
	SystemAuto			=18,//定时布撤防
	SystemAutoQuery		=19,
	SystemRemote		=13,//遥控属性
	SystemRemoteDel		=16,
	SystemZone			=12,//防区属性
	SystemRing			=23,//振铃次数
	SystemSMSAck		=31,//短信回复开关
	SystemUpload		=26,//布撤防上传中心
	SystemSMSReport		=28,//布撤防短信通知
	SystemLangrage		=35,//语言选择	
	
	SystemIllegalArm	=33,//非法布防
	SystemPhoneSet		=10,//设置电话
	SystemPhoneDel		=11,//查询电话
	SystemWirelessSiren	=15,//无线警号
	
}SystemEnum;



#endif

