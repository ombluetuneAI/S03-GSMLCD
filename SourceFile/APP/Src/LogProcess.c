#include "LogProcess.h"
#include "APP.h"
#include "TaskMenu.h"

extern const unsigned char gImage_logIco[3][808];

u32 LogStateAddr[30];

const u8* LogTab[4]=
{
  "Log",
	"Alarm Record",
	"Disarm Record",
	"Arm/Stay Record",
};

const u8 *LogPath[3]=
{
    "0:/ICON/Log/alarmLog.bmp",
    "0:/ICON/Log/disarm.bmp",
    "0:/ICON/Log/armLog.bmp",
};

const string LogString[3]=
{
    "Alarm Log",
    "Disarm Log",
    "Arm Log",
};



/***************************************************************************************************

***************************************************************************************************/
static void LogListInit(void)
{
	static Icon 	LogIconLabel;
	static Label 	LogListName;
	static ListBox 	LogListBox;
	static FillArea LogFill;
	string *items;
	items=(string *)&LogTab[1];

/*--------------------------------日志--------------------------------------*/	
	
	System.Gui.Form.Init(&App.Menu.LogListForm);
	App.Menu.LogListForm.FormID=Form9;
	App.Menu.LogListForm.LabelCount=1;
	App.Menu.LogListForm.ListCount=1;
	App.Menu.LogListForm.FillAreaCount=1;
	
	System.Gui.Form.AddFillArea(&App.Menu.LogListForm,&LogFill);
	LogFill.ID=PromptAreaID7;
	LogFill.PaintColor=TopLabelColor;
	LogFill.RectWrap.x0=0;
	LogFill.RectWrap.y0=30;
	LogFill.RectWrap.x1=320;
	LogFill.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.LogListForm,&LogListName);
	LogListName.ID=LabelID1+9;
	LogListName.Font=ListNameFont;
	LogListName.PaintColor=GUI_BLACK;
	LogListName.TextMode=TransparentText;
	LogListName.Count=0;
	LogListName.StringBlockPointer=(const string *)&LogTab[0];
	LogListName.X=130;
	LogListName.Y=43;
	
			//列表
	System.Gui.Form.AddListBox(&App.Menu.LogListForm,&LogListBox);
	LogListBox.ID=ListBoxID5;
	LogListBox.x0=0;
	LogListBox.y0=80;
	LogListBox.x1=320;
	LogListBox.y1=240;
	LogListBox.Type=IconList;
	LogListBox.selectedItem=0;
	LogListBox.ItemsPerpage=3;//页最大条目
  LogListBox.ItemsHeight=40;
	LogListBox.SelectedIndexes=0;//默认为0，也就是第一条
	LogListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	LogListBox.TotalItems=0;
	
	LogListBox.TextMode=GUI_TEXTMODE_TRANS;
	LogListBox.BackColor=GUI_WHITE;
	LogListBox.PaintColor=GUI_BLACK;
	
	LogListBox.SelectedBackColor=MoveBarColor;
	LogListBox.SelectedPaintColor=GUI_WHITE;
	
	LogListBox.Font=ListFont;	
	
	System.Gui.Form.AddLists(&LogListBox,items,3,null,null,null,(u8**)gImage_logIco,3);
		
}


