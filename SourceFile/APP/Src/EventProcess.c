
#include "EventProcess.h"


#define DtmfArm       0x31 //'1' 布防
#define DtmfDisarm    0x32 //'2' 撤防
#define DtmfMonitor   0x33 //'3' 监听
#define DtmfTalkBack  0x34 //'4' 对讲
#define DtmfHangUp    0x2A //'*' 挂机

#define HangUpTime    21000//自动挂机时间

extern void DelayArmProcess(void);
extern u8 GetPhoneTypeTotal(u8 phoneType);
extern void Simcom800CInit(void);
extern void EnglishConvert(u8 *content,u8 *resPointer);
static u8   passBuffer[6];
char    numberBuffer[30];
/**************************************************************************************************
*
*描述       ：电话机挂机，在定时器中执行
*输入参数   ：无
*输出参数   ：无
*
**************************************************************************************************/
void AutoHangUpTime(void)
{
  App.Data.CallRing=0;
  App.Data.CallRingConut=0;
//  MicPowerCtrl=1;
//  System.Device.Gsm.GsmATSend("AT+CHUP","OK",10);
//  System.Device.Gsm.GsmATSend("AT+CTXVOL=0","OK",2); 
  CD4053Sa = 0;
  CD4053Sb = 0;
  CD4053Sb = 0;
  System.Device.Gsm.GsmATSend("ATH","OK",10);
	memset(passBuffer,0x00,6);
//	CallConnect=0;
//	HT9170DWDN=1;
  App.Data.Calltime5s=1;
  TIM_Cmd(TIM5,ENABLE);
}
/**************************************************************************************************
*
*描述       ：获取号码序号
*输入参数   ：count,第N次拨号;type,要获取的号码类型
*输出参数   ：0，没有号码，其他值是号码序号
*
**************************************************************************************************/
u8 GetNumber(u8 count,u8 type)
{
    u8  i;
    u8  number=0;
    u32 addr;
    u8  numberB[30];
    
    for(i=0;i<8;i++)
    {
        addr=PhoneMember1Addr+(30*i);
        System.Device.Storage.ReadData(addr,numberB,30);
        if(strstr((char*)numberB,"Empty")==NULL)
        {
            addr=PhoneName1Addr+(20*i);
            System.Device.Storage.ReadData(addr,numberB,20);
            if(type==MessageType)
            {
                if(numberB[16])
                {
                    number++;
                    if(number==count) return i+1;
                }
            }
            else if(type==CallType)
            {
                if(numberB[14])
                {
                    number++;
                    if(number==count) return i+1;
                }               
            }
			else if(type==CIDType)
            {
                if(numberB[18])
                {
                    number++;
                    if(number==count) return i+1;
                }               
            }
            memset(numberB,0x00,30);
        }
    }        
    return number;   
}
void test3(u8 state,u8 zone)
{
	char data[16];
	u8	 i,res,checkVal;
	char sendData[64];
	
	System.Device.Storage.ReadData(CIDUserCodeAddr,(u8*)data,4);
	data[0]=data[0]-0x30;
	data[1]=data[1]-0x30;
	data[2]=data[2]-0x30;
	data[3]=data[3]-0x30;
	
	data[4]=1;
	data[5]=8;
	switch(state)
	{
		case SystemStayMode:
			data[6]=3;
			data[7]=4;
			data[8]=5;
			data[9]=6;
			break;
		case SystemDisarmMode:
			data[6]=1;
			data[7]=4;
			data[8]=0;
			data[9]=1;			
			break;
		case SystemArmMode:
			data[6]=3;
			data[7]=4;
			data[8]=0;
			data[9]=1;			
			break;
		case SystemAlarmMode:
			data[6]=1;
			data[7]=1;
			data[8]=3;
			data[9]=0;	
			break;
	}
	data[10]=0;
	data[11]=1;
	
	data[12]=0;
	data[13]=zone/10;
	data[14]=zone%10;
	
	res=0;
	for(i=0;i<15;i++)
	{
		if(data[i]==0)
		    res +=10;
		else
			res +=data[i];
	}
	checkVal=(res+15)/15;
	data[15]=checkVal*15-res;
	memset((char*)sendData,0,64);
	sprintf((char*)sendData,"AT+CLDTMF=1,\"%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1d,%1X\",100",\
		   data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15]);
	
	System.Device.Gsm.GsmATSend((u8*)sendData,(u8*)"OK",100);
}
/**************************************************************************************************
*
*描述       ：电话消息发送
*输入参数   ：无
*输出参数   ：无
*
**************************************************************************************************/
void CallMessageSystick100Routine(void)
{
    if(App.Data.CallMessage)
    {
        PostMessage(MessageCall, App.Data.SystemMode); 
        App.Data.CallMessage=0;
      RTT_printf(0,"PostMessage:Call\r\n");
    }
}
void CidMessageSystick100Routine(void)
{
    if(App.Data.CallData.CallCidMessage)
    {
        PostMessage(MessageCID, App.Data.SystemMode); 
        App.Data.CallData.CallCidMessage=0;
      RTT_printf(0,"PostMessage:CallCid\r\n");
    }
}
/********************************************************************************************************
*
*描述		：拨号报警
*输入参数	：无
*输出参数	：无
*
********************************************************************************************************/
void CallAlarming(void)
{
	u8      number;
	u32     addr;
	
	memset(numberBuffer,0,30);
	App.Data.OriginateCount++;
	number=GetNumber(App.Data.OriginateCount,CallType);
	if(0==number)
	{
		App.Data.CidState.CidEnd=0;
		return;
	}
	addr=PhoneMember1Addr+(30*(number-1));
	System.Device.Storage.Parameter.StorageReadData(addr,(u8*)numberBuffer,30); 
//	if(LineCall)
//	{
//		CallConnect=1;
//		HT9170DWDN=0;
//		System.Device.Telephone.Call((u8*)numberBuffer);
//		System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime);
//	}
//	else
	{
    RTT_printf(0,"CallAlarming\r\n");
		System.Device.Gsm.GsmATSend("ATH","OK",10);
		DelayMs(10);
    System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);
    System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);
		System.Device.Gsm.GsmCall(numberBuffer); 
    App.Data.GsmState = 0;
		//CloseSpeaker();
	}	
}
void CidUpload(void)
{
	u8 number;
	u32 addr;
	
	App.Data.OriginateCount++;
	number=GetNumber(App.Data.OriginateCount,CIDType);
	if(!number) return;
	App.Data.CidState.CidStart=1;
	App.Data.CidState.CidEnd=0;
	addr=PhoneMember1Addr+(30*(number-1));
	System.Device.Storage.Parameter.StorageReadData(addr,(u8*)numberBuffer,30); 
	if(LineCall)
	{
		CallConnect=1;
		HT9170DWDN=0;
		System.Device.Telephone.Call((u8*)numberBuffer);
		System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime);
	}
	else
	{
		System.Device.Gsm.GsmATSend("ATH","OK",10);
		DelayMs(10);
		System.Device.Gsm.GsmCall(numberBuffer); 
//		CloseSpeaker();
	}	
}
void CIDProProcess(u8 data)
{
	App.Data.CallData.CallCidTotal=GetPhoneTypeTotal(CIDType); 
	if(App.Data.CallData.CallCidTotal>App.Data.OriginateCount)
	{
		CidUpload();
		//System.Device.Timer.Start(DelayProcessID,TimerMessageHandle,2000,CidUpload);
	}	
}
/**************************************************************************************************
*
*描述       ：拨打电话处理函数
*输入参数   ：data,来自消息的数据
*输出参数   ：无
*
**************************************************************************************************/
void CallProcess(u32 data)
{
    u8      state;
    
    state=Byte0(data);
    if(state==SystemAlarmMode)
    {
		App.Data.CallTypeTotal=GetPhoneTypeTotal(CallType); 
        if(App.Data.CallTypeTotal>App.Data.OriginateCount)
        {
			CallAlarming();
			//System.Device.Timer.Start(SignalQuality,TimerMessageHandle,2000,CallAlarming);
        }
    } 		
}
/**************************************************************************************************
*
*描述       ：短信处理
*输入参数   ：data,来自消息的数据
*输出参数   ：无
*
**************************************************************************************************/
void MessageProcess(u32 data)
{
  char  number[30];
	u8		numberUCS2[128];
  u8    ucs2BigString[128];
  u8    numberIndex;
  u32   addr;
	u8		state=0;
	
  App.Data.MessageTypeTotal=GetPhoneTypeTotal(MessageType);   
  if(App.Data.MessageTypeTotal>App.Data.SendCount)
  {
    if(0!=System.Device.Gsm.GsmATSend("AT+CPIN?","+CPIN: READY",200))return;
    App.Data.SendCount++;
    numberIndex=GetNumber(App.Data.SendCount,MessageType);
    if(0==numberIndex)return;
    addr=PhoneMember1Addr+(30*(numberIndex-1));
    memset(ucs2BigString,0,128);
    memset(numberUCS2,0,128);        
    System.Device.Storage.ReadData(addr,(u8*)number,30); 
		EnglishConvert((u8*)number,numberUCS2);		
		System.Device.Gsm.GsmATSend("AT+CNMI=2,2,0,0,0","OK",50);
		System.Device.Gsm.GsmATSend("AT+CSMP=17,168,0,8","OK",20);
		EnglishConvert(App.Data.MessageString,(u8*)ucs2BigString);
    System.Device.Gsm.GsmSendSMS((u8*)numberUCS2,(u8*)ucs2BigString);        
  }
}
void SMSCountProcess(void)
{
	u8 state=0;

	if(App.Data.MessageTypeTotal>App.Data.SendCount)
		App.Data.SendMessage=1;
	else if(App.Data.MessageTypeTotal==App.Data.SendCount)
	{
		if(App.Data.CallData.CallCidTotal)
		{
			switch(App.Data.SystemMode)
			{
				case SystemStayMode:
					System.Device.Storage.Parameter.StorageReadData(CIDStayUploadAddr,&state,1);
					break;
				case SystemArmMode:
					System.Device.Storage.Parameter.StorageReadData(CIDArmUploadAddr,&state,1);
					break;
				case SystemDisarmMode:
					System.Device.Storage.Parameter.StorageReadData(CIDDisarmUploadAddr,&state,1);
					break;
				case SystemAlarmMode:
					state=1;
					break;
			}
			if(state)
			{
				App.Data.CallData.CallCidMessage=ENABLE;
				App.Data.OriginateCalltimes=1;	
				App.Data.CallData.CallCidMessage=1;
			}
		}			
		else if((App.Data.CallTypeTotal)&&(SystemAlarmMode==App.Data.SystemMode))
		{
			App.Data.OriginateCalltimes=1;	
			App.Data.CallMessage=1;	
		}
	}
}
/****************************************************************************************
*
*描述     ：对DTMF 进行解码
*输入参数 ：data ,DTMF的值
*输出参数 ：无
****************************************************************************************/
void DtmfProcess(u32 data)
{
  u8          number;
  u8          buffer[6];
  u8          state,i;
	u8			stringPointer[30];
 
	memset(stringPointer,0,30);
  if(App.Data.CallRing==1)//只有外总电话进来时才进行解码
  { 
		number=Byte0(data);
		passBuffer[App.Data.DtmfCount]=number;
		System.Device.Gsm.GsmATSend("AT+CLVL=3","OK",10);   
		System.Device.Gsm.GsmATSend("AT+DTAM=1","OK",10); 
		System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=0","OK",10);//打开GSM mic通道
    CD4053Sb = 1;
    CD4053Sc = 1;//关闭mic采集
    CD4053Sa = 1;//switch voice to GSM mic channel
    if((number>0x2F)&&(number<0x3A))
    {
      SendThreeLines(number-0x2F);   
    }      
//		if(CallConnect==1)
//			sprintf((char*)stringPointer,"AT+CCMXPLAYWAV=\"%d.wav\",2",passBuffer[App.Data.DtmfCount]-0x30);
//		else
//      CD4053Sa = 1;
//      SendThreeLines(number);
//			sprintf((char*)stringPointer,"AT+CCMXPLAYWAV=\"%d.wav\",1",passBuffer[App.Data.DtmfCount]-0x30);
//		System.Device.Gsm.GsmATSend(stringPointer,"OK",20);
		App.Data.DtmfCount++;
    if(6==App.Data.DtmfCount)
    {
      System.Device.Storage.Parameter.StorageReadData(AdminPasswordAddr,buffer,6);
        //if(0==strcmp((const char*)passBuffer,(const char*)buffer)) 不知道为什么用这个函数比对相同结果都是错的
      for(i=0;i<6;i++)
      {
        if(passBuffer[i]!=buffer[i])
        {
//          System.Device.Gsm.GsmATSend("AT+CCMXSTOPWAV","OK",1);
//          System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=1","OK",2); 
          //System.Device.Gsm.GsmATSend("AT+CSDVC=3","OK",2);
          //System.Device.Gsm.GsmATSend("AT+CLVL=3","OK",50);   
          //System.Device.Gsm.GsmATSend("AT+CTXVOL=65535","OK",2); 
//          if(CallConnect==1)
//            System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"73.wav\",2","OK",10);
//          else
//            System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"73.wav\",1","OK",10);
          
          break;
        }
      }
      if(i==6)
			{
//				System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=1","OK",2);
//				System.Device.Gsm.GsmATSend("AT+CCMXSTOPWAV","OK",10);
//				if(CallConnect==1)
//					System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"64.wav\",2","OK",10);
//				else
//					System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"66.wav\",1","OK",10);
        //密码正确语音
        RTT_printf(0,"Remote Control:Right password\r\n");
        App.Data.CallRing=2; 
			}
      else
      {
        memset(passBuffer,0,6);
        //密码错误语音+请输入密码
        RTT_printf(0,"Remote Control:err password,Please enter password\r\n");
      }
      App.Data.DtmfCount=0;            
    }
    System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime); 
  }
  else if(App.Data.CallRing==2)//密码校验成功了
  {
    number=Byte0(data);
    switch(number)
    {
      case DtmfArm:
        RTT_printf(0,"Remote Control:Arm\r\n");
        CD4053Sa = 1;
        CD4053Sb = 1;
        CD4053Sc = 1;
        System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=0","OK",2);
        System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);
        SendThreeLines(12);
        state=SystemArmMode;
        System.Device.Storage.Parameter.StorageWriteData(SystemModeAddr,(u16*)&state,1);
        LogWrite(LogArmAddr,LogArmTotalAddr,4,LogArm,0xFF);
        System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime); 
  //				if(CallConnect==1)
  //					System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"57.wav\",2","OK",10);
  //				else
  //					System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"57.wav\",1","OK",10);
        break;
      case DtmfDisarm:
        CD4053Sa = 1;
        CD4053Sb = 1;
        CD4053Sc = 1;
        System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=0","OK",2);
        System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);
        SendThreeLines(13);
        RTT_printf(0,"Remote Control:Disarm\r\n");
        state=SystemDisarmMode;
        System.Device.Storage.Parameter.StorageWriteData(SystemModeAddr,(u16*)&state,1);
        LogWrite(LogDisarmAddr,LogDisarmTotalAddr,4,LogDisarm,0xFF);
        System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime); 
