
#include "SetupMenu.h"
#include "APP.h"


/************************常数*********************************/
#define Delay			0
#define Auto			1
#define Date			2
#define Volume			3
#define KeyTone			4
#define Language		5
#define ArmOrDisarmSMS	6
#define PowerAlarm		7
#define Siren			8
#define	Ring			9
#define Screensaver		10
#define CIDString		11
#define Power			12
#define AlarmModeString 13

#define	KeyToneCheckBox		0
#define LanguageCheckBox	1
#define SMSCheckBox			2
#define ScreensaverCheckBox	3
#define CIDCheckBox			4
#define PowerOFFCheckBox	5
#define	PowerONCheckBox		6
#define PowerLowCheckBox	7
#define DetectorCheckBox	8
#define AlarmModeSelete     9

/**************************************************变量********************************************************************************/
extern const unsigned char gImage_setIco[][808];



const u8* SettingMenuTab[18] = 
 {
    "System Setting",
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
		"States Info",
    "Alarm Mode",
		"Panel power on",
		"Panel low battery",
		"Detector low bttery"
};
 
//图标位置
const u8 *setupPath[13]=
{
    "0:/ICON/setup/delay.bmp",
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/date.bmp",
    "0:/ICON/setup/vol.bmp",
    "0:/ICON/setup/key_tone.bmp",
    "0:/ICON/setup/language.bmp",
    "0:/ICON/setup/sms.bmp",
    "0:/ICON/setup/siren.bmp",
    "0:/ICON/setup/ring.bmp",
    "0:/ICON/setup/screen.bmp",
    "0:/ICON/setup/smsRep.bmp",
    "0:/ICON/setup/power.bmp",
    "0:/ICON/setup/3GorPSTN.bmp",
};
//列表名
const string SetupString[15][6]=
{
	{"Delay Setting","Exit Delay(s)","Entry Delay(s)",":","(0~250s)"},
	{"Auto Arm/Disarm ",":","Arm/Disarm:","Week:"},
	{"Date and time","Year","Month","Day","Date :","Time :",},
	{"Volume","Voice Volume :","Alarm Volume :","(0~8)"},
	{"Key tone","Key Tone",},
	{"Language","English"},
	{"Arm/Disarm SMS","Arm SMS Notice","Disarm SMS Notice","Stay SMS Notice"},
	{"Power Alarm"," Power Failure Report","panel Power On","Panel Low Battery","Detector Low Battery"},
	{"Siren","Siren Hooting Time:","Min","( 0-20Min )",},
	{"Ring Times","Ring Times :","Min","(0~20Times)"},
	{"Screensaver"},
	{"Contact ID","Arm Upload","Stay Upload","Disarm Upload","User ID:","Report Effect:"},
	{"Voice","Siren","SMS","Call"},
  {"3G","PSTN","3G/PSTN","Alarm mode"},
	{"States Info,"},
};
const string 	WeekStringAuto[1][7]={"Mon","tue","Wed","Thu","Fri","sat","Sun"};
//用到的复选择框的状态 记录地址
static const u32 CheckBoxState[10][6]=
{
	{KeyToneStateAddr,},
	{LanguageSetAddr,},
	{ArmSMSNoticeAddr,DisarmSMSNoticeAddr,HomeSMSNoticeAddr,},
	{Screen30SecAddr,Screen1MinAddr,Screen2MinAddr,Screen5MinAddr,Screen10MinAddr,ScreenNeverAddr,},
	{CIDArmUploadAddr,CIDStayUploadAddr,CIDDisarmUploadAddr,},
	{PowerOFFVoiceAddr,PowerOFFSirenAddr,PowerOFFSMSAddr,PowerOFFCallAddr},
	{PowerONVoiceAddr,PowerONSirenAddr,PowerONSMSAddr,PowerONCallAddr},
	{PowerLowVoiceAddr,PowerLowSirenAddr,PowerLowSMSAddr,PowerLowCallAddr},
	{DetectorVoiceAddr,DetectorSrienAddr,DetectorSMSAddr,DetectorCallAddr},
    {Alarm3GAddr,AlarmPSTNAddr,Alarm3gAndPSTNAddr},
};

static const u32 AutoStateAddr[8]=
{
    Auto1State,
    Auto2State,
    Auto3State,
    Auto4State,
    Auto5State,
    Auto6State,
    Auto7State,
    Auto8State,
};
const string WeekString[3][7] = 
{
	{
		"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday",
	},
	{
		"Arm time","Disarm time","Hour","Min","Sec",
	},
	{
		"30 Seconds","1 Minute","2 Minute","5 Minute","10 Minute","Never",
	}
};
/***************************/
const u32 SetupIconAdd[2][7]=
{
	{
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,	
	},
	{
		PowerOn,
		PowerOff,
		BatteryLow,
		Detector,		
	}
};

const string AutoPath[8]=
{
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/auto.bmp",
    "0:/ICON/setup/auto.bmp",
};
const u32 AutoArmDisarmAddr[8]=
{
	Auto1Addr,
	Auto2Addr,
	Auto3Addr,
	Auto4Addr,
	Auto5Addr,
	Auto6Addr,
	Auto7Addr,
	Auto8Addr,
};
/******************************************函数************************************************/


/***************************************************************************************************

***************************************************************************************************/
static void SettingMenuList(void)
{
	static ListBox 	SetupListBox; 
	static Label 	SetupListName;
	static FillArea SetupFunc;
	static Icon 	SetupIconLabel;
	string *items;
	items=(string *)&SettingMenuTab[1];
/*--------------------------设置界面--------------------------------*/
				
	//列表层
	System.Gui.Form.Init(&App.Menu.SetupListForm);
	App.Menu.SetupListForm.ListCount=1;
	App.Menu.SetupListForm.FillAreaCount=1;
	App.Menu.SetupListForm.LabelCount=1;
	App.Menu.SetupListForm.FormID=Form5;
	App.Menu.SetupListForm.Color=GUI_WHITE;
	App.Menu.SetupListForm.RectWrap.x0=0;
	App.Menu.SetupListForm.RectWrap.y0=30;
	App.Menu.SetupListForm.RectWrap.x1=320;
	App.Menu.SetupListForm.RectWrap.y1=240;
	
	System.Gui.Form.AddFillArea(&App.Menu.SetupListForm,&SetupFunc);
	SetupFunc.ID=PromptAreaID6;
	SetupFunc.PaintColor=TopLabelColor;
	SetupFunc.RectWrap.x0=0;
	SetupFunc.RectWrap.y0=30;
	SetupFunc.RectWrap.x1=320;
	SetupFunc.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.SetupListForm,&SetupListName);
	SetupListName.ID=LabelID1+6;
	SetupListName.Font=ListNameFont;
	SetupListName.PaintColor=GUI_BLACK;
	SetupListName.TextMode=GUI_TEXTMODE_TRANS;
	SetupListName.Count=0;
	SetupListName.StringBlockPointer=(const string *)&SettingMenuTab[0];
	SetupListName.X=85;
	SetupListName.Y=43;
  
	//列表
	System.Gui.Form.AddListBox(&App.Menu.SetupListForm,&SetupListBox);
	SetupListBox.ID=ListBoxID2;
	SetupListBox.x0=0;
	SetupListBox.y0=80;
	SetupListBox.x1=320;
	SetupListBox.y1=210;
	SetupListBox.Type=IconList;
	SetupListBox.selectedItem=0;
  SetupListBox.ItemsHeight=40;
	SetupListBox.ItemsPerpage=4;//页最大条目
	SetupListBox.SelectedIndexes=0;//默认为0，也就是第一条
	SetupListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	SetupListBox.TotalItems=0;
	
	SetupListBox.TextMode=GUI_TEXTMODE_TRANS;
	SetupListBox.BackColor=GUI_WHITE;
	SetupListBox.PaintColor=GUI_BLACK;
	
	SetupListBox.SelectedBackColor=MoveBarColor;
	SetupListBox.SelectedPaintColor=GUI_WHITE;
	
	SetupListBox.Font=ListFont;
	SetupListBox.ID=0x41;

	System.Gui.Form.AddLists(&SetupListBox,items,13,null,null,null,(u8**)gImage_setIco,13);
}

/*********************************************************************************

*描述		：延时设定（延时布防延时报警）
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void Delayfunc(void)
{	
	u8 i;
	//延时设置UI
	static FillArea 	delayArea[3];
	static Label		delayName;
	static Label		armName;
	static Label		disarmName;
	static Label		delayScope[2];
	static TextBox		armTextBox;
	static TextBox		alarmTextBox;
	
	
	System.Gui.Form.Init(&App.Menu.DelayForm);
	App.Menu.DelayForm.FillAreaCount=3;
	App.Menu.DelayForm.LabelCount=5;
	App.Menu.DelayForm.TextBoxCount=2;
	App.Menu.DelayForm.FormID=Form9+12;
	App.Menu.DelayForm.Color=GUI_WHITE;
	App.Menu.DelayForm.RectWrap.x0=0;
	App.Menu.DelayForm.RectWrap.y0=30;
	App.Menu.DelayForm.RectWrap.x1=320;
	App.Menu.DelayForm.RectWrap.y1=240;
	App.Menu.DelayForm.DefaultFocusTextBoxPointer=&armTextBox;
	App.Menu.DelayForm.FocusTextBoxPointer=&armTextBox;//当前页面焦点从ArmTextBox开始
	
	alarmTextBox.ParentTextBoxPointer=&armTextBox;
	armTextBox.ParentTextBoxPointer=&alarmTextBox;
	

	System.Gui.Form.AddFillArea(&App.Menu.DelayForm,&delayArea[0]);
	delayArea[0].ID=PromptAreaID5;
	delayArea[0].PaintColor=TopLabelColor;
	delayArea[0].RectWrap.x0=0;
	delayArea[0].RectWrap.y0=30;
	delayArea[0].RectWrap.x1=320;
	delayArea[0].RectWrap.y1=80;
	
	for(i=1;i<3;i++)
	{
		System.Gui.Form.AddFillArea(&App.Menu.DelayForm,&delayArea[i]);
		delayArea[i].ID=PromptAreaID6+i;
		delayArea[i].PaintColor=GUI_LIGHTGRAY;
		delayArea[i].RectWrap.x0=3;
		delayArea[i].RectWrap.y0=140+((i-1)*50);
		delayArea[i].RectWrap.x1=317;
		delayArea[i].RectWrap.y1=140+((i-1)*50);	
	}

	
	System.Gui.Form.AddLabel(&App.Menu.DelayForm,&delayName);
	delayName.ID=LabelID1+10;
	delayName.Font=ListNameFont;
	delayName.PaintColor=GUI_BLACK;
	delayName.TextMode=GUI_TEXTMODE_TRANS;
	delayName.Count=0;
	delayName.StringBlockPointer=SetupString[Delay];
	delayName.X=130;
	delayName.Y=45;	
	
	System.Gui.Form.AddLabel(&App.Menu.DelayForm,&armName);
	armName.ID=LabelID1+10;
	armName.Font=GUI_FONT_20_ASCII;
	armName.PaintColor=GUI_BROWN;
	armName.TextMode=GUI_TEXTMODE_TRANS;
  armName.Type=0xC0;
	armName.Count=1;
	armName.StringBlockPointer=SetupString[Delay];
	armName.X=10;
	armName.Y=115;

	System.Gui.Form.AddLabel(&App.Menu.DelayForm,&disarmName);
	disarmName.ID=LabelID1+10;
	disarmName.Font=GUI_FONT_20_ASCII;
	disarmName.PaintColor=GUI_BROWN;
	disarmName.TextMode=GUI_TEXTMODE_TRANS;
  disarmName.Type=0xC0;
	disarmName.Count=2;
	disarmName.StringBlockPointer=SetupString[Delay];
	disarmName.X=10;
	disarmName.Y=165;
	for(i=0;i<2;i++)
	{
		System.Gui.Form.AddLabel(&App.Menu.DelayForm,&delayScope[i]);
		delayScope[i].ID=LabelID1+10+i;
		delayScope[i].Font=GUI_FONT_16_1;
		delayScope[i].PaintColor=GUI_GRAY;
		delayScope[i].TextMode=GUI_TEXTMODE_TRANS;
    delayScope[i].Type=0xC0;
		delayScope[i].Count=4;
		delayScope[i].StringBlockPointer=SetupString[Delay];
		delayScope[i].X=250;
		delayScope[i].Y=120+(i*50);
	}	
	
	System.Gui.Form.AddTextBox(&App.Menu.DelayForm,&armTextBox);
	armTextBox.x=196;
	armTextBox.y=116;
	armTextBox.MaxDataPointer=255;
	armTextBox.MinDataPointer=0;
	armTextBox.Type=GuiDataTypeByteDec;
	armTextBox.BackColor=GUI_LIGHTGRAY;
	armTextBox.PaintColor=GUI_RED;
	armTextBox.Font=GUI_FONT_20_ASCII;
	armTextBox.ID=0x01;
	armTextBox.TextMode=GUI_TEXTMODE_NORMAL;
	armTextBox.RectWrap.x0=196;
	armTextBox.RectWrap.x1=245;
	armTextBox.RectWrap.y0=116;
	armTextBox.RectWrap.y1=136;		
	armTextBox.DataPointer=&App.Data.ArmDelay;	
	armTextBox.DataAddr=ArmDelayAddr;	
	System.Device.Storage.Parameter.StorageReadData(ArmDelayAddr,&App.Data.ArmDelay,1);
	
	System.Gui.Form.AddTextBox(&App.Menu.DelayForm,&alarmTextBox);
	alarmTextBox.x=196;
	alarmTextBox.y=166;
	alarmTextBox.MaxDataPointer=255;
	alarmTextBox.MinDataPointer=0;
	alarmTextBox.Type=GuiDataTypeByteDec;
	alarmTextBox.BackColor=GUI_LIGHTGRAY;
	alarmTextBox.PaintColor=GUI_RED;
	alarmTextBox.Font=GUI_FONT_20_ASCII;
	alarmTextBox.ID=0x02;
	alarmTextBox.TextMode=GUI_TEXTMODE_TRANS;	
	alarmTextBox.RectWrap.x0=196;
	alarmTextBox.RectWrap.x1=245;
	alarmTextBox.RectWrap.y0=166;
	alarmTextBox.RectWrap.y1=186;
	alarmTextBox.DataAddr=AlarmDelayAddr;
	alarmTextBox.DataPointer=&App.Data.AlarmDelay;	
	//System.Device.Storage.Parameter.StorageReadData(AlarmDelayAddr,&App.Data.AlarmDelay,1);	
	
}

/*********************************************************************************

*描述		：定时UI(8组定时)
*输入参数	：无
*输出参数	：无

*********************************************************************************/

