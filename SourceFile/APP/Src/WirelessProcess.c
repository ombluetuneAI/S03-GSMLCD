
/*--------------------------------------------ͷ�ļ�-------------------------------------*/


#include "WirelessProcess.h"

/*---------------------------------------------����--------------------------------------*/

#define	NullValue			0x00
#define RemoteValue			0xFE
#define DetectorValue		0xFC

#define DetectorTypeTotal	18
/*--------------------------------------------�궨��-------------------------------------*/
#define  RemoteUnlockProcess()   UnlockProcess()

void SirenAlarm(void);
/*---------------------------------------------����--------------------------------------*/
//ң���б�
const u8 RemoteKeyValueTab[]=
{
	KeyUnlock1527,KeyAlarm1527,KeyAlawy1527,KeyHome1527,
	KeyAlawy2262,KeyHome2262,KeyUnLock2262,KeyAlarm2262
};
//̽ͷ�б�
const u8  DetectorValueTab[]=
{
	KeyDoorOpen1527,KeyHongWai1527,KeyYanGan1527,
	KeyQiGan1527,Key_Help1527,Key_pale1527,
	KeyDoorClose1527,

 	KeyAlawy2262,KeyHome2262,KeyUnLock2262,
 	KeyAlarm2262,KeyDoor2262,KeyYanGan2262,
 	KeyQiGan2262,KeyHongWai2262,KeyWindows2262,
 	KeyHelp2262,KeyPale2262,
};
extern void SendWirelessData(void);
/*---------------------------------------------����--------------------------------------*/
void AlarmSirenClose(void)
{
    CloseSiren();
}
/***************************************************************************************************

*����		�����ң�صĺϷ���
*�������	��keyVal���յ���ң�ؼ�ֵ
*�������	������0�����յ������ݲ��Ϸ�
*
**************************************************************************************************/
u8 RemoteValueCheck(u8 keyVal)
{
	u8 i;

	for(i=0;i<8;i++)
	{
		if(RemoteKeyValueTab[i]==keyVal)
		{
			return(1);
		}
	}
	return(0);
}
void SirenAlarm(void)
{
    u32 time;
    System.Device.Storage.ReadData(SirenDelayAddr,&App.Data.SrienTimes,1);
    if(App.Data.SrienTimes)
    {
        OpenSiren();
        time=App.Data.SrienTimes*45000;
        System.Device.Timer.Start(AlarmTime,TimerSystickHandle,time,AlarmSirenClose);
    }
}
/***************************************************************************************************

*����		�����̽ͷ�ĺϷ���
*�������	��DetectorVal���յ���̽ͷ������
*�������	������0�����յ������ݲ��Ϸ�
*
**************************************************************************************************/
u8 DetectorValueCheck(u8 DetectorVal)
{
	u8 i;

	for(i=0;i<DetectorTypeTotal;i++)
	{
		if(DetectorValueTab[i]==DetectorVal)
		{
			return(1);
		}
	}
	return(0);
}
/***************************************************************************************************

*����		������ң�ش洢��ַ
*�������	��keyVal���յ���ң�ؼ�ֵ
*�������	������0�����յ������ݲ��Ϸ�
*
**************************************************************************************************/
u32 *StorageAddr(void)
{
	MenuListStruct 	*currentMenu;
	u32				*addr;
	Form			*formPointer;
	ListboxList		*listx;
	
	currentMenu=CurrentMenu->Parent;
	currentMenu=currentMenu->Parent;
	
	formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
	listx=System.Gui.Form.ListSearch(formPointer->ListPointer->List,formPointer->ListPointer->SelectedIndexes);
	addr=listx->FlashAddr;
	
	return addr;
}

