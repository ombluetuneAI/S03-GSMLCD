#include "TaskMenu.h"

#include "system.h"

#include "boot.h"

#include "APP.h"
#include "GUI.h"
#include "GUIDEMO.h"
#include "DIALOG.h"

#define MenuFont GUI_FONT_20_ASCII

#define PhoneList			    0
#define SetupList 			  1
#define SecurityList  		2
#define VoiceList			    4


extern void SettingTab(void);
extern void ContactsTab(void);
extern void RemoteInit(void);
extern void VoiceInit(void);
extern void SecurintyInit(void);
extern void LogMenuInit(void);
extern void AboutMenuInit(void);


extern const unsigned char gImage_contacts[6280];
extern const unsigned char gImage_phone[6280];
extern const unsigned char gImage_sensors[6280];
extern const unsigned char gImage_recorder[6280];
extern const unsigned char gImage_setting[6280];
extern const unsigned char gImage_security[6280];
extern const unsigned char gImage_log[6280];
extern const unsigned char gImage_about[6280];

extern const unsigned char gImage_alarm[12808];   

//Flash中的图标地址
#define  AddBASE     2048  //基地址

u8* const MenuIconPath[8]=
{
  "0:/SYSTEM/SPB/ICOS/sms_110.bmp",
  "0:/SYSTEM/SPB/ICOS/phone_110.bmp",
  "0:/SYSTEM/SPB/ICOS/wireless_110.bmp",
  "0:/SYSTEM/SPB/ICOS/recorder_110.bmp",
  "0:/SYSTEM/SPB/ICOS/set_110.bmp",
  "0:/SYSTEM/SPB/ICOS/setup_110.bmp",
  "0:/SYSTEM/SPB/ICOS/notepad_110.bmp",
  "0:/SYSTEM/SPB/ICOS/ebook_110.bmp",
};

const u8 *IconPath[4]=
{
	"0:/ICON/bigICon/Home.bmp",
	"0:/ICON/bigICon/Lock.bmp",
  "0:/ICON/bigICon/Unlock.bmp",
  "0:/ICON/bigICon/alarm.bmp",
};
const u8 *BatterPath[8]=
{
    "0:/ICON/batter/CI.bmp",
    "0:/ICON/batter/CII.bmp",
    "0:/ICON/batter/CIII.bmp",
    "0:/ICON/batter/CIIII.bmp",
    "0:/ICON/batter/I.bmp",
    "0:/ICON/batter/II.bmp",
    "0:/ICON/batter/III.bmp",
    "0:/ICON/batter/IIII.bmp",
};






const string Months[12]=
{
	"Jan.","Feb.","Mar.","Apr.","May","Jun.",
	"Jul.","Aug.","Sep.","Oct.","Nov.","Dec.",

};
/*****************************Flash中的图片地址*******************************/

const u32 FlashAdd[20][20]=
{
	{
		Phonemember,
		PhoneAddMember,		 
		PhoneDelete,   
		PhoneSetup,		 
	},
	{
		SetupDelay,
		SetupTime,
		SetupAuto,
		SetupVolume,
		SetupKeyTone,
		SetupSMS,
		SetupLanguage,
		SetupAlarm,
		SetupSiren,
		SetupRing,
		SetupScreensaver,
	},
	{
		SecurityUser,
		SecurityDefault,
	},
	{
		AlarmRecord,
		DisarmRecord,
		ArmRecord,
	},
	{
		VoiceRecord,
		VoicePlay,
	},
	{
		Remote,
		ZoneAdd,
		ZoneList,	
		ZoneDelete,
		ZoneSetup,		
	},
		
};

/******************************************************************************
*列表功能标题

*****************************************************************************/
const string ListString[9][16] = 
{
  {
		"Alarm Number",
		"Add phone number",
		"Delete the phone number",
		"Phone number setup",
	},
	{
		"Delay",
		"Auto Arm/Disarm",//定时布防
		"Date and Time",
		"Volume Control",
		"KeyTone",
		"Language",
		"SMS Setting",//布撤防短信
		//"Power alarm",
		"Siren Time",
		"Ring Times",
		"Screensaver",
		"Contact ID",
		"Panel Power Off",
    "Alarm Mode",
		"Panel power on",
		"Panel low battery",
		"Detector low bttery"
	},
	{
		"Password Management",
		"Reset Factory Defualts",
	},
	{
		"Alarm Record",
		"Disarm Record",
		"Arm/Stay Record",
	},
	{
		"Record",
		"Play Recroding",
		"Voice Record and Play",
	},
	{
		"Remote",
		"Security Zone List",
		"Wireless Siren",
		"Ready to Arm",
	}
};

