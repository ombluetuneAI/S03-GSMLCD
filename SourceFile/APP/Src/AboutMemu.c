#include "AboutMemu.h"
#include "APP.h"
#include "TaskMenu.h"

const char * AboutString[8]=
{
	"About",
	"PO Box 991",
	"Walla Walla,WA 99362",
	"Support PHONE: 001-(206) 981-5371",
	"EMAIL: support@fortresssecuritystore.com",
	"IMEI: ",
	"V1.0   ",
	"May  09  2017",
};

void AboutMenuInit(void)
{
  static FillArea		aboutTopFill;
  static Label		  aboutName;
  static Label		  aboutName1;
  static Label		  aboutName2;
  static Label		  aboutName3;
	System.Gui.Form.Init(&App.Menu.AboutInfoForm);
	App.Menu.AboutInfoForm.FormID=Form16;
	App.Menu.AboutInfoForm.FillAreaCount=1;
	App.Menu.AboutInfoForm.LabelCount=2;
	App.Menu.AboutInfoForm.Color=GUI_WHITE;
	App.Menu.AboutInfoForm.RectWrap.x0=0;
	App.Menu.AboutInfoForm.RectWrap.y0=30;
	App.Menu.AboutInfoForm.RectWrap.x1=319;
	App.Menu.AboutInfoForm.RectWrap.y1=240;
  
  System.Gui.Form.AddFillArea(&App.Menu.AboutInfoForm,&aboutTopFill);
	aboutTopFill.ID=30;
	aboutTopFill.PaintColor=TopLabelColor;
	aboutTopFill.RectWrap.x0=0;
	aboutTopFill.RectWrap.y0=30;
	aboutTopFill.RectWrap.x1=320;
	aboutTopFill.RectWrap.y1=80;

  System.Gui.Form.AddLabel(&App.Menu.AboutInfoForm,&aboutName);
  aboutName.ID=LabelID1+100;
  aboutName.Font=ListNameFont;
  aboutName.PaintColor=GUI_BLACK;
  aboutName.BackColor=GUI_ORANGE;
  aboutName.TextMode=GUI_TEXTMODE_TRANS;
  aboutName.Count=0;
  aboutName.StringBlockPointer=(const string *)&AboutString[0];
  aboutName.X=100;
  aboutName.Y=43;

  System.Gui.Form.AddLabel(&App.Menu.AboutInfoForm,&aboutName1);
  aboutName1.ID=LabelID1+101;
  aboutName1.Type = 0xF1;
  aboutName1.StringBlockPointer=NULL;

//  System.Gui.Form.AddLabel(&App.Menu.AboutInfoForm,&aboutName1);
//  aboutName1.ID=LabelID1+101;
//  aboutName1.Font=ListFont;
//  aboutName1.PaintColor=GUI_BROWN;
//  aboutName1.BackColor=GUI_ORANGE;
//  aboutName1.TextMode=GUI_TEXTMODE_TRANS;
//  aboutName1.Count=0;
//  aboutName1.StringBlockPointer=&AboutString[1];
//  aboutName1.X=5;
//  aboutName1.Y=200; 
//  
//  System.Gui.Form.AddLabel(&App.Menu.AboutInfoForm,&aboutName2);
//  aboutName2.ID=LabelID1+102;
//  aboutName2.Font=ListFont;
//  aboutName2.PaintColor=GUI_BROWN;
//  aboutName2.BackColor=GUI_ORANGE;
//  aboutName2.TextMode=GUI_TEXTMODE_TRANS;
//  aboutName2.Count=0;
//  aboutName2.StringBlockPointer=&AboutString[2];
//  aboutName2.X=250;
//  aboutName2.Y=200; 
//  
//  System.Gui.Form.AddLabel(&App.Menu.AboutInfoForm,&aboutName3);
//  aboutName3.ID=LabelID1+103;
//  aboutName3.Font=ListFont;
//  aboutName3.PaintColor=GUI_BROWN;
//  aboutName3.BackColor=GUI_ORANGE;
//  aboutName3.TextMode=GUI_TEXTMODE_TRANS;
//  aboutName3.Count=0;
//  aboutName3.StringBlockPointer=&AboutString[3];
//  aboutName3.X=280;
//  aboutName3.Y=200;
}
extern GUI_CONST_STORAGE GUI_BITMAP bmFortress;
void AboutInfoDisp(void)
{
  u16	x = 0,y = 0;

	GUI_DrawBitmap(&bmFortress, 80, 82);
  
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
	GUI_SetFont(GUI_FONT_16_1);
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
	
	y = 140;
	x = 10;
	GUI_DispStringAt(AboutString[1],x,y);
	y += 19;
	GUI_DispStringAt(AboutString[2],x,y);
	y += 19;
	GUI_DispStringAt(AboutString[3],x,y);
	y += 19;
	GUI_DispStringAt(AboutString[4],x,y);
	y += 19;
	GUI_DispStringAt(AboutString[5],x,y);
	GUI_DispString((const char *)App.Data.IMEINum);
	x += 170;
	GUI_DispStringAt(AboutString[6],x,y);
	GUI_DispString(AboutString[7]);
}





