

#include "RecordMenu.h"
#include "APP.h"
#include "TaskMenu.h"

extern const unsigned char gImage_recordeIco[][2600];
//u8* const MenuRecorderTab[3]=
//{
//  "Voice Record and Play",
//  "Record",
//  "Play Recroding",
//};

const u8* RecordListTab[]=
{
  "Recording",
  "1:Recording",
  "2:Play",
  "1:Stop",
  "2:Stop",
};
//const string RecordStringTab[]=
//{
//	"Press 1 to start recording ",
//	"Press 1 to start stop recording ",
//	"Recording",
//	"Press 1 to play recording",
//  "Press 1 to play recording"
//};
//const u8 *RecPath[2]=
//{
//    "0:/ICON/Voice/stopR.bmp",
//    "0:/ICON/Voice/Recing.bmp",
//};

//const string VoicePath[2]=
//{
//    "0:/ICON/Voice/rec.bmp",
//    "0:/ICON/Voice/play.bmp",      
//};

//void VoiceListInit(void)
//{
//  static ListBox 	VoiceListBox;
//	static FillArea VoiceArea;
//	static Label 	VoiceListName;
//  string *items;
//	items=(string *)&MenuRecorderTab[1];
//  
//  System.Gui.Form.Init(&App.Menu.VoiceListForm);
//	App.Menu.VoiceListForm.FormID=29;
//	App.Menu.VoiceListForm.LabelCount=1;
//	App.Menu.VoiceListForm.FillAreaCount=1;
//	App.Menu.VoiceListForm.ListCount=1;
//	
//	System.Gui.Form.AddFillArea(&App.Menu.VoiceListForm,&VoiceArea);
//	VoiceArea.ID=PromptAreaID9;
//	VoiceArea.PaintColor=TopLabelColor;
//	VoiceArea.RectWrap.x0=0;
//	VoiceArea.RectWrap.y0=30;
//	VoiceArea.RectWrap.x1=320;
//	VoiceArea.RectWrap.y1=80;
//	
//	System.Gui.Form.AddLabel(&App.Menu.VoiceListForm,&VoiceListName);
//	VoiceListName.ID=LabelID1+11;
//	VoiceListName.Font=ListNameFont;
//	VoiceListName.PaintColor=GUI_BLACK;
//	VoiceListName.TextMode=GUI_TEXTMODE_TRANS;
//	VoiceListName.Count=0;
//	VoiceListName.StringBlockPointer=(const string*)&MenuRecorderTab[0];
//	VoiceListName.X=60;
//	VoiceListName.Y=43;		

//			//列表
//	System.Gui.Form.AddListBox(&App.Menu.VoiceListForm,&VoiceListBox);
//	VoiceListBox.ID=ListBoxID3;
//	VoiceListBox.x0=0;
//	VoiceListBox.y0=80;
//	VoiceListBox.x1=320;
//	VoiceListBox.y1=240;
//	VoiceListBox.Type=IconList;
//	VoiceListBox.selectedItem=0;
//  VoiceListBox.ItemsHeight=40;
//	VoiceListBox.ItemsPerpage=2;//页最大条目
//	VoiceListBox.SelectedIndexes=0;//默认为0，也就是第一条
//	VoiceListBox.TopItem=0;//默认为0，也就是从第一个索引号开始
//	VoiceListBox.TopItem=0;
//	
//	VoiceListBox.TextMode=GUI_TEXTMODE_TRANS;
//	VoiceListBox.BackColor=GUI_WHITE;
//	VoiceListBox.PaintColor=GUI_BLACK;
//	
//	VoiceListBox.SelectedBackColor=MoveBarColor;
//	VoiceListBox.SelectedPaintColor=GUI_WHITE;
//	
//	VoiceListBox.Font=ListFont;
//	VoiceListBox.ID=0x50;		
//	
//	System.Gui.Form.AddLists(&VoiceListBox,items,2,null,null,null,(u8**)gImage_recordeIco,2);	//(u32*)FlashAdd[4]
//}
/*********************************************************************************

*描述		: 录音播放菜单
*输入参数	：无
*输出参数	：无

*********************************************************************************/
//static void RecordMenu(void)
//{
//    static Label 		RecordRemind;
//    static Label 		RecordName;
//    static FillArea		RecordFill;
//    static AutoDigit    autoDigit;
//    
//	System.Gui.Form.Init(&App.Menu.RecordForm);
//	App.Menu.RecordForm.FormID=Form3;
//	App.Menu.RecordForm.FillAreaCount=1;
//    App.Menu.RecordForm.IconCount=0;
//	App.Menu.RecordForm.LabelCount=2;
//	App.Menu.RecordForm.AutoDigitCount=1;
//	
//	App.Menu.RecordForm.Color=GUI_WHITE;
//	App.Menu.RecordForm.RectWrap.x0=0;
//	App.Menu.RecordForm.RectWrap.y0=30;
//	App.Menu.RecordForm.RectWrap.x1=320;
//	App.Menu.RecordForm.RectWrap.y1=240;
//	
//	
//	System.Gui.Form.AddFillArea(&App.Menu.RecordForm,&RecordFill);
//	RecordFill.ID=PromptAreaID9;
//	RecordFill.PaintColor=0x440;
//	RecordFill.RectWrap.x0=0;
//	RecordFill.RectWrap.y0=30;
//	RecordFill.RectWrap.x1=320;
//	RecordFill.RectWrap.y1=65;
//	
//	System.Gui.Form.AddLabel(&App.Menu.RecordForm,&RecordName);
//	RecordName.ID=LabelID1+23;
//	RecordName.Font=ListFont;
//	RecordName.PaintColor=GUI_BROWN;
//	RecordName.TextMode=GUI_TEXTMODE_TRANS;
//	RecordName.Count=2;
//	RecordName.StringBlockPointer=RecordStringTab;
//	RecordName.X=120;
//	RecordName.Y=35;	

