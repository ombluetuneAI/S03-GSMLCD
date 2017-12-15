#include "SecurintyMenu.h"
#include "APP.h"
#include "TaskMenu.h"


#define Call				1
#define SMS					'M'
#define	CID					'I'
#define CallAndSMS			'S'

#define UserPassName		0
#define UserPassWord		1
#define AdminPassWord		2
#define Restore				3
#define Sound				0x30

/*---------------------------------------------常数--------------------------------------*/
#define ON								1
#define OFF								0
#define DefaultArmDelay					0
#define	DefaultAlarmDelay				0

#define DefaultVoiceVolume				5
#define DefaultAlarmVolume				5

#define DefaultSirenTime				1 //这里分钟 步长为1分钟
#define	DefaultRingTimes				1
#define DefaultKeyTone					ON
#define DefaultScreensaverTime			30 //30秒
#define DefaultSMSReport				ON
#define DefaultArmSMS					ON
#define DefaultDisarmSMS				ON
#define DefaultHomeSMS					ON

#define	DefaultScreen30Sec				ON
#define DefaultScreen1Min				OFF
#define DefaultScreen2Min				OFF
#define DefaultScreen5Min				OFF
#define DefaultScreen10Min				OFF
#define DefaultScreenNever				OFF

#define DefaultPowerOFFVoice			OFF
#define DelaultPowerOFFSiren			OFF
#define DelaultPowerOFFSMS				OFF
#define DelaultPowerOFFCall				OFF

#define DefaultPowerONVoice				OFF
#define DelaultPowerONSiren				OFF
#define DelaultPowerONSMS				OFF
#define DelaultPowerONCall				OFF

#define DefaultPowerLowVoice			OFF
#define DelaultPowerLowSiren			OFF
#define DelaultPowerLowSMS				OFF
#define DelaultPowerLowCall				OFF

#define DefaultDetectorVoice			OFF
#define DelaultDetectorSiren			OFF
#define DelaultDetectorSMS				OFF
#define DelaultDetectorCall				OFF

#define DefaultRreadyArm				OFF

#define DefaultWirelessSiren			OFF
#define DefaultOutputWithalarm			OFF
#define DeleteRemoter					ON
#define DeleteSensors					ON
#define DeletePhone						ON
#define DeleteRest						ON

#define SMSReplyON                      ON

#define DefaultRemote1Sound				ON|Sound
#define DefaultRemote2Sound 			ON|Sound
#define DefaultRemote3Sound				ON|Sound
#define DefaultRemote4Sound				ON|Sound
#define DefaultRemote5Sound				ON|Sound
#define DefaultRemote6Sound				ON|Sound
#define DefaultRemote7Sound				ON|Sound
#define DefaultRemote8Sound				ON|Sound

#define DelaultParameterSize			51

/*--------------------------------------------宏定义-------------------------------------*/

/*---------------------------------------------变量--------------------------------------*/

extern const unsigned char gImage_securityIco[][808];

const string DefaultUserPass="1234";
const string DefaultAdminPass="123456";

const u8* SecurityTab[3]=
{
  "Secrity",
	"Password Management",
	"Restore Factory Settings",
};

const string SecurityPath[2]=
{
    "0:/ICON/Security/admin.bmp",
    "0:/ICON/Security/setup.bmp",    
};

const string SecString[4]=
{
	"Password",
	"User Password:",
	"Admin Password:",
	"Restore Factory Settings",
};

