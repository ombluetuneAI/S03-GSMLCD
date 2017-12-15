#include "TaskLogic.h"
#include "system.h"
#include "GUI.h"
#include "BSP.h"

#include "WirelessProcess.h"

#include "boot.h"
#include "ff.h"	

void GetChipInfo(void);
extern u8 GsmInfo(void);
extern void Test_Write(void);
extern void PhoneMember(void);
extern void Delete(void);
extern ListboxList * ListSearch(ListboxList *listx,u16 index);
extern u8 GsmCall(const char *PhoneNumber);
extern void MenuListInit(void);
extern void MessageProcess(u32 data);
extern void CallProcess(u32 data);
extern void WirelessProcess(u32 data);
extern void DtmfProcess(u32 data);
extern u8 GsmInfo(void);
extern void KeyArmProcess(void);
extern void UnlockProcess(void);
extern void CallRingProcess(u32 data);
extern void KeyToneSetup(void);
extern u8 DtmfToneGet(u8 key);
extern void CallHangUp(void);
extern void Simcom800CInit(void);
extern void SendWirelessData(void);
extern void AlarmCallCount(void);
extern void SingParse(void);
extern void OperatorParse(void);
extern void AuotoDisarmArmSet(void);
extern void CidDataProcess(u8 data);
extern void SMSCountProcess(void);
extern void SendThreeLines(u8  addr);
extern void LineDataToWTR050( u8 data);
extern void WT5880DSetVol(u8 vol);

uint8_t VolumeTab[8]={1,2,3,4,5,6,7,8};

#define	PolynomialsCRC16     0x8005

/*********************************************************************************************************************************
*
*描述：列表向上移动
*输入参数：无
*输出参数：无
*********************************************************************************************************************************/
static void ListItemsUP(void)
{
	if(NULL!=App.Menu.FocusFormPointer->ListPointer)
	{
	
		App.Menu.FocusFormPointer->ListPointer->LastTimeSelected=App.Menu.FocusFormPointer->ListPointer->selectedItem;
		App.Menu.FocusFormPointer->ListPointer->LastTimeIndexes=App.Menu.FocusFormPointer->ListPointer->SelectedIndexes;
		if(0==App.Menu.FocusFormPointer->ListPointer->SelectedIndexes)
		{			
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=App.Menu.FocusFormPointer->ListPointer->TotalItems;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=App.Menu.FocusFormPointer->ListPointer->ItemsPerpage;	
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=App.Menu.FocusFormPointer->ListPointer->SelectedIndexes\
			-(App.Menu.FocusFormPointer->ListPointer->ItemsPerpage);
			
		}
		App.Menu.FocusFormPointer->ListPointer->SelectedIndexes--;
		if(0!=App.Menu.FocusFormPointer->ListPointer->selectedItem)
		{
			App.Menu.FocusFormPointer->ListPointer->selectedItem--;
			if(App.Menu.FocusFormPointer->ListPointer->TotalItems<=App.Menu.FocusFormPointer->ListPointer->ItemsPerpage)
				App.Menu.FocusFormPointer->ListPointer->Redraw=NotRedraw;
			else
			{
				if(App.Menu.FocusFormPointer->ListPointer->selectedItem==App.Menu.FocusFormPointer->ListPointer->ItemsPerpage-1)
					App.Menu.FocusFormPointer->ListPointer->Redraw=Redraws;
				else
					App.Menu.FocusFormPointer->ListPointer->Redraw=NotRedraw;
			}
		}
		else
		{
			if(App.Menu.FocusFormPointer->ListPointer->TotalItems<=App.Menu.FocusFormPointer->ListPointer->ItemsPerpage)
			{
				App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
				App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;	
				App.Menu.FocusFormPointer->ListPointer->Redraw=NotRedraw;
			}
			else
			{
				App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
				App.Menu.FocusFormPointer->ListPointer->TopIndexes--;	
				App.Menu.FocusFormPointer->ListPointer->Redraw=Redraws;
			}
		}
		App.Menu.FocusFormPointer->ListPointer->TopItem=App.Menu.FocusFormPointer->ListPointer->TopIndexes;
	}
}

