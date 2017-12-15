
#include "ScreensaverTime.h"
#include "App.h"

typedef enum
{
  Screen30Sec=1,
  Screen1Min,
  Screen2Min,
  Screen5Min,
  Screen10Min,
  ScreenNever,
}ScreenTime;
//#define Screen30Sec     1
//#define Screen1Min      2
//#define Screen2Min      3
//#define Screen5Min      4
//#define Screen10Min     5
//#define ScreenNever     6

#define SecBase         1000


/*****************************************************************************************
*
*描述         ： 定时返回主界面时间
*输入参数       ：无
*输出参数       ：无
*
*****************************************************************************************/
extern void WirelessOpen(void);
void FocusFormReturn(void)
{
  ArCheck *checkBoxPointer;
  Label   *labelPointer;
  TextBox *textBoxPointer;
  if(&App.Menu.DialForm!=App.Menu.FocusFormPointer)
	{
		while(CurrentMenu->Parent!=NULL)
		{
			if(App.Menu.FocusFormPointer->FocusTextBoxPointer!=NULL)
			{
				App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
				App.Menu.FocusFormPointer->FocusTextBoxPointer->KeyCount=0;

				App.Menu.FocusFormPointer->FocusTextBoxPointer=App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer;	
				App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer->KeyCount=0;
				App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;	
				//App.Menu.FocusFormPointer->ListPointer->Addr=&App.Menu.FocusFormPointer->ListPointer->Addr[0];
			}
			if(App.Menu.FocusFormPointer->ListPointer!=NULL)
			{
				App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
				App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
				App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
				App.Menu.FocusFormPointer->ListPointer->TopItem=0;
				App.Menu.FocusFormPointer->ListPointer->Addr=0;	
				
			}
			if(App.Menu.FocusFormPointer->FocusPointer!=NULL)
			{
				switch(App.Menu.FocusFormPointer->FocusPointer->BoxType)
				{
					case LabelBoxType:
						labelPointer=(Label*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
						labelPointer->BackColor=GUI_WHITE;
						labelPointer->TextMode=GUI_TEXTMODE_TRANS;
						break;
					case TextBoxType:
						textBoxPointer=(TextBox*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
						textBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
						break;
					case ArCheckBoxType:
						checkBoxPointer=(ArCheck*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
						checkBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
						checkBoxPointer->PaintColor=GUI_BLACK;
						checkBoxPointer->BackColor=GUI_WHITE;
						break;
				}
				App.Menu.FocusFormPointer->FocusPointer=App.Menu.AutoSetupForm.DefaultFocus;
				textBoxPointer=(TextBox*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
				textBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
				textBoxPointer->BackColor=GUI_BLUE;
				textBoxPointer->PaintColor=GUI_RED;
			}
			CurrentMenu->curSelected=0;//恢复当前菜单的默认选项
			CurrentMenu=CurrentMenu->Parent;//返回上级菜单
			App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
		}
		if(App.Menu.FocusFormPointer==&App.Menu.CallForm)
		{
			App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
		  App.Menu.FocusFormPointer->FocusTextBoxPointer->KeyCount=0;

			App.Menu.FocusFormPointer->FocusTextBoxPointer=App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer;	
			App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer->KeyCount=0;
			App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
			App.Menu.FocusFormPointer=&App.Menu.StandbyForm;
		}
		if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
    {
			WirelessOpen();
    }
	}
}

/*****************************************************************************************
*
* 描述		：待机页面进入时间
* 输入参数	：无
* 输出参数	：无
* 
*****************************************************************************************/
static void ScreenTimeSet(void)
{
    u8  i;
    u32 addr;
    u8  time=0;
    u32 delayTime=0;
    
    for(i=0;i<6;i++)
    {
        addr=(i*2)+Screen30SecAddr;
        System.Device.Storage.ReadData(addr,&time,1);
        if(1==time)
        {
            i+=1;
            break;
        }
    }
    switch(i)
    {
        case Screen30Sec:
                delayTime=SecBase*30;
            break;
        case Screen1Min:
                delayTime=SecBase*60;
            break;
        case Screen2Min:
                delayTime=SecBase*60*2;
            break;
        case Screen5Min:
                delayTime=SecBase*60*5;
            break;
        case Screen10Min:
                delayTime=SecBase*60*10;
            break;
        case ScreenNever:
        		delayTime=0;
            break;
    }
    System.Device.KeyLed.KeyLedConfig(1);
    if(delayTime)
    {
      System.Device.Timer.Start(KeyLedID,TimerSystickHandle,delayTime,System.Device.KeyLed.KeyCloseLed);
    }
    System.Device.Timer.Start(LcdLedID,TimerMessageHandle,120000,FocusFormReturn);         
}
void ScreenPointerInit(void)
{
	System.SystemApp.ScreenSet=ScreenTimeSet;
}