static string RestListString[]=
{
	"Delete Remotes",
	"Delete Sensors",
	"Delete Phone",
	"Restore Factory Settings",
	"Are you sure?",
	"Enter",
	"Cancel",
	"Device will reboot on success!",
	"This may take a few senconds!",
};
static const u32 RestListCheck[]=
{
	DeleteRemoterAddr,
	DeleteSensorsAddr,
	DeletePhoneAddr,
	DeleteRestAddr,
};
const u8 DefaultData[DelaultParameterSize]=
{
	DefaultArmDelay,//延时布防
	DefaultAlarmDelay,//延时报警
	
	DefaultVoiceVolume,//默认语音音量
	DefaultAlarmVolume,//报警音量
	
	DefaultSirenTime,//警号鸣响时间
	DefaultRingTimes,//振铃次数
	
	DefaultKeyTone,//按键声

	DefaultSMSReport,//短信回复
	
	DefaultArmSMS,	//布防短信回复
	DefaultDisarmSMS,//撤防短信回复
	DefaultHomeSMS,//在家布防短信回复
	
	DefaultScreen30Sec,
	DefaultScreen1Min,
	DefaultScreen2Min,
	DefaultScreen5Min,
	DefaultScreen10Min,
	DefaultScreenNever,
	
	DefaultPowerOFFVoice,
	DelaultPowerOFFSiren,
	DelaultPowerOFFSMS,
	DelaultPowerOFFCall,
	
	DefaultPowerONVoice,
	DelaultPowerONSiren,
	DelaultPowerONSMS,
	DelaultPowerONCall,	
	
	DefaultPowerLowVoice,
	DelaultPowerLowSiren,
	DelaultPowerLowSMS,
	DelaultPowerLowCall,
	
	DefaultDetectorVoice,
	DelaultDetectorSiren,
	DelaultDetectorSMS	,
	DelaultDetectorCall	,
	DefaultRreadyArm,
	
	DefaultWirelessSiren,
	DefaultOutputWithalarm,
	
	DeleteRemoter,
	DeleteSensors,
	DeletePhone,
	DeleteRest,	
	
	SystemDisarmMode,
	0,
	1,
	0,
	0,
	0,
	0,
	0,
  0,
  0,
  1,
};
const u32 ParameterAddrTab[DelaultParameterSize]=
{
	ArmDelayAddr,
	AlarmDelayAddr,
	
	VoiceVolumeAddr,
	AlarmVolumeAddr,
	
	SirenDelayAddr,
	RingTimesAddr,
	
	KeyToneStateAddr,
	
	SMSReportAddr,
	
	ArmSMSNoticeAddr,
	DisarmSMSNoticeAddr,
	HomeSMSNoticeAddr,
	
	Screen30SecAddr,
	Screen1MinAddr,
	Screen2MinAddr,
	Screen5MinAddr,
	Screen10MinAddr,
	ScreenNeverAddr,
	
	PowerOFFVoiceAddr,
	PowerOFFSirenAddr,
	PowerOFFSMSAddr,
	PowerOFFCallAddr,

	PowerONVoiceAddr,
	PowerONSirenAddr,
	PowerONSMSAddr,
	PowerONCallAddr,

	PowerLowVoiceAddr,
	PowerLowSirenAddr,
	PowerLowSMSAddr,
	PowerLowCallAddr,

	DetectorVoiceAddr,
	DetectorSrienAddr,
	DetectorSMSAddr	,
	DetectorCallAddr,
	
	IllegalArmAddr,
	
	WirelessSirenAddr,
	OutputWithalarmAddr,
	
	DeleteRemoterAddr,
	DeleteSensorsAddr,
	DeletePhoneAddr,
	DeleteRestAddr,
	
	SystemModeAddr,
	DetectorTotalAddr,
	
	AttributeNormalAddr,
	AttributeStayAddr,
	AttributeDoorbellAddr,
	AttributeUrgentAddr,
	AttributeHomeAddr,
	AttributeCloseAddr,
    Alarm3GAddr,
    AlarmPSTNAddr,
    Alarm3gAndPSTNAddr
// 	Remote1TypeAddr,
// 	Remote2TypeAddr,
// 	Remote3TypeAddr,
// 	Remote4TypeAddr,
// 	Remote5TypeAddr,
// 	Remote6TypeAddr,
// 	Remote7TypeAddr,
// 	Remote8TypeAddr,
};
/*---------------------------------------------类型--------------------------------------*/



/***************************************************************************
*
*描述		：软件复位
*
**************************************************************************/
void SystemReset(void)
{
	__set_FAULTMASK(1); 
	NVIC_SystemReset();	
}