/*******************************************************************************
*
*描述：切换窗体页面
*输入参数：无
*输出参数：无
*
*******************************************************************************/
static void FormFocusProcessUP(void)
{ 
//  if(App.Menu.FocusFormPointer->FormID==IconMenuForm)
//  {
//    App.Menu.FocusFormPointer->curIconNo = App.Menu.FocusFormPointer->lastIconNo;
//    App.Menu.FocusFormPointer->lastIconNo--;
//  }
  if(App.Menu.FocusFormPointer->FormID==IconMenuForm)
  {
  if(CurrentMenu->curSelected==CurrentMenu->RangeEnd)
	{
    CurrentMenu->lastSelected = CurrentMenu->curSelected;
		CurrentMenu->curSelected=CurrentMenu->RangeFrom;
    CurrentMenu->icoUpdata = 1;
	}else
	{
    CurrentMenu->lastSelected=CurrentMenu->curSelected;
		CurrentMenu->curSelected++;
    CurrentMenu->icoUpdata = 1;
	}
}
  
	
	ListItemsUP();
	App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
	if((App.Menu.FocusFormPointer->FocusTextBoxPointer!=NULL)||(App.Menu.FocusFormPointer->FocusPointer!=NULL))
  {
		System.Gui.Form.SwitchTextBoxFocusUp();
  }
}
/*****************************************************************************************
*
*描述：设定时间
*输入参数：无
*输出参数：无
****************************************************************************************/
static void DateTimeSet(void)
{
	TimeSetStruct timeSet;
	char str[30];
    memset(str,0,30);
	if(App.Menu.FocusFormPointer==&App.Menu.DateForm)
	{
		timeSet.Sec=App.Data.Sec;
		timeSet.Min=App.Data.Min;
		timeSet.Hour=App.Data.Hour;	
		timeSet.Day=App.Data.Day;
		timeSet.Month=App.Data.Month;
		timeSet.Year=App.Data.Year;	
		System.Device.DateSet.RTCDateSet(&timeSet);
        //sprintf(str, "AT+CCLK=\"%4d/%02d/%02d,%02d:%02d:%02d+08\"",timeSet.Year,App.Data.Month,timeSet.Day,timeSet.Hour,timeSet.Min,timeSet.Sec);
        //System.Device.Gsm.GsmATSend((u8*)str,"OK",200);
        //System.Device.Gsm.GsmATSend("AT&W","OK",200);
	}
}
/**********************************************************************************************************************************
*
*描述：保存当前的复选择框状态
*输入参数：无
*输出参数：返回0则操作成功
*
***********************************************************************************************************************************/
static u8 CheckBoxListSaveProcess(void)
{
	u8 temp,res=0xFF;
	ListboxList *listx;
	if(App.Menu.FocusFormPointer->ListPointer!=null)
	{
        if(App.Menu.FocusFormPointer==&App.Menu.AutoForm) return res;
		if((App.Menu.FocusFormPointer->ListPointer->Type==CheckList)||(App.Menu.FocusFormPointer->ListPointer->Type==SwitchList))
		{
            if(App.Menu.FocusFormPointer->ListPointer->Change==1)
            {
                listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,0);
                if(listx==NULL) return res;
                res=0;
                while(listx!=NULL)
                {
                    temp=listx->State;	
                    if(App.Menu.FocusFormPointer!=&App.Menu.RestoreForm)
                        System.Device.Storage.Parameter.StorageWriteData(*(u32*)listx->IconAdd,(u16*)&temp,1);
                    else
                        App.Data.RestoreDefault[res++]=temp;
                    listx=listx->Nextlist;
                }				
                res=0xFE;
                if(CurrentMenu->ProcessSaveFun!=NULL)
                    CurrentMenu->ProcessSaveFun();
                if(App.Menu.FocusFormPointer==&App.Menu.RestoreForm) return res=0xFF;
                if(CurrentMenu->Parent!=NULL)
                {
                    if(App.Menu.FocusFormPointer->ListPointer!=NULL)
                    {
                        App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
                        App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
                        App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
                        App.Menu.FocusFormPointer->ListPointer->TopItem=0;
                        App.Menu.FocusFormPointer->ListPointer->Change=0;
                    }
                    CurrentMenu->lastSelected = 0;
                    CurrentMenu->curSelected=0;
                    CurrentMenu=CurrentMenu->Parent;
                    App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
                    res=0;
                }			
			}
		}									
	}
	return res;		
}
void ContactIDSaveData(void);
/**********************************************************************************************************************************
*
*描述：保存当前的编辑框内容
*输入参数：无
*输出参数：返回0则操作成功
*
***********************************************************************************************************************************/
static u8 TextBoxSaveProcess(void)
{
	u8  type,res=0xff;
	u32 addr;
	u16 s32;
	TextBox *textBoxPointer;
	if(App.Menu.FocusFormPointer->TextBoxPointer!=NULL)
	{	
		if((App.Menu.FocusFormPointer!=&App.Menu.DateForm)&&(App.Menu.FocusFormPointer!=&App.Menu.CallForm))
		{
			textBoxPointer=App.Menu.FocusFormPointer->TextBoxPointer;
			while(textBoxPointer!=(TextBox*)null)
			{
				type=textBoxPointer->Type;
				addr=textBoxPointer->DataAddr;	
				switch(type)
				{
					case GuiDataTypeByteDec:
						s32=*(u8*)textBoxPointer->DataPointer;
						System.Device.Storage.Parameter.StorageWriteData(addr,&s32,1);
						break;
					case GuiDataTypeUintDec:
						break;
					case GuiDataTypeString:
						s32=strlen(textBoxPointer->DataPointer);
						//if(0xFE!=(u8)textBoxPointer->Edit) return res==0xFE;;
						if(s32==textBoxPointer->MaxDataPointer)
						{
							System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)textBoxPointer->DataPointer,textBoxPointer->MaxDataPointer);
							ContactIDSaveData();
						}
						else if(textBoxPointer->RimType)
						{
							System.Device.Storage.Parameter.StorageWriteData(addr,(u16*)textBoxPointer->DataPointer,s32);
							System.Device.Storage.Parameter.StorageWriteData(addr+28,(u16*)0,1);
							textBoxPointer->DataAddr=0;
							memset(textBoxPointer->DataPointer,0,30);
						}
						else
						{	
							return res==0xFE;
						}
						break;
				}
				textBoxPointer=textBoxPointer->NextTextBoxPointer;					
			}
		}
		if(App.Menu.FocusFormPointer!=&App.Menu.CallForm)
		{
			if(App.Menu.FocusFormPointer->FocusTextBoxPointer!=NULL)
			{
				App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
				App.Menu.FocusFormPointer->FocusTextBoxPointer->KeyCount=0;
				
				App.Menu.FocusFormPointer->FocusTextBoxPointer=App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer;	
				App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer->KeyCount=0;
				App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;		
				res=0xFe;			
			}
			if(CurrentMenu->ProcessSaveFun!=NULL)
				CurrentMenu->ProcessSaveFun();		
			if(CurrentMenu->Parent!=NULL)
			{
				if(App.Menu.FocusFormPointer==&App.Menu.DateForm)
					DateTimeSet();
				if(App.Menu.FocusFormPointer->ListPointer!=NULL)
				{
					App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
					App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
					App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
					App.Menu.FocusFormPointer->ListPointer->TopItem=0;
				}
				CurrentMenu->curSelected=0;
				CurrentMenu=CurrentMenu->Parent;
				App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
				res=0;
			}
		}
	}
	return res;
}
/***************************************************************************
*
*描述：保存当前数据
*
***************************************************************************/
static u8 LongSaveProcess(void)
{
	u8 resTextBox=0xff;
	u8 resCheckBox=0xff;
	//if(App.Menu.FocusFormPointer==&App.Menu.SureRestForm) return res=0xFF;
	if(NULL==TextBoxSaveProcess())
		resTextBox=0;
	else resTextBox=TextBoxSaveProcess();

 	if(NULL==CheckBoxListSaveProcess()) 
		resCheckBox=0;
 	else resCheckBox=CheckBoxListSaveProcess();
		
	if((NULL==resTextBox)||(NULL==resCheckBox))
		return 0;
	else if((0xFE==resTextBox)||(0xFE==resCheckBox))
		return 0xFE;
		else
		return 0xFF;
}

