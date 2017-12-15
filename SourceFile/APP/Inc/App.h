#ifndef __APP_H
#define __APP_H

#include "system.h"
#include "TaskLogic.h"
#include "TaskMenu.h"

#define RemoteAlarm             0xFD
#define DetectorAlarm           0xFE

#define AlarmTimes              0x03

#define GsmReset				0xFD	

#define NullRemote				0x32
#define SystemSoundOff			0x00
#define SystemSoundOn			0x01

#define SystemSirenOn			0x37
#define SystemSirenOff			0x36
#define SystemZoneClose			0x30
#define SystemZoneAlarm			0x31
#define SystemZoneDelayAlarm	0x32
#define SystemZoneHomeAlarm		0x33
#define SystemZoneSOS			0x34
#define SystemZoneDoorbell		0x35


#define SystemArmMode				0x01
#define SystemDisarmMode			0x02
#define SystemStayMode				0x03
#define SystemAlarmMode				0x04
#define SystemCodeMode				0x05
#define SystemDoorbellMode          0x06

#define RxRemote					0xFE
#define RxDetector					0xFD

#define MessageType         		0xFE
#define CallType            		0xFD
#define CIDType						0xFC


#define	SendDisarmData				0x02
#define SendArmData					0x04
#define SendAlarmData				0x08

//日志布撤防状态
#define LogDisarm           1
#define LogArm              2
#define LogStay             3
#define LogAlarmZone        4
#define LogAlarmRemote      5
#define LogKeypad           6
#define TransparentText		GUI_TEXTMODE_TRANS

#define MoveBarColor		0xF960
#define TopLabelColor		0x0440

extern const string 	WeekStringAuto[1][7];
extern ListboxList * ListSearch(ListboxList *listx,u16 index);
extern void LogWrite(u32 logAddr,u32 indexAddr,u8 type,u8 state,u8 zone);
typedef  struct MenuItem 
{
	u8    		RangeFrom;//开始序号
	u8    		RangeEnd; //结束序号
	u8    		ItemCount;//菜单在当前层的菜单项总数
  u8        lastSelected;
	u8			  curSelected;//当前选择的项
	Form		  *MenuItems[8];
	void		  (*ProcessFunction)();
	void		  (*ProcessSaveFun)();
	void		  (*FormProcess)(Form *formPointer);
	void		  (*FormAddrfunc)(Form *formPointer);
	void		  (*ProcessFunc)();
	void		  (*ProcessReturn)();
	struct MenuItem  **SubMenus;//子菜单
	struct MenuItem  *Parent;//上级菜单
  u8        icoUpdata;
	 
}MenuListStruct;


extern MenuListStruct *CurrentMenu;
extern MenuListStruct *CurrentCall;


typedef  struct
{
	DataStruct 		Data;
	MenuStruct 		Menu; 
}AppStruct;
extern AppStruct App;
#endif



