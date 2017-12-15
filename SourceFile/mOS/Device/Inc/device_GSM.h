
#ifndef __DEVICE_GSM_H
#define __DEVICE_GSM_H

#include "sys.h"



#define SimCheck 0x01
#define CSQCheck 0x02
#define Operator 0x03

typedef enum
{
	GsmDTMFMessage		= 0xFF00,//DTMF解码
	GsmSMSMessage		= 0xFE00,//SMS通知
	GsmRingMessage		= 0xFD00,//来电
	GsmCallMessage		= 0xFC00,//通话状态
	GsmDateMessage		= 0xFB00,//日期
	GsmCSQMessage		= 0xFA00,//信号量
	GsmReadyMessage		= 0xF900,//GSM就绪
	GsmTSMSINFOMessage	= 0xF800,//附加信息，主报未读，信息满
	GsmSMSRead			= 0xF700,//收到的消息且已读取出来了
	GsmSMSContent		= 0xF600,
	GsmSMSSendOK		= 0xF500,
  GsmMessageFull      = 0xF400,
  GsmMessageCEND      = 0xF300,
  GsmMessageCONN      = 0xF200,
	
	GsmNotMessage		= 0xF100,//不需要处理信息
  GsmMessageOperator  = 0xEF00, 
	GsmBusy				= 0xEE00,
	GsmSimCardQuery		= 0xED00,
	GsmSingQuery		= 0xEC00,
	GsmOperatorQuery	= 0xEB00,
	MessageContactID	= 0xEA00,
  
  GSMSimRemoved      = 0xE900,
  GsmDiscovery       = 0xE800,
  
  GsmNormalMessage   = 0xE700,
	
}GsmMessageEnum;

//当前通话的状态
typedef enum
{
	GsmCallStopOr1	= 0,//当前通话终止，至少还存在一个被保持的通话
	GsmActiveCall	= 1,//开始主动拨打电话
	GsmCallFailure	= 2,//呼叫失败
	GsmCallRing		= 3,//主动拨打电话但对方正在振铃
	GsmIntoCallWait	= 4,//通话被转入呼叫等待
	GsmCallConnect	= 5,//主动呼叫接通
	GsmCallStop		= 6,//通话完全终止
	GsmCallHangUp	= 7,//通话被挂起
	GsmEmergencyCall= 8,//在紧急模式下拨打电话，非紧急号码
	GsmCallNoRes	= 9,//主动拨打电话，对方没反应
	GsmCallBusy		= 10,//主动电话对方忙
}GsmCallStateEnum;
typedef enum
{
	Activation=0xFF,
	StayIn=0xFE,
	Dialing=0xFD,
	Ringing=0xFC,
	Waiting=0xFB,
}GsmCallEnum;
#define GsmPower		PBout(1)//电源开关


//#define SimcomReset		PCout(9)

#endif