extern void ContactIDReadData(void);
/*****************************************************************************************************************************
*
*描述：读取当前数据
*
*****************************************************************************************************************************/
static void ReadFormData(void)
{
	TextBox 	*textBoxPointer;
	ListboxList	*listx;	
	if((App.Menu.FocusFormPointer!=&App.Menu.DateForm)&&(App.Menu.FocusFormPointer!=&App.Menu.CallForm))
	{
		if(App.Menu.FocusFormPointer->TextBoxPointer!=NULL)
		{
			textBoxPointer=App.Menu.FocusFormPointer->TextBoxPointer;
			while(textBoxPointer!=NULL)
			{
				switch(textBoxPointer->Type)
				{
					case GuiDataTypeByteDec:
						System.Device.Storage.Parameter.StorageReadData(textBoxPointer->DataAddr,(u8*)textBoxPointer->DataPointer,1);	
						break;
					case GuiDataTypeString:
						memset(textBoxPointer->DataPointer,NULL,30);
						System.Device.Storage.Parameter.StorageReadData(textBoxPointer->DataAddr,textBoxPointer->DataPointer,textBoxPointer->MaxDataPointer);	
						break;
				}			
				textBoxPointer=textBoxPointer->NextTextBoxPointer;			
				
			}
		}	
	}
	else if(App.Menu.FocusFormPointer==&App.Menu.DateForm)
	{
		App.Data.Sec=Calendar.Sec;
		App.Data.Min=Calendar.Min;
		App.Data.Hour=Calendar.Hour;		
		App.Data.Day=Calendar.Date;
		App.Data.Month=Calendar.Month;
		App.Data.Year=Calendar.Year;		
	}	
	if(App.Menu.FocusFormPointer->ListPointer!=NULL)
		if((App.Menu.FocusFormPointer->ListPointer->Type==CheckList)||(App.Menu.FocusFormPointer->ListPointer->Type==SwitchList))
		{
			listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,0);//
			while(listx!=NULL)
			{
				if(listx!=NULL)
				{
					System.Device.Storage.Parameter.StorageReadData(*(u32*)listx->IconAdd,(u8*)&listx->State,1);
					if(listx->State==SystemSirenOn)
						listx->State=1;
					else if(listx->State==SystemSirenOff)
						listx->State=0;
				}
				listx=listx->Nextlist;
			}	
		}		
	ContactIDReadData();	
}
/*******************************************************************************
* 描述 : Enter按钮
*******************************************************************************/
static void FormProcessEnter(void)
{	
	u32 	*nameAddr=null;
	u32 	*codeAddr=null;
	u32 	*nullAddr=null;
	u8  	selectedIndexes;
	u32	 	addr;
	Label 	*labelPointer;	
    
    ArCheck *checkBoxPointer;
    Label   *labelFocusPointer;
    TextBox *textBoxPointer;	
	ListboxList	*listx;
	if((NULL==LongSaveProcess())||(0xFE==LongSaveProcess())) return;
//  if(App.Menu.FocusFormPointer==&App.Menu.DialForm)
//  {
//    if(App.Menu.FocusFormPointer->TextBoxPointer->DataPointer==NULL)
//    {
//      return;
//    }
//  }
	if(App.Menu.FocusFormPointer->ListPointer!=0)
	{
		addr=App.Menu.FocusFormPointer->ListPointer->Addr;
//		if(App.Menu.FocusFormPointer->ListPointer->Enable==0xff)
			//return;
		if(CurrentMenu->SubMenus[App.Menu.FocusFormPointer->ListPointer->SelectedIndexes]!=NULL)
		{
			listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes);
			nameAddr=listx->IconAdd;
			codeAddr=listx->FlashAddr;
			nullAddr=listx->NullPointer;
			selectedIndexes=App.Menu.FocusFormPointer->ListPointer->SelectedIndexes;
 			if(null!=CurrentMenu->ProcessFunc)
 				CurrentMenu->ProcessFunc();
			CurrentMenu=CurrentMenu->SubMenus[selectedIndexes];

			App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
			
			labelPointer = App.Menu.FocusFormPointer->LabelPointer;	
			while(labelPointer)
			{
				if(labelPointer->Type==0xFE)
				{
					labelPointer->Addr=*nameAddr;
					break;
				}else if(labelPointer->Type==0xFC)
				{
					labelPointer->Addr=*codeAddr;
					break;
				}	
				labelPointer=labelPointer->NextLabelPointer;
			}
			if(null!=App.Menu.FocusFormPointer->ListPointer)
			{
				if(null!=nullAddr)
					App.Menu.FocusFormPointer->ListPointer->Addr=*nullAddr;
			}
			if(App.Menu.AddPhoneForm.TextBoxPointer==App.Menu.FocusFormPointer->TextBoxPointer)
			{
				App.Menu.FocusFormPointer->TextBoxPointer->DataAddr=0;
				if(0==App.Menu.FocusFormPointer->TextBoxPointer->DataAddr)
					App.Menu.FocusFormPointer->TextBoxPointer->DataAddr=addr;
			}
			if(CurrentMenu->ProcessFunction!=NULL)
				CurrentMenu->ProcessFunction();
		}	
	}
	else if((CurrentMenu->SubMenus[CurrentMenu->curSelected]!=NULL)&&(App.Menu.FocusFormPointer!=&App.Menu.StandbyForm))
	{		
		CurrentMenu=CurrentMenu->SubMenus[CurrentMenu->curSelected];
		App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
	}
    if(App.Menu.FocusFormPointer->FocusPointer!=NULL)
    {
        switch(App.Menu.FocusFormPointer->FocusPointer->BoxType)
        {
            case LabelBoxType:
                labelFocusPointer=(Label*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                labelFocusPointer->BackColor=GUI_WHITE;
                labelFocusPointer->TextMode=GUI_TEXTMODE_TRANS;
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
        App.Menu.FocusFormPointer->FocusPointer=App.Menu.FocusFormPointer->DefaultFocus;
        textBoxPointer=(TextBox*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
        textBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
        textBoxPointer->BackColor=GUI_BLUE;
        textBoxPointer->PaintColor=GUI_RED;
    }
	if(CurrentMenu->FormAddrfunc!=NULL)
		CurrentMenu->FormAddrfunc(App.Menu.FocusFormPointer);
	ReadFormData();
}
/******************************************************************************************************************************

*描述：列表向下翻滚

*******************************************************************************************************************************/
static void ListBoxDown(void)
{
	if(App.Menu.FocusFormPointer->ListPointer!=0)
	{
		App.Menu.FocusFormPointer->ListPointer->LastTimeIndexes=App.Menu.FocusFormPointer->ListPointer->SelectedIndexes;
		App.Menu.FocusFormPointer->ListPointer->SelectedIndexes++;
		App.Menu.FocusFormPointer->ListPointer->LastTimeSelected=App.Menu.FocusFormPointer->ListPointer->selectedItem;
		App.Menu.FocusFormPointer->ListPointer->selectedItem++;
		if(App.Menu.FocusFormPointer->ListPointer->selectedItem>App.Menu.FocusFormPointer->ListPointer->ItemsPerpage-1)
		{
			if(App.Menu.FocusFormPointer->ListPointer->TotalItems<=App.Menu.FocusFormPointer->ListPointer->ItemsPerpage)
			{
				App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
				App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;	
				App.Menu.FocusFormPointer->ListPointer->Redraw=NotRedraw;
			}
			else
			{
				App.Menu.FocusFormPointer->ListPointer->selectedItem=App.Menu.FocusFormPointer->ListPointer->ItemsPerpage-1;
				App.Menu.FocusFormPointer->ListPointer->TopIndexes++;	
				App.Menu.FocusFormPointer->ListPointer->Redraw=Redraws;
			}
		}
		else
		{
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=App.Menu.FocusFormPointer->ListPointer->TopItem;
			App.Menu.FocusFormPointer->ListPointer->Redraw=NotRedraw;
		}
		if(App.Menu.FocusFormPointer->ListPointer->SelectedIndexes==App.Menu.FocusFormPointer->ListPointer->TotalItems)
		{
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
		}
		App.Menu.FocusFormPointer->ListPointer->TopItem=App.Menu.FocusFormPointer->ListPointer->TopIndexes;			
	}
}
/*******************************************************************************
*描述 : 页面切换向下
*******************************************************************************/
static void FormFocusProcessDown(void)
{ 
//  if(App.Menu.FocusFormPointer->FormID==IconMenuForm)
//  {
//    App.Menu.FocusFormPointer->curIconNo = App.Menu.FocusFormPointer->lastIconNo;
//    App.Menu.FocusFormPointer->lastIconNo++;
//  }
  if(App.Menu.FocusFormPointer->FormID==IconMenuForm)
  {
	if(CurrentMenu->curSelected==CurrentMenu->RangeFrom)
	{
    CurrentMenu->lastSelected = CurrentMenu->curSelected;
		CurrentMenu->curSelected=CurrentMenu->RangeEnd;
		CurrentMenu->icoUpdata = 1;
	}else
	{
    CurrentMenu->lastSelected = CurrentMenu->curSelected;
		CurrentMenu->curSelected--;
    CurrentMenu->icoUpdata = 1;
	}
  }
	ListBoxDown();
	App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
	if(App.Menu.FocusFormPointer->FocusTextBoxPointer!=NULL)
		System.Gui.Form.SwitchTextBoxFocusDown();	
}
/**************************************************************************************************

*描述：删除 按钮

**************************************************************************************************/
static void DeleteKey(void)
{
	u8 	type;
	u16 s32,s32Min;
	if(App.Menu.FocusFormPointer->FocusTextBoxPointer!=NULL)
	{
    if((App.Menu.FocusFormPointer==&App.Menu.AutoSetupForm)||(App.Menu.FocusFormPointer==&App.Menu.ContactIDForm))
    {
       if(App.Menu.FocusFormPointer->FocusPointer->BoxType!=TextBoxType) 
       {
         return;
       }
    }
		type=App.Menu.FocusFormPointer->FocusTextBoxPointer->Type;
		switch(type)
		{
			case GuiDataTypeByteDec:
				s32=*(u8*)(App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer);
				s32Min=App.Menu.FocusFormPointer->FocusTextBoxPointer->MinDataPointer;			
				s32/=10;	
				if(s32<=s32Min) s32=s32Min;
				*(u8*)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer=s32;				
				break;
			case GuiDataTypeUintDec:
				s32=*(u16*)(App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer);
				s32/=10;		
				*(u16*)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer=s32;		
			case GuiDataTypeString:
				s32=App.Menu.FocusFormPointer->FocusTextBoxPointer->KeyCount;
				if(NULL==s32)
				{
					memset((u8*)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer,NULL,30);
					//*(u8*)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer='|';
					App.Menu.FocusFormPointer->FocusTextBoxPointer->ID=~App.Menu.FocusFormPointer->FocusTextBoxPointer->ID;
					break;
				}else 
					s32--;
				if(NULL==s32)
				{
					memset((u8*)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer,NULL,30);
					//*(u8*)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer='|';
				}else
        {
					*((u8*)App.Menu.FocusFormPointer->FocusTextBoxPointer->DataPointer+s32)=NULL;
        }
				App.Menu.FocusFormPointer->FocusTextBoxPointer->ID=~App.Menu.FocusFormPointer->FocusTextBoxPointer->ID;
				break;
		}	
		if(App.Menu.FocusFormPointer->FocusTextBoxPointer->KeyCount!=0)
    {
			App.Menu.FocusFormPointer->FocusTextBoxPointer->KeyCount--;	
    }
	}
}
/*******************************************************************************
* 描述: 返回上一个页面
*******************************************************************************/
static void FormFocusProcessReturn(void)
{ 
    ArCheck *checkBoxPointer;
    Label   *labelPointer;
    TextBox *textBoxPointer;
    AutoDigit    *autoDigit;
	if(App.Menu.FocusFormPointer->FocusTextBoxPointer!=NULL)
	{
		App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
		App.Menu.FocusFormPointer->FocusTextBoxPointer->KeyCount=0;
		
		App.Menu.FocusFormPointer->FocusTextBoxPointer=App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer;	
		App.Menu.FocusFormPointer->DefaultFocusTextBoxPointer->KeyCount=0;
		App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;	
		//App.Menu.FocusFormPointer->ListPointer->Addr=&App.Menu.FocusFormPointer->ListPointer->Addr[0];
	}
	if(App.Menu.FocusFormPointer->AutoDigitPointer!=null)
	{
		autoDigit=App.Menu.FocusFormPointer->AutoDigitPointer;
		autoDigit->Value=0;
		while(autoDigit->NextPointer!=null)
		{
			autoDigit=autoDigit->NextPointer;
			autoDigit->Value=0;
		}
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
	if(App.Menu.FocusFormPointer==&App.Menu.VoiceListForm)
	{
    if(App.Data.RecordKey)
    {
      System.Device.Gsm.GsmATSend("AT+CREC=2","OK",10);//停止录音
    }
//    System.Device.Gsm.GsmATSend("AT+CREC=4,1,1,100","OK",10);
//		LineDataToWTR050(0xFE);
//		MicPowerCtrl=1;
		App.Data.RecordKey=0;
	}
	if(App.Menu.FocusFormPointer==&App.Menu.VoiceListForm)
	{
    if(App.Data.RecordKey)
    {
      System.Device.Gsm.GsmATSend("AT+CREC=5","OK",10);//停止播放
    }
//		LineDataToWTR050(0xFE);
		App.Data.RecordKey=0;
	}
	if(CurrentMenu->Parent!=NULL)
	{
		if(App.Menu.FocusFormPointer->ListPointer!=NULL)
		{
			App.Menu.FocusFormPointer->ListPointer->SelectedIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->selectedItem=0;
			App.Menu.FocusFormPointer->ListPointer->TopIndexes=0;
			App.Menu.FocusFormPointer->ListPointer->TopItem=0;
			App.Menu.FocusFormPointer->ListPointer->Addr=0;	
			
		}
        if(App.Menu.FocusFormPointer==&App.Menu.VoiceListForm)  
        {
            App.Data.RecTimeValue=0xFF;    
            App.Data.RecordKey=0;
            App.Data.PlayKey = 0;
//            App.Menu.FocusFormPointer->LabelPointer->Count=0;
//            App.Menu.FocusFormPointer->LabelPointer->Update=1;
            App.Menu.FocusFormPointer->AutoDigitPointer->Value=0;            
        }
//        else if(App.Menu.FocusFormPointer==&App.Menu.PlayForm)
//        {
//            App.Data.RecTimeValue=0xFF;
//            App.Data.RecordKey=0;
//            App.Menu.FocusFormPointer->LabelPointer->Update=1;
//            App.Menu.FocusFormPointer->LabelPointer->Count=3;
//            App.Menu.FocusFormPointer->AutoDigitPointer->Value=9;
//        }        
		CurrentMenu->curSelected=0;
		CurrentMenu=CurrentMenu->Parent;
		App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
	}
	else if(App.Menu.FocusFormPointer!=&App.Menu.AlarmForm)	
		App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
	if(NULL!=CurrentMenu->ProcessReturn)
		CurrentMenu->ProcessReturn();
	if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
		App.Menu.FocusFormPointer->FillAreaPointer->ID=0xFF;
}
/**********************************************************************************************************************************************************
*
*鎻忚堪		锛氬閫夋嫨妗嗙殑澶勭悊鍑芥暟
*
**********************************************************************************************************************************************************/
static void ProcesscheckBox(void)
{
	ListboxList *listx;
	ListboxList *focusListx;
	
	if(App.Menu.FocusFormPointer->ListPointer!=NULL)
	if((App.Menu.FocusFormPointer->ListPointer->Type==CheckList)||(App.Menu.FocusFormPointer->ListPointer->Type==SwitchList))
	{
		if(App.Menu.FocusFormPointer->ListPointer->Select==1)//该值为1时表示复选择框列表只有一个被选择中
		{
			focusListx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes);//查找当前需要的索引
			listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,0);	
			if((listx->Nextlist==NULL)&&(listx->Prevlist==NULL)) return;
			
			while(listx!=NULL)
			{
				if(listx!=focusListx)
					listx->State=0;
				listx=listx->Nextlist;								
			}
			listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes);
			listx->State=!listx->State;
			App.Menu.FocusFormPointer->ListPointer->Redraw=Redraws;
			App.Menu.FocusFormPointer->ListPointer->Change=1;
		}else
		{
			listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes);
			listx->State=!listx->State;
			App.Menu.FocusFormPointer->ListPointer->Redraw=NotRedraw;
			App.Menu.FocusFormPointer->ListPointer->Change=1;
			if(App.Menu.FocusFormPointer==&App.Menu.PhoneSetForm)
			{
				if(App.Menu.FocusFormPointer->ListPointer->SelectedIndexes==2)
				{
					listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes-2);
					listx->State=0;
					listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes-1);
					listx->State=0;
					App.Menu.FocusFormPointer->ListPointer->Redraw=Redraws;
				}
				else if(App.Menu.FocusFormPointer->ListPointer->SelectedIndexes==1)
				{
					listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes+1);
					listx->State=0;
					App.Menu.FocusFormPointer->ListPointer->Redraw=Redraws;
				}
				else if(App.Menu.FocusFormPointer->ListPointer->SelectedIndexes==0)
				{
					listx=ListSearch(App.Menu.FocusFormPointer->ListPointer->List,App.Menu.FocusFormPointer->ListPointer->SelectedIndexes+2);
					listx->State=0;
					App.Menu.FocusFormPointer->ListPointer->Redraw=Redraws;
				}
			}	
            if((App.Menu.FocusFormPointer==&App.Menu.AutoForm)&&(App.Menu.FocusFormPointer==&App.Menu.AutoForm))
            {
                System.Device.Storage.Parameter.StorageWriteData(*(u32*)listx->IconAdd,(u16*)&listx->State,1);
            }
		}			
	}
}
extern void ContactIDSeting(void);
/********************************************************************************************************************
*
*描述：进入菜单
*
*********************************************************************************************************************/
static void FormFocusProcessMenu(void)
{
	if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
	{	
    App.Data.SystemModeUpdate=0xFF;
		System.Device.Timer.Stop(RestTimeID);	
		App.Data.UseWireless=0;	
		TIM_Cmd(TIM7, DISABLE);
		System.Device.Timer.Stop(RFOpenTimeID);
		CurrentMenu=CurrentMenu->SubMenus[CurrentMenu->curSelected];
		App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
	}
	DeleteKey();	
	ProcesscheckBox();
  AuotoDisarmArmSet();
	ContactIDSeting();
}