static void SetupAutoFunc(void)
{
	//定时布撤防UI
	static FillArea		autoArea;
	static Label		autoName;
	static ListBox		AutoListBox;
	string *items=(string*)AutoPath;
	         
	System.Gui.Form.Init(&App.Menu.AutoForm);
	App.Menu.AutoForm.FillAreaCount=1;
	App.Menu.AutoForm.ListCount=1;
	App.Menu.AutoForm.LabelCount=1;
	App.Menu.AutoForm.FormID=Form12;
	App.Menu.AutoForm.Color=GUI_WHITE;
	App.Menu.AutoForm.RectWrap.x0=0;
	App.Menu.AutoForm.RectWrap.y0=30;
	App.Menu.AutoForm.RectWrap.x1=320;
	App.Menu.AutoForm.RectWrap.y1=240;	
	
	System.Gui.Form.AddFillArea(&App.Menu.AutoForm,&autoArea);
	autoArea.ID=PromptAreaID12;
	autoArea.PaintColor=TopLabelColor;
	autoArea.RectWrap.x0=0;
	autoArea.RectWrap.y0=30;
	autoArea.RectWrap.x1=320;
	autoArea.RectWrap.y1=80;

	System.Gui.Form.AddLabel(&App.Menu.AutoForm,&autoName);
	autoName.ID=LabelID1+13;
	autoName.Font=ListNameFont;
	autoName.PaintColor=GUI_BLACK;
	autoName.TextMode=GUI_TEXTMODE_TRANS;
	autoName.Count=0;
	autoName.StringBlockPointer=SetupString[Auto];
	autoName.X=55;
	autoName.Y=43;
	
	System.Gui.Form.AddListBox(&App.Menu.AutoForm,&AutoListBox);
	AutoListBox.x0=0;
	AutoListBox.y0=80;
	AutoListBox.x1=320;
	AutoListBox.y1=210;
	AutoListBox.xSize=15;
	AutoListBox.ySize=15;
	
	AutoListBox.Select=4;
	AutoListBox.ItemsHeight=40;
	AutoListBox.selectedItem=0;
	AutoListBox.ItemsPerpage=4;//页最大条目
	AutoListBox.SelectedIndexes=0;//默认为0，也就是第一条
	AutoListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	AutoListBox.TotalItems=0;
	
	AutoListBox.TextMode=GUI_TEXTMODE_TRANS;
	AutoListBox.BackColor=GUI_WHITE;
	AutoListBox.PaintColor=GUI_BLACK;
	
	AutoListBox.SelectedBackColor=MoveBarColor;
	AutoListBox.SelectedPaintColor=GUI_WHITE;
	
	AutoListBox.Font=ListFont;
	AutoListBox.ID=0x01;
	//AutoListBox.Type=IconList;
    AutoListBox.Type=SwitchList;

	System.Gui.Form.AddLists(&AutoListBox,items,8,(u32*)AutoStateAddr,null,(u32*)(AutoArmDisarmAddr),null,8);

	//System.Gui.Form.AddLists(&AutoListBox,null,8,null,(u32*)NULL,(u32*)(AutoArmDisarmAddr),items);
}
/***************************************************************************************************
*
*描述       ：定时布撤防UI 初始化 一个定时组占用30个字节 ，初始状态是自动布防，定时器是关闭的
*                0-15，       16             17     18    19     20      21		22-28
                状态显示    状态           保留    时    保留   分     保留     星期一到星期天
                            布防，0xF0
                            撤防，0xF1
*输入参数   ：无
*输出参数   ：无
*
***************************************************************************************************/
static void AutoArmDisarmInit(void)
{
	static FillArea		autoArea;
	static Label		autoName;
	static Label 		secSymbol;	
    static Label        stateLabel;
	static Label 		armDisarm;
	static Label 		week;
	static TextBox 		autoHourBox;
	static TextBox 		autoMinBox;
    
    static ArCheck      monBox[7];
    u8 i;
    static FocusSturct focus[10];
    
    for(i=0;i<10;i++)
    {
        System.Gui.Form.AddFocus(&App.Menu.AutoSetupForm,&focus[i]);
    }
    focus[0].BoxType=TextBoxType; 
    focus[0].FocusPointer=(void*)&autoHourBox;
    focus[0].ParentPointer=&focus[9];
    focus[0].NextPointer=&focus[1];
    
    focus[1].BoxType=TextBoxType;
    focus[1].FocusPointer=(void*)&autoMinBox;
    focus[1].ParentPointer=&focus[0];
    focus[1].NextPointer=&focus[2];
    
    focus[2].BoxType=LabelBoxType;
    focus[2].FocusPointer=(void*)&stateLabel;
    focus[2].ParentPointer=&focus[1];
    focus[2].NextPointer=&focus[3];

//-------------------------------星期一 -----------------------------------------
    focus[3].BoxType=ArCheckBoxType;
    focus[3].FocusPointer=(void*)&monBox[0];
    focus[3].ParentPointer=&focus[2];
    focus[3].NextPointer=&focus[4];

//------------------------------星期二 ------------------------------------------
    focus[4].BoxType=ArCheckBoxType;
    focus[4].FocusPointer=(void*)&monBox[1];
    focus[4].ParentPointer=&focus[3];
    focus[4].NextPointer=&focus[5];
    
//------------------------------星期三 -------------------------------------------
    focus[5].BoxType=ArCheckBoxType;
    focus[5].FocusPointer=(void*)&monBox[2];
    focus[5].ParentPointer=&focus[4];
    focus[5].NextPointer=&focus[6];


//-----------------------------星期四 --------------------------------------------
    focus[6].BoxType=ArCheckBoxType;
    focus[6].FocusPointer=(void*)&monBox[3];
    focus[6].ParentPointer=&focus[5];
    focus[6].NextPointer=&focus[7];

//-----------------------------星期五 --------------------------------------------
    focus[7].BoxType=ArCheckBoxType;
    focus[7].FocusPointer=(void*)&monBox[4];
    focus[7].ParentPointer=&focus[6];
    focus[7].NextPointer=&focus[8];
    
//-----------------------------星期六 --------------------------------------------
    focus[8].BoxType=ArCheckBoxType;
    focus[8].FocusPointer=(void*)&monBox[5];
    focus[8].ParentPointer=&focus[7];
    focus[8].NextPointer=&focus[9];

//-----------------------------星期天 --------------------------------------------
    focus[9].BoxType=ArCheckBoxType;
    focus[9].FocusPointer=(void*)&monBox[6];
    focus[9].ParentPointer=&focus[8];
    focus[9].NextPointer=&focus[0];
    
    System.Gui.Form.Init(&App.Menu.AutoSetupForm);
    App.Menu.AutoSetupForm.DefaultFocus=&focus[0];
    App.Menu.AutoSetupForm.FocusPointer=&focus[0];
    App.Menu.AutoSetupForm.FillAreaCount=1;
    App.Menu.AutoSetupForm.ArCheckCount=7;
    App.Menu.AutoSetupForm.LabelCount=5;
    App.Menu.AutoSetupForm.TextBoxCount=2;
    App.Menu.AutoSetupForm.FormID=Form9+15;
    App.Menu.AutoSetupForm.Color=GUI_WHITE;
    App.Menu.AutoSetupForm.RectWrap.x0=0;
    App.Menu.AutoSetupForm.RectWrap.y0=30;
    App.Menu.AutoSetupForm.RectWrap.x1=320;
    App.Menu.AutoSetupForm.RectWrap.y1=240;
	App.Menu.AutoSetupForm.DefaultFocusTextBoxPointer=&autoHourBox;	
	App.Menu.AutoSetupForm.FocusTextBoxPointer=&autoHourBox;


    autoHourBox.ParentTextBoxPointer=&autoMinBox;
    autoMinBox.ParentTextBoxPointer=&autoHourBox;

	System.Gui.Form.AddFillArea(&App.Menu.AutoSetupForm,&autoArea);
	autoArea.ID=PromptAreaID12;
	autoArea.PaintColor=TopLabelColor;
	autoArea.RectWrap.x0=0;
	autoArea.RectWrap.y0=30;
	autoArea.RectWrap.x1=320;
	autoArea.RectWrap.y1=80;

	System.Gui.Form.AddLabel(&App.Menu.AutoSetupForm,&autoName);
	autoName.ID=LabelID1+13;
	autoName.Font=ListNameFont;
	autoName.PaintColor=GUI_BLACK;
	autoName.TextMode=GUI_TEXTMODE_TRANS;
	autoName.Count=0;
	autoName.StringBlockPointer=SetupString[Auto];
	autoName.X=75;
	autoName.Y=43;

	System.Gui.Form.AddLabel(&App.Menu.AutoSetupForm,&secSymbol);
	secSymbol.ID=LabelID1+15;
	secSymbol.Font=ListFont;
	secSymbol.PaintColor=GUI_BROWN;
	secSymbol.TextMode=GUI_TEXTMODE_TRANS;
  secSymbol.Type=0xC0;
	secSymbol.Count=1;
	secSymbol.StringBlockPointer=SetupString[Auto];
	secSymbol.X=155;
	secSymbol.Y=105;

	System.Gui.Form.AddLabel(&App.Menu.AutoSetupForm,&armDisarm);
	armDisarm.ID=LabelID1+16;
	armDisarm.Font=ListFont;
	armDisarm.PaintColor=GUI_BROWN;
	armDisarm.TextMode=GUI_TEXTMODE_TRANS;
  armDisarm.Type=0xC0;
	armDisarm.Count=2;
	armDisarm.StringBlockPointer=SetupString[Auto];
	armDisarm.X=10;
	armDisarm.Y=145;

	System.Gui.Form.AddLabel(&App.Menu.AutoSetupForm,&week);
	week.ID=LabelID1;
	week.Font=ListFont;
	week.PaintColor=GUI_BROWN;
	week.TextMode=GUI_TEXTMODE_TRANS;
  week.Type=0xC0;
	week.Count=3;
	week.StringBlockPointer=SetupString[Auto];
	week.X=10;
	week.Y=185;
    
  System.Gui.Form.AddLabel(&App.Menu.AutoSetupForm,&stateLabel);
  stateLabel.ID=LabelID1+19;
  stateLabel.Font=ListFont;
  stateLabel.PaintColor=GUI_RED;
  stateLabel.TextMode=GUI_TEXTMODE_TRANS;
  stateLabel.Count=3;
  stateLabel.Type=0xFE;
  stateLabel.StringBlockPointer=NULL;
  stateLabel.X=130;
  stateLabel.Y=145;


	System.Gui.Form.AddTextBox(&App.Menu.AutoSetupForm,&autoHourBox);
	autoHourBox.x=120;
	autoHourBox.y=105;
  autoHourBox.Leght=2;
	autoHourBox.MaxDataPointer=23;
	autoHourBox.MinDataPointer=0;
	autoHourBox.Type=GuiDataTypeByteDec;
	autoHourBox.BackColor=GUI_BLUE;
	autoHourBox.PaintColor=GUI_RED;
	autoHourBox.Font=GUI_FONT_20_ASCII;
	autoHourBox.ID=0x03;
	autoHourBox.TextMode=GUI_TEXTMODE_NORMAL;
	autoHourBox.RectWrap.x0=120;
	autoHourBox.RectWrap.x1=155;
	autoHourBox.RectWrap.y0=105;
	autoHourBox.RectWrap.y1=125;	
	autoHourBox.DataPointer=(void*)&App.Data.AutoHour;	
  App.Data.AutoHour=20;

	System.Gui.Form.AddTextBox(&App.Menu.AutoSetupForm,&autoMinBox);
	autoMinBox.x=175;
	autoMinBox.y=105;
  autoMinBox.Leght=2;
	autoMinBox.MaxDataPointer=59;
	autoMinBox.MinDataPointer=0;
	autoMinBox.Type=GuiDataTypeByteDec;
	autoMinBox.BackColor=GUI_BLUE;
	autoMinBox.PaintColor=GUI_RED;
	autoMinBox.Font=GUI_FONT_20_ASCII;
	autoMinBox.ID=0x05;
	autoMinBox.TextMode=GUI_TEXTMODE_TRANS;
	autoMinBox.RectWrap.x0=175;
	autoMinBox.RectWrap.x1=215;
	autoMinBox.RectWrap.y0=105;
	autoMinBox.RectWrap.y1=125;	
	autoMinBox.DataPointer=(void*)&App.Data.AutoMin;	
  App.Data.AutoMin=10;
    
  for(i=0;i<7;i++)
  {
    System.Gui.Form.AddArCheck(&App.Menu.AutoSetupForm,&monBox[i]);
    monBox[i].ID=0x01+i;
    monBox[i].x=90+(i*30);
		monBox[i].xString=90+(i*30);
		monBox[i].yString=205;
    monBox[i].PaintColor=GUI_BLACK;
    monBox[i].BackColor=GUI_WHITE;
    monBox[i].Font=GUI_FONT_13B_1;
    monBox[i].TextMode=GUI_TEXTMODE_TRANS;
    monBox[i].SelectedBackColor=GUI_BLUE;
    monBox[i].SelectedPaintColor=GUI_RED;
    monBox[i].Week=i+1;
    monBox[i].y=185;
    monBox[i].Size=18;
    monBox[i].Addr=(void*)&App.Data.AutoAddr;
    monBox[i].stringCount=i;
    monBox[i].StringPointer=WeekStringAuto[0];
  }
    
}
void ContactIDMenuInit(void)
{
	u8	i;
	static FillArea		CIDArea;
	static Label		CIDName;
	static Label		userCode;
	static TextBox 		userBox;
	static ArCheck      checkBox[3];
	static FocusSturct	cidFocus[4];

  System.Gui.Form.Init(&App.Menu.ContactIDForm);
  App.Menu.ContactIDForm.DefaultFocus=&cidFocus[0];
  App.Menu.ContactIDForm.FocusPointer=&cidFocus[0];
  App.Menu.ContactIDForm.FillAreaCount=1;
  App.Menu.ContactIDForm.ArCheckCount=3;
  App.Menu.ContactIDForm.LabelCount=2;
  App.Menu.ContactIDForm.TextBoxCount=1;
  App.Menu.ContactIDForm.FormID=Form9+8;
  App.Menu.ContactIDForm.Color=GUI_WHITE;
  App.Menu.ContactIDForm.RectWrap.x0=0;
  App.Menu.ContactIDForm.RectWrap.y0=30;
  App.Menu.ContactIDForm.RectWrap.x1=320;
  App.Menu.ContactIDForm.RectWrap.y1=240;
	App.Menu.ContactIDForm.DefaultFocusTextBoxPointer=&userBox;	
	App.Menu.ContactIDForm.FocusTextBoxPointer=&userBox;
	
	for(i=0;i<4;i++)
  {
    System.Gui.Form.AddFocus(&App.Menu.ContactIDForm,&cidFocus[i]);
  }
	
	cidFocus[0].BoxType=TextBoxType; 
  cidFocus[0].FocusPointer=(void*)&userBox;
  cidFocus[0].ParentPointer=&cidFocus[3];
  cidFocus[0].NextPointer=&cidFocus[1];
	
	cidFocus[1].BoxType=ArCheckBoxType; 
  cidFocus[1].FocusPointer=(void*)&checkBox[0];
  cidFocus[1].ParentPointer=&cidFocus[0];
  cidFocus[1].NextPointer=&cidFocus[2];	
	
	cidFocus[2].BoxType=ArCheckBoxType; 
  cidFocus[2].FocusPointer=(void*)&checkBox[1];
  cidFocus[2].ParentPointer=&cidFocus[1];
  cidFocus[2].NextPointer=&cidFocus[3];	
	
	cidFocus[3].BoxType=ArCheckBoxType; 
  cidFocus[3].FocusPointer=(void*)&checkBox[2];
  cidFocus[3].ParentPointer=&cidFocus[2];
  cidFocus[3].NextPointer=&cidFocus[0];
  for(i=0;i<3;i++)
  {
    System.Gui.Form.AddArCheck(&App.Menu.ContactIDForm,&checkBox[i]);
    checkBox[i].ID=0x01+i;
    checkBox[i].x=30;
		checkBox[i].xString=60;
		checkBox[i].yString=(140+i*30);
    checkBox[i].PaintColor=GUI_BLACK;
    checkBox[i].BackColor=GUI_WHITE;
    checkBox[i].Font=ListFont;
    checkBox[i].TextMode=GUI_TEXTMODE_TRANS;
    checkBox[i].SelectedBackColor=GUI_BLUE;
    checkBox[i].SelectedPaintColor=GUI_RED;
    checkBox[i].Week=i+1;
    checkBox[i].y=(140+i*30);
    checkBox[i].Size=18;
    checkBox[i].Addr=(void*)&CheckBoxState[4][i];
    checkBox[i].stringCount=i;
    checkBox[i].StringPointer=&SetupString[CIDString][0]+1;
  }		
	System.Gui.Form.AddTextBox(&App.Menu.ContactIDForm,&userBox);
	userBox.x=160;
	userBox.y=90;
	userBox.Leght=4;
	userBox.MaxDataPointer=4;
	userBox.MinDataPointer=0;
	userBox.Type=GuiDataTypeString;
	userBox.BackColor=GUI_BLUE;
	userBox.PaintColor=GUI_RED;
	userBox.Font=GUI_FONT_20_ASCII;
	userBox.ID=0x09;
	userBox.TextMode=GUI_TEXTMODE_NORMAL;
	userBox.RectWrap.x0=160;
	userBox.RectWrap.x1=200;
	userBox.RectWrap.y0=90;
	userBox.RectWrap.y1=110;	
	userBox.DataPointer=&App.Data.KeyBuffer2;;	
	userBox.DataAddr=CIDUserCodeAddr; 

	System.Gui.Form.AddFillArea(&App.Menu.ContactIDForm,&CIDArea);
	CIDArea.ID=PromptAreaID19;
	CIDArea.PaintColor=TopLabelColor;
	CIDArea.RectWrap.x0=0;
	CIDArea.RectWrap.y0=30;
	CIDArea.RectWrap.x1=320;
	CIDArea.RectWrap.y1=80;	
	
	System.Gui.Form.AddLabel(&App.Menu.ContactIDForm,&CIDName);
	CIDName.ID=LabelID1+16;
	CIDName.Font=ListNameFont;
	CIDName.PaintColor=GUI_BLACK;
	CIDName.TextMode=GUI_TEXTMODE_TRANS;
	CIDName.Count=0;
	CIDName.StringBlockPointer=SetupString[CIDString];
	CIDName.X=105;
	CIDName.Y=45;	
	
	System.Gui.Form.AddLabel(&App.Menu.ContactIDForm,&userCode);
	userCode.ID=LabelID1;
	userCode.Font=ListFont;
	userCode.PaintColor=GUI_BLACK;
	userCode.TextMode=GUI_TEXTMODE_TRANS;
  userCode.Type=0xC0;
	userCode.Count=4;
	userCode.StringBlockPointer=SetupString[CIDString];
	userCode.X=30;
	userCode.Y=90;
}
void ContactIDSeting(void)
{
	ArCheck *check;
	
	if(App.Menu.FocusFormPointer==&App.Menu.ContactIDForm)
	{
		if(App.Menu.FocusFormPointer->FocusPointer->BoxType==ArCheckBoxType)
		{
			check=(ArCheck*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
			//System.Device.Storage.Parameter.StorageReadData(*(u32*)check->Addr,(u8*)&check->State,1);
			check->State=!check->State;
			//System.Device.Storage.Parameter.StorageWriteData(*(u32*)check->Addr,(u16*)&check->State,7);
			check->Update+=1;			
		}	
	}
}
void ContactIDReadData(void)
{
	FocusSturct *FocusCheck;
	ArCheck *check;
	u8 i;
	
	if(App.Menu.FocusFormPointer==&App.Menu.ContactIDForm)
	{
		FocusCheck=App.Menu.FocusFormPointer->FocusPointer->NextPointer;
		for(i=0;i<3;i++)
		{
			check=FocusCheck->FocusPointer;
			System.Device.Storage.Parameter.StorageReadData(*(u32*)check->Addr ,(u8*)&check->State,1);
			FocusCheck=FocusCheck->NextPointer;
		}
	
	}
}
void ContactIDSaveData(void)
{
	FocusSturct *FocusCheck;
	ArCheck *check;
	u8 i;
	
	if(App.Menu.FocusFormPointer==&App.Menu.ContactIDForm)
	{
		FocusCheck=App.Menu.FocusFormPointer->DefaultFocus;
		FocusCheck=FocusCheck->NextPointer;
		for(i=0;i<3;i++)
		{
			check=FocusCheck->FocusPointer;
			System.Device.Storage.Parameter.StorageWriteData(*(u32*)check->Addr ,(u16*)&check->State,1);
			FocusCheck=FocusCheck->NextPointer;
		}
	
	}	
}
/*************************************************************************************************************
*
*描述       ：自动布撤防设定状态和星期 的设定
*输入参数   ：无
*输出参数   ：无
*
*************************************************************************************************************/
void AuotoDisarmArmSet(void)
{
    u8 buffer[30];
    ArCheck *check;
    Label   *labelPointer;
    
    if(App.Menu.FocusFormPointer==&App.Menu.AutoSetupForm)
    {
         if(App.Menu.FocusFormPointer->FocusPointer!=NULL)
         {
            System.Device.Storage.Parameter.StorageReadData(App.Data.AutoAddr,(u8*)buffer,30);
            switch(App.Menu.FocusFormPointer->FocusPointer->BoxType)
            {
                case LabelBoxType:
                    labelPointer=(Label*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                    if(buffer[16]==0xF0)
                    {    
                        memset(buffer,0xFF,18);
                        sprintf((char*)buffer,"Auto Disarm");
                        buffer[16]=0xF1;
                        System.Device.Storage.Parameter.StorageWriteData(App.Data.AutoAddr,(u16*)buffer,15);
                        
                    }
                    else
                    {
                        memset(buffer,0xFF,18);
                        sprintf((char*)buffer,"Auto Arm");
                        buffer[16]=0xF0;   
                        System.Device.Storage.Parameter.StorageWriteData(App.Data.AutoAddr,(u16*)buffer,15);                        
                    }
                    labelPointer->Update+=1;
                    labelPointer->TextMode=GUI_TEXTMODE_NORMAL;
                    labelPointer->BackColor=GUI_BLUE;
                    break;
                case ArCheckBoxType:
                    check=(ArCheck*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                    buffer[check->Week+22-1]=!buffer[check->Week+22-1];
                    System.Device.Storage.Parameter.StorageWriteData(App.Data.AutoAddr+22,(u16*)(buffer+22),7);
                    check->Update+=1;
                    break;
            }
         }
    }
}
/*******************************************************************************************************************************
*
*描述       ： 获取定量组的存储地址 
*输入参数   ： 无
*输出参数   ： 无
*` 
*******************************************************************************************************************************/
void AutoAddrGet(Form *pointer)
{
    MenuListStruct 	*currentMenu;
    Form 			*focusFormPointer;
    ListboxList		*listx;
    TextBox 	    *textBoxPointer;
    u32             addr;
    char            buffer[30];
    
    
    if(App.Menu.FocusFormPointer==&App.Menu.AutoSetupForm)
    {
		currentMenu=CurrentMenu->Parent;//获取上一级列表选项
		focusFormPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
        listx=ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);
        addr=*listx->NullPointer;
        App.Data.AutoAddr=addr;
        System.Device.Storage.Parameter.StorageReadData(addr,(u8*)buffer,30);
            
        if(App.Menu.FocusFormPointer->TextBoxPointer!=null)
        {
			textBoxPointer=App.Menu.FocusFormPointer->TextBoxPointer;
            textBoxPointer->DataAddr=addr+20;
            System.Device.Storage.Parameter.StorageReadData(textBoxPointer->DataAddr,(u8*)textBoxPointer->DataPointer,1);	
            textBoxPointer=textBoxPointer->NextTextBoxPointer;		
            textBoxPointer->DataAddr=addr+18;  
            System.Device.Storage.Parameter.StorageReadData(textBoxPointer->DataAddr,(u8*)textBoxPointer->DataPointer,1);	            			
        }
        if(App.Menu.FocusFormPointer->LabelPointer!=NULL)
        {
            if(App.Menu.FocusFormPointer->LabelPointer->Type==0xFE)
            {
                App.Menu.FocusFormPointer->LabelPointer->Addr=addr;
            }
        }
    }
}
/*******************************************************************************************************************************
*
*描述       ： 检测自动布撤防时间
*输入参数   ： 无
*输出参数   ： 无
*` 
*******************************************************************************************************************************/
void AutoArmOrDisarm(void)
{
	u8 buffer[30];
	u8 iCount;
	u8 temp;
	u32 addr;
	static u8 min=0xFF;
	
	if(min==App.Data.Min)return;
	else min=0xFF;
	for(iCount=0;iCount<8;iCount++)
	{

		System.Device.Storage.Parameter.StorageReadData(Auto1State+(iCount*2),&temp,1);
		if(temp)
		{
			addr=Auto1Addr+(iCount*40);
			System.Device.Storage.Parameter.StorageReadData(addr,buffer,30);
			if(buffer[22+App.Data.Week-1])
			{
				if((buffer[18]==App.Data.Hour)&&(buffer[20]==App.Data.Min))
				{
					min=App.Data.Min;
					switch(buffer[16])
					{
						case 0xF0://布防
							PostMessage(MessageAutoType,SystemArmMode);
							return;
						case 0xF1://撤防
							PostMessage(MessageAutoType,SystemDisarmMode);
							return;
					}
				}
			}
		}
	}
}
/*********************************************************************************
*
*描述		：时间日期UI
*输入参数	：无
*输出参数	：无
*
*********************************************************************************/
static void SetupDateFunc(void)
{
	u8 i;
	//日期
	static FillArea		DateArea;
	static Label		DateName;
	static Label		DateClass[6];
	static Label		DateSetup;
	static Label		TimeSetup;
	static FillArea		DateLine[2];
	static TextBox		yearTextBox;
	static TextBox		monthTextBox;
	static TextBox		dayTextBox;
	static TextBox		hourTextBox;
	static TextBox		minTextBox;
	static TextBox		secTextBox;
	
	System.Gui.Form.Init(&App.Menu.DateForm);
	App.Menu.DateForm.FillAreaCount=3;
	App.Menu.DateForm.LabelCount=9;
	App.Menu.DateForm.TextBoxCount=6;
	App.Menu.DateForm.FormID=Form13;
	App.Menu.DateForm.Color=GUI_WHITE;
	App.Menu.DateForm.RectWrap.x0=0;
	App.Menu.DateForm.RectWrap.y0=30;
	App.Menu.DateForm.RectWrap.x1=320;
	App.Menu.DateForm.RectWrap.y1=240;	
	App.Menu.DateForm.DefaultFocusTextBoxPointer=&yearTextBox;	
	App.Menu.DateForm.FocusTextBoxPointer=&yearTextBox;
	
 	yearTextBox.ParentTextBoxPointer=&monthTextBox;
 	monthTextBox.ParentTextBoxPointer=&dayTextBox;
	dayTextBox.ParentTextBoxPointer=&hourTextBox;
	hourTextBox.ParentTextBoxPointer=&minTextBox;
	minTextBox.ParentTextBoxPointer=&secTextBox;
	secTextBox.ParentTextBoxPointer=&yearTextBox;
	
	System.Gui.Form.AddFillArea(&App.Menu.DateForm,&DateArea);
	DateArea.ID=PromptAreaID13;
	DateArea.PaintColor=TopLabelColor;
	DateArea.RectWrap.x0=0;
	DateArea.RectWrap.y0=30;
	DateArea.RectWrap.x1=320;
	DateArea.RectWrap.y1=80;

	System.Gui.Form.AddLabel(&App.Menu.DateForm,&DateName);
	DateName.ID=LabelID1+14;
	DateName.Font=GUI_FONT_24_ASCII;
	DateName.PaintColor=GUI_BLACK;
	DateName.TextMode=GUI_TEXTMODE_TRANS;
	DateName.Count=0;
	DateName.StringBlockPointer=SetupString[Date];
	DateName.X=120;
	DateName.Y=45;	
                
	System.Gui.Form.AddLabel(&App.Menu.DateForm,&DateSetup);
	DateSetup.ID=LabelID1+18;
	DateSetup.Font=GUI_FONT_20_ASCII;
	DateSetup.PaintColor=GUI_BROWN;
	DateSetup.TextMode=GUI_TEXTMODE_TRANS;
  DateSetup.Type=0xC0;
	DateSetup.Count=4;
	DateSetup.StringBlockPointer=SetupString[Date];
	DateSetup.X=5;
	DateSetup.Y=115;
	
	System.Gui.Form.AddLabel(&App.Menu.DateForm,&TimeSetup);
	TimeSetup.ID=LabelID1+19;
	TimeSetup.Font=GUI_FONT_20_ASCII;
	TimeSetup.PaintColor=GUI_BROWN;
	TimeSetup.TextMode=GUI_TEXTMODE_TRANS;
  TimeSetup.Type=0xC0;
	TimeSetup.Count=5;
	TimeSetup.StringBlockPointer=SetupString[Date];
	TimeSetup.X=5;
	TimeSetup.Y=165;

	for(i=0;i<6;i++)
	{
		System.Gui.Form.AddLabel(&App.Menu.DateForm,&DateClass[i]);
		DateClass[i].ID=LabelID1+50+i;
		DateClass[i].PaintColor=GUI_BLUE;
		DateClass[i].Font=GUI_FONT_16_1HK;
		DateClass[i].TextMode=GUI_TEXTMODE_TRANS;
    DateClass[i].Type=0xC0;
		if(i>2)
		{
			DateClass[i].X=110+((i-3)*90);
			DateClass[i].Y=170;	
			DateClass[i].Count=2+(i-3);
			DateClass[i].StringBlockPointer=WeekString[1];

		}
		else
		{
			DateClass[i].X=110+(i*90);
			DateClass[i].Y=120;	
			DateClass[i].Count=1+i;
			DateClass[i].StringBlockPointer=SetupString[Date];
		}
	}
	for(i=0;i<2;i++)
	{
		System.Gui.Form.AddFillArea(&App.Menu.DateForm,&DateLine[i]);
		DateLine[i].ID=PromptAreaID6+i;
		DateLine[i].PaintColor=GUI_LIGHTGRAY;
		DateLine[i].RectWrap.x0=3;
		DateLine[i].RectWrap.y0=138+((i)*50);
		DateLine[i].RectWrap.x1=317;
		DateLine[i].RectWrap.y1=138+((i)*50);
	}
	System.Gui.Form.AddTextBox(&App.Menu.DateForm,&yearTextBox);
	yearTextBox.x=60;
	yearTextBox.y=116;
	yearTextBox.Leght=4;
	yearTextBox.MaxDataPointer=5000;
	yearTextBox.MinDataPointer=1900;
	yearTextBox.Type=GuiDataTypeUintDec;
	yearTextBox.BackColor=GUI_LIGHTGRAY;
	yearTextBox.PaintColor=GUI_RED;
	yearTextBox.Font=GUI_FONT_20_ASCII;
	yearTextBox.ID=0x03;
	yearTextBox.TextMode=GUI_TEXTMODE_NORMAL;
	yearTextBox.RectWrap.x0=60;
	yearTextBox.RectWrap.x1=100;
	yearTextBox.RectWrap.y0=116;
	yearTextBox.RectWrap.y1=136;	
	yearTextBox.DataPointer=(void*)&App.Data.Year;	
	yearTextBox.DataAddr=YearAddr; 

	
	System.Gui.Form.AddTextBox(&App.Menu.DateForm,&monthTextBox);
	monthTextBox.x=160;
	monthTextBox.y=116;
	monthTextBox.Leght=2;
	monthTextBox.MaxDataPointer=12;
	monthTextBox.MinDataPointer=1;
	monthTextBox.Type=GuiDataTypeByteDec;
	monthTextBox.BackColor=GUI_LIGHTGRAY;
	monthTextBox.PaintColor=GUI_RED;
	monthTextBox.Font=GUI_FONT_20_ASCII;
	monthTextBox.ID=0x04;
	monthTextBox.TextMode=GUI_TEXTMODE_TRANS;
	monthTextBox.RectWrap.x0=160;
	monthTextBox.RectWrap.x1=196;
	monthTextBox.RectWrap.y0=116;
	monthTextBox.RectWrap.y1=136;	
	monthTextBox.DataPointer=(void*)&App.Data.Month;	
	monthTextBox.DataAddr=MonthAddr;

	
	System.Gui.Form.AddTextBox(&App.Menu.DateForm,&dayTextBox);
	dayTextBox.x=250;
	dayTextBox.y=116;
	dayTextBox.Leght=2;
	dayTextBox.MaxDataPointer=31;
	dayTextBox.MinDataPointer=1;
	dayTextBox.Type=GuiDataTypeByteDec;
	dayTextBox.BackColor=GUI_LIGHTGRAY;
	dayTextBox.PaintColor=GUI_RED;
	dayTextBox.Font=GUI_FONT_20_ASCII;
	dayTextBox.ID=0x05;
	dayTextBox.TextMode=GUI_TEXTMODE_TRANS;
	dayTextBox.RectWrap.x0=250;
	dayTextBox.RectWrap.x1=286;
	dayTextBox.RectWrap.y0=116;
	dayTextBox.RectWrap.y1=136;	
	dayTextBox.DataPointer=(void*)&App.Data.Day;
	dayTextBox.DataAddr=DayAddr;


	
	System.Gui.Form.AddTextBox(&App.Menu.DateForm,&hourTextBox);
	hourTextBox.x=60;
	hourTextBox.y=166;
	hourTextBox.Leght=2;
	hourTextBox.MaxDataPointer=23;
	hourTextBox.MinDataPointer=0;
	hourTextBox.Type=GuiDataTypeByteDec;
	hourTextBox.BackColor=GUI_LIGHTGRAY;
	hourTextBox.PaintColor=GUI_RED;
	hourTextBox.Font=GUI_FONT_20_ASCII;
	hourTextBox.ID=0x06;
	hourTextBox.TextMode=GUI_TEXTMODE_TRANS;
	hourTextBox.RectWrap.x0=55;
	hourTextBox.RectWrap.x1=95;
	hourTextBox.RectWrap.y0=166;
	hourTextBox.RectWrap.y1=186;	
	hourTextBox.DataPointer=(void*)&App.Data.Hour;	
	
	System.Gui.Form.AddTextBox(&App.Menu.DateForm,&minTextBox);
	minTextBox.x=160;
	minTextBox.y=166;
	minTextBox.Leght=2;
	minTextBox.MaxDataPointer=59;
	minTextBox.MinDataPointer=0;
	minTextBox.Type=GuiDataTypeByteDec;
	minTextBox.BackColor=GUI_LIGHTGRAY;
	minTextBox.PaintColor=GUI_RED;
	minTextBox.Font=GUI_FONT_20_ASCII;
	minTextBox.ID=0x07;
	minTextBox.TextMode=GUI_TEXTMODE_TRANS;
	minTextBox.RectWrap.x0=160;
	minTextBox.RectWrap.x1=196;
	minTextBox.RectWrap.y0=166;
	minTextBox.RectWrap.y1=186;	
	minTextBox.DataPointer=(void*)&App.Data.Min;	
	App.Data.Min=2;
	
	System.Gui.Form.AddTextBox(&App.Menu.DateForm,&secTextBox);
	secTextBox.x=250;
	secTextBox.y=166;
	secTextBox.Leght=2;
	secTextBox.MaxDataPointer=59;
	secTextBox.MinDataPointer=0;
	secTextBox.Type=GuiDataTypeByteDec;
	secTextBox.BackColor=GUI_LIGHTGRAY;
	secTextBox.PaintColor=GUI_RED;
	secTextBox.Font=GUI_FONT_20_ASCII;
	secTextBox.ID=0x08;
	secTextBox.TextMode=GUI_TEXTMODE_TRANS;
	secTextBox.RectWrap.x0=250;
	secTextBox.RectWrap.x1=286;
	secTextBox.RectWrap.y0=166;
	secTextBox.RectWrap.y1=186;	
	secTextBox.DataPointer=(void*)&App.Data.Sec;	
	App.Data.Sec=10;
}

/*********************************************************************************

*描述		：音量设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void SetupVolumeFunc(void)
{
	u8 i;
	//音量设定
	static FillArea		volumeArea[3];
	static Label		volumeName;
	static Label		volumeValue[2];
	static Label		volumeScope[2];
	static TextBox		voiveVolumeTextBox;
	static TextBox		alarmVolumeTextBox;
	
	System.Gui.Form.Init(&App.Menu.VolumeForm);
	App.Menu.VolumeForm.FillAreaCount=3;
	App.Menu.VolumeForm.LabelCount=5;
	App.Menu.VolumeForm.TextBoxCount=2;
	App.Menu.VolumeForm.FormID=Form14;
	App.Menu.VolumeForm.Color=GUI_WHITE;
	App.Menu.VolumeForm.RectWrap.x0=0;
	App.Menu.VolumeForm.RectWrap.y0=30;
	App.Menu.VolumeForm.RectWrap.x1=320;
	App.Menu.VolumeForm.RectWrap.y1=240;
	
	App.Menu.VolumeForm.DefaultFocusTextBoxPointer=&voiveVolumeTextBox;	
	App.Menu.VolumeForm.FocusTextBoxPointer=&voiveVolumeTextBox;
	
	voiveVolumeTextBox.ParentTextBoxPointer=&alarmVolumeTextBox;
	alarmVolumeTextBox.ParentTextBoxPointer=&voiveVolumeTextBox;

	System.Gui.Form.AddFillArea(&App.Menu.VolumeForm,&volumeArea[0]);
	volumeArea[0].ID=PromptAreaID13;
	volumeArea[0].PaintColor=TopLabelColor;
	volumeArea[0].RectWrap.x0=0;
	volumeArea[0].RectWrap.y0=30;
	volumeArea[0].RectWrap.x1=320;
	volumeArea[0].RectWrap.y1=80;
	
	for(i=1;i<3;i++)
	{
		System.Gui.Form.AddFillArea(&App.Menu.VolumeForm,&volumeArea[i]);
		volumeArea[i].ID=PromptAreaID6+i;
		volumeArea[i].PaintColor=GUI_LIGHTGRAY;
		volumeArea[i].RectWrap.x0=3;
		volumeArea[i].RectWrap.y0=138+((i-1)*50);
		volumeArea[i].RectWrap.x1=317;
		volumeArea[i].RectWrap.y1=138+((i-1)*50);	
	}

	System.Gui.Form.AddLabel(&App.Menu.VolumeForm,&volumeName);
	volumeName.ID=LabelID1+15;
	volumeName.Font=GUI_FONT_24_ASCII;
	volumeName.PaintColor=GUI_BLACK;
	volumeName.TextMode=GUI_TEXTMODE_TRANS;
	volumeName.Count=0;
	volumeName.StringBlockPointer=SetupString[Volume];
	volumeName.X=120;
	volumeName.Y=45;	

	for(i=0;i<2;i++)
	{
		System.Gui.Form.AddLabel(&App.Menu.VolumeForm,&volumeValue[i]);
		volumeValue[i].ID=LabelID1+16;
		volumeValue[i].Font=GUI_FONT_20_ASCII;;
		volumeValue[i].PaintColor=GUI_BROWN;
		volumeValue[i].TextMode=GUI_TEXTMODE_TRANS;
    volumeValue[i].Type=0xC0;
		volumeValue[i].Count=i+1;
		volumeValue[i].StringBlockPointer=SetupString[Volume];
		volumeValue[i].X=10;
		volumeValue[i].Y=120+(i*50);			
	}
	
	for(i=0;i<2;i++)
	{
		System.Gui.Form.AddLabel(&App.Menu.VolumeForm,&volumeScope[i]);
		volumeScope[i].ID=LabelID1+10+i;
		volumeScope[i].Font=GUI_FONT_16_1HK;
		volumeScope[i].PaintColor=GUI_GRAY;
		volumeScope[i].TextMode=GUI_TEXTMODE_TRANS;
    volumeScope[i].Type=0xC0;
		volumeScope[i].Count=3;
		volumeScope[i].StringBlockPointer=SetupString[Volume];
		volumeScope[i].X=260;
		volumeScope[i].Y=120+(i*50);
	}
	System.Gui.Form.AddTextBox(&App.Menu.VolumeForm,&voiveVolumeTextBox);
	voiveVolumeTextBox.x=150;
	voiveVolumeTextBox.y=116;
	voiveVolumeTextBox.MaxDataPointer=8;
	voiveVolumeTextBox.MinDataPointer=0;
	voiveVolumeTextBox.Type=GuiDataTypeByteDec;
	voiveVolumeTextBox.BackColor=GUI_LIGHTGRAY;
	voiveVolumeTextBox.PaintColor=GUI_RED;
	voiveVolumeTextBox.Font=GUI_FONT_20_ASCII;
	voiveVolumeTextBox.ID=0x09;
	voiveVolumeTextBox.TextMode=GUI_TEXTMODE_NORMAL;
	voiveVolumeTextBox.RectWrap.x0=150;
	voiveVolumeTextBox.RectWrap.x1=170;
	voiveVolumeTextBox.RectWrap.y0=116;
	voiveVolumeTextBox.RectWrap.y1=136;	
	voiveVolumeTextBox.DataPointer=(void*)&App.Data.VoiveVolume;	
	voiveVolumeTextBox.DataAddr=VoiceVolumeAddr;
	App.Data.VoiveVolume=5;	
	
	System.Gui.Form.AddTextBox(&App.Menu.VolumeForm,&alarmVolumeTextBox);
	alarmVolumeTextBox.x=150;
	alarmVolumeTextBox.y=166;
	alarmVolumeTextBox.MaxDataPointer=8;
	alarmVolumeTextBox.MinDataPointer=0;
	alarmVolumeTextBox.Type=GuiDataTypeByteDec;
	alarmVolumeTextBox.BackColor=GUI_LIGHTGRAY;
	alarmVolumeTextBox.PaintColor=GUI_RED;
	alarmVolumeTextBox.Font=GUI_FONT_20_ASCII;
	alarmVolumeTextBox.ID=0x09;
	alarmVolumeTextBox.TextMode=GUI_TEXTMODE_TRANS;
	alarmVolumeTextBox.RectWrap.x0=150;
	alarmVolumeTextBox.RectWrap.x1=170;
	alarmVolumeTextBox.RectWrap.y0=166;
	alarmVolumeTextBox.RectWrap.y1=186;	
	alarmVolumeTextBox.DataPointer=(void*)&App.Data.AlarmVolume;	
	alarmVolumeTextBox.DataAddr=AlarmVolumeAddr;	
	
}

/*********************************************************************************

*描述		：按键音设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void SetupKeyToneFunc(void)
{
	
	//按键音
	static	FillArea	KeyToneArea;
	static	Label		KeyToneName;
	static 	ListBox		KeyToneList;
	
	string *items;
	items=(string *)SetupString[KeyTone];
	
	System.Gui.Form.Init(&App.Menu.KeyToneForm);
	App.Menu.KeyToneForm.FillAreaCount=1;
	App.Menu.KeyToneForm.ListCount=1;	
	App.Menu.KeyToneForm.LabelCount=1;
	App.Menu.KeyToneForm.FormID=Form15;
	App.Menu.KeyToneForm.Color=GUI_WHITE;
	App.Menu.KeyToneForm.RectWrap.x0=0;
	App.Menu.KeyToneForm.RectWrap.y0=30;
	App.Menu.KeyToneForm.RectWrap.x1=320;
	App.Menu.KeyToneForm.RectWrap.y1=240;
	
	System.Gui.Form.AddFillArea(&App.Menu.KeyToneForm,&KeyToneArea);
	KeyToneArea.ID=PromptAreaID19;
	KeyToneArea.PaintColor=TopLabelColor;
	KeyToneArea.RectWrap.x0=0;
	KeyToneArea.RectWrap.y0=30;
	KeyToneArea.RectWrap.x1=320;
	KeyToneArea.RectWrap.y1=80;	
	
	System.Gui.Form.AddLabel(&App.Menu.KeyToneForm,&KeyToneName);
	KeyToneName.ID=LabelID1+16;
	KeyToneName.Font=ListNameFont;
	KeyToneName.PaintColor=GUI_BLACK;
	KeyToneName.TextMode=GUI_TEXTMODE_TRANS;
	KeyToneName.Count=0;
	KeyToneName.StringBlockPointer=SetupString[KeyTone];
	KeyToneName.X=120;
	KeyToneName.Y=43;	
		
	System.Gui.Form.AddListBox(&App.Menu.KeyToneForm,&KeyToneList);
	KeyToneList.x0=0;
	KeyToneList.y0=80;
	KeyToneList.x1=320;
	KeyToneList.y1=240;
	KeyToneList.xSize=15;
	KeyToneList.ySize=15;
	
	KeyToneList.selectedItem=0;
    KeyToneList.ItemsHeight=40;
	KeyToneList.ItemsPerpage=1;//页最大条目
	KeyToneList.SelectedIndexes=0;//默认为0，也就是第一条
	KeyToneList.TopItem=0;//默认为0，也就是从第一个索引号开始
	KeyToneList.TotalItems=0;
	
	KeyToneList.TextMode=GUI_TEXTMODE_TRANS;
	KeyToneList.BackColor=GUI_WHITE;
	KeyToneList.PaintColor=GUI_BLACK;
	
	KeyToneList.SelectedBackColor=MoveBarColor;
	KeyToneList.SelectedPaintColor=GUI_WHITE;
	
	KeyToneList.Font=ListFont;
	KeyToneList.ID=0x55;
	KeyToneList.Type=SwitchList;

	System.Gui.Form.AddLists(&KeyToneList,&items[1],1,(u32*)CheckBoxState[KeyToneCheckBox],null,null,null,1);
	
}
void KeyToneSetup(void)
{
	System.Device.Storage.Parameter.StorageReadData(KeyToneStateAddr,&App.Data.KeyToneState,1);
}
/*********************************************************************************

*描述		：语言设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void SetupLanguageFunc(void)
{
	string *items;
	//语言
	static FillArea		languageArea;
	static Label		languageName;
	static ListBox		languageList;
	
	items=(string *)SetupString[Language];
	
	System.Gui.Form.Init(&App.Menu.LanguageForm);
	App.Menu.LanguageForm.FillAreaCount=1;
	App.Menu.LanguageForm.LabelCount=1;
	App.Menu.LanguageForm.ListCount=1;
	App.Menu.LanguageForm.FormID=Form14;
	App.Menu.LanguageForm.Color=GUI_WHITE;
	App.Menu.LanguageForm.RectWrap.x0=0;
	App.Menu.LanguageForm.RectWrap.y0=30;
	App.Menu.LanguageForm.RectWrap.x1=320;
	App.Menu.LanguageForm.RectWrap.y1=240;
	
	System.Gui.Form.AddFillArea(&App.Menu.LanguageForm,&languageArea);
	languageArea.ID=PromptAreaID18;
	languageArea.PaintColor=TopLabelColor;
	languageArea.RectWrap.x0=0;
	languageArea.RectWrap.y0=30;
	languageArea.RectWrap.x1=320;
	languageArea.RectWrap.y1=80;

	System.Gui.Form.AddLabel(&App.Menu.LanguageForm,&languageName);
	languageName.ID=LabelID1+15;
	languageName.Font=ListNameFont;
	languageName.PaintColor=GUI_BLACK;
	languageName.TextMode=GUI_TEXTMODE_TRANS;
	languageName.Count=0;
	languageName.StringBlockPointer=SetupString[Language];
	languageName.X=100;
	languageName.Y=43;	
	
	System.Gui.Form.AddListBox(&App.Menu.LanguageForm,&languageList);
	languageList.x0=0;
	languageList.y0=80;
	languageList.x1=320;
	languageList.y1=240;
	languageList.xSize=18;
	languageList.ySize=18;
	
	languageList.Select=1;
	
	languageList.selectedItem=0;
    languageList.ItemsHeight=40;
	languageList.ItemsPerpage=1;//页最大条目
	languageList.SelectedIndexes=0;//默认为0，也就是第一条
	languageList.TopItem=0;//默认为0，也就是从第一个索引号开始
	languageList.TotalItems=0;
	
	languageList.TextMode=GUI_TEXTMODE_TRANS;
	languageList.BackColor=GUI_WHITE;
	languageList.PaintColor=GUI_BLUE;
	
	languageList.SelectedBackColor=MoveBarColor;
	languageList.SelectedPaintColor=GUI_WHITE;
	
	languageList.Font=ListFont;
	languageList.ID=0x55;
	languageList.Type=CheckList;

	System.Gui.Form.AddLists(&languageList,&items[1],1,(u32*)CheckBoxState[LanguageCheckBox],null,null,null,1);
	
	
	
}
/*********************************************************************************

*描述		：布撤防短信设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void SetupSMSFunc(void)
{
	string *items;
	//短信
	static FillArea		SMSArea;
	static Label		SMSName;
	static ListBox		SMSList;
	items=(string *)SetupString[ArmOrDisarmSMS];
	
	System.Gui.Form.Init(&App.Menu.SMSForm);
	App.Menu.SMSForm.FillAreaCount=1;
	App.Menu.SMSForm.ListCount=1;
	App.Menu.SMSForm.LabelCount=1;
	App.Menu.SMSForm.FormID=Form8+20;
	App.Menu.SMSForm.Color=GUI_WHITE;
	App.Menu.SMSForm.RectWrap.x0=0;
	App.Menu.SMSForm.RectWrap.y0=30;
	App.Menu.SMSForm.RectWrap.x1=320;
	App.Menu.SMSForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.SMSForm,&SMSArea);
	SMSArea.ID=PromptAreaID19;
	SMSArea.PaintColor=TopLabelColor;
	SMSArea.RectWrap.x0=0;
	SMSArea.RectWrap.y0=30;
	SMSArea.RectWrap.x1=320;
	SMSArea.RectWrap.y1=80;	
	
	System.Gui.Form.AddLabel(&App.Menu.SMSForm,&SMSName);
	SMSName.ID=LabelID1+16;
	SMSName.Font=ListNameFont;
	SMSName.PaintColor=GUI_BLACK;
	SMSName.TextMode=GUI_TEXTMODE_TRANS;
	SMSName.Count=0;
	SMSName.StringBlockPointer=SetupString[ArmOrDisarmSMS];
	SMSName.X=75;
	SMSName.Y=43;		
	
	System.Gui.Form.AddListBox(&App.Menu.SMSForm,&SMSList);
	SMSList.x0=0;
	SMSList.y0=80;
	SMSList.x1=320;
	SMSList.y1=240;
	SMSList.xSize=15;
	SMSList.ySize=15;
	
	SMSList.selectedItem=0;
    SMSList.ItemsHeight=40;
	SMSList.ItemsPerpage=3;//页最大条目
	SMSList.SelectedIndexes=0;//默认为0，也就是第一条
	SMSList.TopItem=0;//默认为0，也就是从第一个索引号开始
	SMSList.TotalItems=0;
	
	SMSList.TextMode=GUI_TEXTMODE_TRANS;
	SMSList.BackColor=GUI_WHITE;
	SMSList.PaintColor=GUI_BLACK;
	
	SMSList.SelectedBackColor=MoveBarColor;
	SMSList.SelectedPaintColor=GUI_WHITE;
	
	SMSList.Font=ListFont;
	SMSList.ID=0x56;
	SMSList.Type=CheckList;

	System.Gui.Form.AddLists(&SMSList,&items[1],3,(u32*)CheckBoxState[SMSCheckBox],null,null,null,3);
}

// static void SetupAlarmFunc(void)
// {
// 	string *items;
// //电源
// static FillArea		PowerFillArea;
// static Label		PowerName;
// static ListBox		PowerList;
// 	items=(string *)SetupString[PowerAlarm];
// 	
// 	System.Gui.Form.Init(&App.Menu.PowerAlarmForm);
// 	App.Menu.PowerAlarmForm.FillAreaCount=1;
// 	App.Menu.PowerAlarmForm.LabelCount=01;
// 	App.Menu.PowerAlarmForm.ListCount=1;
// 	App.Menu.PowerAlarmForm.FormID=Form14;
// 	App.Menu.PowerAlarmForm.Color=GUI_WHITE;
// 	App.Menu.PowerAlarmForm.RectWrap.x0=0;
// 	App.Menu.PowerAlarmForm.RectWrap.y0=30;
// 	App.Menu.PowerAlarmForm.RectWrap.x1=320;
// 	App.Menu.PowerAlarmForm.RectWrap.y1=210;
// 	
// 	System.Gui.Form.AddFillArea(&App.Menu.PowerAlarmForm,&PowerFillArea);
// 	PowerFillArea.ID=PromptAreaID10;
// 	PowerFillArea.PaintColor=GUI_ORANGE;
// 	PowerFillArea.RectWrap.x0=0;
// 	PowerFillArea.RectWrap.y0=30;
// 	PowerFillArea.RectWrap.x1=320;
// 	PowerFillArea.RectWrap.y1=60;	
// 	
// 	System.Gui.Form.AddLabel(&App.Menu.PowerAlarmForm,&PowerName);
// 	PowerName.ID=LabelID1+17;
// 	PowerName.Font=GUI_FONT_8X18;
// 	PowerName.PaintColor=GUI_BROWN;
// 	PowerName.TextMode=GUI_TEXTMODE_TRANS;
// 	PowerName.Count=0;
// 	PowerName.StringBlockPointer=SetupString[PowerAlarm];
// 	PowerName.X=75;
// 	PowerName.Y=35;	

// 	System.Gui.Form.AddListBox(&App.Menu.PowerAlarmForm,&PowerList);
// 	PowerList.x0=0;
// 	PowerList.y0=50;
// 	PowerList.x1=320;
// 	PowerList.y1=210;
// 	PowerList.xSize=15;
// 	PowerList.ySize=15;
// 	
// 	PowerList.Type=IconList;
// 	PowerList.selectedItem=0;
// 	PowerList.ItemsPerpage=4;//页最大条目
// 	PowerList.SelectedIndexes=0;//默认为0，也就是第一条
// 	PowerList.TopItem=0;//默认为0，也就是从第一个索引号开始
// 	PowerList.TotalItems=0;
// 	
// 	PowerList.TextMode=GUI_TEXTMODE_TRANS;
// 	PowerList.BackColor=GUI_WHITE;
// 	PowerList.PaintColor=GUI_BLACK;
// 	
// 	PowerList.SelectedBackColor=GUI_LIGHTGRAY;
// 	PowerList.SelectedPaintColor=GUI_WHITE;
// 	
// 	PowerList.Font=GUI_FONT_8X18;
// 	PowerList.ID=0x57;

// 	System.Gui.Form.AddLists(&PowerList,&items[1],4,(u32*)SetupIconAdd[1]);
// }
/*********************************************************************************

*描述		：警号鸣响时间设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void SetupSirenTimeFunc(void)
{
	//警鸣时间
	static FillArea		sirenFill;
	static FillArea		line;
	static Label		sirenName;
	static Label		sirenTime;
	static Label		sirenTimes;
	static TextBox		timeTextBox;
	
	System.Gui.Form.Init(&App.Menu.SirenTimeForm);
	App.Menu.SirenTimeForm.FillAreaCount=2;
	App.Menu.SirenTimeForm.LabelCount=3;
	App.Menu.SirenTimeForm.TextBoxCount=1;
	App.Menu.SirenTimeForm.FormID=Form14;
	App.Menu.SirenTimeForm.Color=GUI_WHITE;
	App.Menu.SirenTimeForm.RectWrap.x0=0;
	App.Menu.SirenTimeForm.RectWrap.y0=30;
	App.Menu.SirenTimeForm.RectWrap.x1=320;
	App.Menu.SirenTimeForm.RectWrap.y1=240;	
	App.Menu.SirenTimeForm.DefaultFocusTextBoxPointer=&timeTextBox;
	App.Menu.SirenTimeForm.FocusTextBoxPointer=&timeTextBox;
    
  timeTextBox.NextTextBoxPointer=&timeTextBox;
  timeTextBox.ParentTextBoxPointer=&timeTextBox;
		
	System.Gui.Form.AddFillArea(&App.Menu.SirenTimeForm,&sirenFill);
	sirenFill.ID=PromptAreaID11;
	sirenFill.PaintColor=TopLabelColor;
	sirenFill.RectWrap.x0=0;
	sirenFill.RectWrap.y0=30;
	sirenFill.RectWrap.x1=320;
	sirenFill.RectWrap.y1=80;

	System.Gui.Form.AddFillArea(&App.Menu.SirenTimeForm,&line);
	line.ID=PromptAreaID11;
	line.PaintColor=GUI_LIGHTGRAY;
	line.RectWrap.x0=10;
	line.RectWrap.y0=160;
	line.RectWrap.x1=310;
	line.RectWrap.y1=160;
	
	System.Gui.Form.AddLabel(&App.Menu.SirenTimeForm,&sirenName);
	sirenName.ID=LabelID1+18;
	sirenName.Font=ListNameFont;
	sirenName.PaintColor=GUI_BLACK;
	sirenName.TextMode=GUI_TEXTMODE_TRANS;
	sirenName.Count=0;
	sirenName.StringBlockPointer=SetupString[Siren];
	sirenName.X=75;
	sirenName.Y=45;	
	
	System.Gui.Form.AddLabel(&App.Menu.SirenTimeForm,&sirenTimes);
	sirenTimes.ID=LabelID1+4;
	sirenTimes.Font=ListFont;
	sirenTimes.PaintColor=GUI_LIGHTGRAY;
	sirenTimes.TextMode=GUI_TEXTMODE_TRANS;
  sirenTimes.Type=0xC0;
	sirenTimes.Count=3;
	sirenTimes.StringBlockPointer=SetupString[Siren];
	sirenTimes.X=205;
	sirenTimes.Y=135;

	System.Gui.Form.AddLabel(&App.Menu.SirenTimeForm,&sirenTime);
	sirenTime.ID=LabelID1+5;
	sirenTime.Font=ListFont;
	sirenTime.PaintColor=GUI_BROWN;
	sirenTime.TextMode=GUI_TEXTMODE_TRANS;
  sirenTime.Type=0xC0;
	sirenTime.Count=1;
	sirenTime.StringBlockPointer=SetupString[Siren];
	sirenTime.X=10;
	sirenTime.Y=135;
	
	System.Gui.Form.AddTextBox(&App.Menu.SirenTimeForm,&timeTextBox);
	timeTextBox.x=175;
	timeTextBox.y=136;
	timeTextBox.MaxDataPointer=20;
	timeTextBox.MinDataPointer=0;
	timeTextBox.Type=GuiDataTypeByteDec;
	timeTextBox.BackColor=GUI_LIGHTGRAY;
	timeTextBox.PaintColor=GUI_RED;
	timeTextBox.Font=GUI_FONT_20_ASCII;
	timeTextBox.ID=0x09;
	timeTextBox.TextMode=GUI_TEXTMODE_NORMAL;
	timeTextBox.RectWrap.x0=175;
	timeTextBox.RectWrap.x1=190;
	timeTextBox.RectWrap.y0=136;
	timeTextBox.RectWrap.y1=156;	
	timeTextBox.DataPointer=(void*)&App.Data.SrienTimes;	
	timeTextBox.DataAddr=SirenDelayAddr;
	App.Data.SrienTimes=5;	
	
}
/*********************************************************************************

*描述		：振铃次数设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void SetupRingTimeFunc(void)
{
	//振鈴
	static FillArea		ringFill;
	static FillArea		line;
	static Label		ringName;
	static Label		ringTimes;
	static Label		ringTimesRange;
	static TextBox		ringTimesTextBox;
	
	System.Gui.Form.Init(&App.Menu.RingTimesForm);
	App.Menu.RingTimesForm.FillAreaCount=2;
	App.Menu.RingTimesForm.TextBoxCount=1;
	App.Menu.RingTimesForm.LabelCount=3;
	App.Menu.RingTimesForm.FormID=Form14;
	App.Menu.RingTimesForm.Color=GUI_WHITE;
	App.Menu.RingTimesForm.RectWrap.x0=0;
	App.Menu.RingTimesForm.RectWrap.y0=30;
	App.Menu.RingTimesForm.RectWrap.x1=320;
	App.Menu.RingTimesForm.RectWrap.y1=240;
	App.Menu.RingTimesForm.DefaultFocusTextBoxPointer=&ringTimesTextBox;
	App.Menu.RingTimesForm.FocusTextBoxPointer=&ringTimesTextBox;
    
    ringTimesTextBox.NextTextBoxPointer=&ringTimesTextBox;
    ringTimesTextBox.ParentTextBoxPointer=&ringTimesTextBox;
	
	System.Gui.Form.AddFillArea(&App.Menu.RingTimesForm,&ringFill);
	ringFill.ID=PromptAreaID11;
	ringFill.PaintColor=TopLabelColor;
	ringFill.RectWrap.x0=0;
	ringFill.RectWrap.y0=30;
	ringFill.RectWrap.x1=320;
	ringFill.RectWrap.y1=80;
	
	System.Gui.Form.AddFillArea(&App.Menu.RingTimesForm,&line);
	line.ID=PromptAreaID1+2;
	line.PaintColor=GUI_ORANGE;
	line.RectWrap.x0=10;
	line.RectWrap.y0=160;
	line.RectWrap.x1=310;
	line.RectWrap.y1=160;
	
	System.Gui.Form.AddLabel(&App.Menu.RingTimesForm,&ringName);
	ringName.ID=LabelID1+18;
	ringName.Font=ListNameFont;
	ringName.PaintColor=GUI_BLACK;
	ringName.TextMode=GUI_TEXTMODE_TRANS;
	ringName.Count=0;
	ringName.StringBlockPointer=SetupString[Ring];
	ringName.X=75;
	ringName.Y=45;	
	
	System.Gui.Form.AddLabel(&App.Menu.RingTimesForm,&ringTimesRange);
	ringTimesRange.ID=LabelID1+9;
	ringTimesRange.Font=GUI_FONT_16B_1;
	ringTimesRange.PaintColor=GUI_GRAY;
	ringTimesRange.TextMode=GUI_TEXTMODE_TRANS;
  ringTimesRange.Type=0xC0;
	ringTimesRange.Count=3;
	ringTimesRange.StringBlockPointer=SetupString[Ring];
	ringTimesRange.X=205;
	ringTimesRange.Y=140;

	System.Gui.Form.AddLabel(&App.Menu.RingTimesForm,&ringTimes);
	ringTimes.ID=LabelID1+7;
	ringTimes.Font=ListFont;
	ringTimes.PaintColor=GUI_BROWN;
	ringTimes.TextMode=GUI_TEXTMODE_TRANS;
  ringTimes.Type=0xC0;
	ringTimes.Count=1;
	ringTimes.StringBlockPointer=SetupString[Ring];
	ringTimes.X=10;
	ringTimes.Y=140;
	
	System.Gui.Form.AddTextBox(&App.Menu.RingTimesForm,&ringTimesTextBox);
	ringTimesTextBox.x=150;
	ringTimesTextBox.y=136;
	ringTimesTextBox.MaxDataPointer=20;
	ringTimesTextBox.MinDataPointer=0;
	ringTimesTextBox.Type=GuiDataTypeByteDec;
	ringTimesTextBox.BackColor=GUI_LIGHTGRAY;
	ringTimesTextBox.PaintColor=GUI_RED;
	ringTimesTextBox.Font=GUI_FONT_20_ASCII;
	ringTimesTextBox.ID=0x0C;
	ringTimesTextBox.TextMode=GUI_TEXTMODE_NORMAL;
	ringTimesTextBox.RectWrap.x0=150;
	ringTimesTextBox.RectWrap.x1=170;
	ringTimesTextBox.RectWrap.y0=136;
	ringTimesTextBox.RectWrap.y1=156;	
	ringTimesTextBox.DataPointer=(void*)&App.Data.RingTimes;	
	ringTimesTextBox.DataAddr=RingTimesAddr;
	
}
void RingTimesSetup(void)
{
	u8 buffer[15];
	u8 temp;
	System.Device.Storage.Parameter.StorageReadData(RingTimesAddr,(u8*)&temp,1);
	sprintf((char*)buffer,"ATS0=%d\r\n",temp);
	System.Device.Gsm.GsmATSend(buffer,"OK",2);	
  System.Device.Gsm.GsmATSend("AT&W0","OK",1);
}
/*********************************************************************************

*描述		：屏幕保护时间设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void ScreensaverForm(void)
{
	string *items;
	
	//休眠
	static FillArea		ScreensaverFill;
	static Label		ScreensaverName;
	static ListBox		ScreensaverList;
	
	items=(string *)WeekString[2];
	
	System.Gui.Form.Init(&App.Menu.ScreensaverForm);
	App.Menu.ScreensaverForm.FillAreaCount=1;
	App.Menu.ScreensaverForm.LabelCount=1;
	App.Menu.ScreensaverForm.ListCount=1;
	App.Menu.ScreensaverForm.FormID=Form14;
	App.Menu.ScreensaverForm.Color=GUI_WHITE;
	App.Menu.ScreensaverForm.RectWrap.x0=0;
	App.Menu.ScreensaverForm.RectWrap.y0=30;
	App.Menu.ScreensaverForm.RectWrap.x1=320;
	App.Menu.ScreensaverForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.ScreensaverForm,&ScreensaverFill);
	ScreensaverFill.ID=PromptAreaID11;
	ScreensaverFill.PaintColor=TopLabelColor;
	ScreensaverFill.RectWrap.x0=0;
	ScreensaverFill.RectWrap.y0=30;
	ScreensaverFill.RectWrap.x1=320;
	ScreensaverFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.ScreensaverForm,&ScreensaverName);
	ScreensaverName.ID=LabelID1+18;
	ScreensaverName.Font=ListNameFont;
	ScreensaverName.PaintColor=GUI_BLACK;
	ScreensaverName.TextMode=GUI_TEXTMODE_TRANS;
	ScreensaverName.Count=0;
	ScreensaverName.StringBlockPointer=SetupString[Screensaver];
	ScreensaverName.X=75;
	ScreensaverName.Y=43;		
	
	System.Gui.Form.AddListBox(&App.Menu.ScreensaverForm,&ScreensaverList);
	ScreensaverList.x0=0;
	ScreensaverList.y0=80;
	ScreensaverList.x1=320;
	ScreensaverList.y1=240;
	ScreensaverList.xSize=15;
	ScreensaverList.ySize=15;
	ScreensaverList.Select=1;
	
	ScreensaverList.selectedItem=0;
    ScreensaverList.ItemsHeight=40;
	ScreensaverList.ItemsPerpage=4;//页最大条目
	ScreensaverList.SelectedIndexes=0;//默认为0，也就是第一条
	ScreensaverList.TopItem=0;//默认为0，也就是从第一个索引号开始
	ScreensaverList.TotalItems=0;
	
	ScreensaverList.TextMode=GUI_TEXTMODE_TRANS;
	ScreensaverList.BackColor=GUI_WHITE;
	ScreensaverList.PaintColor=GUI_BLACK;
	
	ScreensaverList.SelectedBackColor=MoveBarColor;
	ScreensaverList.SelectedPaintColor=GUI_WHITE;
	
	ScreensaverList.Font=ListFont;
	ScreensaverList.ID=0x56;
	ScreensaverList.Type=CheckList;

	System.Gui.Form.AddLists(&ScreensaverList,items,6,(u32*)CheckBoxState[ScreensaverCheckBox],null,null,null,6);
}
/*********************************************************************************

*描述		：短信回复设定UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
//static void SetSMSReport(void)
//{
//	//按键音
//	static	FillArea	SMSReportArea;
//	static	Label		SMSReportName;
//	static 	ListBox		SMSReportList;
//	
//	string *items;
//	items=(string *)SetupString[SMSReport];
//	
//	System.Gui.Form.Init(&App.Menu.SMSReportForm);
//	App.Menu.SMSReportForm.FillAreaCount=1;
//	App.Menu.SMSReportForm.ListCount=1;	
//	App.Menu.SMSReportForm.LabelCount=1;
//	App.Menu.SMSReportForm.FormID=Form15;
//	App.Menu.SMSReportForm.Color=GUI_WHITE;
//	App.Menu.SMSReportForm.RectWrap.x0=0;
//	App.Menu.SMSReportForm.RectWrap.y0=30;
//	App.Menu.SMSReportForm.RectWrap.x1=320;
//	App.Menu.SMSReportForm.RectWrap.y1=240;
//	
//	System.Gui.Form.AddFillArea(&App.Menu.SMSReportForm,&SMSReportArea);
//	SMSReportArea.ID=PromptAreaID19;
//	SMSReportArea.PaintColor=GREEN;
//	SMSReportArea.RectWrap.x0=0;
//	SMSReportArea.RectWrap.y0=30;
//	SMSReportArea.RectWrap.x1=320;
//	SMSReportArea.RectWrap.y1=80;	
//	
//	System.Gui.Form.AddLabel(&App.Menu.SMSReportForm,&SMSReportName);
//	SMSReportName.ID=LabelID1+16;
//	SMSReportName.Font=ListNameFont;
//	SMSReportName.PaintColor=GUI_BLACK;
//	SMSReportName.TextMode=GUI_TEXTMODE_TRANS;
//	SMSReportName.Count=0;
//	SMSReportName.StringBlockPointer=SetupString[SMSReport];
//	SMSReportName.X=120;
//	SMSReportName.Y=45;	
//	
//	System.Gui.Form.AddListBox(&App.Menu.SMSReportForm,&SMSReportList);
//	SMSReportList.x0=0;
//	SMSReportList.y0=80;
//	SMSReportList.x1=320;
//	SMSReportList.y1=240;
//	SMSReportList.xSize=15;
//	SMSReportList.ySize=15;
//	
//	SMSReportList.selectedItem=0;
//    SMSReportList.ItemsHeight=40;
//	SMSReportList.ItemsPerpage=1;//页最大条目
//	SMSReportList.SelectedIndexes=0;//默认为0，也就是第一条
//	SMSReportList.TopItem=0;//默认为0，也就是从第一个索引号开始
//	SMSReportList.TotalItems=0;
//	
//	SMSReportList.TextMode=GUI_TEXTMODE_TRANS;
//	SMSReportList.BackColor=GUI_WHITE;
//	SMSReportList.PaintColor=GUI_BLACK;
//	
//	SMSReportList.SelectedBackColor=GUI_BLUE;
//	SMSReportList.SelectedPaintColor=GUI_WHITE;
//	
//	SMSReportList.Font=ListFont;
//	SMSReportList.ID=0x56;
//	SMSReportList.Type=SwitchList;

//	System.Gui.Form.AddLists(&SMSReportList,&items[1],1,(u32*)CheckBoxState[SMSReportCheckBox],null,null,null);	
//}
/*********************************************************************************

*描述		：掉电提示方式设定
*输入参数	：无
*输出参数	：无

*********************************************************************************/
static void	SetPanelPowerOFF(void)
{
	string *items;
	
	//休眠
	static FillArea		PanelPowerOFFFill;
	static Label		PanelPowerOFFName;
	static ListBox		PanelPowerOFFList;
	
	items=(string *)SetupString[Power];
	
	System.Gui.Form.Init(&App.Menu.PanelPowerOFFForm);
	App.Menu.PanelPowerOFFForm.FillAreaCount=1;
	App.Menu.PanelPowerOFFForm.LabelCount=1;
	App.Menu.PanelPowerOFFForm.ListCount=1;
	App.Menu.PanelPowerOFFForm.FormID=Form14;
	App.Menu.PanelPowerOFFForm.Color=GUI_WHITE;
	App.Menu.PanelPowerOFFForm.RectWrap.x0=0;
	App.Menu.PanelPowerOFFForm.RectWrap.y0=30;
	App.Menu.PanelPowerOFFForm.RectWrap.x1=320;
	App.Menu.PanelPowerOFFForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.PanelPowerOFFForm,&PanelPowerOFFFill);
	PanelPowerOFFFill.ID=PromptAreaID11;
	PanelPowerOFFFill.PaintColor=TopLabelColor;
	PanelPowerOFFFill.RectWrap.x0=0;
	PanelPowerOFFFill.RectWrap.y0=30;
	PanelPowerOFFFill.RectWrap.x1=320;
	PanelPowerOFFFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.PanelPowerOFFForm,&PanelPowerOFFName);
	PanelPowerOFFName.ID=LabelID1+18;
	PanelPowerOFFName.Font=ListNameFont;
	PanelPowerOFFName.PaintColor=GUI_BLACK;
	PanelPowerOFFName.TextMode=GUI_TEXTMODE_TRANS;
	PanelPowerOFFName.Count=1;
	PanelPowerOFFName.StringBlockPointer=SetupString[PowerAlarm];
	PanelPowerOFFName.X=75;
	PanelPowerOFFName.Y=45;		
	
	System.Gui.Form.AddListBox(&App.Menu.PanelPowerOFFForm,&PanelPowerOFFList);
	PanelPowerOFFList.x0=0;
	PanelPowerOFFList.y0=80;
	PanelPowerOFFList.x1=320;
	PanelPowerOFFList.y1=240;
	PanelPowerOFFList.xSize=15;
	PanelPowerOFFList.ySize=15;
	PanelPowerOFFList.Select=0;
	
	PanelPowerOFFList.selectedItem=0;
    PanelPowerOFFList.ItemsHeight=40;
	PanelPowerOFFList.ItemsPerpage=4;//页最大条目
	PanelPowerOFFList.SelectedIndexes=0;//默认为0，也就是第一条
	PanelPowerOFFList.TopItem=0;//默认为0，也就是从第一个索引号开始
	PanelPowerOFFList.TotalItems=0;
	
	PanelPowerOFFList.TextMode=GUI_TEXTMODE_TRANS;
	PanelPowerOFFList.BackColor=GUI_WHITE;
	PanelPowerOFFList.PaintColor=GUI_BLACK;
	
	PanelPowerOFFList.SelectedBackColor=MoveBarColor;
	PanelPowerOFFList.SelectedPaintColor=GUI_WHITE;
	
	PanelPowerOFFList.Font=ListFont;
	PanelPowerOFFList.ID=0x56;
	PanelPowerOFFList.Type=CheckList;

	System.Gui.Form.AddLists(&PanelPowerOFFList,items,4,(u32*)CheckBoxState[PowerOFFCheckBox],null,null,null,4);
}
//void ContactIDSeting(void)
//{
//	string *items;