/*********************************************************************************************************************
*
*����		���������ֵ�ַ����������״̬
*�������	����
*�������	�����ص�ַָ��
*********************************************************************************************************************/
u32 *StorageNameAddr(void)
{
	MenuListStruct 	*currentMenu;
	u32				*addr;
	Form			*formPointer;
	ListboxList		*listx;
	
	currentMenu=CurrentMenu->Parent;
	currentMenu=currentMenu->Parent;
	
	formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
	listx=System.Gui.Form.ListSearch(formPointer->ListPointer->List,formPointer->ListPointer->SelectedIndexes);
	addr=listx->IconAdd;
	
	return addr;
}
/*********************************************************************************************************
*����		��	��ȡ������

*********************************************************************************************************/
u8 GetZoneNumber(void)
{
	MenuListStruct 	*currentMenu;
	Form			*formPointer;
	u8				res;
	
	currentMenu=CurrentMenu->Parent;
	currentMenu=currentMenu->Parent;	
	formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
	res=formPointer->ListPointer->SelectedIndexes;
	return res+1;
}
/***************************************************************************************************

*����		���ȶԿ��е�ң��
*�������	��bufferPointer,�������յ���ң�ص�ַ
*�������	��0����ID����û���ҵ��������������ǵ�ǰID�ڿ��е�������
*
**************************************************************************************************/
static u8 ComparisonRemoteID(u32 addr,u8 *bufferPointer,u16 total)
{
	u32 addrOffset;
	u16	i;
	u8 buffer[3];
	addrOffset=addr;
	for(i=0;i<total;i++)
	{
		addrOffset=addr+(i*4);
		System.Device.Storage.ReadData(addrOffset,buffer,3);
		if((buffer[0]==bufferPointer[0])&&(buffer[1]==bufferPointer[1]))
    {
			return (1+i);
    }
		memset(buffer,0,3);
	}
	return 0;
}
/***************************************************************************************************

*����		������������ID
*�������	��addr,ID���׵�ַ
			  addr,���׵�ַ
			  counr��ID������
*�������	��
*
**************************************************************************************************/
static u8 GetRemoteTotal(u32 addr,u8 total)
{
	u32 addrCount;
	u8	i,count=0;
	u8 buffer[3];
	
	for(i=0;i<total;i++)
	{
		addrCount=addr+(i*20);
		System.Device.Storage.ReadData(addrCount,buffer,3);
		if((buffer[0]!=0)||(buffer[1]!=0))
			count++;
		memset(buffer,0x00,3);
	}
	return count;
}
/*****************************************************************************************
*
*����		����ȡ̽ͷ����
*�������	��addr,��ʼ��ַ;total,������
*�������	������̽ͷ������
*
*****************************************************************************************/
static u8 GetDetectorTotal(u32 addr,u16 total)
{
	u32 addrCount;
	u16	i,count=0;
	u8 buffer[4];
	
	for(i=0;i<total;i++)
	{
		addrCount=addr+(i*4);
		System.Device.Storage.ReadData(addrCount,buffer,4);
		if(buffer[2]!=0)
    {
			count++;
    }
		memset(buffer,0x00,4);
	}
	return count;
}
/**************************************************************************************
*
*����		��	���ҿյ�ID��ַ������
*�������	��	total,������
*�������	��	����0����û�пյ��ˣ����򷵻ش洢��ַ
*
**************************************************************************************/
u32 DetectorAddrID(u16 total)
{
	u32 addr;
	u16	i;
	u8 buffer[4];

	for(i=0;i<total;i++)
	{
		addr=ZoneDetectorAddr+(i*4);
		memset(buffer,0x00,4);
		System.Device.Storage.ReadData(addr,buffer,4);
		if((buffer[2]==0)&&(buffer[1]==0)&&(buffer[0]==0))
    {
      return addr;
    }
	}	
	return 0;
}
/************************************************************************************
*
*����		:	�ȶ�̽ͷID
*�������	��	addr,ֱַ��ַ
				bufferPointer��Ҫ�ȶԵ�IDָ��
				total,�ܸ���
*�������	��	����0��û��һ����ID,����������������ͬID�ڿ��е����
*
************************************************************************************/
static u8 ComparisonDetectorID(u32 addr,u8 *bufferPointer,u16 total)
{
	u32 addrOffset;
	u16	i;
	u8 buffer[3];
	addrOffset=addr;
	for(i=0;i<total;i++)
	{
		addrOffset=addr+i*4;
		System.Device.Storage.ReadData(addrOffset,buffer,3);
		if((buffer[0]==bufferPointer[0])&&(buffer[1]==bufferPointer[1]))
    {
			return (1+i);
    }
		memset(buffer,0,3);
	}
	return 0;
}
/*******************************************************************************
*
*����       ����ʱ��������
*�������   ����
*�������   ����
*
*******************************************************************************/
void DelayAlarmProcess(void)
{
    LogWrite(LogAlarmAddr,LogAlarmTotalAddr,1,LogAlarmZone,App.Data.AlarmZone);
    App.Data.SystemMode=SystemAlarmMode;
    if(App.Data.MessageTypeTotal)
        App.Data.SendMessage=1;
    else if(App.Data.CallTypeTotal)
        App.Data.CallMessage=1;
    App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
    SirenAlarm();
}