//	System.Gui.Form.AddLabel(&App.Menu.RecordForm,&RecordRemind);
//	RecordRemind.ID=LabelID1+21;
//	RecordRemind.Font=ListFont;
//	RecordRemind.PaintColor=GUI_BLACK;
//	RecordRemind.TextMode=GUI_TEXTMODE_TRANS;
//  RecordRemind.Type=0xF7;
//	RecordRemind.Count=0;
//	RecordRemind.StringBlockPointer=RecordStringTab;
//	RecordRemind.X=60;
//	RecordRemind.Y=80;	
//    
//  System.Gui.Form.AddAutoDigit(&App.Menu.RecordForm,&autoDigit);//录音数秒
//  autoDigit.ID=150;
//  autoDigit.x=120;
//  autoDigit.y=130;
//  autoDigit.xSize=80;
//  autoDigit.ySize=80;
//  autoDigit.Value=0;
//	autoDigit.bit=1;
//  autoDigit.Font=GUI_FONT_D36X48;
//  autoDigit.PaintColor=GUI_RED;
//  autoDigit.BackColor=GUI_ORANGE;
//  autoDigit.TextMode=GUI_TEXTMODE_TRANS;
//    
//}
/*********************************************************************************

*描述		: 录音播放列表
*输入参数	：无
*输出参数	：无

*********************************************************************************/
//static void PlayTapeMenu(void)
//{
//    static Label 		PlayTapeRemind;
//    static Label		PlayTapeName;
//    static FillArea		playFill;
//    static AutoDigit    autoDigit;
//    
//	System.Gui.Form.Init(&App.Menu.PlayForm);
//	App.Menu.PlayForm.FormID=Form6;
//	App.Menu.PlayForm.FillAreaCount=1;
//	App.Menu.PlayForm.LabelCount=2;
//	App.Menu.PlayForm.AutoDigitCount=1;
//	
//	App.Menu.PlayForm.Color=GUI_WHITE;
//	App.Menu.PlayForm.RectWrap.x0=0;
//	App.Menu.PlayForm.RectWrap.y0=30;
//	App.Menu.PlayForm.RectWrap.x1=320;
//	App.Menu.PlayForm.RectWrap.y1=240;	
//	
//  System.Gui.Form.AddFillArea(&App.Menu.PlayForm,&playFill);
//	playFill.ID=PromptAreaID9;
//	playFill.PaintColor=0x440;
//	playFill.RectWrap.x0=0;
//	playFill.RectWrap.y0=30;
//	playFill.RectWrap.x1=320;
//	playFill.RectWrap.y1=65;
//    
//	System.Gui.Form.AddLabel(&App.Menu.PlayForm,&PlayTapeName);
//	PlayTapeName.ID=LabelID1+23;
//	PlayTapeName.Font=ListFont;
//	PlayTapeName.PaintColor=GUI_BROWN;
//	PlayTapeName.TextMode=GUI_TEXTMODE_TRANS;
//	PlayTapeName.Count=3;
//	PlayTapeName.StringBlockPointer=RecordStringTab;
//	PlayTapeName.X=100;
//	PlayTapeName.Y=35;		
//	
//	System.Gui.Form.AddLabel(&App.Menu.PlayForm,&PlayTapeRemind);
//	PlayTapeRemind.ID=LabelID1+25;
//	PlayTapeRemind.Font=ListFont;
//	PlayTapeRemind.PaintColor=GUI_BROWN;
//	PlayTapeRemind.TextMode=GUI_TEXTMODE_TRANS;
//  PlayTapeRemind.Type=0xF7;
//	PlayTapeRemind.Count=3;
//	PlayTapeRemind.StringBlockPointer=RecordStringTab;
//	PlayTapeRemind.X=60;
//	PlayTapeRemind.Y=80;