//	static FillArea		CIDArea;
//	static Label		CIDName;
//	static ListBox		CIDList;
//	items=(string *)SetupString[CIDString];
//	
//	System.Gui.Form.Init(&App.Menu.ContactIDForm);
//	App.Menu.ContactIDForm.FillAreaCount=1;
//	App.Menu.ContactIDForm.ListCount=1;
//	App.Menu.ContactIDForm.LabelCount=1;
//	App.Menu.ContactIDForm.FormID=Form8+20;
//	App.Menu.ContactIDForm.Color=GUI_WHITE;
//	App.Menu.ContactIDForm.RectWrap.x0=0;
//	App.Menu.ContactIDForm.RectWrap.y0=30;
//	App.Menu.ContactIDForm.RectWrap.x1=320;
//	App.Menu.ContactIDForm.RectWrap.y1=240;

//	System.Gui.Form.AddFillArea(&App.Menu.ContactIDForm,&CIDArea);
//	CIDArea.ID=PromptAreaID19;
//	CIDArea.PaintColor=TopLabelColor;
//	CIDArea.RectWrap.x0=0;
//	CIDArea.RectWrap.y0=30;
//	CIDArea.RectWrap.x1=320;
//	CIDArea.RectWrap.y1=80;	
//	
//	System.Gui.Form.AddLabel(&App.Menu.ContactIDForm,&CIDName);
//	CIDName.ID=LabelID1+16;
//	CIDName.Font=ListNameFont;
//	CIDName.PaintColor=GUI_BLACK;
//	CIDName.TextMode=GUI_TEXTMODE_TRANS;
//	CIDName.Count=0;
//	CIDName.StringBlockPointer=SetupString[CIDString];
//	CIDName.X=105;
//	CIDName.Y=45;		
//	
//	System.Gui.Form.AddListBox(&App.Menu.ContactIDForm,&CIDList);
//	CIDList.x0=0;
//	CIDList.y0=80;
//	CIDList.x1=320;
//	CIDList.y1=240;
//	CIDList.xSize=15;
//	CIDList.ySize=15;
//	
//	CIDList.selectedItem=0;
//    CIDList.ItemsHeight=37;
//	CIDList.ItemsPerpage=3;//页最大条目
//	CIDList.SelectedIndexes=0;//默认为0，也就是第一条
//	CIDList.TopItem=0;//默认为0，也就是从第一个索引号开始
//	CIDList.TotalItems=0;
//	
//	CIDList.TextMode=GUI_TEXTMODE_TRANS;
//	CIDList.BackColor=GUI_WHITE;
//	CIDList.PaintColor=GUI_BLACK;
//	
//	CIDList.SelectedBackColor=MoveBarColor;
//	CIDList.SelectedPaintColor=GUI_WHITE;
//	
//	CIDList.Font=ListFont;
//	CIDList.ID=0x56;
//	CIDList.Type=CheckList;

