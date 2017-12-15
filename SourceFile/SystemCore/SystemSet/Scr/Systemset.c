/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "Systemset.h"
#include "App.h"
/*---------------------------------------------����--------------------------------------*/
#define UserPassword		0x31
#define AdminPassword		0x32
/*--------------------------------------------�궨��-------------------------------------*/
#define Disarm				0x30
#define Arm					0x31
#define Stay				0x32

#define VoiceVolume			0x31
#define AlarmVolume			0x32


#define On					0x31
#define Off					0x30


#define WirelessSirenOff	0x10
#define WirelessSirenOn		0x11
#define IllegalArmAOff		0x00
#define IllegalArmAOn		0x01


#define Call				'C'
#define SMS					'M'
#define	CID					'I'
#define CallAndSMS			'S'

#define SoundOff			0x00
#define SoundOn				0x01
#define DeleteRemote		0x32

#define SirenOff			SystemSirenOff
#define SirenOn				SystemSirenOn
#define ZoneName			0x39
#define ZoneOff				SystemZoneClose
#define ZoneAlarm			SystemZoneAlarm
#define ZoneDelayAlarm		SystemZoneDelayAlarm
#define ZoneHome			SystemZoneHomeAlarm
#define ZoneSOS				SystemZoneSOS
#define	ZoneDoorbell		SystemZoneDoorbell

/*---------------------------------------------����--------------------------------------*/


/*---------------------------------------------����--------------------------------------*/