/***************************************************************************************************

***************************************************************************************************/
static void SecurityListInit(void)
{
	static FillArea SecurityArea;
	static Label 	SecurityListName;
	static ListBox 	SecurityListBox;	
	static Icon 	SecurityIconLabel;
	string *items;
	items=(string *)&SecurityTab[1];
/*--------------------------安全-----------------------------------------*/
	
	System.Gui.Form.Init(&App.Menu.SecurityListForm);
	App.Menu.SecurityListForm.FormID=69;
	App.Menu.SecurityListForm.LabelCount=1;
	App.Menu.SecurityListForm.FillAreaCount=1;
	App.Menu.SecurityListForm.ListCount=1;
	
	System.Gui.Form.AddFillArea(&App.Menu.SecurityListForm,&SecurityArea);
	SecurityArea.ID=PromptAreaID8;
	SecurityArea.PaintColor=TopLabelColor;
	SecurityArea.RectWrap.x0=0;
	SecurityArea.RectWrap.y0=30;
	SecurityArea.RectWrap.x1=320;
	SecurityArea.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.SecurityListForm,&SecurityListName);
	SecurityListName.ID=LabelID1+10;
	SecurityListName.Font=ListNameFont;
	SecurityListName.PaintColor=GUI_BLACK;
	SecurityListName.TextMode=GUI_TEXTMODE_TRANS;
	SecurityListName.Count=0;
	SecurityListName.StringBlockPointer=(const string*)&SecurityTab[0];
	SecurityListName.X=120;
	SecurityListName.Y=45;
	
			//列表
	System.Gui.Form.AddListBox(&App.Menu.SecurityListForm,&SecurityListBox);
	SecurityListBox.ID=ListBoxID3;
	SecurityListBox.x0=0;
	SecurityListBox.y0=80;
	SecurityListBox.x1=320;
	SecurityListBox.y1=240;
	SecurityListBox.Type=IconList;
	SecurityListBox.selectedItem=0;
  SecurityListBox.ItemsHeight=45;
	SecurityListBox.ItemsPerpage=2;//页最大条目
	SecurityListBox.SelectedIndexes=0;//默认为0，也就是第一条
	SecurityListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	SecurityListBox.TopItem=0;
	
	SecurityListBox.TextMode=GUI_TEXTMODE_TRANS;
	SecurityListBox.BackColor=GUI_WHITE;
	SecurityListBox.PaintColor=GUI_BLACK;
	
	SecurityListBox.SelectedBackColor=MoveBarColor;
	SecurityListBox.SelectedPaintColor=GUI_WHITE;
	
	SecurityListBox.Font=ListFont;
	SecurityListBox.ID=0x49;		
	
	System.Gui.Form.AddLists(&SecurityListBox,items,2,null,null,null,(u8**)gImage_securityIco,2);		
		
}


static void ZoneDefaults(void)
{
	u8 buffer[20];
  u8 zoneID[400];
	u16 i=0;
	u32 addr;
	//名字复位
    System.Device.Storage.Parameter.StorageWriteData(DetectorTotalAddr,(u16*)&i,1);
	for(i=1;i<31;i++)
	{	
		addr=(i-1)*20+Zone1NameAddr;
		memset(buffer,0xFF,20);
		sprintf((char*)buffer,"Zone %.2d",i);
		buffer[12]=SystemSirenOn;//警号
		buffer[14]=SystemZoneAlarm;//防区状态
		buffer[16]=0;//防区探头个数
		System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)buffer,10);
	}
	
  memset(zoneID,0,400);
	for(i=0;i<3;i++)
	{
		addr=ZoneDetectorAddr+(i*400);
		System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)zoneID,200);
	}
}
/*************************************************************************************************

*************************************************************************************************/
static void RemoteDefaults(void)
{
	u8 i,buffer[20];
	u32 addr;
	//名字复位
	for(i=0;i<8;i++)
	{	
		addr=i*20+Remote1NameAddr;
		memset(buffer,0x00,20);
		sprintf((char*)buffer,"Remote %.1d",i+1);
		buffer[12]=SystemSoundOn;//默认伴音关
		buffer[14]=NullRemote;//表示没有遥控对入
		buffer[16]=0;
		System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)buffer,10);
		
		addr=i*4+RemoteBaseAddr;
		memset(buffer,0x00,4);
		System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)buffer,2);
	}
	System.Device.Storage.Parameter.StorageWriteData(RemoteTotalAddr,(u16*)&buffer[0],1);
}
/**********************************************************************************************************
*
*描述		：还原系统中的各种参数
*
***********************************************************************************************************/
u8 readTemp=0xff;
static void SystemInitParameter(void)
{
	u8 i;
	
  u8 temp=0;
    
	System.Device.Storage.Parameter.StorageWriteData(AdminPasswordAddr,(u16*)DefaultAdminPass,3);
	System.Device.Storage.Parameter.StorageWriteData(UserPasswordAddr,(u16*)DefaultUserPass,2);
	for(i=0;i<DelaultParameterSize;i++)
  {
		System.Device.Storage.Parameter.StorageWriteData(ParameterAddrTab[i],(u16*)&DefaultData[i],1);
  }
    
	i=DefaultVoiceVolume;
  temp=SMSReplyON;
  System.Device.Storage.Parameter.StorageWriteData(SMSReplyAddr,(u16*)&temp,1);
	temp=0;
	System.Device.Storage.Parameter.StorageWriteData(CIDArmUploadAddr,(u16*)&temp,1);
	System.Device.Storage.Parameter.StorageWriteData(CIDDisarmUploadAddr,(u16*)&temp,1);
	System.Device.Storage.Parameter.StorageWriteData(CIDStayUploadAddr,(u16*)&temp,1);
	
//	temp=0;
//	System.Device.Storage.Parameter.StorageReadData(CIDAlarmUploadAddr,(u8*)&temp,1);
//	temp=0;
//	System.Device.Storage.Parameter.StorageReadData(CIDDisarmUploadAddr,(u8*)&temp,1);
//	temp=0;
//	System.Device.Storage.Parameter.StorageReadData(CIDArmUploadAddr,(u8*)&temp,1);
	temp=0;
	//System.Device.Voice.VoiceConfig(VoiceVolume,&i,1);	

}
/**********************************************************************************************************


*********************************************************************************************************/
static void PhoneDefault(void)
{
	u8 i,buffer[30];
	u8 name[20];
	//u8 state=Call;
	for(i=0;i<8;i++)
	{
		memset(buffer,0x00,30);
		sprintf((char*)buffer,"NO.%1d (Empty)",i+1);
		buffer[28]=0x01;
		System.Device.Storage.Parameter.StorageWriteData(PhoneMember1Addr+(i*30),(u16*)buffer,15);
		memset(name,0x00,20);
		sprintf((char*)name,"No.%1d",i+1);
		name[14]=1;//电话
		name[12]=1;//命名了
		name[16]=0;//短信
		name[18]=0;//CID
		System.Device.Storage.Parameter.StorageWriteData(PhoneName1Addr+(i*20),(u16*)name,10);
	}
	memset(name,0x30,20);
	System.Device.Storage.Parameter.StorageWriteData(CIDUserCodeAddr,(u16*)name,2);
}