//	System.Gui.Form.AddLists(&CIDList,&items[1],3,(u32*)CheckBoxState[CIDCheckBox],null,null,null);
//}
/*********************************************************************************

*描述		：上电提示设定
*输入参数	：无
*输出参数	：无

*********************************************************************************/	
//static void	SetPanelPowerON(void)
//{
//	string *items;
//	
//	//休眠
//	static FillArea		PanelPowerONFill;
//	static Label		PanelPowerONName;
//	static ListBox		PanelPowerONList;
//	
//	items=(string *)SetupString[Power];
//	
//	System.Gui.Form.Init(&App.Menu.PanelPowerONForm);
//	App.Menu.PanelPowerONForm.FillAreaCount=1;
//	App.Menu.PanelPowerONForm.LabelCount=1;
//	App.Menu.PanelPowerONForm.ListCount=1;
//	App.Menu.PanelPowerONForm.FormID=Form14;
//	App.Menu.PanelPowerONForm.Color=GUI_WHITE;
//	App.Menu.PanelPowerONForm.RectWrap.x0=0;
//	App.Menu.PanelPowerONForm.RectWrap.y0=30;
//	App.Menu.PanelPowerONForm.RectWrap.x1=320;
//	App.Menu.PanelPowerONForm.RectWrap.y1=210;