/********************************************************************************
*
*����       ��̽ͷ��������
*�������   ��number,̽��ͷ�ڿ��еı��
*�������   ����
*
********************************************************************************/
static void AlarmProcess(u8 number)
{
	u32 addr;
    u8  buffer[20];
    u8  delayAlarm;
    u8  zoneType;
    u32	DelayCall=0;
    
    App.Data.OriginateCount=0;
    App.Data.SendCount=0;
//    App.Data.OriginateCalltimes=1;
	addr=ZoneDetectorAddr+((number-1)*10);
	System.Device.Storage.ReadData(addr,buffer,6);
    App.Data.AlarmType=DetectorAlarm;
    App.Data.AlarmZone=buffer[3];
    addr=Zone1NameAddr+((App.Data.AlarmZone-1)*20);
    System.Device.Storage.ReadData(addr,buffer,20);
    App.Data.AlarmSiren=buffer[12];
    zoneType=buffer[14];
    
    memset(App.Data.MessageString,0x00,30);
    addr=Zone1NameAddr+((App.Data.AlarmZone-1)*20); 
    System.Device.Storage.ReadData(addr,(u8*)buffer,20); 
    sprintf((char*)App.Data.MessageString, "%.*s %.*s",strlen((char*)buffer),(char*)buffer,strlen("Alarm"),"Alarm");
    
    switch(zoneType)
	{
		case SystemZoneClose:
            return;
		case SystemZoneAlarm://��������״̬
            LogWrite(LogAlarmAddr,LogAlarmTotalAddr,3,LogAlarmZone,App.Data.AlarmZone);
            App.Data.SystemMode=SystemAlarmMode;
            if(App.Data.MessageTypeTotal)
                App.Data.SendMessage=1;
            else if(App.Data.CallTypeTotal)
                App.Data.CallMessage=1;
            App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
            if(buffer[12]==0x37)
                SirenAlarm();
			break;
		case SystemZoneDelayAlarm://��ʱ����
			System.Device.Storage.ReadData(AlarmDelayAddr,&delayAlarm,1);
            if((delayAlarm<256)&&(delayAlarm>0))
            {
                DelayCall=delayAlarm*1000;
                System.Device.Timer.Start(AlarmTimeID,TimerMessageHandle,DelayCall,DelayAlarmProcess);
            }else if(delayAlarm==0)
            {
                LogWrite(LogAlarmAddr,LogAlarmTotalAddr,3,LogAlarmZone,App.Data.AlarmZone);
                App.Data.SystemMode=SystemAlarmMode;
                if(App.Data.MessageTypeTotal)
                    App.Data.SendMessage=1;
                else if(App.Data.CallTypeTotal)
                    App.Data.CallMessage=1;
                App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
                if(buffer[12]==0x37)
                    SirenAlarm();
            }
			break;
		case SystemZoneHomeAlarm://�ڼұ���
			if(App.Data.SystemMode!=SystemStayMode)
			{
                LogWrite(LogAlarmAddr,LogAlarmTotalAddr,3,LogAlarmZone,App.Data.AlarmZone);
                App.Data.SystemMode=SystemAlarmMode;
                if(App.Data.MessageTypeTotal)
                    App.Data.SendMessage=1;
                else if(App.Data.CallTypeTotal)
                    App.Data.CallMessage=1;
                App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
                if(buffer[12]==0x37)
                    SirenAlarm();
			}
			break;
		case SystemZoneSOS://��������
                LogWrite(LogAlarmAddr,LogAlarmTotalAddr,3,LogAlarmZone,App.Data.AlarmZone);
                App.Data.SystemMode=SystemAlarmMode;
                if(App.Data.MessageTypeTotal)
                    App.Data.SendMessage=1;
                else if(App.Data.CallTypeTotal)
                    App.Data.CallMessage=1;
                App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
                if(buffer[12]==0x37)
                    SirenAlarm();
			break;
		case SystemZoneDoorbell://���壬�ڲ�����״̬��Ϊ��������
				if(App.Data.SystemMode!=SystemDisarmMode)
				{
					LogWrite(LogAlarmAddr,LogAlarmTotalAddr,3,LogAlarmZone,App.Data.AlarmZone);
					App.Data.SystemMode=SystemAlarmMode;
					if(App.Data.MessageTypeTotal)
						App.Data.SendMessage=1;
					else if(App.Data.CallTypeTotal)
						App.Data.CallMessage=1;
					App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
					if(buffer[12]==0x37)
						SirenAlarm();
				}
				else
				{
					System.Device.Gsm.GsmATSend("AT+CCMXSTOPWAV","OK",10);
					System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"87.wav\",2","OK",10);	
				}
			break;
	}      
}
/****************************************************************************************************
*
*����       ����ȡ�Ŵ�״̬�����ڷǷ�������
*�������   ����
*�������   �� 0,�Ѿ����ţ�����������û����
*
****************************************************************************************************/
static  u16 GetDoorState(void)
{
	u32 addrCount;
	u16	i,count=0;
	u8 buffer[6];
	
	for(i=0;i<300;i++)
	{
		addrCount=ZoneDetectorAddr+(i*10);
		System.Device.Storage.ReadData(addrCount,buffer,6);
		if(buffer[2]==KeyDoorOpen1527)
			return (i+1);
		memset(buffer,0x00,6);
	}
	return count;    
}
/***************************************************************************
*
*����     �����ͳ������ţ�����ʱ1����ٷ��͵ģ�Ϊ���ǲ����˸о����淴Ӧ��
*
***************************************************************************/
void DisarmSms(void)
{
    memset(App.Data.MessageString,0x00,30);
    sprintf((char*)App.Data.MessageString,"Disarm");
    App.Data.SendMessage=ENABLE;
    App.Data.SendCount=0;
}