void LogRecordListInit(void)
{
  static FillArea		logTopFill;
  static Label		logName;
  static ListBox		logListBox;	

  System.Gui.Form.Init(&App.Menu.LogStateListForm);	
  App.Menu.LogStateListForm.FillAreaCount=1;
  App.Menu.LogStateListForm.LabelCount=1;
  App.Menu.LogStateListForm.ListCount=1;
  App.Menu.LogStateListForm.FormID=Form9;
  App.Menu.LogStateListForm.Color=GUI_WHITE;
  App.Menu.LogStateListForm.RectWrap.x0=0;
  App.Menu.LogStateListForm.RectWrap.y0=30;
  App.Menu.LogStateListForm.RectWrap.x1=320;
  App.Menu.LogStateListForm.RectWrap.y1=240;	
    
  System.Gui.Form.AddFillArea(&App.Menu.LogStateListForm,&logTopFill);
	logTopFill.ID=PromptAreaID6;
	logTopFill.PaintColor=TopLabelColor;
	logTopFill.RectWrap.x0=0;
	logTopFill.RectWrap.y0=30;
	logTopFill.RectWrap.x1=320;
	logTopFill.RectWrap.y1=80;

  System.Gui.Form.AddLabel(&App.Menu.LogStateListForm,&logName);
  logName.ID=LabelID1+2;
  logName.Font=ListNameFont;
  logName.PaintColor=GUI_BLACK;
  logName.TextMode=GUI_TEXTMODE_TRANS;
  logName.Count=0;
  logName.StringBlockPointer=LogString;
  logName.X=100;
  logName.Y=43;  
    
	//列表
	System.Gui.Form.AddListBox(&App.Menu.LogStateListForm,&logListBox);
	logListBox.ID=ListBoxID2;
  
  logListBox.x0=0;
	logListBox.y0=80;
	logListBox.x1=320;
	logListBox.y1=240;

    
	logListBox.Type=LogStateList;
	logListBox.selectedItem=0;
  logListBox.ItemsHeight=40;
	logListBox.ItemsPerpage=4;//页最大条目
	logListBox.SelectedIndexes=0;//默认为0，也就是第一条
	logListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
	logListBox.TotalItems=0;
	
	logListBox.TextMode=GUI_TEXTMODE_TRANS;
	logListBox.BackColor=GUI_WHITE;
	logListBox.PaintColor=GUI_BLACK;
	
	logListBox.SelectedBackColor=MoveBarColor;
	logListBox.SelectedPaintColor=GUI_WHITE;
	
	logListBox.Font=ListFont;
	logListBox.ID=0xF2;

	System.Gui.Form.AddLists(&logListBox,null,30,(u32*)LogStateAddr,null,null,(u8**)gImage_logIco,1);
}


void LogAddrGet(Form *pointer)
{
  MenuListStruct 	*currentMenu;
  Form 			*formPointer;
  ListboxList		*listx;	
  Label			*focusLabel;
  u32             addr;
  u8              i=0;
  u8              *pathPointer;             
    
  if(&App.Menu.LogStateListForm==pointer)
	{
    currentMenu=CurrentMenu->Parent;       
 		formPointer=currentMenu->MenuItems[CurrentMenu->curSelected];
    focusLabel=App.Menu.FocusFormPointer->LabelPointer;
    if(&App.Menu.LogListForm==formPointer)
    {
      if(formPointer->ListPointer->SelectedIndexes==0)
      {
        addr=LogAlarmAddr;
        focusLabel->Count=0;
        pathPointer=(u8*)gImage_logIco[0];
      }
      else if(formPointer->ListPointer->SelectedIndexes==1)
      {
        addr=LogDisarmAddr;
        focusLabel->Count=1;
        pathPointer=(u8*)gImage_logIco[1];
      }
      else if(formPointer->ListPointer->SelectedIndexes==2)
      {
        addr=LogArmAddr;
        focusLabel->Count=2;
        pathPointer=(u8*)gImage_logIco[2];
      }
    }
    listx=System.Gui.Form.ListSearch(pointer->ListPointer->List,0);
    while(listx!=0)
    {
      *listx->IconAdd=addr+(i*16);
      listx->Path=pathPointer;
      listx=listx->Nextlist;
      i++;
    }   
  }
}

/***************************************************************************************************
*
*描述		：	设置菜单的链表
*输入参数	：	logAddr,日志地址，
                indexAddr，当前日志索引号
                type,操作方式，1是按键，2是遥控  3,防区  4，电话远程
                state,记录状态 1，撤防  2，布防  3，在家  4，防区报警 5，遥控报警 6，面板报警 
                zone,防区号 或者遥控号
*输出参数	：	无
*
***************************************************************************************************/
void LogWrite(u32 logAddr,u32 indexAddr,u8 type,u8 state,u8 zone)
{
    u8      LogIndex;
    u8      buffer[10];
    u32     addr;
    
    System.Device.Storage.Parameter.StorageReadData(indexAddr,&LogIndex,1);
    if(LogIndex==30) LogIndex=0;
    LogIndex++;
    System.Device.Storage.Parameter.StorageWriteData(indexAddr,(u16*)&LogIndex,1);
    buffer[0]=App.Data.Year>>8;
    buffer[1]=App.Data.Year&0x00FF;
    buffer[2]=App.Data.Month;
    buffer[3]=App.Data.Day;
    buffer[4]=App.Data.Hour;
    buffer[5]=App.Data.Min;
    buffer[6]=type;
    buffer[7]=0;
    buffer[8]=state;
    buffer[9]=zone; 
    addr=logAddr+((LogIndex-1)*10);
   // memset(buffer,0xff,8);
    
    System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)buffer,5);
}

void LogMenuInit(void)
{
  LogListInit();
  LogRecordListInit();
}