/*****************************************************************************************
*����		��RTCʱ�Ӹ���
*�������	��strPointer����������ʱ�����
			  phone,Ҫ�ش��ĵ绰����
*�������	����
******************************************************************************************/
static void SystemRTCUpdate(u8 *strPointer,u8 *phone)
{
	u16 temp=0;
	u8 str[34];
	TimeSetStruct timeSet;
	strPointer++;
	temp=((*strPointer)-0x30)*1000;
	strPointer++;
	temp+=((*strPointer)-0x30)*100;
	strPointer++;
	temp+=((*strPointer)-0x30)*10;
	strPointer++;
	temp+=((*strPointer)-0x30);
	timeSet.Year=temp;

	strPointer++;
	temp=((*strPointer)-0x30)*10;
	strPointer++;
	temp+=((*strPointer)-0x30);
	timeSet.Month=temp;

	strPointer++;
	temp=((*strPointer)-0x30)*10;
	strPointer++;
	temp+=((*strPointer)-0x30);				
	timeSet.Day=temp;

	strPointer++;
	temp=((*strPointer)-0x30)*10;
	strPointer++;
	temp+=((*strPointer)-0x30);
	timeSet.Hour=temp;

	strPointer++;
	temp=((*strPointer)-0x30)*10;
	strPointer++;
	temp+=((*strPointer)-0x30);				
	timeSet.Min=temp;	

	strPointer++;
	temp=((*strPointer)-0x30)*10;
	strPointer++;
	temp+=((*strPointer)-0x30);				
	timeSet.Sec=temp;
	temp=System.Device.DateSet.RTCDateSet(&timeSet);
	if(temp==0)
	{
		System.Device.DateSet.RTCGetTime();
		sprintf((char*)str,"System time:\n%.04d-%.02d-%.02d\n%.02d:%.02d:%.02d",
				Calendar.Year,Calendar.Month,Calendar.Date,
				Calendar.Hour,Calendar.Min,Calendar.Sec);
		System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
	}	
}
/***************************************************************************************

*����		����ʱ��ת�����ַ������ö��ŷ��ͳ�ȥ
*�������	��phone,Ҫ���͵����ֻ�����ָ��
*�������	����

***************************************************************************************/
static void TimeToString(u8 *phone)
{
	u8 str[34];	
	sprintf((char*)str,"System time:\n%.04d-%.02d-%.02d\n%.02d:%.02d:%.02d",
			Calendar.Year,Calendar.Month,Calendar.Date,
			Calendar.Hour,Calendar.Min,Calendar.Sec);
	System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
}
/***************************************************************************************

*����		����ѯ�����޸ĵ�ǰ���벢�����뷢�͵��ֻ���
*�������	��phone,Ҫ���͵��ֻ�����ָ��
			  strPointer,�޸������ֻ��߲�ѯ��ָ��
*�������	����

***************************************************************************************/
static void PasswordInfo(u8 *phone,u8 *strPointer)
{
	u8 str1[6],str2[4];
	u8 str[41];
	u8 temp;
	strPointer++;
	temp=*strPointer;
	switch(temp)
	{
		case UserPassword://�޸��û�����
				strPointer++;
				if(*strPointer==0x0D)	
					break;
				else
					System.Device.Storage.Parameter.StorageWriteData(UserPasswordAddr,(u16*)strPointer,2);
		break;	
		case AdminPassword://�޸Ĺ���Ա����
				strPointer++;
				if(*strPointer==0x0D)
					break;	
				else
					System.Device.Storage.Parameter.StorageWriteData(AdminPasswordAddr,(u16*)strPointer,3);
		break;			
	}
	System.Device.Storage.Parameter.StorageReadData(AdminPasswordAddr,str1,6);
	System.Device.Storage.Parameter.StorageReadData(UserPasswordAddr,str2,4);
	sprintf((char*)str,"User passcode:%.4s\n""Admin passcode:%.6s",(char*)str2,(char*)str1);
	System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
}
/*********************************************************************************************************************

*����		��ϵͳ״̬��ѯ�����͵��ֻ���
*�������	��phone,�绰����ָ��
*********************************************************************************************************************/
static void SystemState(u8 *phone)
{
	u8 temp;
	u8 len;
	u8 Battery[]="Battery Power!";
	u8 External[]="External Power!";
	u8 *pointer;
	u8 str[32];

	if(PowerSate==1)
		pointer=Battery;
	else
		pointer=External;
	temp=App.Data.SystemState;
	len=strlen((char const*)pointer);
	switch(temp)
	{
		case ArmState:
			sprintf((char*)str, "%.*s%.*s",sizeof("Arm"),"Arm\n",len,pointer); 
			break;
		case DisarmState:
			sprintf((char*)str, "%.*s%.*s", sizeof("Disarm"),"Disarm\n",len,pointer); 
			break;
		case StayState:
			sprintf((char*)str, "%.*s%.*s",sizeof("Stay"),"Stay\n",len,pointer); 
			break;
		case Doorbellstate:
			sprintf((char*)str, "%.*s%.*s",sizeof("Doorbell"),"Doorbell\n",len,pointer); 
			break;
	}
	System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
}
/***************************************************************************************************

*����		���������ڲ��ѵ��ں��ֵ���͵��ֻ���
*�������	��phone,Ҫ���͵��ֻ�����
			  strPointer,����ָ��
*�������	����

****************************************************************************************************/
static void VolumeSet(u8* phone,u8 *strPointer)
{
	u8 temp;
	u8 str[32];
	u8 info=0;
	u8 volume1,volume2;
	
	strPointer++;
	temp=*strPointer;
	switch(temp)
	{
		case VoiceVolume:
			strPointer++;
			if((*strPointer<0x38)&&(*strPointer>0x2F))
			{
				temp=*strPointer-0x30;
				System.Device.Storage.Parameter.StorageWriteData(VoiceVolumeAddr,(u16*)&temp,1);
				info=1;
			}else if(*strPointer==0x0D)
				info=1;
			break;
		case AlarmVolume:
			strPointer++;
			if((*strPointer<0x38)&&(*strPointer>0x2F))
			{
				temp|=*strPointer-0x30;
				System.Device.Storage.Parameter.StorageWriteData(AlarmVolumeAddr,(u16*)&temp,1);
				info=1;
			}else if(*strPointer==0x0D)
				info=1;
			break;
	}
	if(info)
	{		
		System.Device.Storage.Parameter.StorageReadData(VoiceVolumeAddr,&volume1,1);
		System.Device.Storage.Parameter.StorageReadData(AlarmVolumeAddr,&volume2,1);
		sprintf((char*)str,"Voice volume:%.1d\n""Alarm volume:%.1d",volume1,volume2);
		System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
	}
	else
	{
		//System.Device.Storage.Parameter.StorageReadData(VolumeAddr,&temp,1);
		sprintf((char*)str,"Parameter error");
		System.Device.Gsm.GsmSendSMS(phone,str);
	}
}
/******************************************************************************************************

*����		����ʱ�趨�����͵��ֻ���
*�������	��phone,�绰����ָ��
			  strPointer,�����ַ�ָ��
*�������	����

******************************************************************************************************/
static void DelaySet(u8* phone,u8*strPointer)
{
	u8 temp;
	u8 str[32];
	u8 info=0;
	u8 alarmTime;
	u8 armTime;
	strPointer++;
	temp=*strPointer;
	switch(temp)
	{
		case 0x31:
			strPointer++;
			if((*strPointer!=0x0D))
			{
				temp=atoi((char*)strPointer);
				System.Device.Storage.Parameter.StorageWriteData(AlarmDelayAddr,(u16*)&temp,1);
				info=1;
			}else if(*strPointer==0x0D)
				info=1;
			break;
		case 0x32:
			strPointer++;
			if((*strPointer!=0x0D))
			{
				temp=atoi((char*)strPointer);
				System.Device.Storage.Parameter.StorageWriteData(ArmDelayAddr,(u16*)&temp,1);
				info=1;
			}else if(*strPointer==0x0D)
				info=1;
			break;
	}
	if(info)
	{		
		System.Device.Storage.Parameter.StorageReadData(ArmDelayAddr,&armTime,1);
		System.Device.Storage.Parameter.StorageReadData(AlarmDelayAddr,&alarmTime,1);
		sprintf((char*)str,"Arm delay(s):%.1d\n""Alarm delay(s):%.1d",armTime,alarmTime);
		System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
	}
}
/*******************************************************************************************************

*����		����������ʱ��
*�������	��phone,�绰����ָ��
			  strPointer,�����ַ�ָ��
*�������	����

*******************************************************************************************************/
static void SirenTimeSet(u8 *phone,u8 *strPointer)
{
	u8 temp;
	u8 str[32];
	u8 info=0;
	u8 time;
	
	strPointer++;
	if(*strPointer!=0x0D)
	{
		temp=atoi((char*)strPointer);
		if(temp<=20)
		{
			System.Device.Storage.Parameter.StorageWriteData(SirenDelayAddr,(u16*)&temp,1);
			info=1;
		}
	}else if(*strPointer==0x0D)
		info=1;
	if(info)
	{		
		System.Device.Storage.Parameter.StorageReadData(SirenDelayAddr,&time,1);
		sprintf((char*)str,"Siren time:%.1d min",time);
		System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
	}	
}
/***************************************************************************************************

*����		���������
*�������	��phone,�绰����ָ��
			  strPointer,�����ַ�ָ��
*�������	����

***************************************************************************************************/
static void RingTimesSet(u8 *phone,u8* strPointer)
{
	u8 temp;
	u8 str[32];
	u8 info=0;
	u8 times;
	
	strPointer++;
    memset(str,0,32);
	if(*strPointer!=0x0D)
	{
		temp=atoi((char*)strPointer);
		if(temp<=20)
		{
			System.Device.Storage.Parameter.StorageWriteData(RingTimesAddr,(u16*)&temp,1);
			System.Device.Gsm.GsmRingTimes(temp,(u8*)"OK",200);
			info=1;
		}
	}else if(*strPointer==0x0D)
		info=1;
	if(info)
	{		
		System.Device.Storage.Parameter.StorageReadData(RingTimesAddr,&times,1);
		sprintf((char*)str,"Ring time:%.1d times",times);
		System.Device.Gsm.GsmSendSMS(phone,str);
	}		
}
/************************************************************************************************

*����		�����Żظ��رջ��߿����������Ͷ��ŵ��ֻ���
*�������	��phone,�绰����ָ��
			  strPointer,�����ַ�ָ��
*�������	����

************************************************************************************************/
static void SMSReply(u8 *phone,u8 *strPointer)
{
	u8 temp;
	u8 str[32];
	u8 info=0;
	u8 res;
	
	strPointer++;
	if(*strPointer!=0x0D)
	{
		if((On==*strPointer)||(Off==*strPointer))
		{
			temp=atoi((char*)strPointer);
			System.Device.Storage.Parameter.StorageWriteData(SMSReplyAddr,(u16*)&temp,1);
			info=1;
		}
	}else if(*strPointer==0x0D)
		info=1;
	if(info)
	{		
		System.Device.Storage.Parameter.StorageReadData(SMSReportAddr,&res,1);
		if(1==res)
			sprintf((char*)str,"SMS Reply:%s","On");
		else if(0==res)
			sprintf((char*)str,"SMS Reply:%s","Off");
		else
			sprintf((char*)str,"Error");
		System.Device.Gsm.GsmSendSMS(phone,str);
	}	
}
/***********************************************************************************

*����		���������ϴ�����
*�������	��phone,�绰����ָ��
			  strPointer,�����ַ�ָ��
*�������	����

***********************************************************************************/
static void UploadSecurityCenter(u8 *phone,char *strPointer)
{
	u8 str[64];
	u8 stateString[3];
	u8 info=0;
	char *pointer;
	char *armStr;
	char *disarmStr;
	char *stayStr;
	
	pointer=strPointer;
	pointer++;
	if(*pointer!=0x0D)
	{
		stateString[0]=0;
		stateString[1]=0;
		stateString[2]=0;
		while(*pointer!=0x0D)
		{
			switch(*pointer)
			{
				case 0x30:
					stateString[0]=0;
					stateString[1]=0;
					stateString[2]=0;
				break;
				case 0x31:
					stateString[0]=1;		
				break;
				case 0x32:
					stateString[1]=2;
				break;
				case 0x33:
					stateString[2]=3;
				break;	
			}
			pointer++;
		}			
		info=1;
		System.Device.Storage.Parameter.StorageWriteData(SecurityCenterAddr,(u16*)stateString,3);
	}else if(*pointer)
		info=1;
	if(info)
	{
		System.Device.Storage.Parameter.StorageReadData(SecurityCenterAddr,stateString,3);
		if(stateString[0]==0)
			armStr="Off";
		else if(stateString[0]==1)
			armStr="On";
		if(stateString[1]==0)
			disarmStr="Off";
		else if(stateString[1]==2)
			disarmStr="On";
		if(stateString[2]==0)
			stayStr="Off";
		else if(stateString[2]==3)
			stayStr="On";
		sprintf((char*)str,"Arming upload:%.*s\nDisarming upload:%.*s\nStay upload:%.*s",
				strlen(armStr),armStr,strlen(disarmStr),disarmStr,strlen(stayStr),stayStr);
		System.Device.Gsm.GsmSendSMS(phone,str);
	}	
}
/**************************************************************************************************

*����		�����������Źرջ��߿����������Ͷ��ŵ��ֻ���
*�������	��phone,�绰����ָ��
			  strPointer,�����ַ�ָ��
*�������	����

**************************************************************************************************/
static void ArmOrDisarmReport(u8 *phone,char *strPointer)
{
	u8 str[64];
	u8 stateString[3];
	u8 info=0;
	char *pointer;
	char *armStr;
	char *disarmStr;
	char *stayStr;
	
	pointer=strPointer;
	pointer++;
	if(*pointer!=0x0D)
	{
		stateString[0]=0;
		stateString[1]=0;
		stateString[2]=0;
		while(*pointer!=0x0D)
		{
			switch(*pointer)
			{
				case 0x30://��֪ͨ
					stateString[0]=0;
					stateString[1]=0;
					stateString[2]=0;
				break;
				case 0x31://����֪ͨ
					stateString[0]=1;		
				break;
				case 0x32://����֪ͨ
					stateString[1]=2;
				break;
				case 0x33://����֪ͨ
					stateString[2]=3;
				break;	
			}
			pointer++;
		}			
		info=1;
		System.Device.Storage.Parameter.StorageWriteData(ArmSMSNoticeAddr,(u16*)&stateString[1],1);
		System.Device.Storage.Parameter.StorageWriteData(DisarmSMSNoticeAddr,(u16*)&stateString[0],1);
		System.Device.Storage.Parameter.StorageWriteData(HomeSMSNoticeAddr,(u16*)&stateString[2],1);
	}else if(0x0D==*pointer)
		info=1;
	if(info)
	{
		System.Device.Storage.Parameter.StorageReadData(ArmSMSNoticeAddr,&stateString[1],1);
		System.Device.Storage.Parameter.StorageReadData(DisarmSMSNoticeAddr,&stateString[0],1);
		System.Device.Storage.Parameter.StorageReadData(HomeSMSNoticeAddr,&stateString[2],1);
		if(stateString[0]==0)
			armStr="Off";
		else if(stateString[0]==1)
			armStr="On";
		if(stateString[1]==0)
			disarmStr="Off";
		else if(stateString[1]==2)
			disarmStr="On";
		if(stateString[2]==0)
			stayStr="Off";
		else if(stateString[2]==3)
			stayStr="On";
		sprintf((char*)str,"Arming SMS notice:%.*s\nDisarming SMS notice:%.*s\nStay SMS notice:%.*s",
				strlen(armStr),armStr,strlen(disarmStr),disarmStr,strlen(stayStr),stayStr);
		System.Device.Gsm.GsmSendSMS(phone,str);
	}	
}
/***********************************************************************************************************


***********************************************************************************************************/
//static void ArmDisarmAuto(u8 *phone,u8 *strPointer)
//{
//	u8 index;
//	u8 *pointer;
//	
//	pointer=strPointer;
//	pointer=strPointer;
//	pointer++;
//	index=*pointer-0x30;
//	index*=10;
//	pointer++;
//	index+=*pointer-0x30;
//	
//	
//	
//}
/***********************************************************************

*����		���Ƿ���������


***********************************************************************/
static void IllegalArm(u8 *phone,u8 *strPointer)
{
	u8 temp,res;
	strPointer++;
	temp=*strPointer;
	switch(temp)
	{
		case '\r'://��ѯ��ǰ״̬
			System.Device.Storage.Parameter.StorageReadData(IllegalArmAddr,&res,1);
		if(0x31==res)
			System.Device.Gsm.GsmSendSMS(phone,(u8*)"IllegalArm:On");
		else if(0x30==res)
			System.Device.Gsm.GsmSendSMS(phone,(u8*)"IllegalArm:OFF");
			break;
		case '0'://�ر�
			System.Device.Storage.Parameter.StorageWriteData(IllegalArmAddr,(u16*)&temp,1);
			System.Device.Gsm.GsmSendSMS(phone,(u8*)"IllegalArm:OFF");
			break;
		case '1'://����
			System.Device.Storage.Parameter.StorageWriteData(IllegalArmAddr,(u16*)&temp,1);
			DelayMs(50);
			System.Device.Gsm.GsmSendSMS(phone,(u8*)"IllegalArm:ON");
			DelayMs(50);
			break;
	}
}
/*********************************************************************************************************

**����	��	�绰�����趨���ѯ

*********************************************************************************************************/
static void SystemContacts(u8 *phone,char *strPointer)
{
	u8 index,i,len=0;
	u32 typeAddr;
	u32 addr=0;
	u8 buffer[30];
	u8 str[64];
	char *phoneBuffer;
	char *typeBuffer;
	u8 numberType;
	u8 temp;
	char *string;
	
	string=strPointer;
	
	//ȡ����������
	string=strPointer;
	string++;
	index=*string-0x30;
	index*=10;
	string++;
	index+=*string-0x30;//������
	
	//����洢��ַ
	addr=((index-1)*30)+PhoneMember1Addr;
	typeAddr=(((index-1)*20)+PhoneName1Addr)-4;
	string++;
	if('\r'!=*string)
	{	
		for(i=1;i<9;i++)
		{
			if(i==index)
			{
				while(*string++!=',') len++;
				phoneBuffer=(char*)malloc(len);
				string-=(len+1);
				sprintf((char*)phoneBuffer,"%.*s",len,string); 
				string+=(len+1);
				temp=*string++;
				switch(temp)
				{
					case 0x31:
						if(*string++==0x32)
							numberType=CallAndSMS;
						else				
							numberType=Call;
						break;
					case 0x32:
						numberType=SMS;
						break;
					case 0x33:
						numberType=CID;
						break;
				}
				System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)phoneBuffer,len);
				System.Device.Storage.Parameter.StorageWriteData(typeAddr,(u16*)&numberType,1);
				break;				
			}
		}
	}
	len=0;
	System.Device.Storage.Parameter.StorageReadData(addr,buffer,30);
	System.Device.Storage.Parameter.StorageReadData(typeAddr,&numberType,1);
	while((buffer[len]!=0x00)&&(buffer[len]!=0xFF)) len++;//�����ж���λ�绰����
	if(0x01==buffer[28])
	{
		sprintf((char*)str,"NO.%1d: %.*s\n",index,strlen("No phone number"),"No phone number");
		System.Device.Gsm.GsmSendSMS(phone,str);	
		return;		
	}
	phoneBuffer=(char*)malloc(6+len);
	sprintf((char*)phoneBuffer,"NO.%1d: %.*s\n",index,len,(char*)buffer);
	switch(numberType)
	{
		case Call:
			typeBuffer="Call";
		break;
		case SMS:
			typeBuffer="SMS";
		break;
		case CID:
			typeBuffer="CID";
		break;
		case CallAndSMS:
			typeBuffer="Call SMS";
		break;
	}
	sprintf((char*)str,"%.*s %.*s",strlen((char*)phoneBuffer),phoneBuffer,strlen((char*)typeBuffer),typeBuffer);
	System.Device.Gsm.GsmSendSMS(phone,str);
	free(phoneBuffer);
}