/*************************************************************************************************
*
*����       ��ң�س���
*�������   ����
*�������   ����
*
**************************************************************************************************/
void UnlockProcess(void)
{
    u8      systemState;
    u8      state=0;
    u8      numberPhone[30];
    u8      numberName[20];
	u8		wirelessSiren=0;

	SendThreeLines(13) ;
	System.Device.Timer.Stop(CallTimer);
    memset(numberName,0,20);
    memset(numberPhone,0,30);
    App.Data.SendMessage=DISABLE;
    App.Data.OriginateCount=0;
    App.Data.SendCount=0;
    if(App.Data.DelayArmFlag)
    {
        System.Device.Timer.Stop(ArmTimeID);
        App.Data.DelayArmFlag=DISABLE;
    }
	App.Data.CallRingConut=0;
	CallConnect=1;
	System.Device.Gsm.GsmATSend("ATH","OK",10);
	CallConnect=0;
	HT9170DWDN=1;
	App.Data.Calltime5s=1;
	TIM_Cmd(TIM5,ENABLE);
	
	System.Device.Timer.Stop(CallHangUpTime);
	System.Device.Timer.Stop(DelayProcessID);
	
    systemState=SystemDisarmMode;
    App.Data.SystemMode=SystemDisarmMode;
    System.Device.Storage.Parameter.StorageWriteData(SystemModeAddr,(u16*)&systemState,1);
    System.Device.Storage.Parameter.StorageReadData(DisarmSMSNoticeAddr,&state,1);
    if(state&&App.Data.MessageTypeTotal)
	{
		System.Device.Timer.Stop(CallTimer);
        System.Device.Timer.Start(CallTimer,TimerSystickHandle,3000,DisarmSms);
	}
	else
	{
		System.Device.Storage.Parameter.StorageReadData(CIDDisarmUploadAddr,&state,1);
		if(state)
			App.Data.CallData.CallCidMessage=ENABLE;//��ͨ���Ļ�
	}
    App.Menu.FocusFormPointer=&App.Menu.StandbyForm;
	App.Menu.FocusFormPointer->FillAreaPointer->ID=0xFE;
    CloseSiren();
    System.Device.Timer.Stop(AlarmTime);
	System.Device.Timer.Stop(SendTxTimeID);
	System.Device.Storage.Parameter.StorageReadData(WirelessSirenAddr,&wirelessSiren,1);
	if(wirelessSiren)
	{
		if((App.Data.ArmKeypadRemote==2)&&(App.Data.SoundFlag==1))
    {
			System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,2000,SendWirelessData); 
    }
		else if(App.Data.ArmKeypadRemote==1)
    {
			System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,2000,SendWirelessData); 
    }
	}
	
}
/***************************************************************
*
*����       �����Ͷ��ŵ���Ϣ��������ִ��
*�������   ����
*�������   ����
*
***************************************************************/
void SendMessageSystick100Routine(void)
{
    if(App.Data.SendMessage==ENABLE)
    {
        App.Data.SendMessage=0;
        PostMessage(MessageSendSMS, App.Data.SystemMode); 
      RTT_printf(0,"PostMessage:SendSMS\r\n");
    }
}
/*******************************************************************************
*����         �����Ͳ�������
*�������     ����
*�������     ����
*******************************************************************************/
void ArmSms(void)
{
    memset(App.Data.MessageString,0x00,30);
    sprintf((char*)App.Data.MessageString,"Arm");
    App.Data.SendMessage=ENABLE;
    App.Data.SendCount=0;
}