/******************列表名字*****************************/
const string MenuListName[8]=
{
	"Contacts",
	"System Setting",
	"Security",
	"Log",
	"Voice",
	"Sensors",
	"System Alarming",
	"Voice Record and Play "
};

u8* const IconListName[8]=
{
  "Contacts",
  "Phone",
  "Sensors",
  "Recording",
  "Setting",
  "Security",
  "Log",
  "About"
};

/****************************************************************************************************
*描述       :   列表项目添加
*输入参数   ：  list,指向的列表框
                name,条目名
								Size,条目总数
								iconAdd,列表图标的地址
****************************************************************************************************/

#define GRED 			 0XFFE0
extern void StandbyStateGet(void);
/***************************************************************************************************


***************************************************************************************************/
void OpenRF(void)
{
//    System.Device.Gsm.GsmATSend("AT+CSQ","OK",100);
//    System.Device.Gsm.GsmATSend("AT+COPS?","OK",1);
    App.Data.UseWireless=1;//打开接收
	TIM_Cmd(TIM7, ENABLE);
}

void WirelessOpen(void)
{
    StandbyStateGet();
    System.Device.Timer.Start(RFOpenTimeID,TimerMessageHandle,1000,OpenRF);
}


/***************************************************************************************************

***************************************************************************************************/



/****************************************************************************
*
*
*
*
*
****************************************************************************/
static void AlarmFormInit(void)
{

	static Icon 	iconLabel;
	static Label	alarmLabel;
/*--------------------待机-------------------------------*/
	System.Gui.Form.Init(&App.Menu.AlarmForm);	
	App.Menu.AlarmForm.FormID=Form8;
	App.Menu.AlarmForm.IconCount=1;
	App.Menu.AlarmForm.LabelCount=1;
	App.Menu.AlarmForm.RectWrap.x0=0;
	App.Menu.AlarmForm.RectWrap.y0=30;
	App.Menu.AlarmForm.RectWrap.x1=320;
	App.Menu.AlarmForm.RectWrap.y1=240;
	App.Menu.AlarmForm.Color= BLUE;


	System.Gui.Form.AddIcon(&App.Menu.AlarmForm, &iconLabel);
	iconLabel.ID=IconID1+1;
	iconLabel.x = 130;
	iconLabel.y =50;
  iconLabel.Type = Image2Lcd;
	iconLabel.IconPointer=(u8*)gImage_alarm;	
	
	System.Gui.Form.AddLabel(&App.Menu.AlarmForm,&alarmLabel);
	alarmLabel.ID=LabelID1;
	alarmLabel.Font=ListFont;
	alarmLabel.PaintColor=GUI_WHITE;
	alarmLabel.TextMode=GUI_TEXTMODE_TRANS;
	alarmLabel.Count=6;
	alarmLabel.StringBlockPointer=MenuListName;
	alarmLabel.X=100;
	alarmLabel.Y=200;
}
/******************************************************************************
*
*
*
* 
*
******************************************************************************/
static void StandbyMenu(void)
{
//	static FillArea BottomArea;
	static FillArea TopArea;
	static Icon 	iconLabel;
	static Icon 	iconLabel1;
  static Icon 	iconLabel2;
    //static Label    hour,min,sec,year,month,day,week;
    
/*--------------------待机-------------------------------*/
	System.Gui.Form.Init(&App.Menu.StandbyForm);	
	App.Menu.StandbyForm.FormID=Form0;
	App.Menu.StandbyForm.IconCount=3;
	App.Menu.StandbyForm.LabelCount=0;
	App.Menu.StandbyForm.FillAreaCount=1;
	App.Menu.StandbyForm.RectWrap.x0=0;
	App.Menu.StandbyForm.RectWrap.y0=30;
	App.Menu.StandbyForm.RectWrap.x1=320;
	App.Menu.StandbyForm.RectWrap.y1=210;
	App.Menu.StandbyForm.Color= GUI_ORANGE;
    
 
	System.Gui.Form.AddFillArea(&App.Menu.StandbyForm,&TopArea);
	TopArea.ID=0xFE;
	TopArea.PaintColor=BLACK;
	TopArea.RectWrap.x0=0;
	TopArea.RectWrap.y0=0;
	TopArea.RectWrap.x1=319;
	TopArea.RectWrap.y1=30;


	System.Gui.Form.AddIcon(&App.Menu.StandbyForm, &iconLabel);
	iconLabel.ID=IconID1+1;
	iconLabel.x = 42;
	iconLabel.y =150;
	iconLabel.Type=Image2Lcd;
	iconLabel.IconPointer=(u8*)gImage_home;	

	System.Gui.Form.AddIcon(&App.Menu.StandbyForm, &iconLabel1);
	iconLabel1.ID=IconID1+2;
	iconLabel1.x = 132;
	iconLabel1.y =150;
	iconLabel1.Type=Image2Lcd;
	iconLabel1.IconPointer=(u8*)gImage_Arm;	
    
	System.Gui.Form.AddIcon(&App.Menu.StandbyForm, &iconLabel2);
	iconLabel2.ID=IconID1+3;
	iconLabel2.x = 224;
	iconLabel2.y =150;
	iconLabel2.Type=Image2Lcd;
	iconLabel2.IconPointer=(u8*)gImage_Unlock;
    
//     System.Gui.Form.AddIcon(&App.Menu.StandbyForm, &iconLabelBatter);
//     iconLabelBatter.ID=IconID1+5;
//     iconLabelBatter.x = 285;
//     iconLabelBatter.y =0;
//     iconLabelBatter.BitmapPointer=(void*)BatterPath[7];

	System.Device.Storage.Parameter.StorageReadData(SystemModeAddr,&App.Data.SystemMode,1);
	if(App.Data.SystemMode==SystemStayMode)
	{
		iconLabel.IconPointer =(u8*)gImage_Arm;	
		iconLabel1.IconPointer=(u8*)gImage_home;	
		iconLabel2.IconPointer=(u8*)gImage_Unlock;	
	}
	else if(App.Data.SystemMode==SystemDisarmMode)
	{
		iconLabel.IconPointer =(u8*)gImage_Arm;	
		iconLabel1.IconPointer=(u8*)gImage_Unlock;	
		iconLabel2.IconPointer=(u8*)gImage_home;	
	}
	else if(App.Data.SystemMode==SystemArmMode)
	{
		iconLabel.IconPointer =(u8*)gImage_Unlock;	
		iconLabel1.IconPointer=(u8*)gImage_Arm;	
		iconLabel2.IconPointer=(u8*)gImage_home;	
	}
	App.Data.SystemModeUpdate=App.Data.SystemMode;

}
void StandbyStateGet(void)
{
    StandbyMenu();
}