extern u8 UserUpdataVal;
/**************************************************************************************************

**************************************************************************************************/
void RecordStart(void)
{
    Icon *IconPointer;
    
  if(App.Menu.FocusFormPointer == &App.Menu.VoiceListForm)
  {
    IconPointer = App.Menu.FocusFormPointer->IconPointer;
    if(!App.Data.RecordKey)
    {
      UserUpdataVal = 1;
      
      System.Device.Gsm.GsmATSend("AT+CREC=5","OK",10);//停止放音
      App.Data.PlayKey = 0;
      
      System.Device.Gsm.GsmATSend("AT+CMIC=12","OK",10);//设置MIC增益
      System.Device.Gsm.GsmATSend("AT+CREC=1,1,0,100","OK",10);//开始录音
//			LineDataToWTR050(0xF5);
      App.Menu.FocusFormPointer->AutoDigitPointer->Value = 0;
      App.Data.RecordKey = 1;
      App.Data.RecTimeValue=0;
    }
    else
    {
      UserUpdataVal = 2;
      
      App.Data.RecordKey = 0;
      System.Device.Gsm.GsmATSend("AT+CREC=2","OK",10);//停止录音
      App.Menu.FocusFormPointer->AutoDigitPointer->Value = 0;
      App.Data.RecordKey=0;//停止录音
      App.Data.RecTimeValue=0xFF;
    }
  }
}


