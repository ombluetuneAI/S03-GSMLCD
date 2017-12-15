#include "ZoneMenu.h"
#include "APP.h"
#include "TaskMenu.h"

extern const unsigned char gImage_sensorsIco[][808];

u32 remoteSetupAddr[1]={9};

const u8 *ZonePath[4]=
{
    "0:/ICON/zone/remote.bmp",
    "0:/ICON/zone/zone.bmp",
    "0:/ICON/zone/siren.bmp",
    "0:/ICON/zone/Ready.bmp",
};

u8* const SensorsListTab[4]=
{
  "Remote",
	"Sensor",
	"Wireless Siren",
	"Ready to Arm",
};

u8* const SensorsLableTab[5]=
{
  "Sensors",
};
const string RemoteTab[3][3]=
{
	{"Sensors added:","Total","Trigger Sensor to Add"},
	{"Sound:"},
	{"Remote"},
};
const string RemteName[21]=
{
	"Add",
	"Siren",
	"Delete",
	"Code",
	"Total:",
	"Added:",
	"Please Press Any Key on Remote",
	"Pcs",
	"Sound ON/OFF",
	"Sound",
	"Delete Remote",
	"Delete Current Remote",
	"     Delete All",
	"       Cancel",
	"Trigger code",
	"Attribute",
	"Sirne setup",
	"Delete Sensors",
	"Delete Current Sensors",
	"     Delete All",
	"       Cancel",
};
static const string ZoneAttributeString[]=
{
	"Zone Attribute",
	"Normal",
	"Stay",
	"Chime",
	"Emgergency",
	"Delay",
	"Close",
};

const string ZoneTab[6]=
{
	"Zone List",
	"Seneor",
	"Siren",
	"Attribute:",
	"Event Code:",
	"Entry delay:"
	""
};


const string ZoneAattribute[]=
{
	"Disable","Alarm immediately","Doorbell","Chime","Home","Emergency",
};
const string EventCode[]=
{
	"Ambulanc","Fire","Panic","Silent","Burglar","Perimite","Gas",
};
const string WirelessSirenTab[3]=
{
	"Wirless Siren",
	"Output with alarm",
};

const string ReadyArmTab[3]=
{
	"Ready to Arm",
};



static const u32 CheckBoxState[6]=
{
	AttributeNormalAddr,
	AttributeStayAddr,
	AttributeDoorbellAddr,
	AttributeUrgentAddr,
	AttributeHomeAddr,
	AttributeCloseAddr,
};
static const u32 RemoteNameAddr[]=
{
	Remote1NameAddr,
	Remote2NameAddr,
	Remote3NameAddr,
	Remote4NameAddr,
	Remote5NameAddr,
	Remote6NameAddr,
	Remote7NameAddr,
	Remote8NameAddr,
};
static const u32 ZoneNameAddr[]=
{
	Zone1NameAddr,
	Zone2NameAddr,
	Zone3NameAddr,
	Zone4NameAddr,
	Zone5NameAddr,
	Zone6NameAddr,
	Zone7NameAddr,
	Zone8NameAddr,
	Zone9NameAddr,
	Zone10NameAddr,
	Zone11NameAddr,
	Zone12NameAddr,
	Zone13NameAddr,
	Zone14NameAddr,
	Zone15NameAddr,
	Zone16NameAddr,
	Zone17NameAddr,
	Zone18NameAddr,
	Zone19NameAddr,
	Zone20NameAddr,
	Zone21NameAddr,
	Zone22NameAddr,
	Zone23NameAddr,
	Zone24NameAddr,
	Zone25NameAddr,
	Zone26NameAddr,
	Zone27NameAddr,
	Zone28NameAddr,
	Zone29NameAddr,
	Zone30NameAddr,
};


static const u32 ReadyCheckState[]=
{
	IllegalArmAddr,
	WirelessSirenAddr,
	OutputWithalarmAddr,
};

static const u32 RemoteIDTab[8]=
{
	Remote1Addr,
	Remote2Addr,
	Remote3Addr,
	Remote4Addr,
	Remote5Addr,
	Remote6Addr,
	Remote7Addr,
	Remote8Addr,
};

static FillArea		WirelessSirenFill;
static Label		WirelessSirenName;
static ListBox		WirelessSirenBox;

extern u32 DetectorAddrID(u16 total);

extern const u32 FlashAdd[20][20];
#define DefenseList   		5
void SensorsListInit(void)
{
  string *items;
  static ListBox 	SensorsListBox;
	static Label	  SensorsListName;
  static FillArea SensorsArea;
  
  items=(string *)SensorsListTab;
  
  System.Gui.Form.Init(&App.Menu.SensorsListForm);
	App.Menu.SensorsListForm.FormID=Form4;
	App.Menu.SensorsListForm.FillAreaCount=1;
	App.Menu.SensorsListForm.LabelCount=1;
	App.Menu.SensorsListForm.ListCount=1;
	App.Menu.SensorsListForm.Color=GUI_WHITE;
	App.Menu.SensorsListForm.RectWrap.x0=0;
	App.Menu.SensorsListForm.RectWrap.y0=30;
	App.Menu.SensorsListForm.RectWrap.x1=320;
	App.Menu.SensorsListForm.RectWrap.y1=240;
	
	
	System.Gui.Form.AddFillArea(&App.Menu.SensorsListForm,&SensorsArea);
	SensorsArea.ID=PromptAreaID10;
	SensorsArea.PaintColor=TopLabelColor;
	SensorsArea.RectWrap.x0=0;
	SensorsArea.RectWrap.y0=30;
	SensorsArea.RectWrap.x1=320;
	SensorsArea.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.SensorsListForm,&SensorsListName);
	SensorsListName.ID=LabelID1+20;
	SensorsListName.Font=ListNameFont;
	SensorsListName.PaintColor=GUI_BLACK;
	SensorsListName.TextMode=GUI_TEXTMODE_TRANS;
	SensorsListName.Count=0;
//  SensorsListName.StringBlockPointer=(string *)MenuListName;
	SensorsListName.StringBlockPointer=(const string*)&SensorsLableTab[0];
	SensorsListName.X=130;
	SensorsListName.Y=43;
	
	
	//列表
	System.Gui.Form.AddListBox(&App.Menu.SensorsListForm,&SensorsListBox);
	SensorsListBox.ID=ListBoxID1+12;