//				if(CallConnect==1)
//					System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"58.wav\",2","OK",10);
//				else
//					System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"58.wav\",1","OK",10);		
  
      break;
      case DtmfMonitor:
        RTT_printf(0,"Remote Control:Monitor\r\n");
        CD4053Sb = 0;
        CD4053Sc = 0;
        System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=0","OK",2);
//                CloseSpeaker(); 
//        System.Device.Gsm.GsmATSend("AT+CHFA=1","OK",10);
        System.Device.Gsm.GsmATSend("AT+CMIC=15","OK",10);
//        System.Device.Gsm.GsmATSend("AT+CHFA=0","OK",10);
        System.Device.Gsm.GsmATSend("AT+CMIC=15","OK",10);
        System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);
//        System.Device.Gsm.GsmATSend("AT+CHFA=0","OK",10);
//        System.Device.Gsm.GsmATSend("AT+CLVL=100","OK",10);
//                System.Device.Gsm.GsmATSend("AT+CTXVOL=65535","OK",2);
//				System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=0","OK",2); 
        System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime); 
      break;
      case DtmfHangUp:
        RTT_printf(0,"Remote Control:Hang UP\r\n");
        AutoHangUpTime();
//        App.Data.CallRing=0;
//        MicPowerCtrl=1;
//        System.Device.Gsm.GsmATSend("ATH","OK",10);