void PlayStart(void)
{
  Icon *IconPointer;
    
  if(App.Menu.FocusFormPointer == &App.Menu.VoiceListForm)
  {
    IconPointer = App.Menu.FocusFormPointer->IconPointer;
    if(!App.Data.PlayKey)
    {
      UserUpdataVal = 3;
      
      System.Device.Gsm.GsmATSend("AT+CREC=2","OK",10);//停止录音
      App.Data.RecordKey = 0;//释放录音进程
      
//      LineDataToWTR050(0xF6);
      System.Device.Gsm.GsmATSend("AT+CREC=4,1,0,100","OK",10);//开始放音  
      App.Menu.FocusFormPointer->AutoDigitPointer->Value = 9;
      App.Data.PlayKey = 1;
      App.Data.RecTimeValue=9;  
    }
    else
    {
      UserUpdataVal = 4;
      
      App.Data.PlayKey = 0;
      System.Device.Gsm.GsmATSend("AT+CREC=5","OK",10);//停止放音
      App.Menu.FocusFormPointer->AutoDigitPointer->Value = 9;
      App.Data.RecTimeValue=0xFF;
    }
  }
}




void VoiceSendByte(void);
void VoiceVol(void);
extern void RemoteAlawyAndStayProcess(u8 keyValue);

