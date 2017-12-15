#include "CallMenu.h"


const string CallStringTab[5]=
{
	"Call",
	"press \"OK\" to dial",
	"Calling",
	"\"Return\"Hang",
	":"
};

/*********************************************************************
*
*描述		：拨打电话菜单原始参数
*输入参数	：无
*输出参数	：无
*
*********************************************************************/
void CallMenuInit(void)
{
	static Label 	nameLabel;
	static Label    noticeLabel;
	static TextBox	numberPhoneText;
	static FillArea nameFill;
	
	System.Gui.Form.Init(&App.Menu.CallForm);
	App.Menu.CallForm.FillAreaCount=1;
	App.Menu.CallForm.LabelCount=2;
	App.Menu.CallForm.ListCount=0;
	App.Menu.CallForm.TextBoxCount=1;
	App.Menu.CallForm.FormID=Form1+88;
	App.Menu.CallForm.Color=GUI_WHITE;
	App.Menu.CallForm.RectWrap.x0=0;
	App.Menu.CallForm.RectWrap.y0=30;
	App.Menu.CallForm.RectWrap.x1=320;
	App.Menu.CallForm.RectWrap.y1=240;	
	App.Menu.CallForm.DefaultFocusTextBoxPointer=&numberPhoneText;
	App.Menu.CallForm.FocusTextBoxPointer=&numberPhoneText;//当前页面焦点从ArmTextBox开始
	
	System.Gui.Form.AddFillArea(&App.Menu.CallForm,&nameFill);
	nameFill.ID=PromptAreaID1+88;
	nameFill.PaintColor=TopLabelColor;
	nameFill.RectWrap.x0=0;
	nameFill.RectWrap.y0=30;
	nameFill.RectWrap.x1=320;
	nameFill.RectWrap.y1=80;

	System.Gui.Form.AddLabel(&App.Menu.CallForm,&noticeLabel);
	noticeLabel.ID=LabelID1+88;
	noticeLabel.Font=ListFont;
	noticeLabel.PaintColor=GUI_BLACK;
	noticeLabel.TextMode=GUI_TEXTMODE_TRANS;
	noticeLabel.Count=1;
	noticeLabel.StringBlockPointer=CallStringTab;
	noticeLabel.X=10;
	noticeLabel.Y=100;	
  noticeLabel.Type=0xC0;

	System.Gui.Form.AddLabel(&App.Menu.CallForm,&nameLabel);
	nameLabel.ID=LabelID1+89;
	nameLabel.Font=ListNameFont;
	nameLabel.PaintColor=GUI_BLACK;
	nameLabel.TextMode=GUI_TEXTMODE_TRANS;
	nameLabel.Count=0;
	nameLabel.StringBlockPointer=CallStringTab;
	nameLabel.X=125;
	nameLabel.Y=43;	
	
	System.Gui.Form.AddTextBox(&App.Menu.CallForm,&numberPhoneText);
	numberPhoneText.x=21;
	numberPhoneText.y=140;
	numberPhoneText.MaxDataPointer=25;
	numberPhoneText.MinDataPointer=0;
	numberPhoneText.Type=GuiDataTypeString;
	numberPhoneText.RimType=0x01;
	numberPhoneText.BackColor=GUI_WHITE;
	numberPhoneText.PaintColor=GUI_BLACK;
	numberPhoneText.Font=ListFont;
	numberPhoneText.ID=0x89;
	numberPhoneText.TextMode=GUI_TEXTMODE_NORMAL;	
	numberPhoneText.RectWrap.x0=21;
	numberPhoneText.RectWrap.x1=290;
	numberPhoneText.RectWrap.y0=140;
	numberPhoneText.RectWrap.y1=170;
	numberPhoneText.DataAddr=0;
	numberPhoneText.DataPointer=&App.Data.KeyBuffer2;
	
}
void DialBufferClean(Form *pointer)
{
	memset(App.Data.KeyBuffer2,0,30);
	sprintf((char*)App.Data.KeyBuffer2,"Please enter a number");
}
/*************************************************************
*描述		：拨号菜单原始参数
*输入参数	：无
*输出参数	：无
*************************************************************/
static void DialMenuInit(void)
{
	static Label 		numberLabel;
	static AutoDigit	minDigit;
	static Label		signLabel;
	static FillArea 	HangUpFill;
	static Label 		hangUpLabel;
	static AutoDigit    autoDigit;
	
	System.Gui.Form.Init(&App.Menu.DialForm);
	App.Menu.DialForm.FillAreaCount=1;
	App.Menu.DialForm.LabelCount=3;
	App.Menu.DialForm.ListCount=0;
	App.Menu.DialForm.AutoDigitCount=2;
	App.Menu.DialForm.TextBoxCount=0;
	App.Menu.DialForm.FormID=Form2;
	App.Menu.DialForm.Color=0xC618;
	App.Menu.DialForm.RectWrap.x0=0;
	App.Menu.DialForm.RectWrap.y0=30;
	App.Menu.DialForm.RectWrap.x1=320;
	App.Menu.DialForm.RectWrap.y1=240;		
	
	System.Gui.Form.AddLabel(&App.Menu.DialForm,&numberLabel);
	numberLabel.ID=LabelID1;
	numberLabel.Font=GUI_FONT_32_1;
	numberLabel.PaintColor=GUI_BLACK;
	numberLabel.TextMode=GUI_TEXTMODE_TRANS;
	numberLabel.Count=1;
	numberLabel.StringBlockPointer=(const string*)App.Data.KeyBuffer2;
	numberLabel.Type=0xF5;
	numberLabel.X=0;
	numberLabel.Y=70;		
	
	System.Gui.Form.AddLabel(&App.Menu.DialForm,&hangUpLabel);
	hangUpLabel.ID=LabelID1+3;
	hangUpLabel.Font=ListFont;
	hangUpLabel.PaintColor=GUI_WHITE;
	hangUpLabel.BackColor=GUI_RED;
	hangUpLabel.TextMode=GUI_TEXTMODE_NORMAL;
	hangUpLabel.Count=3;
	hangUpLabel.StringBlockPointer=CallStringTab;
	hangUpLabel.Type=0;
	hangUpLabel.X=105;
	hangUpLabel.Y=215;	
	
	System.Gui.Form.AddLabel(&App.Menu.DialForm,&signLabel);
	signLabel.ID=LabelID1+4;
	signLabel.Font=ListFont;
	signLabel.PaintColor=GUI_WHITE;
	signLabel.TextMode=GUI_TEXTMODE_TRANS;
	signLabel.Count=4;
	signLabel.StringBlockPointer=CallStringTab;
	signLabel.Type=0;
	signLabel.X=155;
	signLabel.Y=150;

	System.Gui.Form.AddFillArea(&App.Menu.DialForm,&HangUpFill);
	HangUpFill.ID=PromptAreaID1+88;
	HangUpFill.PaintColor=0xf800;
	HangUpFill.RectWrap.x0=100;
	HangUpFill.RectWrap.y0=210;
	HangUpFill.RectWrap.x1=220;
	HangUpFill.RectWrap.y1=240;

	System.Gui.Form.AddAutoDigit(&App.Menu.DialForm,&minDigit);
  minDigit.ID=151;
  minDigit.x=130;
  minDigit.y=150;
  minDigit.xSize=0;
  minDigit.ySize=0;
  minDigit.Value=0;
	minDigit.bit=2;
  minDigit.Font=ListFont;
  minDigit.PaintColor=GUI_WHITE;
  minDigit.BackColor=GUI_LIGHTGRAY;
  minDigit.TextMode=GUI_TEXTMODE_TRANS;
	
  System.Gui.Form.AddAutoDigit(&App.Menu.DialForm,&autoDigit);
  autoDigit.ID=150;
  autoDigit.x=165;
  autoDigit.y=150;
  autoDigit.xSize=0;
  autoDigit.ySize=0;
  autoDigit.Value=0;
	autoDigit.bit=2;
  autoDigit.Font=ListFont;
  autoDigit.PaintColor=GUI_WHITE;
  autoDigit.BackColor=GUI_LIGHTGRAY;
  autoDigit.TextMode=GUI_TEXTMODE_TRANS;
}
/*****************************************************************************
*
*描述		：获取按键值，并发送相应的DTMF值
*输入参数	：无
*输出参数	：无
*
*****************************************************************************/
u8 DtmfToneGet(u8 key)
{
	u8 keyDTMF=0xFF;
	u8 stringDTMF[20];
	u8 generationDTMF[20];
	u8 res=0xFF;
	
	memset(stringDTMF,0,20);
	memset(generationDTMF,0,20);
	if(App.Menu.FocusFormPointer==&App.Menu.DialForm)
	{
		switch(key)
		{
			case KeyOne:
				keyDTMF=1;
				break;
			case KeyTwo:
				keyDTMF=2;
				break;
			case KeyThree:
				keyDTMF=3;
				break;
			case KeyFour:
				keyDTMF=4;
				break;
			case KeyFive:
				keyDTMF=5;
				break;
			case KeySix:
				keyDTMF=6;
				break;
			case KeySeven:
				keyDTMF=7;
				break;
			case KeyEight:
				keyDTMF=8;
				break;
			case KeyNine:
				keyDTMF=9;
				break;
			case KeyZero:
				keyDTMF=0;
				break;
		}
		if(keyDTMF<=10)
		{
			sprintf((char*)stringDTMF,"AT+CLDTMF=1,\"%.1d,\",100",keyDTMF);
			System.Device.Gsm.GsmATSend(stringDTMF,"OK",200);
			System.Device.Timer.Stop(LcdLedID);
			System.Device.Timer.Stop(LcdLedID);
			res=0;
		}
	}
	return res;		
}
/****************************************************************************
*
*描述		：挂机
*输入参数	：无
*输出参数	：无
*
****************************************************************************/
void CallHangUp(void)
{
	if(App.Menu.FocusFormPointer==&App.Menu.DialForm)
		System.Device.Gsm.GsmATSend("ATH","OK",10);	

}
/****************************************************************************


****************************************************************************/
void KeyPadCallProcess(Form *focusFormPointer)
{
	System.Device.Gsm.GsmATSend("AT+CHFA=0","OK",50);
	System.Device.Gsm.GsmATSend("AT+CMIC=0,15","OK",2);
	System.Device.Gsm.GsmATSend("AT+CLVL=100","OK",2);
	System.Device.Gsm.GsmCall((const char *)App.Data.KeyBuffer2);
  App.Data.GsmState = 0;
}
void CallInit(void)
{
	DialMenuInit();
	CallMenuInit();
}