//	System.Gui.Form.AddFillArea(&App.Menu.PanelPowerONForm,&PanelPowerONFill);
//	PanelPowerONFill.ID=PromptAreaID11;
//	PanelPowerONFill.PaintColor=GUI_ORANGE;
//	PanelPowerONFill.RectWrap.x0=0;
//	PanelPowerONFill.RectWrap.y0=30;
//	PanelPowerONFill.RectWrap.x1=320;
//	PanelPowerONFill.RectWrap.y1=60;
//	
//	System.Gui.Form.AddLabel(&App.Menu.PanelPowerONForm,&PanelPowerONName);
//	PanelPowerONName.ID=LabelID1+18;
//	PanelPowerONName.Font=GUI_FONT_8X18;
//	PanelPowerONName.PaintColor=GUI_BROWN;
//	PanelPowerONName.TextMode=GUI_TEXTMODE_TRANS;
//	PanelPowerONName.Count=2;
//	PanelPowerONName.StringBlockPointer=SetupString[PowerAlarm];
//	PanelPowerONName.X=75;
//	PanelPowerONName.Y=35;		
//	
//	System.Gui.Form.AddListBox(&App.Menu.PanelPowerONForm,&PanelPowerONList);
//	PanelPowerONList.x0=0;
//	PanelPowerONList.y0=60;
//	PanelPowerONList.x1=320;
//	PanelPowerONList.y1=210;
//	PanelPowerONList.xSize=15;
//	PanelPowerONList.ySize=15;
//	PanelPowerONList.Select=0;
//	
//	PanelPowerONList.selectedItem=0;
//	PanelPowerONList.ItemsPerpage=4;//页最大条目
//	PanelPowerONList.SelectedIndexes=0;//默认为0，也就是第一条
//	PanelPowerONList.TopItem=0;//默认为0，也就是从第一个索引号开始
//	PanelPowerONList.TotalItems=0;
//	
//	PanelPowerONList.TextMode=GUI_TEXTMODE_TRANS;
//	PanelPowerONList.BackColor=GUI_WHITE;
//	PanelPowerONList.PaintColor=GUI_DARKGRAY;
//	
//	PanelPowerONList.SelectedBackColor=GUI_BLUE;
//	PanelPowerONList.SelectedPaintColor=GUI_WHITE;
//	
//	PanelPowerONList.Font=GUI_FONT_8X18;
//	PanelPowerONList.ID=0x56;
//	PanelPowerONList.Type=CheckList;