/******************************************************************************
*
*鎻忚堪		锛氭寜閸靛０闊?
*杈撳叆鍙傛暟	锛歵emp,閿�艰緭鍏?
*杈撳嚭鍙傛暟	锛氭棤
*
*******************************************************************************/
extern void M62429WriteByte(u8 data,u8 sum);
void KeyTone(u8 temp)
{
	u8 volume;
	
	if(App.Data.KeyToneState)
	{
//		OpenSpeaker();
		System.Device.Storage.Parameter.StorageReadData(VoiceVolumeAddr,&volume,1);
    WT5880DSetVol(volume);
//		volume=(8-volume)*10;
//		M62429WriteByte(volume,0);
		if((temp!=KeyRing)&&(temp!=KeyHome)&&(temp!=KeyDisarm)&&(temp!=KeyARM))
    {
//      System.Device.Gsm.GsmATSend("AT+CMEDPLAY=1,C:\\User\\93.amr,1,100","OK",10);
			SendThreeLines(30) ;
    }
	}
}
void KeyVolumeSetup(void)
{
	u8		volume;
	static	u8 temp=0;
	
	System.Device.Storage.Parameter.StorageReadData(VoiceVolumeAddr,&volume,1);
	if(!temp)
	{		
		if(volume==8)
			temp=1;
		else 
			volume++;
	}
	else if(temp)
	{
		if(volume==0)
			temp=0;
		else 
			volume--;
	}
	System.Device.Storage.Parameter.StorageWriteData(VoiceVolumeAddr,(u16*)&volume,1);
	System.Device.Gsm.GsmATSend("AT+CCMXPLAYWAV=\"DEFEAT.WAV\",2","OK",10);	
}
void PanelAlarm(void)
{
	App.Data.MessageTypeTotal=GetPhoneTypeTotal(MessageType);
  App.Data.CallTypeTotal=GetPhoneTypeTotal(CallType);
	App.Data.CallData.CallCidTotal=GetPhoneTypeTotal(CIDType);
  RTT_printf(0,"MessageTypeTotal =%d\r\nCallTypeTotal = %d\r\nCallCidTotal = %d\r\n",App.Data.MessageTypeTotal/
                                                                                     App.Data.CallTypeTotal/
                                                                                     App.Data.CallData.CallCidTotal);
	memset(App.Data.MessageString,0x00,30); 
	sprintf((char*)App.Data.MessageString,"Panel Alarm"); 
	if(App.Data.MessageTypeTotal)
		App.Data.SendMessage=1;
	else if(App.Data.CallData.CallCidTotal)
		App.Data.CallData.CallCidMessage=1;
	else if(App.Data.CallTypeTotal)
	{
		App.Data.CallMessage=1;
		App.Data.OriginateCalltimes=1;	
	}
	App.Data.SendCount=0;
	App.Data.OriginateCount=0;
}
/******************************************************************************************************************

*描述：按键处理

******************************************************************************************************************/

void KeyProcess(uint32_t data)
{
	u16 	temp;
	static u8 voiceTemp=0;
	temp=Byte1(data);
	temp<<=8;
	switch(temp)
	{
		case KeyReturnDown:
			{				
				temp=Byte0(data);
//				SendThreeLines(0xE7) ;
//				if(voiceTemp==36)voiceTemp=0;
//				SendThreeLines(voiceTemp) ;
//				voiceTemp++;
        if(LcdLED==1)
        {
					System.SystemApp.ScreenSet();
          return;
        }
        else
        {
					System.SystemApp.ScreenSet();
        }
				if(App.Menu.FocusFormPointer->FocusTextBoxPointer!=NULL)
        {
					System.Gui.Form.ModifyTextBoxData(temp);
        }
				if(DtmfToneGet(temp))
        {
					KeyTone(temp);
        }
				switch(temp)
				{
					case  KeyMenu:
						FormFocusProcessMenu();
						break;
					case  KeyHome : 
						if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
						{
							temp=SystemStayMode;
							if(App.Data.DelayArmFlag)
							{
								System.Device.Timer.Stop(ArmTimeID);
								App.Data.DelayArmFlag=DISABLE;
							}
							RemoteAlawyAndStayProcess(KeyHome1527);
						}
						break;
//					case KeyCall:
//						if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
//						{
//							App.Data.UseWireless=0;
//							System.Device.Timer.Stop(RFOpenTimeID);
//							memset(App.Data.KeyBuffer2,0,30);
//							sprintf((char*)App.Data.KeyBuffer2,"Empty");
//							CurrentMenu=CurrentCall;
//							App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
//						}
//						 break;
//					case KeyRing:
//							KeyVolumeSetup();	
//						break;
				
					case KeyDisarm:
						if((App.Menu.FocusFormPointer==&App.Menu.StandbyForm)||(App.Menu.FocusFormPointer==&App.Menu.AlarmForm))
						{
							UnlockProcess();
							temp=SystemDisarmMode;						
							System.Device.Storage.Parameter.StorageWriteData(SystemModeAddr,(u16*)&temp,1);  
							LogWrite(LogDisarmAddr,LogDisarmTotalAddr,1,LogDisarm,0xFF);
						}							
						break;
					case KeyARM:	
						if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
						{
							App.Data.ArmKeypadRemote=1;
							//KeyArmProcess();
							if(&App.Menu.StandbyForm==App.Menu.FocusFormPointer)
								RemoteAlawyAndStayProcess(KeyAlawy1527);
						}
						break;		
//					case KeySOS:							
//						break;						
					case  KeyReturn:
						CallHangUp();
						FormFocusProcessReturn();						 
						break;	
 					case  KeyEnter:
						FormProcessEnter();
						break;					
 					case  KeyUP:
						  if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
						  {
							App.Data.ArmKeypadRemote=1;
							//KeyArmProcess();
							if(&App.Menu.StandbyForm==App.Menu.FocusFormPointer)
								RemoteAlawyAndStayProcess(KeyAlawy1527);					
						  }
						  else
							FormFocusProcessUP();  
						break;
 					case  KeyDown:
							if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
							{
								temp=SystemStayMode;
								if(App.Data.DelayArmFlag)
								{
									System.Device.Timer.Stop(ArmTimeID);
									App.Data.DelayArmFlag=DISABLE;
								}
								RemoteAlawyAndStayProcess(KeyHome1527);
							}
						  else
							FormFocusProcessDown();
 						  break;	
           case KeyOne:
              RecordStart();
              break;	
           case KeyTwo:
             PlayStart();
             break;
				}
			}break;
			
		case KeyReturnLong://长按
			 temp=Byte0(data);	
			 switch(temp)
			 {
				 case KeyEnter:
					if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
					{
						System.Device.Timer.Start(CallTimer,TimerSystickHandle,3000,PanelAlarm);
						LogWrite(LogAlarmAddr,LogAlarmTotalAddr,1,LogKeypad,0xFF);
						App.Data.SystemMode=SystemAlarmMode;
						App.Menu.FocusFormPointer=&App.Menu.AlarmForm;
						System.Device.Timer.Start(SendTxTimeID,TimerMessageHandle,5000,SendWirelessData);
						SirenAlarm();	
            RTT_printf(0,"PanelAlarm\r\n");
					}						 
					 break;
			 }
			break;
         default:
           break;
	}
}
unsigned short calcByte(int crc, char b)
{
    int i;
    crc = crc ^ (int)b << 8;
  
  
    for ( i = 0; i < 8; i++)
    {
        if ((crc & 0x8000) == 0x8000)
        {
            crc = crc << 1 ^ PolynomialsCRC16;
        }
        else
        {
            crc = crc << 1;
        }
    }
  
    return crc & 0xffff;
}
  