extern u8 UserUpdataVal;
void RecTimeUpdate(void)
{
	AutoDigit    *autoDigit;
	
    if(App.Menu.FocusFormPointer==&App.Menu.VoiceListForm)
    {
      if(App.Data.Sec!=Calendar.Sec)
      {
        App.Data.Sec=Calendar.Sec;
        if(App.Data.RecTimeValue==0)
        {
           if(App.Menu.FocusFormPointer->AutoDigitPointer->Value==9)
           {
             System.Device.Gsm.GsmATSend("AT+CREC=2","OK",10);//停止录音
             App.Data.RecordKey = 0;
             UserUpdataVal = 2;
//              LineDataToWTR050(0xFE);
             return;
           }
           App.Menu.FocusFormPointer->AutoDigitPointer->Value++;
        }
        else if(App.Data.RecTimeValue==9)
        {
          if(App.Menu.FocusFormPointer->AutoDigitPointer->Value==0)
          {
            System.Device.Gsm.GsmATSend("AT+CREC=5","OK",10);//停止播放
            App.Data.PlayKey = 0;
            UserUpdataVal = 4;
//             LineDataToWTR050(0xFE);
            return;
          }
          App.Menu.FocusFormPointer->AutoDigitPointer->Value--;
        }
                
      }
    }
	else if(&App.Menu.DialForm==App.Menu.FocusFormPointer)
	{
		if(App.Data.Sec!=Calendar.Sec)
    {
      App.Data.Sec=Calendar.Sec;
			autoDigit=App.Menu.FocusFormPointer->AutoDigitPointer;
			autoDigit->Value++;
			if(autoDigit->Value==59)
			{
				autoDigit->Value=0;
				autoDigit=autoDigit->NextPointer;
				autoDigit->Value++;
			}
		}
	}	
}


/*****************************************************************************************************

*****************************************************************************************************/
void CallInit(void);