//	System.Gui.Form.AddLists(&PanelPowerONList,items,4,(u32*)CheckBoxState[PowerONCheckBox],null,null);
//}
/*********************************************************************************

*描述		：主机低电提示设置 选项UI
*输入参数	：无
*输出参数	：无

*********************************************************************************/
//static void	SetPanelLowBattery(void)
//{
//	string *items;
//	
//	static FillArea		batteryLowFill;
//	static Label		batteryLowName;
//	static ListBox		batteryLowList;
//	
//	items=(string *)SetupString[Power];
//	
//	System.Gui.Form.Init(&App.Menu.PanelLowForm);
//	App.Menu.PanelLowForm.FillAreaCount=1;
//	App.Menu.PanelLowForm.LabelCount=1;
//	App.Menu.PanelLowForm.ListCount=1;
//	App.Menu.PanelLowForm.FormID=Form14;
//	App.Menu.PanelLowForm.Color=GUI_WHITE;
//	App.Menu.PanelLowForm.RectWrap.x0=0;
//	App.Menu.PanelLowForm.RectWrap.y0=30;
//	App.Menu.PanelLowForm.RectWrap.x1=320;
//	App.Menu.PanelLowForm.RectWrap.y1=210;

//	System.Gui.Form.AddFillArea(&App.Menu.PanelLowForm,&batteryLowFill);
//	batteryLowFill.ID=PromptAreaID11;
//	batteryLowFill.PaintColor=GUI_ORANGE;
//	batteryLowFill.RectWrap.x0=0;
//	batteryLowFill.RectWrap.y0=30;
//	batteryLowFill.RectWrap.x1=320;
//	batteryLowFill.RectWrap.y1=60;
//	
//	System.Gui.Form.AddLabel(&App.Menu.PanelLowForm,&batteryLowName);
//	batteryLowName.ID=LabelID1+18;
//	batteryLowName.Font=GUI_FONT_8X18;
//	batteryLowName.PaintColor=GUI_BROWN;
//	batteryLowName.TextMode=GUI_TEXTMODE_TRANS;
//	batteryLowName.Count=3;
//	batteryLowName.StringBlockPointer=SetupString[PowerAlarm];
//	batteryLowName.X=75;
//	batteryLowName.Y=35;		
//	
//	System.Gui.Form.AddListBox(&App.Menu.PanelLowForm,&batteryLowList);
//	batteryLowList.x0=0;
//	batteryLowList.y0=60;
//	batteryLowList.x1=320;
//	batteryLowList.y1=210;
//	batteryLowList.xSize=15;
//	batteryLowList.ySize=15;
//	batteryLowList.Select=0;
//	
//	batteryLowList.selectedItem=0;
//	batteryLowList.ItemsPerpage=4;//页最大条目
//	batteryLowList.SelectedIndexes=0;//默认为0，也就是第一条
//	batteryLowList.TopItem=0;//默认为0，也就是从第一个索引号开始
//	batteryLowList.TotalItems=0;
//	
//	batteryLowList.TextMode=GUI_TEXTMODE_TRANS;
//	batteryLowList.BackColor=GUI_WHITE;
//	batteryLowList.PaintColor=GUI_DARKGRAY;
//	
//	batteryLowList.SelectedBackColor=GUI_BLUE;
//	batteryLowList.SelectedPaintColor=GUI_WHITE;
//	
//	batteryLowList.Font=GUI_FONT_8X18;
//	batteryLowList.ID=0x56;
//	batteryLowList.Type=CheckList;