//	DefenseListBox.RimColor=GUI_RED;
	SensorsListBox.x0=0;
	SensorsListBox.y0=80;
	SensorsListBox.x1=320;
	SensorsListBox.y1=240;
	SensorsListBox.Type=IconList;
	SensorsListBox.selectedItem=0;
  SensorsListBox.ItemsHeight=40;
	SensorsListBox.ItemsPerpage=4;//页最大条目
	SensorsListBox.SelectedIndexes=0;//默认为0，也就是第一条
	SensorsListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	SensorsListBox.TopItem=0;
	
	SensorsListBox.TextMode=GUI_TEXTMODE_TRANS;
	SensorsListBox.BackColor=GUI_WHITE;
	SensorsListBox.PaintColor=GUI_BLACK;
	
	SensorsListBox.SelectedBackColor=MoveBarColor;
	SensorsListBox.SelectedPaintColor=GUI_WHITE;
	
	SensorsListBox.Font=ListFont;
	
	System.Gui.Form.AddLists(&SensorsListBox,items,4,(u32*)FlashAdd[5],null,null,(u8**)gImage_sensorsIco,4);	
}
/*********************************************************************************************
*
*描述		：遥控器列表项（8个遥控）
*
*********************************************************************************************/
void RemoteList(void)
{
	static FillArea 	RemoteFill;
	static Label 		RemoteName;
	static ListBox		RemoteListBox;	
	string *items;
	
	items=(string *)RemteName;
	System.Gui.Form.Init(&App.Menu.RemoteForm);
	App.Menu.RemoteForm.FillAreaCount=1;
	App.Menu.RemoteForm.LabelCount=1;
	App.Menu.RemoteForm.ListCount=1;
	App.Menu.RemoteForm.FormID=Form14;
	App.Menu.RemoteForm.Color=GUI_WHITE;
	App.Menu.RemoteForm.RectWrap.x0=0;
	App.Menu.RemoteForm.RectWrap.y0=30;
	App.Menu.RemoteForm.RectWrap.x1=320;
	App.Menu.RemoteForm.RectWrap.y1=210;

	System.Gui.Form.AddFillArea(&App.Menu.RemoteForm,&RemoteFill);
	RemoteFill.ID=PromptAreaID5;
	RemoteFill.PaintColor=TopLabelColor;
	RemoteFill.RectWrap.x0=0;
	RemoteFill.RectWrap.y0=30;
	RemoteFill.RectWrap.x1=320;
	RemoteFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.RemoteForm,&RemoteName);
	RemoteName.ID=LabelID1;
	RemoteName.Font=ListFont;
	RemoteName.PaintColor=GUI_BLACK;
	RemoteName.TextMode=GUI_TEXTMODE_TRANS;
  RemoteName.Font = ListNameFont;
	RemoteName.Count=0;
	RemoteName.StringBlockPointer=RemoteTab[2];
	RemoteName.X=115;
	RemoteName.Y=43;	

	System.Gui.Form.AddListBox(&App.Menu.RemoteForm,&RemoteListBox);
	RemoteListBox.x0=0;
	RemoteListBox.y0=80;
	RemoteListBox.x1=320;
	RemoteListBox.y1=210;
	RemoteListBox.xSize=15;
	RemoteListBox.ySize=15;
	
	RemoteListBox.Select=2;
	RemoteListBox.ItemsHeight=40;
	RemoteListBox.selectedItem=0;
	RemoteListBox.ItemsPerpage=4;//页最大条目
	RemoteListBox.SelectedIndexes=0;//默认为0，也就是第一条
	RemoteListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	RemoteListBox.TotalItems=0;
	
	RemoteListBox.TextMode=GUI_TEXTMODE_TRANS;
	RemoteListBox.BackColor=GUI_WHITE;
	RemoteListBox.PaintColor=GUI_BLACK;
	
	RemoteListBox.SelectedBackColor=MoveBarColor;
	RemoteListBox.SelectedPaintColor=GUI_WHITE;
	
	RemoteListBox.Font=ListFont;
	RemoteListBox.ID=0x01;
	RemoteListBox.Type=TextList;

	System.Gui.Form.AddLists(&RemoteListBox,items,8,(u32*)&RemoteNameAddr,(u32*)(RemoteIDTab),null,null,8);
}
/*********************************************************************************************
*
*描述		：防区列表选择项(30个防区)
*
*********************************************************************************************/
static void ZoneSetList(void)
{
	static FillArea		ZoneFill;
	static Label		ZoneName;
	static ListBox		ZoneListBox;
	
	string *items;
	items=NULL;
	
	System.Gui.Form.Init(&App.Menu.ZoneSetForm);
	App.Menu.ZoneSetForm.FillAreaCount=1;
	App.Menu.ZoneSetForm.LabelCount=1;
	App.Menu.ZoneSetForm.ListCount=1;
	App.Menu.ZoneSetForm.FormID=Form14;
	App.Menu.ZoneSetForm.Color=GUI_WHITE;
	App.Menu.ZoneSetForm.RectWrap.x0=0;
	App.Menu.ZoneSetForm.RectWrap.y0=30;
	App.Menu.ZoneSetForm.RectWrap.x1=320;
	App.Menu.ZoneSetForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.ZoneSetForm,&ZoneFill);
	ZoneFill.ID=PromptAreaID7;
	ZoneFill.PaintColor=TopLabelColor;
	ZoneFill.RectWrap.x0=0;
	ZoneFill.RectWrap.y0=30;
	ZoneFill.RectWrap.x1=320;
	ZoneFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.ZoneSetForm,&ZoneName);
	ZoneName.ID=LabelID1+20;
	ZoneName.Font=ListFont;
	ZoneName.PaintColor=GUI_BLACK;
	ZoneName.TextMode=GUI_TEXTMODE_TRANS;
  ZoneName.Font = ListNameFont;
	ZoneName.Count=0;
	ZoneName.StringBlockPointer=ZoneTab;
	ZoneName.X=115;
	ZoneName.Y=43;	

	System.Gui.Form.AddListBox(&App.Menu.ZoneSetForm,&ZoneListBox);
	ZoneListBox.x0=0;
	ZoneListBox.y0=80;
	ZoneListBox.x1=320;
	ZoneListBox.y1=240;
	ZoneListBox.xSize=15;
	ZoneListBox.ySize=15;
	
	ZoneListBox.Select=2;
	ZoneListBox.ItemsHeight=40;
	ZoneListBox.selectedItem=0;
	ZoneListBox.ItemsPerpage=4;//页最大条目
	ZoneListBox.SelectedIndexes=0;//默认为0，也就是第一条
	ZoneListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	ZoneListBox.TotalItems=0;
	
	ZoneListBox.TextMode=GUI_TEXTMODE_TRANS;
	ZoneListBox.BackColor=GUI_WHITE;
	ZoneListBox.PaintColor=GUI_BLACK;
	
	ZoneListBox.SelectedBackColor=MoveBarColor;
	ZoneListBox.SelectedPaintColor=GUI_WHITE;
	
	ZoneListBox.Font=ListFont;
	ZoneListBox.ID=0x56;
	ZoneListBox.Type=TextList;
	System.Gui.Form.AddLists(&ZoneListBox,items,30,(u32*)&ZoneNameAddr,null,null,null,30);	
}
/*********************************************************************************************
*
*描述		：无线警号设置
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void WirelessSirenList(void)
{
	string *items;
	items=(string *)WirelessSirenTab;
	
	System.Gui.Form.Init(&App.Menu.WirelessSirenForm);
	App.Menu.WirelessSirenForm.FillAreaCount=1;
	App.Menu.WirelessSirenForm.LabelCount=1;
	App.Menu.WirelessSirenForm.ListCount=1;
	App.Menu.WirelessSirenForm.FormID=Form15;
	App.Menu.WirelessSirenForm.Color=GUI_WHITE;
	App.Menu.WirelessSirenForm.RectWrap.x0=0;
	App.Menu.WirelessSirenForm.RectWrap.y0=30;
	App.Menu.WirelessSirenForm.RectWrap.x1=320;
	App.Menu.WirelessSirenForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.WirelessSirenForm,&WirelessSirenFill);
	WirelessSirenFill.ID=PromptAreaID8;
	WirelessSirenFill.PaintColor=TopLabelColor;
	WirelessSirenFill.RectWrap.x0=0;
	WirelessSirenFill.RectWrap.y0=30;
	WirelessSirenFill.RectWrap.x1=320;
	WirelessSirenFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.WirelessSirenForm,&WirelessSirenName);
	WirelessSirenName.ID=LabelID1+21;
	WirelessSirenName.Font=ListNameFont;
	WirelessSirenName.PaintColor=GUI_BLACK;
	WirelessSirenName.TextMode=GUI_TEXTMODE_TRANS;
	WirelessSirenName.Count=0;
	WirelessSirenName.StringBlockPointer=WirelessSirenTab;
	WirelessSirenName.X=75;
	WirelessSirenName.Y=43;	
	
	System.Gui.Form.AddListBox(&App.Menu.WirelessSirenForm,&WirelessSirenBox);
	WirelessSirenBox.x0=0;
	WirelessSirenBox.y0=80;
	WirelessSirenBox.x1=320;
	WirelessSirenBox.y1=240;
	WirelessSirenBox.xSize=15;
	WirelessSirenBox.ySize=15;
	
	WirelessSirenBox.selectedItem=0;
    WirelessSirenBox.ItemsHeight=40;
	WirelessSirenBox.ItemsPerpage=1;//页最大条目
	WirelessSirenBox.SelectedIndexes=0;//默认为0，也就是第一条
	WirelessSirenBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	WirelessSirenBox.TotalItems=0;
	
	WirelessSirenBox.TextMode=GUI_TEXTMODE_TRANS;
	WirelessSirenBox.BackColor=GUI_WHITE;
	WirelessSirenBox.PaintColor=GUI_BLACK;
	
	WirelessSirenBox.SelectedBackColor=MoveBarColor;
	WirelessSirenBox.SelectedPaintColor=GUI_WHITE;
	
	WirelessSirenBox.Font=ListFont;
	WirelessSirenBox.ID=0x56;
	WirelessSirenBox.Type=SwitchList;

	System.Gui.Form.AddLists(&WirelessSirenBox,items,1,(u32*)&ReadyCheckState[1],null,null,null,1);	
}
/*********************************************************************************************
*
*描述		：非法布防开关
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void ReadyArmList(void)
{
	string *items;
	static FillArea		ReadyArmFill;
	static Label		ReadyArmName;
	static ListBox		ReadyArmListBox;
	items=(string *)ReadyArmTab;
	
	System.Gui.Form.Init(&App.Menu.ReadyArmForm);
	App.Menu.ReadyArmForm.FillAreaCount=1;
	App.Menu.ReadyArmForm.LabelCount=1;
	App.Menu.ReadyArmForm.ListCount=1;
	App.Menu.ReadyArmForm.FormID=Form12;
	App.Menu.ReadyArmForm.Color=GUI_WHITE;
	App.Menu.ReadyArmForm.RectWrap.x0=0;
	App.Menu.ReadyArmForm.RectWrap.y0=30;
	App.Menu.ReadyArmForm.RectWrap.x1=320;
	App.Menu.ReadyArmForm.RectWrap.y1=210;

	System.Gui.Form.AddFillArea(&App.Menu.ReadyArmForm,&ReadyArmFill);
	ReadyArmFill.ID=PromptAreaID9;
	ReadyArmFill.PaintColor=TopLabelColor;
	ReadyArmFill.RectWrap.x0=0;
	ReadyArmFill.RectWrap.y0=30;
	ReadyArmFill.RectWrap.x1=320;
	ReadyArmFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.ReadyArmForm,&ReadyArmName);
	ReadyArmName.ID=LabelID1+22;
	ReadyArmName.Font=ListNameFont;
	ReadyArmName.PaintColor=GUI_BROWN;
	ReadyArmName.TextMode=GUI_TEXTMODE_TRANS;
	ReadyArmName.Count=0;
	ReadyArmName.StringBlockPointer=ReadyArmTab;
	ReadyArmName.X=75;
	ReadyArmName.Y=45;	

	System.Gui.Form.AddListBox(&App.Menu.ReadyArmForm,&ReadyArmListBox);
	ReadyArmListBox.x0=0;
	ReadyArmListBox.y0=80;
	ReadyArmListBox.x1=320;
	ReadyArmListBox.y1=240;
	ReadyArmListBox.xSize=15;
	ReadyArmListBox.ySize=15;
	
	ReadyArmListBox.selectedItem=0;
	ReadyArmListBox.ItemsPerpage=1;//页最大条目
  ReadyArmListBox.ItemsHeight=37;
	ReadyArmListBox.SelectedIndexes=0;//默认为0，也就是第一条
	ReadyArmListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	ReadyArmListBox.TotalItems=0;
	
	ReadyArmListBox.TextMode=GUI_TEXTMODE_TRANS;
	ReadyArmListBox.BackColor=GUI_WHITE;
	ReadyArmListBox.PaintColor=GUI_DARKGRAY;
	
	ReadyArmListBox.SelectedBackColor=MoveBarColor;
	ReadyArmListBox.SelectedPaintColor=GUI_WHITE;
	
	ReadyArmListBox.Font=ListFont;
	ReadyArmListBox.ID=0x56;
	ReadyArmListBox.Type=SwitchList;

	System.Gui.Form.AddLists(&ReadyArmListBox,items,1,(u32*)ReadyCheckState,null,null,null,1);
}


/*********************************************************************************************
*
*描述		：对码，设置，删除 选择页面
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void RemoteConfig(void)
{
	static FillArea		RemoteTopFill;
	static FillArea		RemoteBottomFill;
	static Label		RemoteName;
	static ListBox		RemoteListBox;	
	string *items=(string*)&RemteName[0];
	
	System.Gui.Form.Init(&App.Menu.RemoteConfigForm);	
	App.Menu.RemoteConfigForm.FillAreaCount=1;
	App.Menu.RemoteConfigForm.LabelCount=1;
	App.Menu.RemoteConfigForm.ListCount=1;
	App.Menu.RemoteConfigForm.FormID=Form9;
	App.Menu.RemoteConfigForm.Color=GUI_WHITE;
	App.Menu.RemoteConfigForm.RectWrap.x0=0;
	App.Menu.RemoteConfigForm.RectWrap.y0=30;
	App.Menu.RemoteConfigForm.RectWrap.x1=320;
	App.Menu.RemoteConfigForm.RectWrap.y1=210;	

	System.Gui.Form.AddFillArea(&App.Menu.RemoteConfigForm,&RemoteTopFill);
	RemoteTopFill.ID=PromptAreaID1;
	RemoteTopFill.PaintColor=TopLabelColor;
	RemoteTopFill.RectWrap.x0=0;
	RemoteTopFill.RectWrap.y0=30;
	RemoteTopFill.RectWrap.x1=320;
	RemoteTopFill.RectWrap.y1=80;	
	
//	System.Gui.Form.AddFillArea(&App.Menu.RemoteConfigForm,&RemoteBottomFill);
//	RemoteBottomFill.ID=PromptAreaID2;
//	RemoteBottomFill.PaintColor=GUI_ORANGE;
//	RemoteBottomFill.RectWrap.x0=0;
//	RemoteBottomFill.RectWrap.y0=210;
//	RemoteBottomFill.RectWrap.x1=320;
//	RemoteBottomFill.RectWrap.y1=240;

	System.Gui.Form.AddLabel(&App.Menu.RemoteConfigForm,&RemoteName);
	RemoteName.ID=LabelID1+2;
	RemoteName.Font=ListFont;
	RemoteName.PaintColor=GUI_BLACK;
	RemoteName.TextMode=GUI_TEXTMODE_TRANS;
  RemoteName.Font = ListNameFont;
	RemoteName.Count=0;
	RemoteName.Type=0xFE;
	RemoteName.StringBlockPointer=NULL;
	RemoteName.X=100;
	RemoteName.Y=43;	
	
	System.Gui.Form.AddListBox(&App.Menu.RemoteConfigForm,&RemoteListBox);
	RemoteListBox.x0=0;
	RemoteListBox.y0=80;
	RemoteListBox.x1=320;
	RemoteListBox.y1=240;
	RemoteListBox.xSize=15;
	RemoteListBox.ySize=15;
	
	RemoteListBox.ItemsHeight=40;
	RemoteListBox.selectedItem=0;
	RemoteListBox.ItemsPerpage=3;//页最大条目
	RemoteListBox.SelectedIndexes=0;//默认为0，也就是第一条
	RemoteListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	RemoteListBox.TotalItems=0;
	
	RemoteListBox.TextMode=GUI_TEXTMODE_TRANS;
	RemoteListBox.BackColor=GUI_WHITE;
	RemoteListBox.PaintColor=GUI_BLACK;
	
	RemoteListBox.SelectedBackColor=MoveBarColor;
	RemoteListBox.SelectedPaintColor=GUI_WHITE;
	
	RemoteListBox.Font=ListFont;
	RemoteListBox.ID=0x01;
	RemoteListBox.Type=TextList;

	System.Gui.Form.AddLists(&RemoteListBox,items,3,(u32*)NULL,null,null,null,3);
}
/*********************************************************************************************
*
*描述		：对码，设置，删除 选择页面
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void ZoneConfigInit(void)
{
	static FillArea		zoneTopFill;
//	static FillArea		zoneBottomFill;
	static Label		zoneName;
	static ListBox		zoneListBox;	
	string *items=(string*)&RemteName[14];
	
	System.Gui.Form.Init(&App.Menu.ZoneConfigListForm);	
	App.Menu.ZoneConfigListForm.FillAreaCount=1;
	App.Menu.ZoneConfigListForm.LabelCount=1;
	App.Menu.ZoneConfigListForm.ListCount=1;
	App.Menu.ZoneConfigListForm.FormID=Form9;
	App.Menu.ZoneConfigListForm.Color=GUI_WHITE;
	App.Menu.ZoneConfigListForm.RectWrap.x0=0;
	App.Menu.ZoneConfigListForm.RectWrap.y0=30;
	App.Menu.ZoneConfigListForm.RectWrap.x1=320;
	App.Menu.ZoneConfigListForm.RectWrap.y1=240;	

	System.Gui.Form.AddFillArea(&App.Menu.ZoneConfigListForm,&zoneTopFill);
	zoneTopFill.ID=PromptAreaID9;
	zoneTopFill.PaintColor=TopLabelColor;
	zoneTopFill.RectWrap.x0=0;
	zoneTopFill.RectWrap.y0=30;
	zoneTopFill.RectWrap.x1=320;
	zoneTopFill.RectWrap.y1=80;	
	
// 	System.Gui.Form.AddFillArea(&App.Menu.ZoneConfigListForm,&zoneBottomFill);
// 	zoneBottomFill.ID=PromptAreaID2;
// 	zoneBottomFill.PaintColor=GUI_ORANGE;
// 	zoneBottomFill.RectWrap.x0=0;
// 	zoneBottomFill.RectWrap.y0=210;
// 	zoneBottomFill.RectWrap.x1=320;
// 	zoneBottomFill.RectWrap.y1=240;

	System.Gui.Form.AddLabel(&App.Menu.ZoneConfigListForm,&zoneName);
	zoneName.ID=LabelID1+2;
	zoneName.Font=ListNameFont;
	zoneName.PaintColor=GUI_BLACK;
	zoneName.TextMode=GUI_TEXTMODE_TRANS;
	zoneName.Count=0;
	zoneName.Type=0xFE;
	zoneName.StringBlockPointer=NULL;
	zoneName.X=100;
	zoneName.Y=45;	
	
	System.Gui.Form.AddListBox(&App.Menu.ZoneConfigListForm,&zoneListBox);
	zoneListBox.x0=0;
	zoneListBox.y0=80;
	zoneListBox.x1=320;
	zoneListBox.y1=240;
	zoneListBox.xSize=15;
	zoneListBox.ySize=15;
	
	zoneListBox.ItemsHeight=40;
	zoneListBox.selectedItem=0;
	zoneListBox.ItemsPerpage=4;//页最大条目
	zoneListBox.SelectedIndexes=0;//默认为0，也就是第一条
	zoneListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	zoneListBox.TotalItems=0;
	
	zoneListBox.TextMode=GUI_TEXTMODE_TRANS;
	zoneListBox.BackColor=GUI_WHITE;
	zoneListBox.PaintColor=GUI_BLACK;
	
	zoneListBox.SelectedBackColor=MoveBarColor;
	zoneListBox.SelectedPaintColor=GUI_WHITE;
	
	zoneListBox.Font=ListFont;
	zoneListBox.ID=0x01;
	zoneListBox.Type=TextList;

	System.Gui.Form.AddLists(&zoneListBox,items,4,(u32*)NULL,null,null,null,4);
}
void ZoneWirelessClose(void)
{
	App.Data.UseWireless=0;
	App.Data.CodeWireless=0;	
}
/*********************************************************************************************
*
*描述		：防区属性设置
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void ZoneAttributeInit(void)
{
	static FillArea		AttributeTopFill;
//	static FillArea		AttributeBottomFill;
	static Label		AttributeName;
	static ListBox		AttributeListBox;	
	string *items=(string*)&ZoneAttributeString[1];
	
	System.Gui.Form.Init(&App.Menu.ZoneAttributeForm);	
	App.Menu.ZoneAttributeForm.FillAreaCount=1;
	App.Menu.ZoneAttributeForm.LabelCount=1;
	App.Menu.ZoneAttributeForm.ListCount=1;
	App.Menu.ZoneAttributeForm.FormID=Form9;
	App.Menu.ZoneAttributeForm.Color=GUI_WHITE;
	App.Menu.ZoneAttributeForm.RectWrap.x0=0;
	App.Menu.ZoneAttributeForm.RectWrap.y0=30;
	App.Menu.ZoneAttributeForm.RectWrap.x1=320;
	App.Menu.ZoneAttributeForm.RectWrap.y1=210;	

	System.Gui.Form.AddFillArea(&App.Menu.ZoneAttributeForm,&AttributeTopFill);
	AttributeTopFill.ID=PromptAreaID5;
	AttributeTopFill.PaintColor=TopLabelColor;
	AttributeTopFill.RectWrap.x0=0;
	AttributeTopFill.RectWrap.y0=30;
	AttributeTopFill.RectWrap.x1=320;
	AttributeTopFill.RectWrap.y1=60;	
	
// 	System.Gui.Form.AddFillArea(&App.Menu.ZoneAttributeForm,&AttributeBottomFill);
// 	AttributeBottomFill.ID=PromptAreaID3;
// 	AttributeBottomFill.PaintColor=GUI_ORANGE;
// 	AttributeBottomFill.RectWrap.x0=0;
// 	AttributeBottomFill.RectWrap.y0=210;
// 	AttributeBottomFill.RectWrap.x1=320;
// 	AttributeBottomFill.RectWrap.y1=240;

	System.Gui.Form.AddLabel(&App.Menu.ZoneAttributeForm,&AttributeName);
	AttributeName.ID=LabelID1+3;
	AttributeName.Font=ListFont;
	AttributeName.PaintColor=GUI_BROWN;
	AttributeName.TextMode=GUI_TEXTMODE_TRANS;
	AttributeName.Count=0;
	AttributeName.Type=0;
	AttributeName.StringBlockPointer=ZoneAttributeString;
	AttributeName.X=100;
	AttributeName.Y=35;	
	
	System.Gui.Form.AddListBox(&App.Menu.ZoneAttributeForm,&AttributeListBox);
	AttributeListBox.x0=0;
	AttributeListBox.y0=60;
	AttributeListBox.x1=320;
	AttributeListBox.y1=240;
	AttributeListBox.xSize=15;
	AttributeListBox.ySize=15;
	AttributeListBox.Select=1;
	
	AttributeListBox.selectedItem=0;
	AttributeListBox.ItemsPerpage=6;//页最大条目
	AttributeListBox.SelectedIndexes=0;//默认为0，也就是第一条
	AttributeListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	AttributeListBox.TotalItems=0;
	
	AttributeListBox.TextMode=GUI_TEXTMODE_TRANS;
	AttributeListBox.BackColor=GUI_WHITE;
	AttributeListBox.PaintColor=GUI_DARKGRAY;
	
	AttributeListBox.SelectedBackColor=MoveBarColor;
	AttributeListBox.SelectedPaintColor=GUI_WHITE;
	
	AttributeListBox.Font=GUI_FONT_8X18;
	AttributeListBox.ID=0x56;
	AttributeListBox.Type=CheckList;

	System.Gui.Form.AddLists(&AttributeListBox,items,6,(u32*)CheckBoxState,null,null,null,6);
}
/*********************************************************************************************
*
*描述		：遥控对码页面
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void RemoteCodeFormInit(void)
{
	static Label 		name;
	static Label 		total;
	static Label 		haveAdded;
	static Label		prompt;
	static Label		pcs1;
	static Label		pcs2;
	static Label		remoteTotal;
	static Label		haveAddCount;
	static FillArea		nameFill;
	static FillArea		bottomFill;
	static FillArea		line;
	static char			buffer[36];
	
	System.Gui.Form.Init(&App.Menu.RemoteCodeForm);	
	App.Menu.RemoteCodeForm.LabelCount=8;
	App.Menu.RemoteCodeForm.FillAreaCount=3;
	App.Menu.RemoteCodeForm.FormID=Form6+20;
	App.Menu.RemoteCodeForm.Color=GUI_WHITE;
	App.Menu.RemoteCodeForm.RectWrap.x0=0;
	App.Menu.RemoteCodeForm.RectWrap.y0=30;
	App.Menu.RemoteCodeForm.RectWrap.x1=320;
	App.Menu.RemoteCodeForm.RectWrap.y1=240;	

	
	System.Gui.Form.AddFillArea(&App.Menu.RemoteCodeForm,&nameFill);
	nameFill.ID=PromptAreaID2;
	nameFill.PaintColor=GUI_ORANGE;
	nameFill.RectWrap.x0=0;
	nameFill.RectWrap.y0=30;
	nameFill.RectWrap.x1=320;
	nameFill.RectWrap.y1=60;
	
	System.Gui.Form.AddFillArea(&App.Menu.RemoteCodeForm,&bottomFill);
	bottomFill.ID=PromptAreaID3;
	bottomFill.PaintColor=BLUE;
	bottomFill.RectWrap.x0=0;
	bottomFill.RectWrap.y0=210;
	bottomFill.RectWrap.x1=320;
	bottomFill.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.RemoteCodeForm,&line);
	line.ID=PromptAreaID2;
	line.PaintColor=GUI_ORANGE;
	line.RectWrap.x0=5;
	line.RectWrap.y0=159;
	line.RectWrap.x1=315;
	line.RectWrap.y1=160;

	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&name);
	name.ID=LabelID1+2;
	name.Font=ListFont;
	name.PaintColor=GUI_BROWN;
	name.TextMode=GUI_TEXTMODE_TRANS;
	name.Count=3;
//	name.Type=0;
  name.Type=0xC0;
	name.StringBlockPointer=RemteName;
	name.X=80;
	name.Y=35;	

	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&remoteTotal);
	remoteTotal.ID=LabelID1+6;
	remoteTotal.Font=ListFont;
	remoteTotal.PaintColor=GUI_BROWN;
	remoteTotal.TextMode=GUI_TEXTMODE_TRANS;
	remoteTotal.Count=3;
	remoteTotal.Type=0xF9;
	remoteTotal.Addr=RemoteTotalAddr;
	remoteTotal.X=180;
	remoteTotal.Y=85;	
	
	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&haveAddCount);
	haveAddCount.ID=LabelID1+9;
	haveAddCount.Font=ListFont;
	haveAddCount.PaintColor=GUI_BROWN;
	haveAddCount.TextMode=GUI_TEXTMODE_TRANS;
	haveAddCount.Count=0;
	haveAddCount.Type=0xFA;
	haveAddCount.Addr=0;
	haveAddCount.X=180;
	haveAddCount.Y=125;

	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&total);
	total.ID=LabelID1+16;
	total.Font=ListFont;
	total.PaintColor=GUI_BROWN;
	total.TextMode=GUI_TEXTMODE_TRANS;
	total.Count=4;
//	total.Type=0;
  total.Type=0xC0;
	total.StringBlockPointer=RemteName;
	total.X=60;
	total.Y=85;

	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&haveAdded);
	haveAdded.ID=LabelID1+17;
	haveAdded.Font=ListFont;
	haveAdded.PaintColor=GUI_BROWN;
	haveAdded.TextMode=GUI_TEXTMODE_TRANS;
	haveAdded.Count=5;
//	haveAdded.Type=0;
  haveAdded.Type=0xC0;
	haveAdded.StringBlockPointer=RemteName;
	haveAdded.X=60;
	haveAdded.Y=125;
	
	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&prompt);
	prompt.ID=LabelID1+7;
	prompt.Font=ListFont;
	prompt.PaintColor=GUI_BROWN;
	prompt.TextMode=GUI_TEXTMODE_TRANS;
	prompt.Count=0;
	prompt.Type=0xF8;
	prompt.StringBlockPointer=(const string*)buffer;
	prompt.X=30;
	prompt.Y=165;
	
	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&pcs1);
	pcs1.ID=LabelID1+5;
	pcs1.Font=ListFont;
	pcs1.PaintColor=GUI_BROWN;
	pcs1.TextMode=GUI_TEXTMODE_TRANS;
	pcs1.Count=7;
//	pcs1.Type=0;
  pcs1.Type=0xC0;
	pcs1.StringBlockPointer=RemteName;
	pcs1.X=220;
	pcs1.Y=125;
	
	System.Gui.Form.AddLabel(&App.Menu.RemoteCodeForm,&pcs2);
	pcs2.ID=LabelID1+1;
	pcs2.Font=ListFont;
	pcs2.PaintColor=GUI_BROWN;
	pcs2.TextMode=GUI_TEXTMODE_TRANS;
	pcs2.Count=7;
//	pcs2.Type=0;
  pcs2.Type=0xC0;
	pcs2.StringBlockPointer=RemteName;
	pcs2.X=220;
	pcs2.Y=85;
}
/*******************************************************************************************************************

*描述		：获取无线探头的总个数和该防区的总个数 的存储地址
*输入参数	：无
*输出参数	：无

*******************************************************************************************************************/
void CodeWirelessOpen(void)
{
	MenuListStruct 	*currentMenu;
	Form 			*formPointer;
	ListboxList		*listx;
	Label			*focusLabel;
	static char 	buffer[30];
	
	if(&App.Menu.RemoteCodeForm==App.Menu.FocusFormPointer)
	{
		currentMenu=CurrentMenu->Parent;
		currentMenu=currentMenu->Parent;		
		formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
		listx=System.Gui.Form.ListSearch(formPointer->ListPointer->List,formPointer->ListPointer->SelectedIndexes);
		focusLabel=App.Menu.FocusFormPointer->LabelPointer;
		while(null!=focusLabel)
		{
			if(focusLabel->Type==0xFA)
				focusLabel->Addr=*(u32*)listx->IconAdd+16;
			if(focusLabel->Type==0xF8)
			{
				if((*(u32*)listx->IconAdd<=Zone30NameAddr)&&(*(u32*)listx->IconAdd>=Zone1NameAddr))
				{
					sprintf((char*)focusLabel->StringBlockPointer,"Please trigger Detector");
					App.Data.RxDis=RxDetector;
				}
				else if((*(u32*)listx->IconAdd<=Remote8NameAddr)&&(*(u32*)listx->IconAdd>=Remote1NameAddr))
				{
					sprintf(buffer,"Please Press Any Key on Remote");
					memcpy((char*)focusLabel->StringBlockPointer,buffer,strlen(buffer));
					App.Data.RxDis=RxRemote;
				}
			}
			if((*(u32*)listx->IconAdd<=Zone30NameAddr)&&(*(u32*)listx->IconAdd>=Zone1NameAddr))
			{
				if(focusLabel->Type==0xF9)
					focusLabel->Addr=DetectorTotalAddr;
			}
			else if((*(u32*)listx->IconAdd<=Remote8NameAddr)&&(*(u32*)listx->IconAdd>=Remote1NameAddr))
				if(focusLabel->Type==0xF9)
					focusLabel->Addr=RemoteTotalAddr;
			focusLabel=focusLabel->NextLabelPointer;				
		}	
		
	}	
	TIM_Cmd(TIM7, ENABLE);
	App.Data.UseWireless=1;
	App.Data.CodeWireless=1;
}
/*********************************************************************************************
*
*描述		：遥控伴音开关
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void RemoteSoundInit(void)
{
	string *items;
	static FillArea		nameFill;
	static Label		name;
	static ListBox		soundListBox;
	items=(string *)RemteName;
	
	System.Gui.Form.Init(&App.Menu.RemoteSoundForm);
	App.Menu.RemoteSoundForm.FillAreaCount=1;
	App.Menu.RemoteSoundForm.LabelCount=1;
	App.Menu.RemoteSoundForm.ListCount=1;
	App.Menu.RemoteSoundForm.FormID=Form1;
	App.Menu.RemoteSoundForm.Color=GUI_WHITE;
	App.Menu.RemoteSoundForm.RectWrap.x0=0;
	App.Menu.RemoteSoundForm.RectWrap.y0=30;
	App.Menu.RemoteSoundForm.RectWrap.x1=320;
	App.Menu.RemoteSoundForm.RectWrap.y1=210;

	System.Gui.Form.AddFillArea(&App.Menu.RemoteSoundForm,&nameFill);
	nameFill.ID=PromptAreaID9;
	nameFill.PaintColor=TopLabelColor;
	nameFill.RectWrap.x0=0;
	nameFill.RectWrap.y0=30;
	nameFill.RectWrap.x1=320;
	nameFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.RemoteSoundForm,&name);
	name.ID=LabelID1+22;
	name.Font=ListFont;
	name.PaintColor=GUI_BLACK;
	name.TextMode=GUI_TEXTMODE_TRANS;
  name.Font = ListNameFont;
	name.Count=8;
	name.StringBlockPointer=RemteName;
	name.X=75;
	name.Y=43;	

	System.Gui.Form.AddListBox(&App.Menu.RemoteSoundForm,&soundListBox);
	soundListBox.x0=0;
	soundListBox.y0=80;
	soundListBox.x1=320;
	soundListBox.y1=210;
	soundListBox.xSize=15;
	soundListBox.ySize=15;
	
	soundListBox.selectedItem=0;
	soundListBox.ItemsPerpage=1;//页最大条目
    soundListBox.ItemsHeight=40;//页最大条目
	soundListBox.SelectedIndexes=0;//默认为0，也就是第一条
	soundListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	soundListBox.TotalItems=0;
	
	soundListBox.TextMode=GUI_TEXTMODE_TRANS;
	soundListBox.BackColor=GUI_WHITE;
	soundListBox.PaintColor=GUI_DARKGRAY;
	
	soundListBox.SelectedBackColor=MoveBarColor;
	soundListBox.SelectedPaintColor=GUI_WHITE;
	
	soundListBox.Font=ListFont;
	soundListBox.ID=0x56;
	soundListBox.Type=SwitchList;

	System.Gui.Form.AddLists(&soundListBox,&items[9],1,(u32*)remoteSetupAddr,null,null,null,1);	
}
/*********************************************************************************************
*
*描述		：遥控伴音复选择框地址获取
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
void RemoteTypeAddr(Form *formPointer)
{
	MenuListStruct 	*currentMenu;
	Form 			*focusFormPointer;
	ListboxList		*listx,*modifyList;	
	
	currentMenu=CurrentMenu->Parent;
	currentMenu=currentMenu->Parent;
	focusFormPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
	listx=System.Gui.Form.ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);
	modifyList=System.Gui.Form.ListSearch(formPointer->ListPointer->List,0);
	*(u32*)modifyList->IconAdd=*(u32*)listx->IconAdd+12;

}
void SirenStateSave(void)
{
	MenuListStruct 	*currentMenu;
	Form 			*focusFormPointer;
	ListboxList		*listx,*modifyList;
	u8				state;
	
	currentMenu=CurrentMenu->Parent;
	currentMenu=currentMenu->Parent;
	focusFormPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
	listx=System.Gui.Form.ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);

	modifyList=System.Gui.Form.ListSearch(App.Menu.FocusFormPointer->ListPointer->List,0);
	if(modifyList->State)
		state=SystemSirenOn;
	else if(modifyList->State==0)
		state=SystemSirenOff;
	System.Device.Storage.Parameter.StorageWriteData(*(u32*)listx->IconAdd+12,(u16*)&state,1);	
}
/******************************************************************************************************************************
*
*描述		：	防区属性数据转换处理
*输入参数	：	formPointer，当前的页面
*输出参数	：	无
*
******************************************************************************************************************************/
void ZoneAttributeProcess(Form *formPointer)
{
	MenuListStruct 	*currentMenu;
	Form 			*focusFormPointer;
	ListboxList		*listx,*modifyList;
	u8				state;
	
	currentMenu=CurrentMenu->Parent;
	currentMenu=currentMenu->Parent;
	focusFormPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
	listx=System.Gui.Form.ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);
	System.Device.Storage.Parameter.StorageReadData(*(u32*)listx->IconAdd+14,&state,1);
	switch(state)
	{
		case SystemZoneClose:                                                                                                                                                
			state=5;
			break;
		case SystemZoneAlarm:
			state=0;
			break;
		case SystemZoneHomeAlarm:
			state=1;
			break;
		case SystemZoneDelayAlarm:
			state=4;
			break;
		case SystemZoneSOS:
			state=3;
			break;
		case SystemZoneDoorbell:
			state=2;
			break;			
	}
	modifyList=System.Gui.Form.ListSearch(formPointer->ListPointer->List,0);
	while(null!=modifyList)
	{
		System.Device.Storage.Parameter.StorageReadData(*(u32*)modifyList->IconAdd,&modifyList->State,1);
		if(modifyList->State)
		{
			modifyList->State=0;
			System.Device.Storage.Parameter.StorageWriteData(*(u32*)modifyList->IconAdd,(u16*)&modifyList->State,1);
		}
		modifyList=modifyList->Nextlist;
	}
	modifyList=System.Gui.Form.ListSearch(formPointer->ListPointer->List,state);
	modifyList->State=1;
	System.Device.Storage.Parameter.StorageWriteData(*(u32*)modifyList->IconAdd,(u16*)&modifyList->State,1);		
}
/******************************************************************************************************************************
*
*描述		：	防区属性设置保存
*输入参数	：	无
*输出参数	：	无
*
******************************************************************************************************************************/
void ZoneAttributeSave(void)
{
	MenuListStruct 	*currentMenu;
	Form 			*focusFormPointer;
	ListboxList		*listx,*modifyList;
	u8				state;
	u8				i=0;
	
	currentMenu=CurrentMenu->Parent;
	currentMenu=currentMenu->Parent;
	focusFormPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
	listx=System.Gui.Form.ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);

	modifyList=System.Gui.Form.ListSearch(App.Menu.FocusFormPointer->ListPointer->List,0);
	while(null!=modifyList)
	{
		if(modifyList->State) break;
		i++;
		modifyList=modifyList->Nextlist;		
	}
	switch(i)
	{
		case 5:
			state=SystemZoneClose;
			break;
		case 0:
			state=SystemZoneAlarm;
			break;
		case 1:
			state=SystemZoneHomeAlarm;
			break;
		case 4:
			state=SystemZoneDelayAlarm;
			break;
		case 3:
			state=SystemZoneSOS;
			break;
		case 2:
			state=SystemZoneDoorbell;
			break;			
	}
	System.Device.Storage.Parameter.StorageWriteData(*(u32*)listx->IconAdd+14,(u16*)&state,1);
}
/*********************************************************************************************
*
*描述		：遥控删除页面，删除当前号码，删除全部号码，或者返回
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void RemoteDelFomInit(void)
{
	//static FillArea	sureFill;
	//static Label	sureName;
	static ListBox	sureList;
	
	string *items;
	items=(string *)RemteName;
	
	System.Gui.Form.Init(&App.Menu.RemoteDelForm);
	App.Menu.RemoteDelForm.FillAreaCount=1;
	App.Menu.RemoteDelForm.LabelCount=1;
	App.Menu.RemoteDelForm.ListCount=1;
	App.Menu.RemoteDelForm.FormID=Form9+16;
	App.Menu.RemoteDelForm.Color=GUI_ORANGE;
	App.Menu.RemoteDelForm.RectWrap.x0=70;
	App.Menu.RemoteDelForm.RectWrap.y0=80;
	App.Menu.RemoteDelForm.RectWrap.x1=250;
	App.Menu.RemoteDelForm.RectWrap.y1=200;	

//	System.Gui.Form.AddFillArea(&App.Menu.RemoteDelForm,&sureFill);
//	sureFill.ID=PromptAreaID5;
//	sureFill.PaintColor=GREEN;
//	sureFill.RectWrap.x0=70;
//	sureFill.RectWrap.y0=75;
//	sureFill.RectWrap.x1=250;
//	sureFill.RectWrap.y1=110;	

//	System.Gui.Form.AddLabel(&App.Menu.RemoteDelForm,&sureName);
//	sureName.ID=LabelID1+10;
//	sureName.Font=GUI_FONT_20_1;
//	sureName.PaintColor=GUI_BLUE;
//	sureName.TextMode=GUI_TEXTMODE_TRANS;
//	sureName.Count=10;
//	sureName.StringBlockPointer=RemteName;
//	sureName.X=100;
//	sureName.Y=85;	
	
	System.Gui.Form.AddListBox(&App.Menu.RemoteDelForm,&sureList);
	sureList.x0=70;
	sureList.y0=110;
	sureList.x1=270;
	sureList.y1=200;
	sureList.xSize=10;//纯文本列表时这里是文字显示偏移量
	sureList.ySize=15;
	
	sureList.selectedItem=0;
	sureList.ItemsPerpage=3;//页最大条目
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
	
	System.Gui.Form.AddLists(&sureList,&items[11],3,(u32*)NULL,null,null,null,3);
}
/*************************************************************************************


************************************************************************************/
static void DetectorDelFomInit(void)
{
//	static FillArea	sureFill;
	static Label	sureName;
	static ListBox	sureList;
	
	string *items;
	items=(string *)RemteName;
	
	System.Gui.Form.Init(&App.Menu.DetectorDelForm);
	App.Menu.DetectorDelForm.FillAreaCount=0;
	App.Menu.DetectorDelForm.LabelCount=0;
	App.Menu.DetectorDelForm.ListCount=1;
	App.Menu.DetectorDelForm.FormID=Form9+16;
	App.Menu.DetectorDelForm.Color=GUI_ORANGE;
	App.Menu.DetectorDelForm.RectWrap.x0=50;
	App.Menu.DetectorDelForm.RectWrap.y0=80;
	App.Menu.DetectorDelForm.RectWrap.x1=280;
	App.Menu.DetectorDelForm.RectWrap.y1=200;	

//	System.Gui.Form.AddFillArea(&App.Menu.DetectorDelForm,&sureFill);
//	sureFill.ID=PromptAreaID5;
//	sureFill.PaintColor=GREEN;
//	sureFill.RectWrap.x0=50;
//	sureFill.RectWrap.y0=75;
//	sureFill.RectWrap.x1=270;
//	sureFill.RectWrap.y1=110;	

//	System.Gui.Form.AddLabel(&App.Menu.DetectorDelForm,&sureName);
//	sureName.ID=LabelID1+10;
//	sureName.Font=GUI_FONT_20_1;
//	sureName.PaintColor=GUI_BLUE;
//	sureName.TextMode=GUI_TEXTMODE_TRANS;
//	sureName.Count=17;
//	sureName.StringBlockPointer=RemteName;
//	sureName.X=100;
//	sureName.Y=85;	
	
	System.Gui.Form.AddListBox(&App.Menu.DetectorDelForm,&sureList);
	sureList.x0=50;
	sureList.y0=110;
	sureList.x1=270;
	sureList.y1=200;
	sureList.xSize=10;//纯文本列表时这里是文字显示偏移量
	sureList.ySize=15;
	
	sureList.selectedItem=0;
	sureList.ItemsPerpage=3;//页最大条目
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
	
	System.Gui.Form.AddLists(&sureList,&items[18],3,(u32*)NULL,null,null,null,3);
}