void LogDefaults(void)
{
    u16 i=0;

    System.Device.Storage.Parameter.StorageWriteData(LogAlarmTotalAddr,(u16*)&i,1);
    System.Device.Storage.Parameter.StorageWriteData(LogArmTotalAddr,(u16*)&i,1);
    System.Device.Storage.Parameter.StorageWriteData(LogDisarmTotalAddr,(u16*)&i,1);
//    temp=0xFF;

    FLASH_Unlock();
    FLASH_ErasePage(LogAlarmAddr);
    FLASH_Lock();
    // System.Device.Storage.Parameter.StorageWriteData(LogAlarmAddr,(u16*)&temp,1);
}

/***************************************************************************************************
*
*描述       ：恢复默认值，一个定时组占用30个字节 ，初始状态是自动布防，定时器是关闭的
*                0-15，       16             17     18    19     20      21
                状态显示    状态           保留    时    保留   分     保留
                            布防，0xF0
                            撤防，0xF1
*输入参数   ：无
*输出参数   ：无
*
***************************************************************************************************/
void AutoArmDisarmDefaults(void)
{
	u8 buffer[30];
    u8 i,temp=0;
	for(i=0;i<8;i++)
	{
		memset(buffer,0,30);
        memset(buffer+22,1,8);
		sprintf((char*)buffer,"Auto Arm %d",i+1);
        buffer[16]=0xF0;//初始状态是自动布防
		System.Device.Storage.Parameter.StorageWriteData(Auto1Addr+(i*AutoBase),(u16*)buffer,15);
		memset(buffer,0,30);
        System.Device.Storage.Parameter.StorageWriteData(Auto1State+(i*2),(u16*)(&temp),1);//关闭定时器
	}    
}
extern void GsmPowe(void);
extern u32 SystemRunTime;
void DefaultRest(void)
{
  u32 lastTime,curTime;
  u32 firstTime;
	u8 temp=GsmReset;
  firstTime = SystemRunTime;
	System.Device.Gsm.GsmATSend("ATS0=1","OK",10);
	System.Device.Gsm.GsmATSend("AT&W","OK",10);
  lastTime = SystemRunTime;
	GsmPowe();
  curTime = SystemRunTime - lastTime;
  RTT_printf(0,"GSM:%dms\r\n",curTime);
	if(1==App.Data.RestoreDefault[0])//删除遥控
	{
    lastTime = SystemRunTime;
		RemoteDefaults();
    curTime = SystemRunTime - lastTime;
    RTT_printf(0,"remote:%dms\r\n",curTime);
	}
		
	if(1==App.Data.RestoreDefault[1])//删除探头
	{
    lastTime = SystemRunTime;
		ZoneDefaults();	
    curTime = SystemRunTime - lastTime;
    RTT_printf(0,"sensors:%dms\r\n",curTime);
	}
	if(1==App.Data.RestoreDefault[2])//删除电话
	{
    lastTime = SystemRunTime;
		PhoneDefault();	
    curTime = SystemRunTime - lastTime;
    RTT_printf(0,"phone:%dms\r\n",curTime);
	}
	if(1==App.Data.RestoreDefault[3])//恢复参数
	{
    lastTime = SystemRunTime;
		SystemInitParameter();
    curTime = SystemRunTime - lastTime;
    RTT_printf(0,"para:%dms\r\n",curTime);
	}
  lastTime = SystemRunTime;
  LogDefaults();
  curTime = SystemRunTime - lastTime;
  RTT_printf(0,"log:%dms\r\n",curTime);
  
  lastTime = SystemRunTime;
  AutoArmDisarmDefaults();
  curTime = SystemRunTime - lastTime;
  RTT_printf(0,"auto:%dms\r\n",curTime);
  curTime = SystemRunTime - firstTime;
   RTT_printf(0,"totle:%dms\r\n",curTime);
  
	System.Device.Storage.Parameter.StorageWriteData(ResetAddr,(u16*)&temp,1);
//   System.Device.Storage.Parameter.StorageReadData(DetectorTotalAddr,&i,1);
//    for(i=0;i<8;i++)
//    {
//        System.Device.Storage.Parameter.StorageReadData(Auto1State+(i*2),&readTemp,1);
//        readTemp=99;
//    }

	SystemReset();//复位
}
/*********************************************************************************************
*
*描述		：计时复位
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
void DefaultsSetup(void)
{	
  //DefaultRest();
	System.Device.Timer.Start(RestTimeID,TimerSystickHandle,100,DefaultRest);//计时重启动
}

void LoadDefaultPara(void)
{
  u32 lastTime,curTime;
  u32 firstTime;
	u8 temp=GsmReset;
   //删除遥控
	{
		RemoteDefaults();
	}
		
	//删除探头
	{
		ZoneDefaults();	
	}
	//删除电话
	{
		PhoneDefault();	
	}
	//恢复参数
	{
		SystemInitParameter();
	}
  LogDefaults();

  AutoArmDisarmDefaults();
  
	System.Device.Storage.Parameter.StorageWriteData(ResetAddr,(u16*)&temp,1);

}



/*********************************************************************************************
*
*描述		：密码管理页面
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void PasswordFormInit(void)
{
	static Label 		userPassName;
	static FillArea		userPassFill;
	
	static Label		userPassWord;
	static FillArea		userline;
	static TextBox		userTextBox;

	static Label		adminPassWord;
	static FillArea		adminline;
	static TextBox		adminTextBox;

	System.Gui.Form.Init(&App.Menu.PasswordForm);
	App.Menu.PasswordForm.FillAreaCount=3;
	App.Menu.PasswordForm.LabelCount=3;
	App.Menu.PasswordForm.TextBoxCount=2;
	App.Menu.PasswordForm.FormID=Form13;
	App.Menu.PasswordForm.Color=GUI_WHITE;
	App.Menu.PasswordForm.RectWrap.x0=0;
	App.Menu.PasswordForm.RectWrap.y0=30;
	App.Menu.PasswordForm.RectWrap.x1=320;
	App.Menu.PasswordForm.RectWrap.y1=210;	
	App.Menu.PasswordForm.DefaultFocusTextBoxPointer=&userTextBox;
	App.Menu.PasswordForm.FocusTextBoxPointer=&userTextBox;//当前页面焦点从ArmTextBox开始
	userTextBox.ParentTextBoxPointer=&adminTextBox;
	adminTextBox.ParentTextBoxPointer=&userTextBox;
	
	System.Gui.Form.AddFillArea(&App.Menu.PasswordForm,&userPassFill);
	userPassFill.ID=PromptAreaID2;
	userPassFill.PaintColor=TopLabelColor;
	userPassFill.RectWrap.x0=0;
	userPassFill.RectWrap.y0=30;
	userPassFill.RectWrap.x1=320;
	userPassFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.PasswordForm,&userPassName);
	userPassName.ID=LabelID1;
	userPassName.Font=ListNameFont;
	userPassName.PaintColor=GUI_BLACK;
	userPassName.TextMode=GUI_TEXTMODE_TRANS;
	userPassName.Count=UserPassName;
	userPassName.StringBlockPointer=SecString;
	userPassName.X=100;
	userPassName.Y=43;		
	
	System.Gui.Form.AddFillArea(&App.Menu.PasswordForm,&userline);
	userline.ID=PromptAreaID3;
	userline.PaintColor=GUI_ORANGE;
	userline.RectWrap.x0=0;
	userline.RectWrap.y0=124;
	userline.RectWrap.x1=320;
	userline.RectWrap.y1=124;
	
	System.Gui.Form.AddLabel(&App.Menu.PasswordForm,&userPassWord);
	userPassWord.ID=LabelID1+1;
	userPassWord.Font=ListFont;
	userPassWord.PaintColor=GUI_BROWN;
	userPassWord.TextMode=GUI_TEXTMODE_TRANS;
  userPassWord.Type=0xC0;
	userPassWord.Count=UserPassWord;
	userPassWord.StringBlockPointer=SecString;
	userPassWord.X=20;
	userPassWord.Y=100;	
	
	
	System.Gui.Form.AddTextBox(&App.Menu.PasswordForm,&userTextBox);
	userTextBox.x=196;
	userTextBox.y=100;
	userTextBox.MaxDataPointer=4;
	userTextBox.MinDataPointer=0;
	userTextBox.Type=GuiDataTypeString;
	userTextBox.BackColor=GUI_LIGHTGRAY;
	userTextBox.PaintColor=GUI_RED;
	userTextBox.Font=ListFont;
	userTextBox.ID=0x01;
	userTextBox.TextMode=GUI_TEXTMODE_NORMAL;	
	userTextBox.RectWrap.x0=196;
	userTextBox.RectWrap.x1=256;
	userTextBox.RectWrap.y0=100;
	userTextBox.RectWrap.y1=120;
	userTextBox.DataAddr=UserPasswordAddr;
	userTextBox.DataPointer=&App.Data.KeyBuffer1;
	
/*_______________________________________________________________________________________________________________________*/
	System.Gui.Form.AddFillArea(&App.Menu.PasswordForm,&adminline);
	adminline.ID=PromptAreaID4;
	adminline.PaintColor=GUI_ORANGE;
	adminline.RectWrap.x0=0;
	adminline.RectWrap.y0=164;
	adminline.RectWrap.x1=320;
	adminline.RectWrap.y1=164;
	
	System.Gui.Form.AddLabel(&App.Menu.PasswordForm,&adminPassWord);
	adminPassWord.ID=LabelID1+5;
	adminPassWord.Font=ListFont;
	adminPassWord.PaintColor=GUI_BROWN;
	adminPassWord.TextMode=GUI_TEXTMODE_TRANS;
  adminPassWord.Type=0xC0;
	adminPassWord.Count=AdminPassWord;
	adminPassWord.StringBlockPointer=SecString;
	adminPassWord.X=20;
	adminPassWord.Y=140;	
	
	
	System.Gui.Form.AddTextBox(&App.Menu.PasswordForm,&adminTextBox);
	adminTextBox.x=196;
	adminTextBox.y=140;
	adminTextBox.MaxDataPointer=6;
	adminTextBox.MinDataPointer=0;
	adminTextBox.Type=GuiDataTypeString;
	adminTextBox.BackColor=GUI_LIGHTGRAY;
	adminTextBox.PaintColor=GUI_RED;
	adminTextBox.Font=ListFont;
	adminTextBox.ID=0x03;
	adminTextBox.TextMode=GUI_TEXTMODE_TRANS;	
	adminTextBox.RectWrap.x0=196;
	adminTextBox.RectWrap.x1=256;
	adminTextBox.RectWrap.y0=140;
	adminTextBox.RectWrap.y1=160;
	adminTextBox.DataAddr=AdminPasswordAddr;
	adminTextBox.DataPointer=&App.Data.KeyBuffer2;
}
/*********************************************************************************************
*
*描述		：恢复出厂默认
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void FactoryDefaults(void)
{
	static Label 		listName;
	static FillArea		listFill;	
	static ListBox		restListBox;
	string *items;
	
	items=(string *)RestListString;
	
	System.Gui.Form.Init(&App.Menu.RestoreForm);
	App.Menu.RestoreForm.ListCount=1;
	App.Menu.RestoreForm.FillAreaCount=1;
	App.Menu.RestoreForm.LabelCount=1;
	App.Menu.RestoreForm.FormID=Form15;
	App.Menu.RestoreForm.Color=GUI_WHITE;
	App.Menu.RestoreForm.RectWrap.x0=0;
	App.Menu.RestoreForm.RectWrap.y0=30;
	App.Menu.RestoreForm.RectWrap.x1=320;
	App.Menu.RestoreForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.RestoreForm,&listFill);
	listFill.ID=PromptAreaID1;
	listFill.PaintColor=TopLabelColor;
	listFill.RectWrap.x0=0;
	listFill.RectWrap.y0=30;
	listFill.RectWrap.x1=320;
	listFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.RestoreForm,&listName);
	listName.ID=LabelID1;
	listName.Font=ListNameFont;
	listName.PaintColor=GUI_BLACK;
	listName.TextMode=GUI_TEXTMODE_TRANS;
	listName.Count=Restore;
	listName.StringBlockPointer=SecString;
	listName.X=50;
	listName.Y=40;		
	
	System.Gui.Form.AddListBox(&App.Menu.RestoreForm,&restListBox);
	restListBox.x0=0;
	restListBox.y0=80;
	restListBox.x1=320;
	restListBox.y1=240;
	restListBox.xSize=15;
	restListBox.ySize=15;
//	restListBox.Select=0;
	
	restListBox.selectedItem=0;
	restListBox.ItemsHeight=40;
	restListBox.ItemsPerpage=4;//页最大条目
	restListBox.SelectedIndexes=0;//默认为0，也就是第一条
	restListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	restListBox.TotalItems=0;
	
	restListBox.TextMode=GUI_TEXTMODE_TRANS;
	restListBox.BackColor=GUI_WHITE;
	restListBox.PaintColor=GUI_BLACK;
	
	restListBox.SelectedBackColor=MoveBarColor;
	restListBox.SelectedPaintColor=GUI_WHITE;
	
	restListBox.Font=ListFont;
	restListBox.ID=0x56;
	restListBox.Type=CheckList;

	System.Gui.Form.AddLists(&restListBox,items,4,(u32*)&RestListCheck,NULL,null,null,4);	
}
/*********************************************************************************************
*
*描述		：询问页面
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/

static void SureFomInit(void)
{
//	static FillArea	sureFill;
//	static Label	sureName;
	static ListBox	sureList;
	
	string *items;
	items=(string *)RestListString;
	
	System.Gui.Form.Init(&App.Menu.SureRestForm);
	App.Menu.SureRestForm.FillAreaCount=0;
	App.Menu.SureRestForm.LabelCount=0;
	App.Menu.SureRestForm.ListCount=1;
	App.Menu.SureRestForm.FormID=Form9+16;
	App.Menu.SureRestForm.Color=GUI_ORANGE;
	App.Menu.SureRestForm.RectWrap.x0=70;
	App.Menu.SureRestForm.RectWrap.y0=80;
	App.Menu.SureRestForm.RectWrap.x1=250;
	App.Menu.SureRestForm.RectWrap.y1=160;	

//	System.Gui.Form.AddFillArea(&App.Menu.SureRestForm,&sureFill);
//	sureFill.ID=PromptAreaID5;
//	sureFill.PaintColor=TopLabelColor;
//	sureFill.RectWrap.x0=70;
//	sureFill.RectWrap.y0=75;
//	sureFill.RectWrap.x1=250;
//	sureFill.RectWrap.y1=110;	

//	System.Gui.Form.AddLabel(&App.Menu.SureRestForm,&sureName);
//	sureName.ID=LabelID1+10;
//	sureName.Font=GUI_FONT_20_1;
//	sureName.PaintColor=GUI_BLUE;
//	sureName.TextMode=GUI_TEXTMODE_TRANS;
//	sureName.Count=4;
//	sureName.StringBlockPointer=RestListString;
//	sureName.X=100;
//	sureName.Y=85;	
	
	System.Gui.Form.AddListBox(&App.Menu.SureRestForm,&sureList);
	sureList.x0=70;
	sureList.y0=130;
	sureList.x1=250;
	sureList.y1=200;
	sureList.xSize=60;//纯文本列表时这里是文字显示偏移量
	sureList.ySize=10;
	
	sureList.selectedItem=0;
	sureList.ItemsPerpage=2;//页最大条目
	sureList.SelectedIndexes=0;//默认为0，也就是第一条
	sureList.TopItem=0;//默认为0，也就是从第一个索引号开始
	sureList.TotalItems=0;
	
	sureList.TextMode=GUI_TEXTMODE_TRANS;
	sureList.BackColor=GUI_WHITE;
	sureList.PaintColor=GUI_BLACK;
	
	sureList.SelectedBackColor=MoveBarColor;
	sureList.SelectedPaintColor=GUI_WHITE;
	sureList.RimColor=BLUE;
	
	sureList.Font=ListFont;;
	sureList.ID=0x56;
	sureList.Type=TextList;
	
	System.Gui.Form.AddLists(&sureList,&items[5],2,(u32*)NULL,null,null,null,2);
}
/*********************************************************************************************
*
*描述		：等待页面
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void WatitInit(void)
{
	static FillArea	waitFill;
	static FillArea	waitRect;
	static Label	waitText;
	static Label	waitText2;
	
	System.Gui.Form.Init(&App.Menu.WaitForm);
	App.Menu.WaitForm.FillAreaCount=2;
	App.Menu.WaitForm.LabelCount=2;
	App.Menu.WaitForm.ListCount=0;
	App.Menu.WaitForm.FormID=Form6;
	App.Menu.WaitForm.Color=BLACK;
	App.Menu.WaitForm.RectWrap.x0=0;
	App.Menu.WaitForm.RectWrap.y0=0;
	App.Menu.WaitForm.RectWrap.x1=319;
	App.Menu.WaitForm.RectWrap.y1=239;	

	System.Gui.Form.AddFillArea(&App.Menu.WaitForm,&waitFill);
	waitFill.ID=PromptAreaID5;
	waitFill.PaintColor=BLUE;
	waitFill.RectWrap.x0=35;
	waitFill.RectWrap.y0=50;
	waitFill.RectWrap.x1=285;
	waitFill.RectWrap.y1=180;	
	
	System.Gui.Form.AddFillArea(&App.Menu.WaitForm,&waitRect);
	waitRect.ID=PromptAreaID2;
	waitRect.PaintColor=RED;
	waitRect.RectWrap.x0=32;
	waitRect.RectWrap.y0=47;
	waitRect.RectWrap.x1=288;
	waitRect.RectWrap.y1=183;	

	System.Gui.Form.AddLabel(&App.Menu.WaitForm,&waitText);
	waitText.ID=LabelID1+10;
	waitText.Font=ListFont;
	waitText.PaintColor=GUI_YELLOW;
	waitText.TextMode=GUI_TEXTMODE_TRANS;
	waitText.Count=8;
	waitText.StringBlockPointer=RestListString;
	waitText.X=50;
	waitText.Y=75;	
	
	System.Gui.Form.AddLabel(&App.Menu.WaitForm,&waitText2);
	waitText2.ID=LabelID1+10;
	waitText2.Font=ListFont;
	waitText2.PaintColor=GUI_YELLOW;
	waitText2.TextMode=GUI_TEXTMODE_TRANS;
	waitText2.Count=7;
	waitText2.StringBlockPointer=RestListString;
	waitText2.X=80;
	waitText2.Y=120;	
}
void WatiFunc(void)
{
	App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
	App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
	App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
	App.Menu.FocusFormPointer->ListPointer->TopItem=0;			
}
/**************************************************************************************************

***************************************************************************************************/
void SecurintyInit(void)
{
  SecurityListInit();
	PasswordFormInit();
	FactoryDefaults();
	SureFomInit();
	WatitInit();
	
	System.SystemDefaultsParameter=DefaultsSetup;
	System.SystemApp.DeleteAllNumber=PhoneDefault;
	System.SystemApp.DeleteAllRemote=RemoteDefaults;
	System.SystemApp.DeleteAllDetector=ZoneDefaults;
}