void IconMenuInit()
{
  static Icon 	ContactsIconLabel;
  static Icon 	PhoneIconLabel;
  static Icon 	SensorsIconLabel;
  static Icon 	VoiceIconLabel;
  static Icon 	SettingIconLabel;
  static Icon 	SecurityIconLabel;
  static Icon 	LogIconLabel;
  static Icon 	AboutIconLabel;
  
  u16 x=0,y=0;
  u16 xoffset=0;
  xoffset = (320 - MainIconSize*4)/5;
  x = xoffset;
  y = 45;

/*---------------------------联系人界面----------------------------*/		
	System.Gui.Form.Init(&App.Menu.ContactsForm);
	App.Menu.ContactsForm.FormID=IconMenuForm;
  App.Menu.ContactsForm.IconCount=8;
	App.Menu.ContactsForm.Color=GUI_WHITE;
	App.Menu.ContactsForm.RectWrap.x0=0;
	App.Menu.ContactsForm.RectWrap.y0=30;
	App.Menu.ContactsForm.RectWrap.x1=320;
	App.Menu.ContactsForm.RectWrap.y1=240;

	//联系人图标
	System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &ContactsIconLabel);
	ContactsIconLabel.ID=IconID1;
	ContactsIconLabel.x = x;
	ContactsIconLabel.y = y;
  x += xoffset + MainIconSize;
	ContactsIconLabel.StringPointer=IconListName[0];
	ContactsIconLabel.IconPointer=(u8*)gImage_contacts;
  
  System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &PhoneIconLabel);
	PhoneIconLabel.ID=IconID2;
	PhoneIconLabel.x = x;
	PhoneIconLabel.y =y;
  x += xoffset + MainIconSize;
	PhoneIconLabel.StringPointer=IconListName[1];
	PhoneIconLabel.IconPointer=(u8*)gImage_phone;
   
  System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &SensorsIconLabel);
	SensorsIconLabel.ID=IconID3;
	SensorsIconLabel.x = x;
	SensorsIconLabel.y =y;
  x += xoffset + MainIconSize;
	SensorsIconLabel.StringPointer=IconListName[2];
	SensorsIconLabel.IconPointer=(u8*)gImage_sensors;
     
  System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &SecurityIconLabel);
	SecurityIconLabel.ID=IconID4;
	SecurityIconLabel.x = x;
	SecurityIconLabel.y =y;
  x = xoffset;
  y += 95;
	SecurityIconLabel.StringPointer=IconListName[3];
	SecurityIconLabel.IconPointer=(u8*)gImage_security;
  
  System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &VoiceIconLabel);
	VoiceIconLabel.ID=IconID5;
	VoiceIconLabel.x = x;
	VoiceIconLabel.y =y;
  x += xoffset + MainIconSize;
	VoiceIconLabel.StringPointer=IconListName[4];
	VoiceIconLabel.IconPointer=(u8*)gImage_recorder;
   
  System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &SettingIconLabel);
	SettingIconLabel.ID=IconID6;
	SettingIconLabel.x = x;
	SettingIconLabel.y =y;
  x += xoffset + MainIconSize;
	SettingIconLabel.StringPointer=IconListName[5];
	SettingIconLabel.IconPointer=(u8*)gImage_setting;
   
  System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &LogIconLabel);
	LogIconLabel.ID=IconID7;
	LogIconLabel.x = x;
	LogIconLabel.y =y;
  x += xoffset + MainIconSize;
	LogIconLabel.StringPointer=IconListName[6];
	LogIconLabel.IconPointer=(u8*)gImage_log;
   
  System.Gui.Form.AddIcon(&App.Menu.ContactsForm, &AboutIconLabel);
	AboutIconLabel.ID=IconID8;
	AboutIconLabel.x = x;
	AboutIconLabel.y =y;
	AboutIconLabel.StringPointer=IconListName[7];
	AboutIconLabel.IconPointer=(u8*)gImage_about;
}



void AllMenuListInit(void)
{
  StandbyMenu();
  AlarmFormInit();
  
  ContactsTab();
  CallInit();
  RemoteInit();
  VoiceInit();
  SettingTab();
  SecurintyInit();
  LogMenuInit();
  AboutMenuInit();
}
/*******************************************************

*******************************************************/
void MenuInit(void)
{			
	IconMenuInit();
  AllMenuListInit();
}
/****************************************************************************

****************************************************************************/
void OpenRx(void)
{
    App.Data.UseWireless=1;//打开接收
    DelayMs(500);
}
/***************************************************************************



***************************************************************************/
void MenuTask (void)
{
	static u16 delay=0;
	MenuInit();
	System.Device.Timer.Start(RFOpenTimeID,TimerSystickHandle,1000,OpenRx);
	while(true)
	{
		if(++delay==200)
		{
			delay=0;
			System.Gui.Parse(App.Menu.FocusFormPointer);
		}
	}
}


