/************************************************************************
* 描述: CRC16
* 输入参数： 
*        pBuffer  待计算数据
*        length   待计算数据长度
*输出参数： 
         wCRC16 计算后的结果
************************************************************************/
u16 CRC16(u8 *pBuffer, u16 length)
{
    unsigned short wCRC16=0x0; 
	u16	i;
    if ((NULL == pBuffer)||(0 == length))
    {
        return 0;
    }
    for (i = 0; i<length; i++)
    {
        wCRC16 = calcByte(wCRC16, pBuffer[i]);
    }
    return wCRC16;
}

/*************************************************************************************************************************

*描述：读芯片ID Flash信息

*************************************************************************************************************************/
void GetChipInfo(void)
{
	uint32_t ChipUniqueID[3];
	u16 res=0;
	u8	temp;
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7E8); 
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7F0); 
	App.Data.IdentifyNumber0=ChipUniqueID[0];
	
	temp=Byte0(App.Data.IdentifyNumber0);
	res=calcByte(res, temp);
	
	temp=Byte1(App.Data.IdentifyNumber0);
	res=calcByte(res, temp);
	
	temp=Byte2(App.Data.IdentifyNumber0);
	res=calcByte(res, temp);
	
	temp=Byte3(App.Data.IdentifyNumber0);
	res=calcByte(res, temp);
	App.Data.SystemID=res;
	
//	Capacity= *(u16*)(0x1FFFF7E0);    //  
//	printf("ID号   	:	 %X-%X-%X \r\n", ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);  
// 	printf("Flash大小	:	 %dK \r\n", Capacity);
// 	printf("日期:	 "__DATE__" - "__TIME__"\r\n");
// 	printf("库版本： V %d.%d.%d \r\n",__STM32F10X_STDPERIPH_VERSION_MAIN,__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);  
}

/******************************************************************************************************

*描述：处理GSM状态
*输入参数：data,来自系统的信息
*输出参数：无

******************************************************************************************************/

void GsmStateProcess(u32 data)
{
	u16 messageEnum;
	u8 *strPointer=NULL;
	u8 i;
	u8 temp=0xFF;
	char str1[6],str2[6];
	messageEnum=Byte1(data);
	messageEnum<<=8;
	switch(messageEnum)
	{
		case GsmCSQMessage :
			SingParse();
			App.Data.GsmState=0;
			break;
		case GsmCallMessage:
			App.Data.GsmState=0;
			break;
		case GsmDTMFMessage:
			App.Data.GsmState=0;
			break;
		case GsmSMSMessage:
			System.Device.Gsm.GsmATSend("AT+CMGD=1,4","OK",300);		
			App.Data.GsmState=0;
			break;
		case GsmSMSContent:
			strPointer=(u8*)strstr((const char*)App.Data.GsmSMSInfo,"\r\n");
			strPointer+=2;
			for(i=0;i<6;i++)
			{
				str1[i]=*strPointer;
				strPointer++;
			}
			System.Device.Storage.Parameter.StorageReadData(AdminPasswordAddr,(u8*)str2,6);
			for(i=0;i<6;i++)
			{
				if(str1[i]!=str2[i])
				{
					temp=1;
					break;
				}
				temp=0;
			}
			if(temp==0)
			{
				strPointer=(u8*)strstr((const char*)App.Data.GsmSMSInfo,"REC UNREAD");
				temp=strlen("REC UNREAD")+2;
				strPointer+=temp;
				System.SystemApp.SystemSet(strPointer);
			}
			App.Data.GsmState=0;
			break;
		case GsmRingMessage:
      System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);
      System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=1","OK",10);
			App.Data.GsmState=0;
      App.Data.CallRing=1; 
			break;
		case GsmDateMessage:
			App.Data.GsmState=0;
			break;
		case GsmReadyMessage:
//      GsmATSend("AT+COPS?","OK",100);         //选择运营商 
//      GsmATSend("AT+CSQ?","OK",100);
      Simcom800CInit();
			App.Data.GsmState=0;
			break;
		case GsmSMSRead:
			strPointer=(u8*)strstr((const char*)App.Data.GsmSMSInfo,"\r\n");
			strPointer=strPointer+2;
			for(i=0;i<6;i++)
			{
				str1[i]=*strPointer;
				strPointer++;
			}
			System.Device.Storage.Parameter.StorageReadData(AdminPasswordAddr,(u8*)str2,6);
			for(i=0;i<6;i++)
			{
				if(str1[i]!=str2[i])
				{
					temp=1;	
					break;
				}
				temp=0;
			}
			if(temp==0)
			{
				System.SystemApp.SystemSet(App.Data.GsmSMSInfo);
			}
			App.Data.GsmState=0;
			break;
		case GsmTSMSINFOMessage:
			System.Device.Gsm.GsmATSend("AT+CMGD=1,4","OK",300);
			App.Data.GsmState=0;
			break;
		case GsmNotMessage:
			App.Data.GsmState=0;
			break;
		case GsmSMSSendOK:
			SMSCountProcess();
			App.Data.GsmState=0;
			break;
    case GsmMessageFull:
      System.Device.Gsm.GsmATSend("AT+CMGD=1,4","OK",500);
      break;
    case GsmMessageCEND://结束通话
			System.Device.Gsm.GsmATSend("ATH","OK",50);
//			System.Device.Gsm.GsmATSend("AT+CHFA=1","OK",50);
			if(!App.Data.CidState.CidEnd)
			{
				if(AlarmTimes>App.Data.OriginateCalltimes)
				{
					if(App.Data.CallData.CallCidTotal==App.Data.OriginateCount)
					{
						App.Data.OriginateCount=0;
						App.Data.OriginateCalltimes++;					
					}
					App.Data.CallData.CallCidMessage=1;
				}
				else
				{
					if(App.Data.CallData.CallCidTotal>App.Data.OriginateCount)
						App.Data.CallData.CallCidMessage=1;
					else
					{
						App.Data.OriginateCalltimes=0;
						App.Data.OriginateCount=0;
						if(App.Data.SystemMode==SystemAlarmMode)
						{
							App.Data.CallMessage=1;
							App.Data.OriginateCalltimes=1;
						}
					}
				}
			}
			else if(App.Data.SystemMode==SystemAlarmMode)
			{
				App.Data.CallMessage=1;
				App.Data.OriginateCalltimes=1;
			}
			App.Data.CallRing=0;
      App.Data.GsmState=0;
      break;
    case GsmMessageCONN://接通电话
      RTT_printf(0,"GsmStateProcess:GsmMessageCONN\r\n");
      if(App.Data.SystemMode==SystemAlarmMode)
			{
				System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=1","OK",10);//关闭MIC
        System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);//关闭喇叭
        System.Device.Gsm.GsmATSend("AT+DTAM=1","OK",10);//远程播放语音模式
        System.Device.Gsm.GsmATSend("AT+CEXTERNTONE=1","OK",10);
        System.Device.Gsm.GsmATSend("AT+CLVL=0","OK",10);
        System.Device.Gsm.GsmATSend("AT+DTAM=1","OK",10);
        System.Device.Gsm.GsmATSend("AT+CREC=4,1,0,100","OK",10);
        
			}
      
      else//这里要加上拨打电话的状态
      {
        App.Data.CallRing=1;
        //播放语音，输入密码
      }
			 App.Data.GsmState=0;
       break;
    case GsmMessageOperator:
//			OperatorParse();
			App.Data.GsmState=0;
      break;
		case GsmBusy:
			 AlarmCallCount();
			 App.Data.GsmState=0;
			break;
		case GsmSimCardQuery:
      if(System.Device.Gsm.GsmATSend("AT+CPIN?","ERROR",10))
      {
        App.Data.GsmSimCardState=0;
      }
//			 if(0==System.Device.Gsm.GsmATSend("AT+CPIN?","OK",100))
//       {
//				 App.Data.GsmSimCardState=0;
//       }
			 App.Data.GsmState=0;
			break;
		case GsmOperatorQuery:
			 App.Data.OperatorState=1;
			 System.Device.Gsm.GsmATSend("AT+COPS?","OK",0);
			 App.Data.GsmState=0;
			 break;
		case GsmSingQuery:
			System.Device.Gsm.GsmATSend("AT+CSQ","OK",0);
		  App.Data.GsmState=0;
			break;
		case MessageContactID:
			temp=Byte1(data);
			CidDataProcess(temp);
			break;
    case GSMSimRemoved:
      System.Device.Gsm.GsmATSend("AT+CFUN=0","OK",0);
      App.Data.GsmSimCardState=0;
      App.Data.GsmState=0;
      break;
    case GsmDiscovery:
      System.Device.Gsm.GsmATSend("AT+CFUN=1;","OK",0);
      break;
		default: 
      App.Data.GsmState=0;
      break;
	}
}
/*******************************************************************************************************************


*******************************************************************************************************************/
void AutoArmOrDisarmProcess(u32 data)
{
	u16 messageEnum;
	u8 temp=0xFF;
	messageEnum=Byte0(data);
	
	switch(messageEnum)
	{
		case SystemDisarmMode:
			LogWrite(LogDisarmAddr,LogDisarmTotalAddr,1,LogDisarm,0xFF);
			UnlockProcess();
			temp=SystemDisarmMode;

			System.Device.Storage.Parameter.StorageWriteData(SystemModeAddr,(u16*)&temp,1);                 
			break;
		case SystemArmMode:
			App.Data.ArmKeypadRemote=1;
			if(&App.Menu.StandbyForm==App.Menu.FocusFormPointer)
				RemoteAlawyAndStayProcess(KeyAlawy1527);
			break;
	}			
}
void DateUpdateProcess(u32 data)
{
	if(&App.Menu.StandbyForm==App.Menu.FocusFormPointer)
	App.Data.Sec=Calendar.Sec;
}
/*************************************************************************************
*
*描述：逻辑任务
*
*************************************************************************************/
void LogicTask(void)
{
	uint32_t message;
	uint32_t data; 
  FATFS *fs;
    
  fs=malloc(sizeof(FATFS));
  f_mount(0,fs);	
	System.Device.Adc.Register(AdcChannel0, (float*)(&App.Data.Voltage));
	MenuListInit();
	GetChipInfo();
	App.Menu.FocusFormPointer=CurrentMenu->MenuItems[CurrentMenu->curSelected];
  System.Device.Storage.Parameter.StorageReadData(SystemModeAddr,&App.Data.SystemMode,1);
  System.Device.Storage.Parameter.StorageReadData(RingTimesAddr,&App.Data.RingTimes,1);
  KeyToneSetup();
  System.SystemApp.ScreenSet();
	while(true)
	{
    message = System.OS.PendMessageQueue();
    data = message & 0x00FFFFFF;
		switch(Byte3(message))
		{
			case MessageKeyType: 
				KeyProcess(data);
				break;
			case MessageTimerType:
				Function(data + RomBase);
				break;
			case MessageGsmType:
				GsmStateProcess(data);
			case MessageWireless:
				WirelessProcess(data);
				break;
      case MessageSendSMS:
        MessageProcess(data);
        break;
      case MessageCall:
        CallProcess(data);
			case MessageCID:
				CIDProProcess(data);
        break;
      case MessageCallTDMF:
				DtmfProcess(data);
				break;
			case MessageCallRing:
				CallRingProcess(data);
				break;
			case MessageAutoType:
				AutoArmOrDisarmProcess(data);
				break;
			case MessageDateType:
				DateUpdateProcess(data);
				break;
			default:
        System.Device.Wireless.WirelessDecode();
				break;
		}

	}

}