//        CallConnect=0;
//        App.Data.Calltime5s=1;
//        System.Device.Timer.Stop(CallHangUpTime); 
//        TIM_Cmd(TIM5,ENABLE);
      break;
      case DtmfTalkBack:
        CD4053Sb = 0;
        CD4053Sc = 0;
        System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=0","OK",2); 
        RTT_printf(0,"Remote Control:TalkBack\r\n");
        System.Device.Gsm.GsmATSend("AT+CMIC=15","OK",10);
        System.Device.Gsm.GsmATSend("AT+CLVL=100","OK",10);
            //System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=1","OK",2); 
       System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime); 
       break;
    }
  }
  App.Data.GsmState=0;
}
/**************************************************************************************
*
*描述     ：按键布防
*输入参数 ：无
*输出参数 ：无
*
**************************************************************************************/
void KeyArmProcess(void)
{
    u32 time;
    u8  state=SystemArmMode;

    System.Device.Storage.ReadData(ArmDelayAddr,&App.Data.ArmDelay,1);
    sprintf((char*)App.Data.MessageString,"Arm");
    App.Data.SendMessage=ENABLE;
    App.Data.SendCount=0;
    if(App.Data.ArmDelay>0)
    {
        if(App.Data.DelayArmFlag)
        {
            App.Data.DelayArmFlag=0;
            System.Device.Timer.Stop(ArmTimeID);
            System.Device.Storage.WriteData(SystemModeAddr,(u16*)&state,1);
        }
        else
        {
            time=App.Data.ArmDelay*1000;
            System.Device.Timer.Start(ArmTimeID,TimerMessageHandle,time,DelayArmProcess); 
            App.Data.DelayArmFlag= ENABLE;            
        }
    }
    else
        DelayArmProcess();
}