/*******************************************************************************
*
*����       ����ʱ����
*�������   ����
*�������   ����
*
********************************************************************************/
void DelayArmProcess(void)
{
    u8  state;
    u8  temp=0;
	u8	wirelessSiren;
    
    state=SystemArmMode;
    App.Data.DelayArmFlag=DISABLE;
    System.Device.Storage.WriteData(SystemModeAddr,(u16*)&state,1);
    App.Data.SystemMode=state;
    System.Device.Storage.Parameter.StorageReadData(ArmSMSNoticeAddr,&temp,1);
    LogWrite(LogArmAddr,LogArmTotalAddr,App.Data.ArmKeypadRemote,3,0xFF);
    if(temp&&App.Data.MessageTypeTotal)
        System.Device.Timer.Start(CallTimer2,TimerSystickHandle,1000,ArmSms);
	else
	{
		temp=0;
		System.Device.Storage.Parameter.StorageReadData(CIDArmUploadAddr,&temp,1);
		if(temp)
			App.Data.CallMessage=ENABLE;
	}
//	OpenSpeaker();
	System.Device.Timer.Stop(SendTxTimeID);
	System.Device.Storage.Parameter.StorageReadData(WirelessSirenAddr,&wirelessSiren,1);
	if(wirelessSiren)
	{
		if((App.Data.ArmKeypadRemote==2)&&(App.Data.SoundFlag==1))
			System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,2000,SendWirelessData); 
		else if(App.Data.ArmKeypadRemote==1)
			System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,2000,SendWirelessData); 
	} 
}
/***********************************************************************************************
*
*����       ��ң���ڼҲ������������
*�������   ��keyValue,�յ���ң�ؼ�ֵ
*�������   ����
*
***********************************************************************************************/
void SMSSendDelay(void)
{
    App.Data.SendMessage=ENABLE;
}
void RemoteAlawyAndStayProcess(u8 keyValue)
{
    u8      readyToArm;
    u8      doorState;
    u8      systemState;
    u8      temp=0;
    u8      armTime=0;
    u32     time;
	u8		wirelessSiren=0;
    
    if(App.Data.SystemMode==SystemAlarmMode)return;
    App.Data.SendMessage=DISABLE;
	System.Device.Timer.Stop(CallTimer);
	System.Device.Timer.Stop(SIMCheckID);
    if(keyValue==KeyAlawy1527||KeyAlawy2262==keyValue)
        systemState=SystemArmMode;
    if(keyValue==KeyHome1527||KeyHome2262==keyValue)
        systemState=SystemStayMode;
    System.Device.Storage.ReadData(IllegalArmAddr,&readyToArm,1);
    if(readyToArm)
    {
        doorState=GetDoorState();
        if(doorState)
        {
            //����û��������
            return;
        }
    }
    System.Device.Storage.ReadData(ArmDelayAddr,&armTime,1);
    if((armTime)&&(SystemArmMode==systemState))
    {
        time=armTime*1000;
        if(App.Data.DelayArmFlag)
        {
            System.Device.Timer.Stop(ArmTimeID);
            App.Data.DelayArmFlag=DISABLE;
        }
        else
        {
            System.Device.Timer.Start(ArmTimeID,TimerMessageHandle,time,DelayArmProcess); 
            App.Data.DelayArmFlag= ENABLE;
            return;
        }
    }
	System.Device.Storage.WriteData(SystemModeAddr,(u16*)&systemState,1);
	App.Data.SystemMode=systemState;
	if(systemState==SystemStayMode)
	{
		SendThreeLines(14) ;
//		OpenSpeaker();
		LogWrite(LogArmAddr,LogArmTotalAddr,App.Data.ArmKeypadRemote,LogStay,0xFF);
		if(App.Data.DelayArmFlag)
		{
			System.Device.Timer.Stop(ArmTimeID);
			App.Data.DelayArmFlag=DISABLE;
		}
		System.Device.Timer.Stop(SendTxTimeID);
		System.Device.Storage.Parameter.StorageReadData(HomeSMSNoticeAddr,&temp,1); 
	}
	else if(systemState==SystemArmMode)
	{
		SendThreeLines(12) ;
		LogWrite(LogArmAddr,LogArmTotalAddr,App.Data.ArmKeypadRemote,LogArm,0xFF);
		System.Device.Storage.Parameter.StorageReadData(ArmSMSNoticeAddr,&temp,1);
		System.Device.Timer.Stop(SendTxTimeID);
		System.Device.Storage.Parameter.StorageReadData(WirelessSirenAddr,&wirelessSiren,1);
		if(wirelessSiren)
		{
			if((App.Data.ArmKeypadRemote==2)&&(App.Data.SoundFlag==1))
				System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,3500,SendWirelessData); 
			else if(App.Data.ArmKeypadRemote==1)
				System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,3500,SendWirelessData); 
		}
	}
	if(temp&&App.Data.MessageTypeTotal)
	{
		System.Device.Timer.Stop(SIMCheckID); 
		memset(App.Data.MessageString,0x00,30);
		if(App.Data.SystemMode==SystemStayMode)
			sprintf((char*)App.Data.MessageString,"Stay");
		else
			 sprintf((char*)App.Data.MessageString,"Arm");
		System.Device.Timer.Start(SIMCheckID,TimerMessageHandle,3000,SMSSendDelay); 
		App.Data.SendCount=0;
	}
	else
	{
		temp=0;
		if(App.Data.SystemMode==SystemStayMode)
			System.Device.Storage.Parameter.StorageReadData(CIDStayUploadAddr,&temp,1);
		else
			System.Device.Storage.Parameter.StorageReadData(CIDArmUploadAddr,&temp,1);
		if(temp)
			App.Data.CallData.CallCidMessage=ENABLE;
	}
}