//	System.Gui.Form.AddLists(&batteryLowList,items,4,(u32*)CheckBoxState[PowerLowCheckBox],null,null);
//}
/*********************************************************************************

*描述		：配件低电提示
*输入参数	：无
*输出参数	：无

*********************************************************************************/
//static void	SetDetectorLow(void)
//{
//	string *items;
//	
//	//休眠
//	static FillArea		detectorLowFill;
//	static Label		detectorLowName;
//	static ListBox		detectorLowList;
//	
//	items=(string *)SetupString[Power];
//	
//	System.Gui.Form.Init(&App.Menu.DetectorLowForm);
//	App.Menu.DetectorLowForm.FillAreaCount=1;
//	App.Menu.DetectorLowForm.LabelCount=1;
//	App.Menu.DetectorLowForm.ListCount=1;
//	App.Menu.DetectorLowForm.FormID=Form14;
//	App.Menu.DetectorLowForm.Color=GUI_WHITE;
//	App.Menu.DetectorLowForm.RectWrap.x0=0;
//	App.Menu.DetectorLowForm.RectWrap.y0=30;
//	App.Menu.DetectorLowForm.RectWrap.x1=320;
//	App.Menu.DetectorLowForm.RectWrap.y1=210;

//	System.Gui.Form.AddFillArea(&App.Menu.DetectorLowForm,&detectorLowFill);
//	detectorLowFill.ID=PromptAreaID11;
//	detectorLowFill.PaintColor=GUI_ORANGE;
//	detectorLowFill.RectWrap.x0=0;
//	detectorLowFill.RectWrap.y0=30;
//	detectorLowFill.RectWrap.x1=320;
//	detectorLowFill.RectWrap.y1=60;
//	
//	System.Gui.Form.AddLabel(&App.Menu.DetectorLowForm,&detectorLowName);
//	detectorLowName.ID=LabelID1+18;
//	detectorLowName.Font=GUI_FONT_8X18;
//	detectorLowName.PaintColor=GUI_BROWN;
//	detectorLowName.TextMode=GUI_TEXTMODE_TRANS;
//	detectorLowName.Count=4;
//	detectorLowName.StringBlockPointer=SetupString[PowerAlarm];
//	detectorLowName.X=75;
//	detectorLowName.Y=35;		
//	
//	System.Gui.Form.AddListBox(&App.Menu.DetectorLowForm,&detectorLowList);
//	detectorLowList.x0=0;
//	detectorLowList.y0=60;
//	detectorLowList.x1=320;
//	detectorLowList.y1=210;
//	detectorLowList.xSize=15;
//	detectorLowList.ySize=15;
//	detectorLowList.Select=0;
//	
//	detectorLowList.selectedItem=0;
//	detectorLowList.ItemsPerpage=4;//页最大条目
//	detectorLowList.SelectedIndexes=0;//默认为0，也就是第一条
//	detectorLowList.TopItem=0;//默认为0，也就是从第一个索引号开始
//	detectorLowList.TotalItems=0;
//	
//	detectorLowList.TextMode=GUI_TEXTMODE_TRANS;
//	detectorLowList.BackColor=GUI_WHITE;
//	detectorLowList.PaintColor=GUI_DARKGRAY;
//	
//	detectorLowList.SelectedBackColor=GUI_BLUE;
//	detectorLowList.SelectedPaintColor=GUI_WHITE;
//	
//	detectorLowList.Font=GUI_FONT_8X18;
//	detectorLowList.ID=0x56;
//	detectorLowList.Type=CheckList;