/********************************************************************************************************************

**********************************************************************************************************************/
static void SystemDeletPhone(u8 *phone,u8 *strPointer)
{
	u8 *pointer;
	u8 index,i;
	u32 typeAddr=0;
	u32 phoneAddr=0;
	u8 buffer[22];
	u8 numberType;
	char str[64];
	u8 	full[22];
	pointer=strPointer;
	memset(full,0xFF,22);
	//ȡ����������
	pointer++;
	if(*pointer!='\r')//�����ɾ��
	{
		pointer=strPointer;
		pointer++;
		index=*pointer-0x30;
		index*=10;
		pointer++;
		index+=*pointer-0x30;//
		
		phoneAddr=((index-1)*22)+PhoneMember1Addr;
		typeAddr=((index-1)*2)+Phone1TypeAddr;
		
		for(i=1;i<9;i++)
		{
			if(i==index)
			{
				System.Device.Storage.Parameter.StorageWriteData(phoneAddr,(u16*)full,22);
				System.Device.Storage.Parameter.StorageWriteData(typeAddr,(u16*)full,2);
				
				System.Device.Storage.Parameter.StorageReadData(phoneAddr,buffer,22);
				System.Device.Storage.Parameter.StorageReadData(typeAddr,&numberType,1);
				for(i=0;i<22;i++)
				{
					if((0xFF!=buffer[i])||(numberType!=0xFF))
					{
						sprintf(str,"NO.%1d: %.*s\n",index,strlen("Delete failed"),"Delete failed");
						System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
						return;
					}
				}
				sprintf(str,"NO.%1d: %.*s\n",index,strlen("Delete success"),"Delete success");
				System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
				return;
			}
		}
	}
	else//ɾ������
	{
		for(i=1;i<9;i++)
		{
			phoneAddr=((i-1)*22)+PhoneMember1Addr;
			typeAddr=((i-1)*2)+Phone1TypeAddr;
			System.Device.Storage.Parameter.StorageWriteData(phoneAddr,(u16*)full,22);
			System.Device.Storage.Parameter.StorageWriteData(typeAddr,(u16*)full,2);			
		}
		for(i=1;i<9;i++)
		{
			phoneAddr=((i-1)*22)+PhoneMember1Addr;
			typeAddr=((i-1)*2)+Phone1TypeAddr;
			System.Device.Storage.Parameter.StorageReadData(phoneAddr,buffer,22);
			System.Device.Storage.Parameter.StorageReadData(typeAddr,&numberType,1);
			for(i=0;i<22;i++)
			{
				if((0xFF!=buffer[i])||(numberType!=0xFF))
				{
					sprintf(str,"NO.%1d: %.*s\n",i,strlen("Delete failed"),"Delete failed");
					System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
					return;
				}
			}
		}
		sprintf(str,"%.*s\n",strlen("All numbers deleted successfully"),"All numbers deleted successfully");
		System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
	}
}
/*******************************************************************************************************


*******************************************************************************************************/
static void WirelessSirenSetm(u8 *phone,char *strPointer)
{
	char 	*pointer;
	u8 		state;
	char 	*str;
	
	pointer=strPointer;
	
	pointer++;
	if('\r'!=*pointer)//��������߾���
	{
		if(On==*pointer)
		{
			state=1;//�����߾���
			System.Device.Storage.Parameter.StorageWriteData(WirelessSirenAddr,(u16*)&state,1);
		}
		else if(Off==*pointer)
		{
			state=0;//�ر����߾���
			System.Device.Storage.Parameter.StorageWriteData(WirelessSirenAddr,(u16*)&state,1);
		}
	}
	System.Device.Storage.Parameter.StorageReadData(WirelessSirenAddr,&state,1);
	if(1==state)
		str="Wireless siren: On";
	else if(0==state)
		str="Wireless siren: Off";
	if((0==state)||(1==state))
		System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
}
/**********************************************************************************************************

����		:��Ϊң����Զֻ�����������һ�������� ���������������������
**********************************************************************************************************/
static void RemoteSet(u8 *phone,u8 *strPointer)
{
	u8 *pointer;
	u8 index;
	u32 remoteAddr,typeAddr;
	u16 temp;
	u8 *str;
	u8 len=0;
	u8 buffer[20];
	char *string;
	u8 nameInfo=0;
	//u8 state=0xFF;

	pointer=strPointer;
	pointer++;
	index=*pointer-0x30;
	index*=10;
	pointer++;
	index+=*pointer-0x30;//
	remoteAddr=((index-1)*20)+Remote1NameAddr;
	typeAddr=remoteAddr+12;
	System.Device.Storage.Parameter.StorageReadData(remoteAddr,buffer,20);
	if((index>=1)||(index<=8))
	{
		pointer++;
		if('\r'!=*pointer)
		{
			temp=*pointer;
			if((0x33>=temp)&&(0x30<=temp))
			{
				temp<<=8;
				if(temp!=0x3300)
				{
					pointer++;
					temp|=*pointer;
				}
				switch(temp)
				{
					case 0x300D://�رհ���
						temp=SoundOff;
						break;
					case 0x3033://�رհ���������
						temp=SoundOff;
						nameInfo=1;
						pointer+=1;
						break;
					case 0x310D://��������
						temp=SoundOn;
						break;
					case 0x3133://��������������
						temp=SoundOn;
						nameInfo=1;
						pointer+=1;
						break;
					case 0x3300://����
						nameInfo=1;
						pointer++;
						break;
					case 0x320D://�h��
						temp=DeleteRemote;
						pointer++;
						break;
				}
				if((temp==SoundOff)||(temp==SoundOn))
					System.Device.Storage.Parameter.StorageWriteData(typeAddr,(u16*)&temp,1);
				else if(temp==DeleteRemote)
				{
					temp=1;
					string=malloc(40);
					System.Device.Storage.Parameter.StorageWriteData(typeAddr,(u16*)&temp,1);
					sprintf(string,"NO.%1d Remote",index);
					System.Device.Storage.Parameter.StorageWriteData(remoteAddr,(u16*)string,strlen(string));
					sprintf(string,"NO.%1d remote Delete successfully",index);
					System.Device.Gsm.GsmSendSMS(phone,(u8*)string);
					free(string);
					return;
				}
				if(nameInfo)
				{
					len=0;
					while('\r'!=*pointer++)len++;
					string=malloc(len);
					memset(string,0xFF,len);
					pointer-=(len+1);
					sprintf(string,"%.*s",len,pointer);
					System.Device.Storage.Parameter.StorageWriteData(remoteAddr,(u16*)string,len);
					free(string);
					string=NULL;	
				}			
			}
		}
		len=0;
		temp=0;
		remoteAddr=((index-1)*20)+Remote1NameAddr;
		typeAddr=((index-1)*2)+Remote1TypeAddr;
		memset(buffer,0xff,20);
		System.Device.Storage.Parameter.StorageReadData(remoteAddr,buffer,20);
		if(buffer[12]==(SoundOn)) 
			str=(u8*)"Sound: On";
		else if(buffer[12]==(SoundOff)) 
			str=(u8*)"Sound: Off";
		if(buffer[17]==NullRemote)
		{
			string=malloc(100);
			sprintf(string,"NO.%1d remote\nWithout this remote",index);
			System.Device.Gsm.GsmSendSMS(phone,(u8*)string);
			free(string);
			return;
		}
		while(0x00!=buffer[len])len++;

		string=malloc(100);
		sprintf(string,"NO.%1d remote\n%.*s\nUser: %.*s",index,strlen((char*)str),str,strlen((char*)buffer),buffer);
		
		System.Device.Gsm.GsmSendSMS(phone,(u8*)string);
		free(string);
	}
}
/***************************************************************************************************************


***************************************************************************************************************/
static void DeleteAllRemote(u8 *phone)
{
	u8 index;
//	u8 temp=DeleteRemote;
	u8 *string=null;
	u32 typeAddr,remoteAddr;
	u8 buffer[20];
	
	string=malloc(50);
	for(index=1;index<9;index++)
	{
		
		remoteAddr=(index-1)*20+Remote1NameAddr;
		memset(buffer,0xFF,20);
		sprintf((char*)buffer,"Remote%.1d",index);
		buffer[18]=SystemSoundOff;//Ĭ�ϰ�����
		buffer[17]=NullRemote;//��ʾû��ң�ض���
		System.Device.Storage.Parameter.StorageWriteData(remoteAddr,(u16*)buffer,20);
		
		remoteAddr=(index-1)*20+Remote1Addr;
		memset(buffer,0xFF,20);
		System.Device.Storage.Parameter.StorageWriteData(remoteAddr,(u16*)buffer,20);
	}
//	temp=0;
	for(index=1;index<9;index++)
	{
		typeAddr=((index-1)*20)+Remote1NameAddr;
		System.Device.Storage.Parameter.StorageReadData(typeAddr,buffer,20);
		if(buffer[17]!=NullRemote)
		{
			string=(u8*)"Delete remote failed";
			System.Device.Gsm.GsmSendSMS(phone,(u8*)string);
			return;			
		}
	}
	string=(u8*)"Delete remote successfully";
	System.Device.Gsm.GsmSendSMS(phone,(u8*)string);
	
}
/*********************************************************************************************

*����	��	��������������

*********************************************************************************************/
static void ZoneSet(u8 *phone,u8 *strPointer)
{
	u8 *pointer;
	u8 index,i=3;
	u8 count;
	u8 sirenState=0xFF,nameState=0xFF,zoneState=0xFF;
	u32 zoneNameAddr;
	char *type;
	char *siren;
	u8 *str;
	//u8 name[20];
	u8 buffer[18];
	
	pointer=strPointer;
	pointer++;
	index=*pointer-0x30;
	index*=10;
	pointer++;
	index+=*pointer-0x30;//������
	zoneNameAddr=(index-1)*20+Zone1NameAddr;	
	//zoneTypeAddr=zoneNameAddr+14;//������
//	sirenAddr   =zoneNameAddr+12;//���ſ��ص�ַ
	pointer++;
	if((31>index)&&(0<index))//���Ʒ���
	{
		if('\r'!=*pointer)
		{
			sirenState=*pointer;
			pointer++;
			nameState=*pointer;
			pointer++;
			zoneState=*pointer;
			memset(buffer,0x00,20);
			buffer[12]=sirenState;
			System.Device.Storage.Parameter.StorageWriteData(zoneNameAddr+12,(u16*)&buffer[12],1);
			buffer[14]=zoneState;
			System.Device.Storage.Parameter.StorageWriteData(zoneNameAddr+14,(u16*)&buffer[14],1);
			System.Device.Storage.Parameter.StorageReadData(zoneNameAddr+16,&count,1);
			buffer[16]=count;
			if(ZoneName==nameState)
			{
				pointer+=2;
				i=0;
				while('\r'!=*pointer)
				{
					buffer[i]=*pointer;
					pointer++;
					i++;
				}
				System.Device.Storage.Parameter.StorageWriteData(zoneNameAddr,(u16*)buffer,i);
			}
			//System.Device.Storage.Parameter.StorageWriteData(zoneNameAddr,(u16*)buffer,18);
		}
	}
	memset(buffer,0x00,20);
	System.Device.Storage.Parameter.StorageReadData(zoneNameAddr,buffer,16);
	if(buffer[12]==SirenOff)
		siren="Off";
	else if(SirenOn==buffer[12])
		siren="On";
	switch(buffer[14])
	{
		case ZoneOff:
			 type="Close";
			 break;
		case ZoneAlarm:
			 type="Normal";
			break;
		case ZoneDelayAlarm:
			 type="Delay Alarm";
			break;
		case ZoneHome:
			type="stay";
			break;
		case ZoneSOS:
			 type="Critical";
			break;
		case ZoneDoorbell:
			type="DoorBell";
			break;
	}
	str=malloc(100);
	if(str==null)
	{
		free(str);
		return;
	}
	sprintf((char*)str,"NO.%2d %.*s\nSiren: %.*s\nZone state:%.*s\rDetector total:%2d Pcs",index,strlen((char*)buffer),buffer,strlen((char*)siren),siren,strlen((char*)type),type,buffer[16]);
	System.Device.Gsm.GsmSendSMS(phone,(u8*)str);
	free(str);
	str=NULL;	
}
/******************************************************************************************************

*����		��APP����ϵͳ�������
*�������	����
*�������	����

******************************************************************************************************/
void SystemSet(u8 *textPointer)
{
	u8 *strPointer=NULL;
	u8 phoneNumber[20];
	u8 i;
	u8 number;
	
	memset(phoneNumber,0,20);
	textPointer=textPointer+2;
	for(i=0;i<20;i++)
	{
		if(*textPointer!='"')
		{
			phoneNumber[i]=*textPointer;
			textPointer++;
		}
		else 
		{
			break;
		}
	}
	strPointer=(u8*)strstr((const char*)textPointer,"\r\n")+8;
	if((*strPointer<0x3A)&&(*strPointer>0x2F))
	{
		number=(*strPointer)-0x30;
		strPointer++;
		if(*strPointer!=0x0D)
		{
			number*=10;
			number+=(*strPointer)-0x30;
		}
	}
	switch(number)
	{
		case SystemDisarm://����
				System.Device.Gsm.GsmSendSMS(phoneNumber,(u8*)"Disarm\nSet successfully!");
			 break;
		case SystemArm://����
				System.Device.Gsm.GsmSendSMS(phoneNumber,(u8*)"Arm\nSet successfully!");
			 break;
		case SystemMonitor://�ز�����
//				CloseSpeaker();
				System.Device.Gsm.GsmCall((const char*)phoneNumber);		
			 break;
		case SysteHome://�ڼ�
				System.Device.Gsm.GsmSendSMS(phoneNumber,(u8*)"Stay\nSet successfully!");
			 break;
		case SystemQuery://��ѯ
				SystemState(phoneNumber);
			 break;
		case SystemPassWord://����
				PasswordInfo(phoneNumber,strPointer);
			 break;
		case SystemDate ://����
				strPointer++;
				if(*strPointer!=0x0D)
				{
					strPointer--;
					SystemRTCUpdate(strPointer,phoneNumber);
				}
				else
				{
					System.Device.DateSet.RTCGetTime();
					TimeToString(phoneNumber);
				}
			 break;
		case SystemDelay://��ʱ����/��ʱ����
				DelaySet(phoneNumber,strPointer);
			 break;
		case SystemSiren ://��������ʱ��
				SirenTimeSet(phoneNumber,strPointer);
			 break;
		
		case SystemVolume://����
				VolumeSet(phoneNumber,strPointer);
			 break;			
		case SystemAuto:
			 break;			
		case SystemRemote://ң��alani123456�趨
			 RemoteSet(phoneNumber,strPointer);
		     break;
		case SystemRemoteDel://ɾ��ң��
			 DeleteAllRemote(phoneNumber);
			 break;
		case SystemZone :
			 ZoneSet(phoneNumber,strPointer);
			 break;
		case SystemRing ://�������
				RingTimesSet(phoneNumber,strPointer);
			 break;
		case SystemSMSAck ://���Żظ�
				SMSReply(phoneNumber,strPointer);
			 break;			
		case SystemUpload: //�������ϴ�����
				UploadSecurityCenter(phoneNumber,(char*)strPointer);
			 break;
		case SystemSMSReport: //����������֪ͨ
				ArmOrDisarmReport(phoneNumber,(char*)strPointer);
			 break;
		case SystemPhoneSet://�绰�趨
				SystemContacts(phoneNumber,(char*)strPointer);
			break;
		case SystemPhoneDel:
			SystemDeletPhone(phoneNumber,strPointer);
			break;
		case SystemLangrage://����
			 break;
		case SystemIllegalArm://�Ƿ�����
			IllegalArm(phoneNumber,strPointer);
			break;
		case SystemWirelessSiren:
			WirelessSirenSetm(phoneNumber,(char*)strPointer);
			break;
			
	}
}
/******************************************************************************

*����		����ʼ��������
*�������	����
*�������	����

******************************************************************************/
void InitsystemApp(void)
{
	System.SystemApp.SystemSet=SystemSet;	
}