//  System.Gui.Form.AddAutoDigit(&App.Menu.PlayForm,&autoDigit);//数字显示
//  autoDigit.ID=10;
//  autoDigit.x=120;
//  autoDigit.y=130;
//  autoDigit.xSize=80;
//  autoDigit.ySize=80;
//  autoDigit.Value=9;
//	autoDigit.bit=1;
//  autoDigit.Font=GUI_FONT_D36X48;
//  autoDigit.PaintColor=GUI_RED;
//  autoDigit.BackColor=GUI_ORANGE;
//  autoDigit.TextMode=GUI_TEXTMODE_TRANS;
//}

static void RecordingMenuInit(void)
{
  static ListBox 	VoiceListBox;
	static FillArea VoiceArea;
	static Label 	VoiceListName;
  static Icon   VoiceListIcon1;
  static Icon   VoiceListIcon2;
  static AutoDigit    autoDigit;
  
  System.Gui.Form.Init(&App.Menu.VoiceListForm);
	App.Menu.VoiceListForm.FormID=29;
	App.Menu.VoiceListForm.LabelCount=1;
	App.Menu.VoiceListForm.FillAreaCount=1;
  App.Menu.VoiceListForm.AutoDigitCount = 1;
  App.Menu.VoiceListForm.IconCount = 2;
  App.Menu.VoiceListForm.RectWrap.x0=0;
	App.Menu.VoiceListForm.RectWrap.y0=30;
	App.Menu.VoiceListForm.RectWrap.x1=320;
	App.Menu.VoiceListForm.RectWrap.y1=240;
  App.Menu.VoiceListForm.Color = GUI_WHITE;
	
	System.Gui.Form.AddFillArea(&App.Menu.VoiceListForm,&VoiceArea);
	VoiceArea.ID=PromptAreaID9;
	VoiceArea.PaintColor=TopLabelColor;
	VoiceArea.RectWrap.x0=0;
	VoiceArea.RectWrap.y0=30;
	VoiceArea.RectWrap.x1=320;
	VoiceArea.RectWrap.y1=80;
	
	System.Gui.Form.AddLabel(&App.Menu.VoiceListForm,&VoiceListName);
	VoiceListName.ID=LabelID1+11;
	VoiceListName.Font=ListNameFont;
	VoiceListName.PaintColor=GUI_BLACK;
	VoiceListName.TextMode=GUI_TEXTMODE_TRANS;
	VoiceListName.Count=0;
	VoiceListName.StringBlockPointer=(const string*)&RecordListTab[0];
	VoiceListName.X=60;
	VoiceListName.Y=43;	

  System.Gui.Form.AddIcon(&App.Menu.VoiceListForm,&VoiceListIcon1);
  VoiceListIcon1.ID = IconID1;
  VoiceListIcon1.x = 83;
  VoiceListIcon1.y = 160;
  VoiceListIcon1.IconPointer = (u8*)gImage_recordeIco[2];
  //VoiceListIcon1.Type = Image2Lcd;
  VoiceListIcon1.StringPointer = (u8*)RecordListTab[1];
  
  System.Gui.Form.AddIcon(&App.Menu.VoiceListForm,&VoiceListIcon2);
  VoiceListIcon2.ID = IconID1+1;
  VoiceListIcon2.x = 202;
  VoiceListIcon2.y = 160;
  VoiceListIcon2.IconPointer = (u8*)gImage_recordeIco[3];
  //VoiceListIcon2.Type = Image2Lcd;
  VoiceListIcon2.StringPointer = (u8*)RecordListTab[2];
  
  System.Gui.Form.AddAutoDigit(&App.Menu.VoiceListForm,&autoDigit);//录音数秒
  autoDigit.ID=150;
  autoDigit.x=140;
  autoDigit.y=100;
  autoDigit.xSize=40;
  autoDigit.ySize=40;
  autoDigit.Value=0;
	autoDigit.bit=1;
  autoDigit.Font=GUI_FONT_D24X32;
  autoDigit.PaintColor=GUI_RED;
  autoDigit.BackColor=MoveBarColor;
  autoDigit.TextMode=GUI_TEXTMODE_TRANS;
  
}
/*************************************************************************


*************************************************************************/
void VoiceInit(void)
{
  RecordingMenuInit();
//  VoiceListInit();
//	RecordMenu();
//  PlayTapeMenu();
}