/********************************************************************************************************
*
*����       ��ң�ر������������ȡ�����ţ�ң�ر�ţ��ͱ�������
*�������   ��pointer���ӵ�ң�ص�ָ��
*�������   ����
*��ע       ��ң����ֱ�ӱ���û��
********************************************************************************************************/
static void RemoteAlarmProcess(u8 *pointer)
{
    char    nameBuffer[20];
    u32     addr;
    
    App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
	SendThreeLines(11) ;
    App.Data.AlarmZone=ComparisonRemoteID(Remote1Addr,pointer,8);
    App.Data.SystemMode=SystemAlarmMode;
    App.Data.AlarmType=RemoteAlarm;
    
    memset(App.Data.MessageString,0x00,30);
    addr=Remote1NameAddr+((App.Data.AlarmZone-1)*20); 
    System.Device.Storage.ReadData(addr,(u8*)nameBuffer,20); 
    sprintf((char*)App.Data.MessageString,"%.*s %.*s",strlen(nameBuffer),nameBuffer,strlen("Alarm"),"Alarm"); 
    
    if(App.Data.MessageTypeTotal)
        App.Data.SendMessage=1;
	if(App.Data.CallData.CallCidTotal)
		 App.Data.CallData.CallCidMessage=1;	
	if(App.Data.CallTypeTotal)
		App.Data.CallMessage=1;
    App.Data.SendCount=0;
    App.Data.OriginateCount=0;
//    App.Data.OriginateCalltimes=1;
    SirenAlarm();
	System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,3000,SendWirelessData);
}
/********************************************************************************************************
*����		������ң�������������״̬
*�������	��mode,����ģʽ
			  bufferPointer,��ǰ�յ���ID
*�������	����
********************************************************************************************************/
static void RemoteProcess(u8 mode,u8 *bufferPointer)
{
	u32     addr;
	u8      temp;
	u8      keyValue; 
	Label   *label;
	char    buffer[30];
  u8      remoteNumber=0;
	
	if(mode==SystemCodeMode)
	{
		temp=ComparisonRemoteID(RemoteBaseAddr,bufferPointer,8);
		if(temp)
		{
			label=App.Menu.FocusFormPointer->LabelPointer;
			while(null!=label)
			{
				if(0xF8==label->Type)
				{
					memset(buffer,0x00,30);
					sprintf((char*)buffer,"Configred %.2d remote",temp);
					memcpy((char*)label->StringBlockPointer,buffer, strlen(buffer));
				}
				label=label->NextLabelPointer;								
			}
			App.Menu.FocusFormPointer->LabelUpdate=0xFE;
			return;
		}
		addr=*StorageAddr();//��ȡ��ַ
		System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)bufferPointer,2);
		addr=*StorageNameAddr();
		temp=1;
		System.Device.Storage.Parameter.StorageWriteData(addr+16,(u16*)&temp,1);

		temp=GetRemoteTotal(Remote1Addr,8);
		System.Device.Storage.Parameter.StorageWriteData(RemoteTotalAddr,(u16*)&temp,1);
		
		label=App.Menu.FocusFormPointer->LabelPointer;
		while(null!=label)
		{
			if(0xF8==label->Type)
			{
				memset(buffer,0x00,30);
				sprintf((char*)buffer,"Please Press Any Key on Remote");
				memcpy((char*)label->StringBlockPointer,buffer, strlen(buffer));
			}
			label=label->NextLabelPointer;								
		}
		App.Menu.FocusFormPointer->LabelUpdate=0xFE;
	}
	else
	{
    remoteNumber=ComparisonRemoteID(RemoteBaseAddr,bufferPointer,8);
		if(remoteNumber)
		{
      keyValue=bufferPointer[2];
      //remoteNumber=ComparisonRemoteID(RemoteBaseAddr,bufferPointer,8);
      addr=Remote1NameAddr+((remoteNumber-1)*20);
      System.Device.Storage.ReadData(addr,(u8*)buffer,20); 
      if(buffer[12])
      {
        App.Data.SoundFlag=1;
      }
      else
      {
        App.Data.SoundFlag=0;
      }
      System.SystemApp.ScreenSet();
      switch(keyValue)
      {
        case KeyUnlock1527:
        case KeyUnLock2262:
          {
            LogWrite(LogDisarmAddr,LogDisarmTotalAddr,2,LogDisarm,0xFF);
            RemoteUnlockProcess();
          }
          break;
        case KeyAlawy1527:
        case KeyAlawy2262:
        case KeyHome1527:
        case KeyHome2262:
          {
            App.Data.ArmKeypadRemote=2;
            RemoteAlawyAndStayProcess(keyValue);
          }
          break;
        case KeyAlarm1527:
        case KeyAlarm2262:
          {
            LogWrite(LogAlarmAddr,LogAlarmTotalAddr,2,LogAlarmRemote,remoteNumber);
            RemoteAlarmProcess(bufferPointer);
          }
          break;
        }
          
          
//            if(keyValue==KeyUnlock1527||KeyUnLock2262==keyValue)
//            {
//                LogWrite(LogDisarmAddr,LogDisarmTotalAddr,2,LogDisarm,0xFF);
//                RemoteUnlockProcess();
//            }
//            if(keyValue==KeyAlawy1527||KeyAlawy2262==keyValue||keyValue==KeyHome1527||KeyHome2262==keyValue)
//            {
//                App.Data.ArmKeypadRemote=2;
//                RemoteAlawyAndStayProcess(keyValue);
//            }
//            if(keyValue==KeyAlarm1527||KeyAlarm2262==keyValue)
//            {
//                LogWrite(LogAlarmAddr,LogAlarmTotalAddr,2,LogAlarmRemote,remoteNumber);
//                RemoteAlarmProcess(bufferPointer);
//            }
//            System.SystemApp.ScreenSet();
        }
      else if(!remoteNumber)return;//�Ƿ�ң��ֱ�ӷ���
		
	}
}
/**************************************************************************************************
*
*����       ��̽ͷ�Ĵ�����
*�������   ��mode,ϵͳģʽ
*              pointer,�յ������߿�ͷ����ָ��
*�������   ����
*
**************************************************************************************************/
static void DetectorProcess(u8 mode,u8 *pointer)
{
	u8 		detectorTotal=0;
	u8		buffer[30];
	u8		zoneID[6];
	u8		temp;
	Label	*label;	
	u32		addrID;
	u32 	addrName;
	u8		zoneNumber;
  u8      zoneType;
	
	if(SystemCodeMode==mode)
	{
		temp=ComparisonDetectorID(ZoneDetectorAddr,pointer,300);
		if(temp)//���ظ���
		{
			label=App.Menu.FocusFormPointer->LabelPointer;
			while(null!=label)
			{
				if(0xF8==label->Type)
				{
					memset(buffer,0x00,30);
					sprintf((char*)buffer,"Configred %.2d Detector",temp);
					memcpy((char*)label->StringBlockPointer,buffer, strlen((char*)buffer));
				}
				label=label->NextLabelPointer;								
			}
			App.Menu.FocusFormPointer->LabelUpdate=0xFE;
			return;
		}
		else
		{
      detectorTotal=GetDetectorTotal(ZoneDetectorAddr,300);
			zoneNumber=GetZoneNumber();//��ȡ������
			addrName=*StorageNameAddr();//���ط������֣������ţ�����״̬��������̽ͷ���� �ĵ�ַ
			addrID=DetectorAddrID(300);//��ȡ��ǰ�յ�ID
			if(addrID)
			{
				zoneID[0]=pointer[0];//addr1
				zoneID[1]=pointer[1];//addr2
				zoneID[2]=pointer[2];//data
				zoneID[3]=zoneNumber;//zone number
//				zoneID[4]=0;
//				zoneID[5]=0;
				System.Device.Storage.Parameter.StorageWriteData(addrID,(u16*)zoneID,2);//д��
				//System.Device.Storage.Parameter.StorageReadData(addrID+6,zoneID,6);//
				
				addrName+=16;
				System.Device.Storage.Parameter.StorageReadData(addrName,&detectorTotal,1);//
				detectorTotal+=1;
				System.Device.Storage.Parameter.StorageWriteData(addrName,(u16*)&detectorTotal,1);//
				
				detectorTotal=GetDetectorTotal(ZoneDetectorAddr,300);
				System.Device.Storage.Parameter.StorageWriteData(DetectorTotalAddr,(u16*)&detectorTotal,1);
				App.Menu.FocusFormPointer->LabelUpdate=0xFE;
			}
		}
	}
	else 
	{
		temp=ComparisonDetectorID(ZoneDetectorAddr,pointer,300);//�ȶԽ��
		if(0==temp) return;
        //��¼������
        System.SystemApp.ScreenSet();
        if((pointer[2]==KeyDoorClose1527)||(pointer[2]==KeyDoorOpen1527))
        {
			addrID=ZoneDetectorAddr+((temp-1)*10);
			System.Device.Storage.Parameter.StorageReadData(addrID,zoneID,6);//
			pointer[3]=zoneID[3];
			System.Device.Storage.Parameter.StorageWriteData(addrID,(u16*)pointer,2);
            if(pointer[2]==KeyDoorClose1527) return;//����
        }
        switch(mode)
        {
            case SystemDisarmMode:
                    addrID=ZoneDetectorAddr+((temp-1)*10);
                    System.Device.Storage.Parameter.StorageReadData(addrID,zoneID,6);
                    App.Data.AlarmZone=zoneID[3];
                    addrName=Zone1NameAddr+((App.Data.AlarmZone-1)*20);
                    System.Device.Storage.Parameter.StorageReadData(addrName,buffer,20);
                    zoneType=buffer[14];
                    if((zoneType==SystemZoneSOS)||(SystemZoneDoorbell==zoneType))
                    {
                        AlarmProcess(temp);
                        return;
                    }
//                    if((pointer[2]==KeyQiGan1527)||(pointer[2]==KeyQiGan2262))      res=1;
//                    if((pointer[2]==KeyYanGan1527)||(pointer[2]==KeyYanGan2262))    res=1;
//                    if((pointer[2]==KeyAlarm1527)||(pointer[2]==KeyAlarm2262))      res=1;
//                    if(0==res) return;
//                    else
//                        AlarmProcess(temp);                   
                    break;
            case SystemArmMode:
                    AlarmProcess(temp);           
                break;
            case SystemStayMode:
                   /* if((pointer[2]==KeyHongWai2262)&&(pointer[2]==KeyHongWai1527)) //�ڼ�ģʽ�� ���ⲻ����
                        return;
                    else*/
                        AlarmProcess(temp);  
                break;
            case SystemAlarmMode:
                AlarmProcess(temp);  
                break;
            case SystemDoorbellMode:
                break;
        }
	}
}
/**************************************************************************************************
*
*����       ����ȡ�绰������������
*�������   ��phoneType,Ҫ��ȡ��һ���͵ĺ���
*�������   �����ػ�ȡ���ĺ�������������0����û�и����͵ĺ���
*
**************************************************************************************************/
u8 GetPhoneTypeTotal(u8 phoneType)
{
    u8  i;
    u8  total=0;
    u8  numberBuffer[30];
    u32 addr;
    
    for(i=0;i<8;i++)
    {
      addr=PhoneMember1Addr+(30*i);
      System.Device.Storage.ReadData(addr,numberBuffer,30);
      if(strstr((char*)numberBuffer,"Empty")==NULL)
      {
        addr=PhoneName1Addr+(20*i);
        System.Device.Storage.ReadData(addr,numberBuffer,20);
        if(phoneType==MessageType)
        {
          if(numberBuffer[16])
          {
            total++;
          }
        }
        else if(phoneType==CallType)
        {
          if(numberBuffer[14])
          {
            total++;
          }
        }
			  else if(phoneType==CIDType)
			  {
				  if(numberBuffer[18])
          {
					  total++;
          }
			  }
      } 
      memset(numberBuffer,0x00,30);
    } 
    return total;
}
/**************************************************************************************************
*
*����       �������źŴ���
*�������   ��data,�յ�����������
*�������   ����
*
**************************************************************************************************/
void WirelessProcess(u32 data)
{
	u8 		addrBuffer[4];
	u8		ctrlData;
	u8		valType=0;
	
	addrBuffer[0]=Byte2(data);//��ַ0
	addrBuffer[1]=Byte1(data);//��ַ1
	addrBuffer[2]=Byte0(data);//������
	addrBuffer[3]=0;//������
	
	ctrlData=addrBuffer[2];
	if(1==RemoteValueCheck(ctrlData))
  {
		valType=RemoteValue;
  }
	else if(DetectorValueCheck(ctrlData))
  {
		valType=DetectorValue;
  }
	if((App.Data.UseWireless)&&(App.Data.CodeWireless))
	{
		if((App.Data.RxDis==RxDetector)&&(valType==RemoteValue))return;//��̽ͷ����ģʽ�� �յ�ң�ص�ֱֵ�ӷ���
		if((App.Data.RxDis==RxRemote)&&(valType==DetectorValue))return;//��ң�ض���ģʽ�� �յ�̽ͷ��ֱֵ�ӷ���
		App.Data.SystemMode=SystemCodeMode;
	}
  else if(App.Data.SystemMode!=SystemAlarmMode)
  {
    System.Device.Storage.Parameter.StorageReadData(SystemModeAddr,&App.Data.SystemMode,1);
  }
  App.Data.MessageTypeTotal=GetPhoneTypeTotal(MessageType);
  App.Data.CallTypeTotal=GetPhoneTypeTotal(CallType);
	App.Data.CallData.CallCidTotal=GetPhoneTypeTotal(CIDType);
	switch(valType)
	{
		case NullValue://�յ������ݲ��Ϸ�
			return;
		case RemoteValue:
			RemoteProcess(App.Data.SystemMode,addrBuffer);
			break;
		case DetectorValue:
			DetectorProcess(App.Data.SystemMode,addrBuffer);
			break;
	}
}

