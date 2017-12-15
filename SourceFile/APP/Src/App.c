
/*--------------------------------------------头文件-------------------------------------*/


#include "App.h"

/*---------------------------------------------常数--------------------------------------*/

/*--------------------------------------------宏定义-------------------------------------*/


/*---------------------------------------------变量--------------------------------------*/
AppStruct App;
MenuListStruct *CurrentMenu;
MenuListStruct *CurrentCall;


/*---------------------------------------------类型--------------------------------------*/
int main(void)
{
	AppDataPointer = &(App.Data);
	App.Data.RxDis=0;
	App.Data.SystemModeUpdate=0xff;
	App.Data.AlarmDelay=3; 
	App.Data.AlarmVolume=5;
	App.Data.ArmDelay=1;
	App.Data.GsmState=0;
	App.Data.CallRingEnable=0;
	App.Data.AlarmDelay=0;
	App.Data.RingTime=0;   
	App.Data.GsmRxdCount=0;
	App.Data.GsmSMSInfo=NULL;
	App.Data.SystemState=2;
	App.Data.ArmDelay=66;
	App.Data.KeyCount=0;
	App.Data.UseWireless=0;
	App.Data.CodeWireless=0;
	App.Data.SendMessage=0;
	App.Data.CallMessage=0;
	App.Data.SendCount=0;
	App.Data.OriginateCalltimes=0;
	App.Data.Sec=0;
	App.Data.Min=0;
	App.Data.Hour=50;
	App.Data.CallTypeTotal=0;
	App.Data.OriginateCount=0;
	App.Data.DelayArmFlag=0;
	App.Data.CallRing=0;
	App.Data.DtmfCount=0;
	AppDataPointer->CallRingEnable=0;
	AppDataPointer->AutoFocus=0;
	AppDataPointer->ArCheckUpdate=0;
	App.Data.OperatorUpdate=0;
	App.Data.GsmSimCardState=0;
	App.Data.CidState.CidDialSend=0;
	
	App.Data.CidState.CidStart=0;
	App.Data.CidState.CidEnd=1;
	App.Data.CidState.CidNumTotal=0;

	App.Data.Day=0;
	App.Data.Month=0;
	App.Data.Year=0;
	App.Data.Week=0;
	App.Data.Calltime5s=1;
	
	App.Data.CallData.CallCidTotal=90;
	App.Data.CallData.CallConnectCount=0;
	App.Data.RecTimeValue=0xFF;
	App.Data.RecordKey=0;
	App.Data.FrequencyTotal=0;
	App.Data.FrequencyCount=0;
	App.Data.AlarmDelay=0;

	memset(App.Data.RestoreDefault,1,4);

	memset(App.Data.KeyBuffer1,0,30);
	memset(App.Data.KeyBuffer2,0,30);
	memset(App.Data.OperatorName,0,10);
  memset(App.Data.IMEINum,0,15);

	System.Init(); 
	
  #if Open_RTT_printf
  RTT_printf(0,"System Init OK\r\n");
  
  #endif
	System.OS.CreateLogicTask(LogicTask);   
	System.OS.CreateMenuTask(MenuTask);

	System.OS.Start();
}