//	System.Gui.Form.AddLists(&detectorLowList,items,4,(u32*)CheckBoxState[DetectorCheckBox],null,null);	
//}

/*static void AlarmModeInit(void)
{
	string *items;
	//短信
	static FillArea		AlarmModeArea;
	static Label		AlarmModeName;
	static ListBox		AlarmModeList;
	items=(string *)SetupString[AlarmModeString];
	
	System.Gui.Form.Init(&App.Menu.AlarmModeForm);
	App.Menu.AlarmModeForm.FillAreaCount=1;
	App.Menu.AlarmModeForm.ListCount=1;
	App.Menu.AlarmModeForm.LabelCount=1;
	App.Menu.AlarmModeForm.FormID=Form8+20;
	App.Menu.AlarmModeForm.Color=GUI_WHITE;
	App.Menu.AlarmModeForm.RectWrap.x0=0;
	App.Menu.AlarmModeForm.RectWrap.y0=30;
	App.Menu.AlarmModeForm.RectWrap.x1=320;
	App.Menu.AlarmModeForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.AlarmModeForm,&AlarmModeArea);
	AlarmModeArea.ID=PromptAreaID19;
	AlarmModeArea.PaintColor=GREEN;
	AlarmModeArea.RectWrap.x0=0;
	AlarmModeArea.RectWrap.y0=30;
	AlarmModeArea.RectWrap.x1=320;
	AlarmModeArea.RectWrap.y1=80;	
	
	System.Gui.Form.AddLabel(&App.Menu.AlarmModeForm,&AlarmModeName);
	AlarmModeName.ID=LabelID1+16;
	AlarmModeName.Font=ListNameFont;
	AlarmModeName.PaintColor=GUI_BROWN;
	AlarmModeName.TextMode=GUI_TEXTMODE_TRANS;
	AlarmModeName.Count=3;
	AlarmModeName.StringBlockPointer=SetupString[AlarmModeString];
	AlarmModeName.X=75;
	AlarmModeName.Y=45;		
	
	System.Gui.Form.AddListBox(&App.Menu.AlarmModeForm,&AlarmModeList);
	AlarmModeList.x0=0;
	AlarmModeList.y0=80;
	AlarmModeList.x1=320;
	AlarmModeList.y1=240;
	AlarmModeList.xSize=15;
	AlarmModeList.ySize=15;
	
    AlarmModeList.Select=1;
	AlarmModeList.selectedItem=0;
    AlarmModeList.ItemsHeight=40;
	AlarmModeList.ItemsPerpage=3;//页最大条目
	AlarmModeList.SelectedIndexes=0;//默认为0，也就是第一条
	AlarmModeList.TopItem=0;//默认为0，也就是从第一个索引号开始
	AlarmModeList.TotalItems=0;
	
	AlarmModeList.TextMode=GUI_TEXTMODE_TRANS;
	AlarmModeList.BackColor=GUI_WHITE;
	AlarmModeList.PaintColor=GUI_BLACK;
	
	AlarmModeList.SelectedBackColor=GUI_BLUE;
	AlarmModeList.SelectedPaintColor=GUI_WHITE;
	
	AlarmModeList.Font=ListFont;
	AlarmModeList.ID=0x56;
	AlarmModeList.Type=CheckList;

	System.Gui.Form.AddLists(&AlarmModeList,&items[0],3,(u32*)CheckBoxState[AlarmModeSelete],null,null,null);    
}*/



void SystemStatesInfo()
{
	string *items;
	//短信
	static FillArea		SystemStatesArea;
	static Label		SystemStatesName;
	static ListBox		SystemStatesList;
//	items=(string *)SetupString[AlarmModeString];
	
	System.Gui.Form.Init(&App.Menu.SystemStatesForm);
	App.Menu.SystemStatesForm.FillAreaCount=1;
	App.Menu.SystemStatesForm.ListCount=1;
	App.Menu.SystemStatesForm.LabelCount=1;
	App.Menu.SystemStatesForm.FormID=Form8+20;
	App.Menu.SystemStatesForm.Color=GUI_WHITE;
	App.Menu.SystemStatesForm.RectWrap.x0=0;
	App.Menu.SystemStatesForm.RectWrap.y0=30;
	App.Menu.SystemStatesForm.RectWrap.x1=320;
	App.Menu.SystemStatesForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.SystemStatesForm,&SystemStatesArea);
	SystemStatesArea.ID=PromptAreaID19;
	SystemStatesArea.PaintColor=GREEN;
	SystemStatesArea.RectWrap.x0=0;
	SystemStatesArea.RectWrap.y0=30;
	SystemStatesArea.RectWrap.x1=320;
	SystemStatesArea.RectWrap.y1=80;	
	
	System.Gui.Form.AddLabel(&App.Menu.SystemStatesForm,&SystemStatesName);
	SystemStatesName.ID=LabelID1+16;
	SystemStatesName.Font=ListNameFont;
	SystemStatesName.PaintColor=GUI_BROWN;
	SystemStatesName.TextMode=GUI_TEXTMODE_TRANS;
	SystemStatesName.Count=3;
	SystemStatesName.StringBlockPointer=SetupString[14];
	SystemStatesName.X=75;
	SystemStatesName.Y=43;		
	
//	System.Gui.Form.AddListBox(&App.Menu.AlarmModeForm,&AlarmModeList);
//	AlarmModeList.x0=0;
//	AlarmModeList.y0=80;
//	AlarmModeList.x1=320;
//	AlarmModeList.y1=240;
//	AlarmModeList.xSize=15;
//	AlarmModeList.ySize=15;
//	
//    AlarmModeList.Select=1;
//	AlarmModeList.selectedItem=0;
//    AlarmModeList.ItemsHeight=40;
//	AlarmModeList.ItemsPerpage=3;//页最大条目
//	AlarmModeList.SelectedIndexes=0;//默认为0，也就是第一条
//	AlarmModeList.TopItem=0;//默认为0，也就是从第一个索引号开始
//	AlarmModeList.TotalItems=0;
//	
//	AlarmModeList.TextMode=GUI_TEXTMODE_TRANS;
//	AlarmModeList.BackColor=GUI_WHITE;
//	AlarmModeList.PaintColor=GUI_BLACK;
//	
//	AlarmModeList.SelectedBackColor=GUI_BLUE;
//	AlarmModeList.SelectedPaintColor=GUI_WHITE;
//	
//	AlarmModeList.Font=ListFont;
//	AlarmModeList.ID=0x56;
//	AlarmModeList.Type=CheckList;

//	System.Gui.Form.AddLists(&AlarmModeList,&items[0],3,(u32*)CheckBoxState[AlarmModeSelete],null,null,null);  
}
/*********************************************************************************

*描述		：设置页面下的所有菜单和默认参数初始化
*输入参数	：无
*输出参数	：无

*********************************************************************************/
void SettingTab(void)
{
  SettingMenuList();
  
	Delayfunc();
	
	SetupAutoFunc();
	
	SetupDateFunc();	
	
	SetupVolumeFunc();
	
	SetupKeyToneFunc();
	
	SetupLanguageFunc();
	
	SetupSMSFunc();
	
//	SetupAlarmFunc();
	
	SetupSirenTimeFunc();
	
	SetupRingTimeFunc();
	
	ScreensaverForm();
	
	ContactIDMenuInit();
	
	SetPanelPowerOFF();

	AutoArmDisarmInit();
	
	SystemStatesInfo();
	
// AlarmModeInit();
//	SetPanelPowerON();
//	
//	SetPanelLowBattery();
//	
//	SetDetectorLow();
	
}
