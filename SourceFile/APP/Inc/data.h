#ifndef __DATA_H_
#define __DATA_H_

#define DcDetPort	PAin(10)

#include "sys.h"
typedef __packed struct
{
	u8 CidStart		:1;
	u8 CidEnd  		:1;
	u8 CidDialSend	:1;
	u8 CidNumTotal	:4;
	u8 CallStart	:1;
//	u8 Retained2	:1;
}CidBit;

typedef __packed struct
{
	u8 CallCidTotal;
	u8 CallCidMessage;
	u8 CallConnectCount;
}call;
typedef __packed struct
{
	float	Voltage;
	CidBit 	CidState;
	call	CallData;
	u8		SystemMode;//系统模式
	u8		UseWireless;
	u8		CodeWireless;
  u8    SystemModeUpdate;
	u8		RxDis;
	u8		SoundFlag;//是否启动遥控伴音
	u8		CallRing;//有电话进来
	u16 	GsmState;
	u16 	GsmRxdCount;
	u8 		DelayArmFlag;//延时报警启动
//	u32		GsmInfo;//信号强度0~31,第一字节为信号强度，每二字节每一位是初始化标志，第二位是注册网络
  u8    Calltime5s;
	u8		GsmCSQ;
	u8    GsmSimCardState;
	u8		RingTime;//自动接听振铃次数
	u8		*GsmSMSInfo;
  u8    IMEINum[15];
  u8    OperatorUpdate;
  u8    RecTimeValue;
	u8 		CallRingConut;
	u8		CallRingEnable;
	u8		RestoreDefault[4];
  u8    AlarmZone;
	u8		WirelessSiren;
  u8    AlarmSiren;
	u8    AlarmType;
	u8 		ArmDelay;
	u8		AlarmDelay;
	u8		KeyCount;
	u8		SrienTimes;
	u8		RingTimes;
	u8		KeyToneState;
  u8    OriginateCalltimes;
  u8    MessageString[30];
  u8    DtmfCount;
  u8    ArmKeypadRemote;
  u8    RecordKey;
  u8    PlayKey;
  u16   FrequencyTotal;//计算振铃频率的
  u16   FrequencyCount;
  u8    CallMessage;
  u8    SendMessage;
  u8    MessageTypeTotal;
  u8    CallTypeTotal;
  u8    OriginateCount;
  u8    SendCount;
  u8    AlarmNumber;
  u8    ArCheckUpdate;
  u8    OperatorName[10];
	u8		OperatorState;
	
	u8		SystemState;
	u8		PowerState;
	u8		ArmDisarmSMS;
	u8		KeyBuffer1[30];
	u8		KeyBuffer2[30];
	u8		PassBuffer[6];
	u32 	SerialNumbert; //SN
	u16 	Year;
	u8	 	Month;
	u8	 	Day;
	u8		Hour;
	u8		Min;
	u8		Sec;
  u8    Week;
	u8		VoiveVolume;
	u8		AlarmVolume;
  u8    AutoHour;
  u8    AutoMin;
  u32   AutoAddr;
  u8    AutoFocus;
	u16		SystemID;
	u16		UserCode;
	u32 	IdentifyNumber0;           //ID0
	u32 	IdentifyNumber1;           //ID1
	u32 	IdentifyNumber2;//ID2
	
	
}DataStruct;


#endif