void SirneState(Form *focusFormPointer)
{
	MenuListStruct 	*currentMenu;
	Form 			*formPointer;
	ListboxList		*listx;
	u8				state;
	
 	currentMenu=CurrentMenu->Parent;
 	currentMenu=currentMenu->Parent;		
 	formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
 	listx=System.Gui.Form.ListSearch(formPointer->ListPointer->List,formPointer->ListPointer->SelectedIndexes);
 	System.Device.Storage.Parameter.StorageReadData(*(u32*)listx->IconAdd+12,&listx->State,1);
	if(SystemSirenOn==listx->State)
		state=1;
	else if(SystemSirenOff==listx->State)
		state=0;
	listx=System.Gui.Form.ListSearch(focusFormPointer->ListPointer->List,focusFormPointer->ListPointer->SelectedIndexes);
	System.Device.Storage.Parameter.StorageWriteData(*(u32*)listx->IconAdd,(u16*)&state,1);
}
const u32 SirenCheck[]=
{
	SirenStateAddr,
};
/*********************************************************************************************
*
*描述		：
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
static void SirenFomInit(void)
{
	string *items;
	static FillArea		sirenFill;
	static Label		sirenName;
	static ListBox		sirenListBox;
	items=(string *)ZoneTab;
	
	System.Gui.Form.Init(&App.Menu.ZoneSirenForm);
	App.Menu.ZoneSirenForm.FillAreaCount=1;
	App.Menu.ZoneSirenForm.LabelCount=1;
	App.Menu.ZoneSirenForm.ListCount=1;
	App.Menu.ZoneSirenForm.FormID=Form12;
	App.Menu.ZoneSirenForm.Color=GUI_WHITE;
	App.Menu.ZoneSirenForm.RectWrap.x0=0;
	App.Menu.ZoneSirenForm.RectWrap.y0=30;
	App.Menu.ZoneSirenForm.RectWrap.x1=320;
	App.Menu.ZoneSirenForm.RectWrap.y1=240;

	System.Gui.Form.AddFillArea(&App.Menu.ZoneSirenForm,&sirenFill);
	sirenFill.ID=PromptAreaID9;
	sirenFill.PaintColor=TopLabelColor;
	sirenFill.RectWrap.x0=0;
	sirenFill.RectWrap.y0=30;
	sirenFill.RectWrap.x1=320;
	sirenFill.RectWrap.y1=60;
	
	System.Gui.Form.AddLabel(&App.Menu.ZoneSirenForm,&sirenName);
	sirenName.ID=LabelID1+52;
	sirenName.Font=ListFont;
	sirenName.PaintColor=GUI_BROWN;
	sirenName.TextMode=GUI_TEXTMODE_TRANS;
	sirenName.Count=2;
	sirenName.StringBlockPointer=ZoneTab;
	sirenName.X=75;
	sirenName.Y=35;	

	System.Gui.Form.AddListBox(&App.Menu.ZoneSirenForm,&sirenListBox);
	sirenListBox.x0=0;
	sirenListBox.y0=60;
	sirenListBox.x1=320;
	sirenListBox.y1=240;
	sirenListBox.xSize=15;
	sirenListBox.ySize=15;
	
	sirenListBox.selectedItem=0;
	sirenListBox.ItemsPerpage=1;//页最大条目
	sirenListBox.ItemsHeight=37;
	sirenListBox.SelectedIndexes=0;//默认为0，也就是第一条
	sirenListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	sirenListBox.TotalItems=0;
	
	sirenListBox.TextMode=GUI_TEXTMODE_TRANS;
	sirenListBox.BackColor=GUI_WHITE;
	sirenListBox.PaintColor=GUI_DARKGRAY;
	
	sirenListBox.SelectedBackColor=MoveBarColor;
	sirenListBox.SelectedPaintColor=GUI_WHITE;
	
	sirenListBox.Font=ListFont;
	sirenListBox.ID=0x56;
	sirenListBox.Type=SwitchList;

	System.Gui.Form.AddLists(&sirenListBox,&items[2],1,(u32*)SirenCheck,null,null,null,1);
}
/*********************************************************************************************
*
*描述		：遥控删除具体函数
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
void DeleteRemoteCancel(void)
{
	MenuListStruct 	*currentMenu;
	Form 			*formPointer;
	u8 				indexes;
	u32				nameAddr,IDAddr;
	ListboxList		*listx;
	u8 				buffer[4];
	u8 				name[20];
	u8 				state=1;
	indexes=App.Menu.FocusFormPointer->ListPointer->SelectedIndexes;
	if(&App.Menu.RemoteDelForm==App.Menu.FocusFormPointer)
	{
		if(1==indexes)//第二个选择项为删除所有号码
		{
			System.SystemApp.DeleteAllRemote();	
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
			currentMenu=CurrentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
			formPointer->ListPointer->SelectedIndexes=0;
			formPointer->ListPointer->selectedItem=0;
			formPointer->ListPointer->TopIndexes=0;
			formPointer->ListPointer->TopItem=0;				
		}
		else if(0==indexes)
		{
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
			currentMenu=CurrentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
			formPointer->ListPointer->SelectedIndexes=0;
			formPointer->ListPointer->selectedItem=0;
			formPointer->ListPointer->TopIndexes=0;
			formPointer->ListPointer->TopItem=0;

			currentMenu=CurrentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
			formPointer->ListPointer->SelectedIndexes=0;
			formPointer->ListPointer->selectedItem=0;
			formPointer->ListPointer->TopIndexes=0;
			formPointer->ListPointer->TopItem=0;

			currentMenu=currentMenu->Parent;
			formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];			
			listx=System.Gui.Form.ListSearch(formPointer->ListPointer->List,formPointer->ListPointer->SelectedIndexes);
			nameAddr=*(u32*)listx->IconAdd;
			IDAddr=*listx->FlashAddr;
			memset(buffer,0x00,4);
			System.Device.Storage.Parameter.StorageWriteData(IDAddr,(u16*)buffer,2);
			
			memset(name,0x00,20);
			sprintf((char*)name,"Remote%1d",formPointer->ListPointer->SelectedIndexes+1);
			//System.Device.Storage.Parameter.StorageWriteData(nameAddr,(u16*)name,10);
			name[12]=SystemSoundOn;//默认伴音开
			name[14]=NullRemote;//表示没有遥控对入
			System.Device.Storage.Parameter.StorageWriteData(nameAddr,(u16*)&name,10);	
			//System.Device.Storage.Parameter.StorageWriteData(nameAddr+14,(u16*)&name[14],1);
			
			System.Device.Storage.Parameter.StorageReadData(RemoteTotalAddr,&state,1);
      if(state)
      {
        state-=1;
      }
			System.Device.Storage.Parameter.StorageWriteData(RemoteTotalAddr,(u16*)&state,1);
		}
		else
		{
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
		}
	}		
}
static u16 DeleteZonedetector(u8 zoneNumber,u32 addr)
{
	u32 addrOffset;
	u16	i;
	u8 buffer[4];
  u16 deleteCount=0;
    
	addrOffset=addr;
	for(i=0;i<300;i++)
	{
		addrOffset=addr+i*4;
    System.Device.Storage.ReadData(addrOffset,buffer,4);
    if(buffer[3]==zoneNumber)
    {
      deleteCount++;
      memset(buffer,0x00,4);
      System.Device.Storage.Parameter.StorageWriteData(addrOffset,(u16*)buffer,2);
    }
    memset(buffer,0x00,4);     
  }
    return deleteCount;
}


u16 DetectorTotalCheck(u16 total)
{
	u32 addr;
	u16	i;
	u8 buffer[6];

	for(i=0;i<total;i++)
	{
		addr=ZoneDetectorAddr+(i*10);
		memset(buffer,0x00,10);
		System.Device.Storage.ReadData(addr,buffer,6);
		if((buffer[2]==0)&&(buffer[1]==0)&&(buffer[0]==0))
		return i;
	}	
	return 0;
}

void DelectorDeleteAll(void)
{
	u8 buffer[20];
	u8 zoneID[10];
	u16 i=0;
	u32 addr;
	u16 total;
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
		System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)buffer,20);
	}
	total=DetectorTotalCheck(300);
	for(i=0;i<total;i++)
	{
    memset(zoneID,0,10);
		addr=ZoneDetectorAddr+(i*10);
		System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)zoneID,6);
	}
}

void DelectorDeleteCancel(void)
{
    u8              indexes;
	MenuListStruct 	*currentMenu;
	Form 			*formPointer;
	u32				nameAddr;
	ListboxList		*listx;
	u8 				name[20];  
  u8        count;
    
    indexes=App.Menu.FocusFormPointer->ListPointer->SelectedIndexes;
    if(&App.Menu.DetectorDelForm==App.Menu.FocusFormPointer)
    {
      if(0==indexes)
      {
        App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
        App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
        App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
        App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
        currentMenu=CurrentMenu->Parent;
        formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
        formPointer->ListPointer->SelectedIndexes=0;
        formPointer->ListPointer->selectedItem=0;
        formPointer->ListPointer->TopIndexes=0;
        formPointer->ListPointer->TopItem=0;

        currentMenu=CurrentMenu->Parent;
        formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
        formPointer->ListPointer->SelectedIndexes=0;
        formPointer->ListPointer->selectedItem=0;
        formPointer->ListPointer->TopIndexes=0;
        formPointer->ListPointer->TopItem=0;

        currentMenu=currentMenu->Parent;
        formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];			
        listx=System.Gui.Form.ListSearch(formPointer->ListPointer->List,formPointer->ListPointer->SelectedIndexes);
        nameAddr=*(u32*)listx->IconAdd;
        memset(name,0x00,20);
        sprintf((char*)name,"Zone %.2d",formPointer->ListPointer->SelectedIndexes+1);
			//System.Device.Storage.Parameter.StorageWriteData(nameAddr,(u16*)name,10);
        name[12]=SystemSirenOn;//默认伴音关
        name[14]=SystemZoneAlarm;//表示没有遥控对入
        name[16]=0;
			//System.Device.Storage.Parameter.StorageWriteData(nameAddr+12,(u16*)&name[12],1);	
			//System.Device.Storage.Parameter.StorageWriteData(nameAddr+14,(u16*)&name[14],1);
        System.Device.Storage.Parameter.StorageWriteData(nameAddr,(u16*)name,8);
            
        count=DeleteZonedetector(formPointer->ListPointer->SelectedIndexes+1,ZoneDetectorAddr);
        if(count)
        {
          System.Device.Storage.Parameter.StorageReadData(DetectorTotalAddr,&indexes,1);
          indexes-=count;
          System.Device.Storage.Parameter.StorageWriteData(DetectorTotalAddr,(u16*)&indexes,1);
        }
      }
      else if(1==indexes)
      {
            //System.SystemApp.DeleteAllDetector();	
        DelectorDeleteAll();
        App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
        App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
        App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
        App.Menu.FocusFormPointer->ListPointer->TopItem=0;	
        currentMenu=CurrentMenu->Parent;
        formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
        formPointer->ListPointer->SelectedIndexes=0;
        formPointer->ListPointer->selectedItem=0;
        formPointer->ListPointer->TopIndexes=0;
        formPointer->ListPointer->TopItem=0;	
       }
       else
       {
          App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
          App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
          App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
          App.Menu.FocusFormPointer->ListPointer->TopItem=0;
       }
    }
}
/*********************************************************************************************
*
*描述		：无线防区和遥控初始化
*输入参数	：无
*输出参数	：无
*
*********************************************************************************************/
void RemoteInit(void)
{
  SensorsListInit();              //3.1
  
	RemoteList();                   //3.1.1
  RemoteConfig();                 //3.1.1.1
  RemoteCodeFormInit();           //3.1.1.1.1
  RemoteSoundInit();              //3.1.1.1.2
  RemoteDelFomInit();             //3.1.1.1.3
    
 	ZoneSetList();                  //3.1.2
  ZoneConfigInit();//功能配置列表 //3.1.2.1
  ZoneAttributeInit();//属性选择  //3.1.2.1.1
  SirenFomInit();                 //3.1.2.1.2
  DetectorDelFomInit();           //3.1.2.1.3
   
 	WirelessSirenList();            //3.1.3
  
 	ReadyArmList();                 //3.1.4

}