/**************************************************************************************************
*
*描述       ：运营商解析
*输入参数   ：无
*输出参数   ：无
*
**************************************************************************************************/
void OperatorParse(void)
{
  u8 i; 
  App.Data.OperatorUpdate=1;
	App.Data.GsmSMSInfo++;
	for(i=0;i<10;i++)
  {
		App.Data.OperatorName[i]=*(App.Data.GsmSMSInfo++);   
  }    
}
/**************************************************************************************************
*
*描述       ：信号质量解析
*输入参数   ：无
*输出参数   ：无
*
**************************************************************************************************/
void SingParse(void)
{
	u8 temp;
	u8 *strPointer;
	strPointer=App.Data.GsmSMSInfo;
	strPointer++;
	temp=*strPointer-0x30;
	strPointer++;
	if((*strPointer>=0)&&(*strPointer<=0x39))
	{
		temp*=10;
		temp+=(*strPointer-0x30);
	}
	App.Data.GsmCSQ=temp;
	memset(App.Data.GsmSMSInfo,0x00,128);
}

void AlarmCallCount(void)
{
	if(App.Data.SystemMode==SystemAlarmMode)
	{
		if(AlarmTimes>App.Data.OriginateCalltimes)
		{
			if(App.Data.CallTypeTotal==App.Data.OriginateCount)
			{
				App.Data.OriginateCount=0;
				App.Data.OriginateCalltimes++;					
			}
			App.Data.CallMessage=1;
		}
		else
		{
			if(App.Data.CallTypeTotal>App.Data.OriginateCount)
				App.Data.CallMessage=1;
			else
			{
				App.Data.OriginateCalltimes=0;
				App.Data.OriginateCount=0;
				App.Data.CallMessage=0;
			}
		}
			
	}
}

void CidDataProcess(u8 data)
{
	u8 state=0;
	
	if(data==1)
		state=9;
	else if(data==2)
		state=7;
		
}
/**************************************************************************************************
*
*描述       ：电话机振铃记数和摘机
*输入参数   ：data,来自消息的数据
*输出参数   ：无
*
**************************************************************************************************/
void CallRingProcess(u32 data)
{

    App.Data.CallRingConut++;;
    System.Device.Storage.ReadData(RingTimesAddr,&App.Data.RingTimes,1);
    if((App.Data.CallRingConut==App.Data.RingTimes)&&(CallConnect==0))
    {
        App.Data.CallRingConut=0;
        CallConnect=1;
		HT9170DWDN=0;
        App.Data.CallRing=1;
        TIM_Cmd(TIM4, DISABLE);
        App.Data.Calltime5s=0;
        App.Data.FrequencyTotal=0;
        App.Data.FrequencyCount=0;
        TIM_Cmd(TIM5,DISABLE );
        System.Device.Timer.Start(CallHangUpTime,TimerMessageHandle,HangUpTime,AutoHangUpTime); 
    }
}




