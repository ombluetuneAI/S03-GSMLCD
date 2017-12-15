
#include "system.h"
#include "App.h"
#define IconBuffer						0
#define IconString						1
#define PromptAreaString				2
#define LabelString  					3

#define BatCapacity						3.7

#define xPos							0
#define yPos							1	

#define UpdateEnable					1
#define UpdateDisable					0

#define ZoneTypePosX					210
#define SoundPosX						150					
#define PcsPosY							18						
#define SirenPosX						120
#define PcsPosX							140
#define NumberPosX						180
 
extern void AboutInfoDisp(void);
 
extern u32 SystemRunTime;
extern const string Months[12];
static Form * FocusFormPointer;
static u8 record;
typedef struct
{
	u8 Sec;
	__packed struct 
	{
		byte 		    ID;
		byte 		    Type;//图标类型
//		u32 		    BackColor;//背景颜色
//		u32		      PaintColor;//画笔颜色
//		int  		    TextMode;//文本模式
//		GuiFont 	  Font;//安体
//		u16 		    ClearX0;
//		u16 		    ClearY0;
//		u16 		    ClearX1;
//		u16 		    ClearY1;		
		u16 		    IconXY[2];//坐标
//		u16 		    TextXY[2];
		void 		    *IconPointer;//
		u8 		      *TextPointer;
	}Icon[8];
	
	__packed struct FillArea
	{
		byte 		    ID;
		uint	 	    BackColor;
		GuiRECT    	RectWrap;
		LcdColor   	PaintColor;//
	}FillArea[5];
	
	__packed struct Label
	{
		byte		    ID;
		u16 		    xy[2];
		uint       	TextMode;
		uint		    BackColor;
		LcdColor   	PaintColor;
		GuiFont		  Font;
		u32			    Addr;
		u8			    Type;
    u8          Update;
		void		    *TextPointer;
	}Label[20];
  
	__packed struct List
	{
		byte  		  ID;
		byte		    Type;
		byte 		    Redraw;
		byte		    Select;//判断该列表是不是只需要一个选择项
		byte		    ItemsHeight;
		byte 		    CheckBoxState;
		byte 		    LastTimeSelected;
		byte 		    LastTimeIndexes;
		u16 		    ListXY[2];
		u16		      CheckSize[2];
		u16 		    Size[2];//Size[0],存放xSize;Size[1],存放ySize
		u8	 		    ItemsPerpage;//页显示条目
		u8	 		    TopItem;//总条目
		u8	 		    TopIndexes;
		u8	 		    TotalItems;//顶层条目
		u8 			    selectedItem;
		uint 		    SelectedBackColor;//选中的背景颜色
		uint 		    SelectedPaintColor;//选中的画笔颜色
		u8	 		    SelectedIndexes;//选择的索引
		u32 		    BackColor;//背景颜色
		uint 		    RimColor;	
		uint   		  PaintColor;//画笔颜色
		int         TextMode;
		GuiFont 	  Font;
		string 		  Name[30];
    u8          *Path[30];
		u32 		    *IconAdd[30];
		u32			    *FlashAddr[30];
		u32			    *NullPointer[30];
		u32			    *Addr;//显示存储名字
		u8			    State[30];
	}List[2];
  
	__packed struct TextBox
	{
		u16			    x;
		u16			    y;
		u8			    ID;	
		u8			    Type;
		u8			    RimType;
		u8			    Lenght;
		uint	 	    BackColor;//
		LcdColor   	PaintColor;//
		int         TextMode;//
		GuiFont		  Font;//	
		GuiRECT  	  RectWrap;
		void		    *DataPointer;
		const string *StringPointer;		
	}TextBox[6];
  
	__packed struct ArCheckBox
	{
    u8          ID;
    u16         x;
    u16         y;
		u16		      xString;
		u16		      yString;
    u8          Size;//
    uint 	      BackColor;//
    uint 	      PaintColor;//
    int  	      TextMode;//
    GuiFont     Font;//
    void        *StringPointer;
    u32         Addr;
    u8          Update;
    uint 	      SelectedBackColor;//
    uint 	      SelectedPaintColor;//    
    u8          State;
    u8          ArCheckUpdate;
    u8          Week;
    struct  ArCheck *NextPointer;		
	}ArCheckBox[7];  
  
  __packed  struct Digit
  {
    u8          ID;
    u8          Value;
		u8		      bit;
    u16         x;
    u16         y;
    u16         xSize;
    u16         ySize;
    GuiFont     Font;//
    uint 	      BackColor;
    uint 	      PaintColor;//
    int  	      TextMode;//
   }AutoDigit[2];    
    
}UpdateData;


static UpdateData UpdateDisplay;
static byte FillAreaID[10];
static byte LabelID[20];
static byte IconID[10];
static byte ListBoxID[2];
static byte TextBoxID[6];
static byte FormID;
static int  TextMode[6];
static byte	CheckState[30];
static byte ArCheckID[7];
static u8   Rectime[2];
static u8 LabelUpdate[20];

static  void DrawCheckBox(CheckBox *pointer);
extern void ImageDraw(u16 x,u16 y,u8 *color);
/******************************************************************************************
*
*描述		：复选框解析
*输入参数	：无
*输出参数	：无
*
*******************************************************************************************/
static void ParseArCheckBox(void)
{
	byte count=0;
	ArCheck *arCheckPointer;

	arCheckPointer = FocusFormPointer->ArCheckPointer;

	while(arCheckPointer != (ArCheck*)null)
	{

		UpdateDisplay.ArCheckBox[count].x=arCheckPointer->x;
		UpdateDisplay.ArCheckBox[count].y=arCheckPointer->y;
		UpdateDisplay.ArCheckBox[count].xString=arCheckPointer->xString;
		UpdateDisplay.ArCheckBox[count].yString=arCheckPointer->yString;
		UpdateDisplay.ArCheckBox[count].Size=arCheckPointer->Size;
		UpdateDisplay.ArCheckBox[count].BackColor=arCheckPointer->BackColor;
		UpdateDisplay.ArCheckBox[count].PaintColor=arCheckPointer->PaintColor;
		UpdateDisplay.ArCheckBox[count].TextMode=arCheckPointer->TextMode;
		UpdateDisplay.ArCheckBox[count].Font=arCheckPointer->Font;
		UpdateDisplay.ArCheckBox[count].StringPointer=(void*)arCheckPointer->StringPointer[arCheckPointer->stringCount];
		UpdateDisplay.ArCheckBox[count].Addr=*(u32*)arCheckPointer->Addr;
		UpdateDisplay.ArCheckBox[count].SelectedBackColor=arCheckPointer->SelectedBackColor;
		UpdateDisplay.ArCheckBox[count].SelectedPaintColor=arCheckPointer->SelectedPaintColor;
		UpdateDisplay.ArCheckBox[count].State=arCheckPointer->State;
    UpdateDisplay.ArCheckBox[count].Week=arCheckPointer->Week;
    UpdateDisplay.ArCheckBox[count].Update=arCheckPointer->Update;
		count++;
		arCheckPointer=arCheckPointer->NextPointer;
	}

}
/******************************************************************************************
*
*描述		：复选框更新，这里是独立的复选择框，可任意位置显示
*输入参数	：无
*输出参数	：无
*
*******************************************************************************************/
static void UpdateArCheckBox(void)
{
	u8 i;
	static u8 update=0;
	CheckBox checkBoxPointer;
	u32 addr;
	u8 weekBuffer[7];

	for(i=0;i<FocusFormPointer->ArCheckCount;i++)
	{
		if((ArCheckID[i]!=UpdateDisplay.ArCheckBox[i].ID)||(UpdateDisplay.ArCheckBox[i].ArCheckUpdate!=UpdateDisplay.ArCheckBox[i].Update))
		{
			update=1;
            UpdateDisplay.ArCheckBox[i].ArCheckUpdate=UpdateDisplay.ArCheckBox[i].Update;
			ArCheckID[i]=UpdateDisplay.ArCheckBox[i].ID;
			addr=UpdateDisplay.ArCheckBox[i].Addr;
			if(App.Menu.FocusFormPointer==&App.Menu.AutoSetupForm)
				System.Device.Storage.Parameter.StorageReadData(addr+22,weekBuffer,7);
			else
				System.Device.Storage.Parameter.StorageReadData(addr,&weekBuffer[0],1);
		}
		if(update)
		{
			checkBoxPointer.x=UpdateDisplay.ArCheckBox[i].x;
			checkBoxPointer.y=UpdateDisplay.ArCheckBox[i].y;
			checkBoxPointer.xSize=UpdateDisplay.ArCheckBox[i].Size;
			checkBoxPointer.ySize=UpdateDisplay.ArCheckBox[i].Size;
            checkBoxPointer.RimColor=UpdateDisplay.ArCheckBox[i].PaintColor;
			if(App.Menu.FocusFormPointer==&App.Menu.AutoSetupForm)
			{
				if(weekBuffer[UpdateDisplay.ArCheckBox[i].Week-1])
					checkBoxPointer.FillColor=UpdateDisplay.ArCheckBox[i].PaintColor;
				else
					checkBoxPointer.FillColor=UpdateDisplay.ArCheckBox[i].BackColor;
			}
			else 
			{
				if(UpdateDisplay.ArCheckBox[i].State)
					checkBoxPointer.FillColor=UpdateDisplay.ArCheckBox[i].PaintColor;
				else
					checkBoxPointer.FillColor=UpdateDisplay.ArCheckBox[i].BackColor;
			}
			DrawCheckBox(&checkBoxPointer);

			GUI_SetColor(UpdateDisplay.ArCheckBox[i].PaintColor);	  
			GUI_SetTextMode(UpdateDisplay.ArCheckBox[i].TextMode);
			GUI_SetFont(UpdateDisplay.ArCheckBox[i].Font); 
			System.emWin.emWinDispStringAt(UpdateDisplay.ArCheckBox[i].StringPointer,UpdateDisplay.ArCheckBox[i].xString,UpdateDisplay.ArCheckBox[i].yString);	
            update=0;

		}
	}
    App.Data.ArCheckUpdate=0;

}
/*******************************************************************************************


*******************************************************************************************/
//static void BatteryState(u16 x,u16 y,float Electricity,u16 color)
//{

//	static float temp;
//	temp= Electricity/BatCapacity;
//	
//	if(Electricity<3.3)
//	{
//		color=Red;
//		GUI_SetColor(GUI_RED);
//	}
//	else
//	{
//		GUI_SetColor(GUI_WHITE);
//	}
//	LCDDrawVerticalLine(x,y+3,6,color);
//	LCDDrawVerticalLine(x+30,y,12,color);
//	
//	LCDDrawVerticalLine(x+4,y,3,color);
//	LCDDrawVerticalLine(x+4,y+9,3,color);
//	
//	LCDDrawHorizontalLine(x+4,y-1,26,color);
//	LCDDrawHorizontalLine(x+4,y+12,26,color);
//	
//	LCDDrawHorizontalLine(x,y+3,4,color);
//	LCDDrawHorizontalLine(x,y+9,4,color);	
//	
//	GUI_SetFont(GUI_FONT_6X8);
//	
//	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
//	if(1<=temp)
//	{
//		System.emWin.emWinDispStringAt("100%",x+5,y+3);
//	}
//	else if(Electricity<3.3)
//	{
//		System.emWin.emWinDispStringAt("00",x+13,y+3);
//	}
//	
//}

/*********************************************************************************************************************

**********************************************************************************************************************/
static  void DrawCheckBox(CheckBox *pointer)
{

	GUI_SetColor(pointer->FillColor);
	if(App.Menu.FocusFormPointer->ListPointer==0)
		GUI_FillRect(pointer->x+4,pointer->y+4,(pointer->xSize-8)+pointer->x+4,(pointer->ySize-8)+pointer->y+4);
	else 
		System.Device.Lcd.LCDColorFill(pointer->x+4,pointer->y+4,(pointer->xSize-8)+pointer->x+4,(pointer->ySize-8)+pointer->y+4,pointer->FillColor);
	
	GUI_SetColor(pointer->RimColor);
	GUI_DrawRect(pointer->x,pointer->y,pointer->xSize+pointer->x,pointer->ySize+pointer->y);
}
/*******************************************************************************
* 描述	    : TextBox控件扫描刷新，TextBox不仅用于文本显示，还能按键输入修改参数
* 输入参数  : formPointer 当前窗体指针
*******************************************************************************/
static void ParseTextBox(void)
{
	byte count=0;
  TextBox * textBoxPointer;
	
  textBoxPointer = FocusFormPointer->TextBoxPointer;
  if(App.Data.AutoHour==240)
  {
    App.Data.AutoHour=20;
  }

  while(textBoxPointer != (TextBox *)0)
  {    
		UpdateDisplay.TextBox[count].x=textBoxPointer->x;
		UpdateDisplay.TextBox[count].y=textBoxPointer->y;
		UpdateDisplay.TextBox[count].BackColor=textBoxPointer->BackColor;
		UpdateDisplay.TextBox[count].PaintColor=textBoxPointer->PaintColor;
		UpdateDisplay.TextBox[count].Font=textBoxPointer->Font;
		UpdateDisplay.TextBox[count].TextMode=textBoxPointer->TextMode;
		UpdateDisplay.TextBox[count].Type=textBoxPointer->Type;
		UpdateDisplay.TextBox[count].RimType=textBoxPointer->RimType;
		
		UpdateDisplay.TextBox[count].ID=textBoxPointer->ID;
		UpdateDisplay.TextBox[count].DataPointer=textBoxPointer->DataPointer;
		UpdateDisplay.TextBox[count].RectWrap.x0=textBoxPointer->RectWrap.x0;
		UpdateDisplay.TextBox[count].RectWrap.y0=textBoxPointer->RectWrap.y0;
		UpdateDisplay.TextBox[count].RectWrap.x1=textBoxPointer->RectWrap.x1;
		UpdateDisplay.TextBox[count].RectWrap.y1=textBoxPointer->RectWrap.y1;
		UpdateDisplay.TextBox[count].Lenght=textBoxPointer->Leght;	
		count++;											
    textBoxPointer = textBoxPointer->NextTextBoxPointer;
  }
}

/*********************************************************************************************************


**********************************************************************************************************/
static void UpdateFillArea(void)
{
	byte i;
	bool update;
	//
	for(i=0;i<FocusFormPointer->FillAreaCount;i++)
	{
		update=0;
		if(FillAreaID[i]!=UpdateDisplay.FillArea[i].ID)
		{
			update=1;
			FillAreaID[i]=UpdateDisplay.FillArea[i].ID;
		}
		if(update)
		{
			update=0;
 			//GUI_SetColor(UpdateDisplay.FillArea[i].PaintColor);
 			//GUI_FillRectEx(&UpdateDisplay.FillArea[i].RectWrap);		
			System.Device.Lcd.LCDColorFill(UpdateDisplay.FillArea[i].RectWrap.x0,\
                                     UpdateDisplay.FillArea[i].RectWrap.y0,\
                                     UpdateDisplay.FillArea[i].RectWrap.x1,\
                                     UpdateDisplay.FillArea[i].RectWrap.y1,\
                                     UpdateDisplay.FillArea[i].PaintColor\
                                    );		
			record=0;			
		}
	}
}
/**************************************************************************************************************************
*
*描述		：更新标签
*输入参数	：无
*输出参数	：无
*
***************************************************************************************************************************/
static void UpdateLabel(void)
{
	byte  i;
  byte  stringSize;
	bool  update;
	u8    buffer[20];
	u8	  stringLen;
	for(i=0;i<FocusFormPointer->LabelCount;i++)
	{
		update=0;
		if((LabelID[i]!=UpdateDisplay.Label[i].ID)||(LabelUpdate[i]!=UpdateDisplay.Label[i].Update))
		{
			LabelID[i]=UpdateDisplay.Label[i].ID;
      LabelUpdate[i]=UpdateDisplay.Label[i].Update;
			update=1;
		}
		if(FocusFormPointer->LabelUpdate==0xFE)
    {
			update=1;
    }
		if(update)
		{
			GUI_SetTextMode(UpdateDisplay.Label[i].TextMode);
			GUI_SetFont(UpdateDisplay.Label[i].Font);
			GUI_SetColor(UpdateDisplay.Label[i].PaintColor);
      GUI_SetBkColor(UpdateDisplay.Label[i].BackColor);
      switch(UpdateDisplay.Label[i].Type)
      {
        case 0://居中
//          System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
          GUI_DispStringHCenterAt(UpdateDisplay.Label[i].TextPointer,LCDXSize/2,UpdateDisplay.Label[i].xy[1]);
          break;
        case 0xC0:
          {
            System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
          }
          break;
        case 0xF5:
          {
            stringLen=strlen(UpdateDisplay.Label[i].TextPointer);
            if(stringLen>=14)
            {
              GUI_SetFont(ListFont);
              UpdateDisplay.Label[i].xy[0]=(320-(stringLen*10))/2;
            }
            else
            {
              UpdateDisplay.Label[i].xy[0]=(320-(stringLen*17))/2;
            }
//            System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
            GUI_DispStringHCenterAt(UpdateDisplay.Label[i].TextPointer,LCDXSize/2,UpdateDisplay.Label[i].xy[1]);
          }
          break;
        case 0xFC:
        case 0xFE: 
          {
            memset(buffer,0xFF,20);
            EnterCritical();
            System.Device.Storage.Parameter.StorageReadData(UpdateDisplay.Label[i].Addr,(u8*)buffer,20);
            ExitCritical();
            stringSize=strlen((const char *)buffer);
            stringSize*=30;
            if(FocusFormPointer==&App.Menu.AutoSetupForm)
            {
              System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+200,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
            }
            System.emWin.emWinDispStringAt((char*)buffer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
//            GUI_DispStringHCenterAt((char*)buffer,LCDXSize/2,UpdateDisplay.Label[i].xy[1]);
          }
          break;
        case 0xFB:
          {
            if((u32)UpdateDisplay.Label[i].TextPointer>=10)
            {
              GUI_DispDecAt((u32)UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],2);
            }
            else
            {
              GUI_DispDecAt((u32)UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],1);
            }
          }
          break;
        case 0xF9:
        case 0xFA:
          {
            System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+20,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
            memset(buffer,0xFF,20);
            EnterCritical();
            System.Device.Storage.Parameter.StorageReadData(UpdateDisplay.Label[i].Addr,(u8*)&buffer[0],1);
            ExitCritical();
            if(buffer[0]>=10)
            {
              GUI_DispDecAt((u32)buffer[0],UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],2);
            }
            else
            {
              GUI_DispDecAt((u32)buffer[0],UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],1);	
            }
          }
          break;
        case 0xF8:
          {
            System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+280,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
            System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
//            GUI_DispStringHCenterAt((char*)buffer,LCDXSize/2,UpdateDisplay.Label[i].xy[1]);
            
          }
          break;
        case 0xF7:
          {
            System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+280,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
            System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
//            GUI_DispStringHCenterAt((char*)buffer,LCDXSize/2,UpdateDisplay.Label[i].xy[1]);
          }
          break;
        case 0xF1:
          AboutInfoDisp();
          break;
        default:
          break;
      }
//			if(0==UpdateDisplay.Label[i].Type)
//      {
//				System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
//      }
//			if(0xF5==UpdateDisplay.Label[i].Type)
//			{
//				stringLen=strlen(UpdateDisplay.Label[i].TextPointer);
//				if(stringLen>=14)
//				{
//					GUI_SetFont(GUI_FONT_20_1);
//					UpdateDisplay.Label[i].xy[0]=(320-(stringLen*10))/2;
//				}
//				else
//        {
//					UpdateDisplay.Label[i].xy[0]=(320-(stringLen*17))/2;
//        }
//				System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
//				
//			}
//			else if((0xFE==UpdateDisplay.Label[i].Type)||(0xFC==UpdateDisplay.Label[i].Type))
//			{
//				memset(buffer,0xFF,20);
//        EnterCritical();
//				System.Device.Storage.Parameter.StorageReadData(UpdateDisplay.Label[i].Addr,(u8*)buffer,20);
//        ExitCritical();
//        stringSize=strlen((const char *)buffer);
//        stringSize*=30;
//        if(FocusFormPointer==&App.Menu.AutoSetupForm)
//        {
//          System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+200,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
//        }
//				System.emWin.emWinDispStringAt((char*)buffer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
//			}
//			else if(0xFB==UpdateDisplay.Label[i].Type)
//			{
//				if((u32)UpdateDisplay.Label[i].TextPointer>=10)
//        {
//					GUI_DispDecAt((u32)UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],2);
//        }
//				else
//        {
//					GUI_DispDecAt((u32)UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],1);
//        }
//			}
//			else if((0xFA==UpdateDisplay.Label[i].Type)||(0xF9==UpdateDisplay.Label[i].Type))
//			{
//				System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+20,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
//				memset(buffer,0xFF,20);
//        EnterCritical();
//				System.Device.Storage.Parameter.StorageReadData(UpdateDisplay.Label[i].Addr,(u8*)&buffer[0],1);
//        ExitCritical();
//				if(buffer[0]>=10)
//        {
//					GUI_DispDecAt((u32)buffer[0],UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],2);
//        }
//				else
//        {
//					GUI_DispDecAt((u32)buffer[0],UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],1);	
//        }          
//			}
//			else if(0xF8==UpdateDisplay.Label[i].Type)
//			{
//				System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+280,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
//				System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);
//			}
//      else if(0xF7==UpdateDisplay.Label[i].Type)
//      {
//				System.Device.Lcd.LCDColorFill(UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1],UpdateDisplay.Label[i].xy[0]+280,UpdateDisplay.Label[i].xy[1]+20,FocusFormPointer->Color);
//				System.emWin.emWinDispStringAt(UpdateDisplay.Label[i].TextPointer,UpdateDisplay.Label[i].xy[0],UpdateDisplay.Label[i].xy[1]);                
//      }
		}
	}
	FocusFormPointer->LabelUpdate=0;
}
/********************************************************************************************
*描述		：更新显示图标
*********************************************************************************************/
static void UpdateIcon(void)
{
  const u8 icoArr[8]={7,0,1,2,3,4,5,6};
//  u32 curTime,lastTime;
	byte i;
  u8 curIcoNo,lastIcoNo;
	static byte systemMode=0xFF;
	u8 update;
	for(i=0;i<FocusFormPointer->IconCount;i++)
	{
		update=0;
		if(IconID[i]!=UpdateDisplay.Icon[i].ID)
		{
			IconID[i]=UpdateDisplay.Icon[i].ID;
			update=1;
		}
		else if(App.Data.SystemModeUpdate!=systemMode)
		{
			//update=1;
			//systemMode=App.Data.SystemModeUpdate;
		}
    if(CurrentMenu->icoUpdata)
    {
      CurrentMenu->icoUpdata = 0;
      update =  2;
      curIcoNo = icoArr[CurrentMenu->curSelected];
      lastIcoNo = icoArr[CurrentMenu->lastSelected];
    }

		if(update)
		{
			if(UpdateDisplay.Icon[i].TextPointer!=NULL)
			{
        GUI_SetFont(GUI_FONT_16B_1);
        GUI_SetTextMode(GUI_TEXTMODE_TRANS);
        if(update==1)
        {
          if(App.Menu.FocusFormPointer == &App.Menu.VoiceListForm)
          {
            ImageDrawAlpah(UpdateDisplay.Icon[i].IconXY[0],UpdateDisplay.Icon[i].IconXY[1],(u8*)UpdateDisplay.Icon[i].IconPointer);
            GUI_DispStringHCenterAt((const char *)UpdateDisplay.Icon[i].TextPointer,UpdateDisplay.Icon[i].IconXY[0]+36/2,\
                                    200);
          }
          else
          {
          ImageDrawAlpah(UpdateDisplay.Icon[i].IconXY[0],UpdateDisplay.Icon[i].IconXY[1],(u8*)UpdateDisplay.Icon[i].IconPointer);
          GUI_SetColor(GUI_BLACK);
          GUI_DispStringHCenterAt((const char *)UpdateDisplay.Icon[i].TextPointer,UpdateDisplay.Icon[i].IconXY[0]+MainIconSize/2,\
                                  UpdateDisplay.Icon[i].IconXY[1]+MainIconSize+5);
          if((FocusFormPointer->FormID==IconMenuForm)&&(i==7))
          {
            curIcoNo = icoArr[CurrentMenu->curSelected];
            LCDFillAlpha(UpdateDisplay.Icon[curIcoNo].IconXY[0]-2,\
                         UpdateDisplay.Icon[curIcoNo].IconXY[1]-2,\
                         60,60,MoveBarColor);
            GUI_SetColor(GUI_BROWN);
            GUI_DispStringHCenterAt((const char *)UpdateDisplay.Icon[curIcoNo].TextPointer,UpdateDisplay.Icon[curIcoNo].IconXY[0]+MainIconSize/2,\
                                    UpdateDisplay.Icon[curIcoNo].IconXY[1]+MainIconSize+5);
          }
        }
        }
        if(update==2)
        {
          LcdFillArea(UpdateDisplay.Icon[lastIcoNo].IconXY[0]-2,\
                      UpdateDisplay.Icon[lastIcoNo].IconXY[1]-2,\
                      UpdateDisplay.Icon[lastIcoNo].IconXY[0]-2+59,\
                      UpdateDisplay.Icon[lastIcoNo].IconXY[1]-2+59,\
                      WHITE);
          ImageDrawAlpah(UpdateDisplay.Icon[lastIcoNo].IconXY[0],\
                         UpdateDisplay.Icon[lastIcoNo].IconXY[1],\
                         (u8*)UpdateDisplay.Icon[lastIcoNo].IconPointer);
          GUI_SetColor(GUI_BLACK);
          GUI_DispStringHCenterAt((const char *)UpdateDisplay.Icon[lastIcoNo].TextPointer,\
                                  UpdateDisplay.Icon[lastIcoNo].IconXY[0]+MainIconSize/2,\
                                  UpdateDisplay.Icon[lastIcoNo].IconXY[1]+MainIconSize+5);
          LCDFillAlpha(UpdateDisplay.Icon[curIcoNo].IconXY[0]-2,\
                       UpdateDisplay.Icon[curIcoNo].IconXY[1]-2,\
                       60,60,MoveBarColor);
          GUI_SetColor(GUI_BROWN);
          GUI_DispStringHCenterAt((const char *)UpdateDisplay.Icon[curIcoNo].TextPointer,\
                                  UpdateDisplay.Icon[curIcoNo].IconXY[0]+MainIconSize/2,\
                                  UpdateDisplay.Icon[curIcoNo].IconXY[1]+MainIconSize+5);
        }
			}
			else if(UpdateDisplay.Icon[i].Type==Image2Lcd)
      {
				ImageDrawAlpah(UpdateDisplay.Icon[i].IconXY[0],UpdateDisplay.Icon[i].IconXY[1],(u8*)UpdateDisplay.Icon[i].IconPointer);
        //GUI_DispStringHCenterAt((const char *)UpdateDisplay.Icon[curIcoNo].TextPointer,UpdateDisplay.Icon[curIcoNo].IconXY[0]+36/2,\
                                    UpdateDisplay.Icon[curIcoNo].IconXY[1]+36+5);
      }
			else
      {
				BmpDecode((u8*)UpdateDisplay.Icon[i].IconPointer,UpdateDisplay.Icon[i].IconXY[0],UpdateDisplay.Icon[i].IconXY[1],70,90,0,0);
      }
		}
	}
}
/*******************************************************************************
* 描述	    : Label控件扫描刷新，Label一般用于文本显示，但不能按键输入操作
* 输入参数  : formPointer 当前窗体指针
*******************************************************************************/
static void ParseLabel(void)
{
	Label * labelPointer;	
	uint stringAddCount;
	byte count=0;
	labelPointer = FocusFormPointer->LabelPointer;
		
  while(labelPointer != (Label *)0)
  {			
		UpdateDisplay.Label[count].BackColor=labelPointer->BackColor;
		UpdateDisplay.Label[count].Font=labelPointer->Font;
		UpdateDisplay.Label[count].PaintColor=labelPointer->PaintColor;
		UpdateDisplay.Label[count].TextMode=labelPointer->TextMode;

		stringAddCount =labelPointer->Count;
		if(0xF5==labelPointer->Type)
    {
			UpdateDisplay.Label[count].TextPointer=(void*)labelPointer->StringBlockPointer;
    }
		if((0==labelPointer->Type)||(0xFB==labelPointer->Type)||(0xF7==labelPointer->Type)||(0xC0==labelPointer->Type))
    {
			UpdateDisplay.Label[count].TextPointer= labelPointer->StringBlockPointer[stringAddCount];
    }
		if(0xF8==labelPointer->Type)
    {
			UpdateDisplay.Label[count].TextPointer=(void*)labelPointer->StringBlockPointer;
    }
    UpdateDisplay.Label[count].Update=labelPointer->Update;
		UpdateDisplay.Label[count].Type=labelPointer->Type;
		UpdateDisplay.Label[count].Addr=labelPointer->Addr;
		UpdateDisplay.Label[count].xy[0]=labelPointer->X;
		UpdateDisplay.Label[count].xy[1]=labelPointer->Y;		
		UpdateDisplay.Label[count].ID=labelPointer->ID;
        
    //-----------2016-10-16----
    //添加的原因是 不可直接去更新后面的显示 必须是先要解析数据后再显示 
    UpdateDisplay.Label[count].Update=labelPointer->Update;//这是后来添加的 操作方式是每操作一次状态都发生变化 在这里更背后事才去显示
		count++;
		labelPointer = labelPointer->NextLabelPointer;
  }
}

/****************************************************************************************************************
*
*描述		：对ICON进行解析，把数据放到缓存里后面显示
*输入参数	：无
*输出参数	：
*
****************************************************************************************************************/
static void ParseIcon(void)
{
  Icon * pointer;
	byte count=0;
  
  pointer = FocusFormPointer->IconPointer;
	while(pointer != (Icon *)0)
	{
		UpdateDisplay.Icon[count].IconXY[0]=pointer->x;
		UpdateDisplay.Icon[count].IconXY[1]=pointer->y;
		UpdateDisplay.Icon[count].IconPointer=pointer->IconPointer;
		UpdateDisplay.Icon[count].TextPointer=pointer->StringPointer;
		UpdateDisplay.Icon[count].ID=pointer->ID;
		UpdateDisplay.Icon[count].Type=pointer->Type;
		count++;
		pointer = pointer->NextIconPointer;
	}
}
/********************************************************************************
*
*描述		：解析填充区域
*输入参数	：无
*输出参数	：无
*
*******************************************************************************/
static void ParseFillArea(void)
{
	FillArea *FillAreaPointer;
	byte count=0;

	FillAreaPointer=FocusFormPointer->FillAreaPointer;
	while(FillAreaPointer!=(FillArea *)0)
	{
		UpdateDisplay.FillArea[count].RectWrap.x0=FillAreaPointer->RectWrap.x0;
		UpdateDisplay.FillArea[count].RectWrap.y0=FillAreaPointer->RectWrap.y0;
		UpdateDisplay.FillArea[count].RectWrap.x1=FillAreaPointer->RectWrap.x1;
		UpdateDisplay.FillArea[count].RectWrap.y1=FillAreaPointer->RectWrap.y1;
//		UpdateDisplay.FillArea[count].ColorStart=FillAreaPointer->ColorStart;
//		UpdateDisplay.FillArea[count].ColorEnd=FillAreaPointer->ColorEnd;
		UpdateDisplay.FillArea[count].PaintColor=FillAreaPointer->PaintColor;
		UpdateDisplay.FillArea[count].ID=FillAreaPointer->ID;
		count++;
		FillAreaPointer=FillAreaPointer->NextFillAreaPointer;
	}
}
/***********************************************************************************************



************************************************************************************************/
static void TextListUpdate(u8 count,u16 x0,u16 y0,u16 x1,u16 y1)
{
	u8 		listPos;
	char 	*textPointer;
	u32 	*addr;
	u8 		buffer[20];
	u8		number[30];
	u8 		i;
	
	GUI_SetTextMode(UpdateDisplay.List[count].TextMode);
	if(NotRedraw==UpdateDisplay.List[count].Redraw)
	{
		//恢复上次选择项
		listPos=UpdateDisplay.List[count].LastTimeSelected*UpdateDisplay.List[count].ItemsHeight+y0;
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);	
		GUI_SetColor(UpdateDisplay.List[count].PaintColor);
		textPointer=UpdateDisplay.List[count].Name[UpdateDisplay.List[count].LastTimeIndexes];
		if(2==UpdateDisplay.List[count].Select)							
		{
			addr=UpdateDisplay.List[count].IconAdd[UpdateDisplay.List[count].LastTimeIndexes];
			System.Device.Storage.Parameter.StorageReadData(*addr,(u8*)buffer,20);
			switch((u8)buffer[12])
			{
				case SystemSoundOn://伴音开
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Sound on",x0+UpdateDisplay.List[count].CheckSize[xPos]+SoundPosX,listPos+yPosOffset);
					break;
				case SystemSoundOff://伴音关
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Sound off",x0+UpdateDisplay.List[count].CheckSize[xPos]+SoundPosX,listPos+yPosOffset);
					break;
				case SystemSirenOn://警号开
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Siren on",x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset);
					break;
				case SystemSirenOff://警号关
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Siren off",x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset);
					break;
				
			}
			if((SystemSirenOn==(u8)buffer[12])||(SystemSirenOff==(u8)buffer[12]))
			{
				if(buffer[16]<10)//探头个数
					GUI_DispDecAt(buffer[16],x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset+PcsPosY,1);
				else
					GUI_DispDecAt(buffer[16],x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset+PcsPosY,2);
				System.emWin.emWinDispStringAt("Pcs",x0+UpdateDisplay.List[count].CheckSize[xPos]+PcsPosX,listPos+yPosOffset+PcsPosY);
				switch((u8)buffer[14])
				{
					case SystemZoneClose:
						System.emWin.emWinDispStringAt("Close",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneAlarm:
						System.emWin.emWinDispStringAt("Normal",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneDelayAlarm:
						System.emWin.emWinDispStringAt("Delay",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneHomeAlarm:
						System.emWin.emWinDispStringAt("Stay",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneSOS:
						System.emWin.emWinDispStringAt("Critical.",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneDoorbell:
						System.emWin.emWinDispStringAt("Doorbell",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
				}
			}
			GUI_SetFont(UpdateDisplay.List[count].Font);
			System.emWin.emWinDispStringAt((char*)buffer,x0+UpdateDisplay.List[count].CheckSize[xPos],listPos+yPosOffset);
		}
		else
    {
      GUI_SetFont(UpdateDisplay.List[count].Font);
			System.emWin.emWinDispStringAt(textPointer,x0+UpdateDisplay.List[count].CheckSize[xPos],listPos+yPosOffset);
    }
		
		//选择项
		GUI_SetFont(UpdateDisplay.List[count].Font);
		listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
		//GUI_SetColor(UpdateDisplay.List[j].SelectedBackColor);
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
		
		GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
		textPointer=UpdateDisplay.List[count].Name[UpdateDisplay.List[count].SelectedIndexes];	
		if(2==UpdateDisplay.List[count].Select)	//还有显示后面的						
		{
			addr=UpdateDisplay.List[count].IconAdd[UpdateDisplay.List[count].SelectedIndexes];
			System.Device.Storage.Parameter.StorageReadData(*addr,(u8*)buffer,20);
			switch((u8)buffer[12])
			{
				case SystemSoundOn://伴音开
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Sound on",x0+UpdateDisplay.List[count].CheckSize[xPos]+SoundPosX,listPos+yPosOffset);
					break;
				case SystemSoundOff://伴音关
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Sound off",x0+UpdateDisplay.List[count].CheckSize[xPos]+SoundPosX,listPos+yPosOffset);
					break;
				case SystemSirenOn://警号开
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Siren on",x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset);
					break;
				case SystemSirenOff://警号关
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Siren off",x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset);
					break;
				default:;
			}
			if((SystemSirenOn==(u8)buffer[12])||(SystemSirenOff==(u8)buffer[12]))
			{
				if(buffer[16]<10)
					GUI_DispDecAt(buffer[16],x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset+PcsPosY,1);
				else
					GUI_DispDecAt(buffer[16],x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset+PcsPosY,2);
				System.emWin.emWinDispStringAt("Pcs",x0+UpdateDisplay.List[count].CheckSize[xPos]+PcsPosX,listPos+yPosOffset+PcsPosY);
				switch((u8)buffer[14])
				{
					case SystemZoneClose:
						System.emWin.emWinDispStringAt("Close",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneAlarm:
						System.emWin.emWinDispStringAt("Normal",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneDelayAlarm:
						System.emWin.emWinDispStringAt("Delay",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneHomeAlarm:
						System.emWin.emWinDispStringAt("Stay",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneSOS:
						System.emWin.emWinDispStringAt("Critical.",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
					case SystemZoneDoorbell:
						System.emWin.emWinDispStringAt("Doorbell",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
						break;
				}
			}
			GUI_SetFont(UpdateDisplay.List[count].Font);
			System.emWin.emWinDispStringAt((char*)buffer,x0+UpdateDisplay.List[count].CheckSize[xPos],listPos+yPosOffset);
		}
		else
			System.emWin.emWinDispStringAt(textPointer,x0+UpdateDisplay.List[count].CheckSize[xPos],listPos+yPosOffset);	
	}
	else if(Redraws==UpdateDisplay.List[count].Redraw)
	{
		for(i=0;i<UpdateDisplay.List[count].ItemsPerpage;i++)
		{
			if(i==UpdateDisplay.List[count].selectedItem)
			{
				listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
				//GUI_SetColor(UpdateDisplay.List[j].SelectedBackColor);
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
				GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);																											
			}
			else
			{
				listPos=i*UpdateDisplay.List[count].ItemsHeight+y0;
				//GUI_SetColor(UpdateDisplay.List[j].BackColor);
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);	
				GUI_SetColor(UpdateDisplay.List[count].PaintColor);	
			}
			textPointer=UpdateDisplay.List[count].Name[i+UpdateDisplay.List[count].TopIndexes];
			if((2==UpdateDisplay.List[count].Select))						
			{
				if(2==UpdateDisplay.List[count].Select)
				{
					addr=UpdateDisplay.List[count].IconAdd[i+UpdateDisplay.List[count].TopIndexes];
					System.Device.Storage.Parameter.StorageReadData(*addr,(u8*)buffer,20);
				}
				else if(3==UpdateDisplay.List[count].Select)
				{
					System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].IconAdd[i+UpdateDisplay.List[count].TopIndexes],(u8*)buffer,20);//Ãû×Ö
					System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].FlashAddr[i+UpdateDisplay.List[count].TopIndexes],(u8*)number,30);//ºÅÂë			
				}
				switch((u8)buffer[12])
				{
					case SystemSoundOn://伴音开
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt("Sound on",x0+UpdateDisplay.List[count].CheckSize[xPos]+SoundPosX,listPos+yPosOffset);
						break;
					case SystemSoundOff://伴音关
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt("Sound off",x0+UpdateDisplay.List[count].CheckSize[xPos]+SoundPosX,listPos+yPosOffset);
						break;
					case SystemSirenOn://警号开
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt("Siren on",x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset);
						break;
					case SystemSirenOff://警号关
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt("Siren off",x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset);
						break;
				}
				if((SystemSirenOn==(u8)buffer[12])||(SystemSirenOff==(u8)buffer[12]))
				{
					if(buffer[16]<10)
						GUI_DispDecAt(buffer[16],x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset+PcsPosY,1);
					else
						GUI_DispDecAt(buffer[16],x0+UpdateDisplay.List[count].CheckSize[xPos]+SirenPosX,listPos+yPosOffset+PcsPosY,2);
					System.emWin.emWinDispStringAt("Pcs",x0+UpdateDisplay.List[count].CheckSize[xPos]+PcsPosX,listPos+yPosOffset+PcsPosY);
					
					switch((u8)buffer[14])
					{
						case SystemZoneClose:
							System.emWin.emWinDispStringAt("Close",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
							break;
						case SystemZoneAlarm:
							System.emWin.emWinDispStringAt("Normal",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
							break;
						case SystemZoneDelayAlarm:
							System.emWin.emWinDispStringAt("Delay",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
							break;
						case SystemZoneHomeAlarm:
							System.emWin.emWinDispStringAt("Stay",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
							break;
						case SystemZoneSOS:
							System.emWin.emWinDispStringAt("Critical.",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
							break;
						case SystemZoneDoorbell:
							System.emWin.emWinDispStringAt("Doorbell",x0+UpdateDisplay.List[count].CheckSize[xPos]+ZoneTypePosX,listPos+yPosOffset);
							break;
					}
				}
				GUI_SetFont(UpdateDisplay.List[count].Font);
				System.emWin.emWinDispStringAt((char*)buffer,x0+UpdateDisplay.List[count].CheckSize[xPos],listPos+yPosOffset);
			}
			else
			{
				GUI_SetFont(UpdateDisplay.List[count].Font);
				System.emWin.emWinDispStringAt(textPointer,x0+UpdateDisplay.List[count].CheckSize[xPos],listPos+yPosOffset);	
			}
			listPos+=UpdateDisplay.List[count].ItemsHeight;					
		}
	}
}
/*****************************************************************************
*
*描述         ：图形开关
*输入参数     ：x,y;显示坐标
*               color,字体颜色
*输出参数     ： 无
*
*****************************************************************************/
void DrawSwitch(u16 x,u16 y,u8 state,u32 color)
{
    
    GUI_SetPenSize(4);
    if(state)//打开
    {
        GUI_SetColor(GUI_GREEN);
        GUI_AA_FillRoundedRect(x+20,y,x+40+20,y+10,5);
    
        GUI_SetColor(GUI_GREEN);
        GUI_AA_FillCircle(x+30,y+5,10);
    }
    else if(0==state)
    {
        GUI_SetColor(GUI_LIGHTGRAY);
        GUI_AA_FillRoundedRect(x+20,y,x+40+20,y+10,5);

        GUI_SetColor(GUI_LIGHTGRAY);
        GUI_AA_FillCircle(x+22+30,y+5,10);           
    } 
    GUI_SetColor(color);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetFont(GUI_FONT_8X10_ASCII);
    GUI_DispStringAt("On",x,y);
    GUI_DispStringAt("Off",x+65,y);      
}
/**************************************************************************************************
*
*描述		：图形开关
*输入参数	：无
*输出参数	：无
*
**************************************************************************************************/
static void SwitchListUpdate(u8 count,u16 x0,u16 y0,u16 x1,u16 y1)
{
    u8 		listPos;
    u8      state;
    u8      i;
    char    *textPointer;
    char      buffer[22];

	if(NotRedraw==UpdateDisplay.List[count].Redraw)
	{
		//恢复
		if(1<UpdateDisplay.List[count].TotalItems)
		{
			listPos=UpdateDisplay.List[count].LastTimeSelected*UpdateDisplay.List[count].ItemsHeight+y0;
			System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);

			GUI_SetColor(UpdateDisplay.List[count].PaintColor);
			listPos=UpdateDisplay.List[count].LastTimeSelected*UpdateDisplay.List[count].ItemsHeight+y0;
            if(!UpdateDisplay.List[count].State[UpdateDisplay.List[count].LastTimeIndexes])
               state=0;
            else
               state=1;
            DrawSwitch(220,listPos+yPosOffset+5,state,UpdateDisplay.List[count].PaintColor);
			GUI_SetColor(UpdateDisplay.List[count].PaintColor);
            if(4==UpdateDisplay.List[count].Select)
            {
                System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].NullPointer[UpdateDisplay.List[count].LastTimeIndexes],(u8*)buffer,22);
                textPointer=buffer;
            }
            else
                textPointer=UpdateDisplay.List[count].Name[UpdateDisplay.List[count].LastTimeIndexes];
            GUI_SetTextMode(UpdateDisplay.List[count].TextMode);
            GUI_SetFont(UpdateDisplay.List[count].Font);
            System.emWin.emWinDispStringAt(textPointer,x0+5,listPos+yPosOffset);	
		}						
		//选择项
		listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
		GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
        if(!UpdateDisplay.List[count].State[UpdateDisplay.List[count].SelectedIndexes])
           state=0;
        else
           state=1;
        DrawSwitch(220,listPos+yPosOffset+5,state,UpdateDisplay.List[count].SelectedPaintColor);
		GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
        GUI_SetTextMode(UpdateDisplay.List[count].TextMode);
        GUI_SetFont(UpdateDisplay.List[count].Font);
        if(4==UpdateDisplay.List[count].Select)
        {
            System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].NullPointer[UpdateDisplay.List[count].SelectedIndexes],(u8*)buffer,22);
            textPointer=buffer;
        }
        else
            textPointer=UpdateDisplay.List[count].Name[UpdateDisplay.List[count].SelectedIndexes];
		System.emWin.emWinDispStringAt(textPointer,x0+5,listPos+yPosOffset);	
	}
	else if(Redraws==UpdateDisplay.List[count].Redraw)
	{
		for(i=0;i<UpdateDisplay.List[count].ItemsPerpage;i++)
		{
			if(i==UpdateDisplay.List[count].selectedItem)
			{
				listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
				if(!UpdateDisplay.List[count].State[i+UpdateDisplay.List[count].TopIndexes])
					state=0;
				else
					state=1;
                DrawSwitch(220,listPos+yPosOffset+5,state,UpdateDisplay.List[count].SelectedPaintColor);    
                GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);	
			}
			else
			{
				listPos=i*UpdateDisplay.List[count].ItemsHeight+y0;
				//GUI_SetColor(UpdateDisplay.List[j].BackColor);
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);	
				if(!UpdateDisplay.List[count].State[i+UpdateDisplay.List[count].TopIndexes])
					state=0;
				else
					state=1;
                DrawSwitch(220,listPos+yPosOffset+5,state,UpdateDisplay.List[count].PaintColor);
                GUI_SetColor(UpdateDisplay.List[count].PaintColor);	
			}
            GUI_SetTextMode(UpdateDisplay.List[count].TextMode);
            GUI_SetFont(UpdateDisplay.List[count].Font);
            if(4==UpdateDisplay.List[count].Select)
            {
                System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].NullPointer[i+UpdateDisplay.List[count].TopIndexes],(u8*)buffer,22);
                textPointer=buffer;
            }
            else
                textPointer=UpdateDisplay.List[count].Name[i+UpdateDisplay.List[count].TopIndexes];
			System.emWin.emWinDispStringAt(textPointer,x0+5,listPos+yPosOffset);	
			listPos+=UpdateDisplay.List[count].ItemsHeight;						
		}
	}
}
/****************************************************************************************************************************************



*****************************************************************************************************************************************/
static void CheckListUpdate(u8 count,u16 x0,u16 y0,u16 x1,u16 y1)
{
	u8 listPos;
	char *textPointer;
	u8 i;
	CheckBox checkBoxPointer;
	
	GUI_SetTextMode(UpdateDisplay.List[count].TextMode);
	GUI_SetFont(UpdateDisplay.List[count].Font);
	if(NotRedraw==UpdateDisplay.List[count].Redraw)
	{
		//恢复
		if(1<UpdateDisplay.List[count].TotalItems)
		{
			listPos=UpdateDisplay.List[count].LastTimeSelected*UpdateDisplay.List[count].ItemsHeight+y0;
			System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);

			GUI_SetColor(UpdateDisplay.List[count].PaintColor);
			listPos=UpdateDisplay.List[count].LastTimeSelected*UpdateDisplay.List[count].ItemsHeight+y0;
			checkBoxPointer.RimColor=UpdateDisplay.List[count].PaintColor;
			if(!UpdateDisplay.List[count].State[UpdateDisplay.List[count].LastTimeIndexes])
				checkBoxPointer.FillColor=UpdateDisplay.List[count].BackColor;
			else
				checkBoxPointer.FillColor=UpdateDisplay.List[count].PaintColor;
			checkBoxPointer.x=x0+xIconPosOffset;;
			checkBoxPointer.y=listPos+yIconPosOffset;
			checkBoxPointer.xSize=UpdateDisplay.List[count].CheckSize[xPos];
			checkBoxPointer.ySize=UpdateDisplay.List[count].CheckSize[yPos];
			DrawCheckBox(&checkBoxPointer);	
			GUI_SetColor(UpdateDisplay.List[count].PaintColor);
			System.emWin.emWinDispStringAt(UpdateDisplay.List[count].Name[UpdateDisplay.List[count].LastTimeIndexes],x0+xPosOffset,listPos+yPosOffset);	
		}						
		//选择项
		listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
		//GUI_SetColor(UpdateDisplay.List[j].SelectedBackColor);
		//GUI_FillRect(x0,listPos,x1,listPos+listHeight);
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
		GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
		checkBoxPointer.RimColor=UpdateDisplay.List[count].SelectedPaintColor;
		
		if(!UpdateDisplay.List[count].State[UpdateDisplay.List[count].SelectedIndexes])
			checkBoxPointer.FillColor=UpdateDisplay.List[count].SelectedBackColor;
		else
			checkBoxPointer.FillColor=UpdateDisplay.List[count].SelectedPaintColor;
		
		checkBoxPointer.x=x0+xIconPosOffset;;
		checkBoxPointer.y=listPos+yIconPosOffset;
		checkBoxPointer.xSize=UpdateDisplay.List[count].CheckSize[xPos];
		checkBoxPointer.ySize=UpdateDisplay.List[count].CheckSize[yPos];
		DrawCheckBox(&checkBoxPointer);	
		GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
		textPointer=UpdateDisplay.List[count].Name[UpdateDisplay.List[count].SelectedIndexes];
		System.emWin.emWinDispStringAt(textPointer,x0+xPosOffset,listPos+yPosOffset);	
	}
	else if(Redraws==UpdateDisplay.List[count].Redraw)
	{
		for(i=0;i<UpdateDisplay.List[count].ItemsPerpage;i++)
		{
			if(i==UpdateDisplay.List[count].selectedItem)
			{
				listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
				GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);	
				checkBoxPointer.RimColor=UpdateDisplay.List[count].SelectedPaintColor;
				if(!UpdateDisplay.List[count].State[i+UpdateDisplay.List[count].TopIndexes])
					checkBoxPointer.FillColor=UpdateDisplay.List[count].SelectedBackColor;
				else
					checkBoxPointer.FillColor=UpdateDisplay.List[count].SelectedPaintColor;							
			}
			else
			{
				listPos=i*UpdateDisplay.List[count].ItemsHeight+y0;
				//GUI_SetColor(UpdateDisplay.List[j].BackColor);
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);	
				GUI_SetColor(UpdateDisplay.List[count].PaintColor);	
				checkBoxPointer.RimColor=UpdateDisplay.List[count].PaintColor;
				if(!UpdateDisplay.List[count].State[i+UpdateDisplay.List[count].TopIndexes])
					checkBoxPointer.FillColor=UpdateDisplay.List[count].BackColor;
				else
					checkBoxPointer.FillColor=UpdateDisplay.List[count].PaintColor;
			}
			checkBoxPointer.x=x0+xIconPosOffset;
			checkBoxPointer.y=listPos+yIconPosOffset;
			checkBoxPointer.xSize=UpdateDisplay.List[count].CheckSize[xPos];
			checkBoxPointer.ySize=UpdateDisplay.List[count].CheckSize[yPos];
			DrawCheckBox(&checkBoxPointer);	
			textPointer=UpdateDisplay.List[count].Name[i+UpdateDisplay.List[count].TopIndexes];
			System.emWin.emWinDispStringAt(textPointer,x0+xPosOffset,listPos+yPosOffset);	
			listPos+=UpdateDisplay.List[count].ItemsHeight;						
		}
	}
}
/****************************************************************************************************************************************
*
*描述		：图标列表更新
*输入参数	：count,列表个数；x0,y0,x1,y1 列表坐标
*输出参数	：无
*
*****************************************************************************************************************************************/
static void ICONListUpdate(u8 count,u16 x0,u16 y0,u16 x1,u16 y1)
{
	u16  listPos;
	char *textPointer;
  u8 *path;
	Bitmap ICON;
	u8 i;
	u8 buffer[20],number[30];
	
	GUI_SetTextMode(UpdateDisplay.List[count].TextMode);
	GUI_SetFont(UpdateDisplay.List[count].Font);
	if(NotRedraw==UpdateDisplay.List[count].Redraw)//不重画
	{
		listPos=UpdateDisplay.List[count].LastTimeSelected*UpdateDisplay.List[count].ItemsHeight+y0;//
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);
		
		ICON.x0=x0+xIconPosOffset;
		ICON.y0=listPos+yIconPosOffset;
		path=UpdateDisplay.List[count].Path[UpdateDisplay.List[count].LastTimeIndexes];
    ImageDrawAlpah(ICON.x0,ICON.y0,path);
//    BmpDecode((u8*)path,ICON.x0,ICON.y0,26,26,0,0);
		//System.Device.Lcd.LCDDrawingBitmap(&ICON);	
		if(3==UpdateDisplay.List[count].Select)
		{
			memset((char*)number,0xFF,30);
			System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].NullPointer[UpdateDisplay.List[count].LastTimeIndexes],(u8*)number,30);
			if(0x01==number[28])//没有号码
			{	
				GUI_SetFont(UpdateDisplay.List[count].Font);
				GUI_SetColor(UpdateDisplay.List[count].PaintColor);
				System.emWin.emWinDispStringAt((char*)number,x0+xPosOffset,listPos+yPosOffset);
			}
			else
			{
				GUI_SetColor(UpdateDisplay.List[count].PaintColor);
				GUI_SetFont(UpdateDisplay.List[count].Font);
				System.emWin.emWinDispStringAt((char*)number,x0+xPosOffset,listPos+yPosOffset);	//有号码
				System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].FlashAddr[UpdateDisplay.List[count].LastTimeIndexes],(u8*)buffer,20);
				if(1==buffer[12])//进行了命名
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt((char*)buffer,x0+xPosOffset,listPos+yPosOffset+PcsPosY);
				}
				if(1==buffer[14])//电话
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Call",x0+xPosOffset+NumberPosX,listPos+yPosOffset);
				}
				if(1==buffer[16])//短信
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("SMS",x0+xPosOffset+NumberPosX,listPos+yPosOffset+PcsPosY);						
				}
				if(1==buffer[18])//CID
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("CID",x0+xPosOffset+NumberPosX,listPos+yPosOffset);
				}
					
			}
		}
		else
		{			
			textPointer=UpdateDisplay.List[count].Name[UpdateDisplay.List[count].LastTimeIndexes];
			GUI_SetColor(UpdateDisplay.List[count].PaintColor);
			System.emWin.emWinDispStringAt(textPointer,x0+xPosOffset,listPos+yPosOffset);	
		}
		
		/*-------------------------------------当前选择项------------------------------------------------*/
		listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
		ICON.x0=UpdateDisplay.List[count].ListXY[0]+xIconPosOffset;
		ICON.y0=listPos+yIconPosOffset;
		path=UpdateDisplay.List[count].Path[UpdateDisplay.List[count].SelectedIndexes];
    ImageDrawAlpah(ICON.x0,ICON.y0,path);
//    BmpDecode((u8*)path,ICON.x0,ICON.y0,26,26,0,0);
		//System.Device.Lcd.LCDDrawingBitmap(&ICON);
		if(3==UpdateDisplay.List[count].Select)
		{
			System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].NullPointer[UpdateDisplay.List[count].SelectedIndexes],(u8*)number,30);
			if(0x01==number[28])//没有号码
			{	
				GUI_SetFont(UpdateDisplay.List[count].Font);
				GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
				System.emWin.emWinDispStringAt((char*)number,x0+xPosOffset,listPos+yPosOffset);
			}
			else
			{
				GUI_SetFont(UpdateDisplay.List[count].Font);
				GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
				System.emWin.emWinDispStringAt((char*)number,x0+xPosOffset,listPos+yPosOffset);	//
				System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].FlashAddr[UpdateDisplay.List[count].SelectedIndexes],(u8*)buffer,20);
				if(1==buffer[12])//进行了命名
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt((char*)buffer,x0+xPosOffset,listPos+yPosOffset+PcsPosY);
				}
				if(1==buffer[14])//电话
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("Call",x0+xPosOffset+NumberPosX,listPos+yPosOffset);
				}
				if(1==buffer[16])//短信
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("SMS",x0+xPosOffset+NumberPosX,listPos+yPosOffset+PcsPosY);						
				}
				if(1==buffer[18])//CID
				{
					GUI_SetFont(GUI_FONT_8X13_1);
					System.emWin.emWinDispStringAt("CID",x0+xPosOffset+NumberPosX,listPos+yPosOffset);
				}
					
			}
		}
		else
		{			
			GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
			textPointer=UpdateDisplay.List[count].Name[UpdateDisplay.List[count].SelectedIndexes];					
			System.emWin.emWinDispStringAt(textPointer,x0+xPosOffset,listPos+yPosOffset);	
		}			
	}
	else if(Redraws==UpdateDisplay.List[count].Redraw)
	{
		for(i=0;i<UpdateDisplay.List[count].ItemsPerpage;i++)
		{
			if(i==UpdateDisplay.List[count].selectedItem)
			{
				listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
				GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);																											
			}
			else
			{
				listPos=i*UpdateDisplay.List[count].ItemsHeight+y0;
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);	
				GUI_SetColor(UpdateDisplay.List[count].PaintColor);	
			}
			ICON.x0=x0+xIconPosOffset;
			ICON.y0=listPos+yIconPosOffset;
			path=UpdateDisplay.List[count].Path[i+UpdateDisplay.List[count].TopIndexes];
      ImageDrawAlpah(ICON.x0,ICON.y0,path);
//      BmpDecode((u8*)path,ICON.x0,ICON.y0,26,26,0,0);
			//System.Device.Lcd.LCDDrawingBitmap(&ICON);	
			if(3==UpdateDisplay.List[count].Select)
			{				
				System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].NullPointer[i+UpdateDisplay.List[count].TopIndexes],(u8*)number,30);
				if(0x01==number[28])//没有号码
				{	
					GUI_SetFont(UpdateDisplay.List[count].Font);
					
					System.emWin.emWinDispStringAt((char*)number,x0+xPosOffset,listPos+yPosOffset);
				}
				else
				{
					GUI_SetFont(UpdateDisplay.List[count].Font);
					System.emWin.emWinDispStringAt((char*)number,x0+xPosOffset,listPos+yPosOffset);	//ÓÐºÅÂë
					System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].FlashAddr[i+UpdateDisplay.List[count].TopIndexes],(u8*)buffer,20);
					if(1==buffer[12])//进行了命名
					{
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt((char*)buffer,x0+xPosOffset,listPos+yPosOffset+PcsPosY);
					}
					if(1==buffer[14])//电话
					{
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt("Call",x0+xPosOffset+NumberPosX,listPos+yPosOffset);
					}
					if(1==buffer[16])//短信
					{
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt("SMS",x0+xPosOffset+NumberPosX,listPos+yPosOffset+PcsPosY);						
					}
					if(1==buffer[18])//CID
					{
						GUI_SetFont(GUI_FONT_8X13_1);
						System.emWin.emWinDispStringAt("CID",x0+xPosOffset+NumberPosX,listPos+yPosOffset);
					}
						
				}
			}
			else
			{
				textPointer=UpdateDisplay.List[count].Name[i+UpdateDisplay.List[count].TopIndexes];
				System.emWin.emWinDispStringAt(textPointer,x0+xPosOffset,listPos+yPosOffset);	
				listPos+=UpdateDisplay.List[count].ItemsHeight;		
			}				
		}
	}
}
void LogListUpdate(u8 count,u16 x0,u16 y0,u16 x1,u16 y1)
{
  u16  listPos;
  u8 *path;
   // char *textPointer;
  Bitmap ICON;
  u8 i;
  u8 buffer[10];
  u16 year;
  u8 LogAddrIndex;  
  u32 NumAddr;
  u32 LogAddr;
  
	GUI_SetTextMode(UpdateDisplay.List[count].TextMode);
	
  //Add
  if((*UpdateDisplay.List[count].IconAdd[UpdateDisplay.List[count].TopIndexes])>=LogDisarmAddr)
  {
    if((*UpdateDisplay.List[count].IconAdd[UpdateDisplay.List[count].TopIndexes])>=LogArmAddr)
    {
      NumAddr = LogArmTotalAddr;
      LogAddr = LogArmAddr;
    }
    else 
    {
      NumAddr = LogDisarmTotalAddr;
      LogAddr = LogDisarmAddr;
    }
  }
  else
  {
    NumAddr = LogAlarmTotalAddr;
    LogAddr = LogAlarmAddr;
  }
   //   
	if(NotRedraw==UpdateDisplay.List[count].Redraw)//不重画
	{
		listPos=UpdateDisplay.List[count].LastTimeSelected*UpdateDisplay.List[count].ItemsHeight+y0;
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);
		
		ICON.x0=x0+xIconPosOffset;
		ICON.y0=listPos+yIconPosOffset;
		path=UpdateDisplay.List[count].Path[UpdateDisplay.List[count].LastTimeIndexes];
    ImageDrawAlpah(ICON.x0,ICON.y0,path);
//        BmpDecode((u8*)path,ICON.x0,ICON.y0,26,26,0,0);	
    GUI_SetColor(UpdateDisplay.List[count].PaintColor);
    
    System.Device.Storage.Parameter.StorageReadData(NumAddr,(u8*)&buffer[0],1);
    if(buffer[0]>UpdateDisplay.List[count].LastTimeIndexes)
    {
      LogAddrIndex = buffer[0] - UpdateDisplay.List[count].LastTimeIndexes - 1;
    }
    else
    {
      LogAddrIndex = 30 + buffer[0] - UpdateDisplay.List[count].LastTimeIndexes - 1;
    }
    System.Device.Storage.Parameter.StorageReadData(LogAddr+LogAddrIndex*10,(u8*)buffer,10); 
      
      
//        System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].IconAdd[UpdateDisplay.List[count].LastTimeIndexes],(u8*)buffer,10);    
    if(0==buffer[7])//有记录
    {
      year=buffer[0];
      year<<=8;
      year|=buffer[1];
            
      GUI_SetFont(GUI_FONT_8X13_1);
      GUI_DispDecAt (year,x0+xPosOffset+140,listPos+yPosOffset-5,4);
            
      System.emWin.emWinDispStringAt(Months[buffer[2]-1],x0+xPosOffset+90,listPos+yPosOffset-5);
            
      GUI_DispDecAt (buffer[3],x0+xPosOffset+120,listPos+yPosOffset-5,2);
            
      GUI_DispDecAt (buffer[4],x0+xPosOffset+190,listPos+yPosOffset-5,2);
            
      System.emWin.emWinDispStringAt(":",x0+xPosOffset+210,listPos+yPosOffset-5);
            
      GUI_DispDecAt (buffer[5],x0+xPosOffset+220,listPos+yPosOffset-5,2);
      
      switch(buffer[6])
      {
        case 1:
          System.emWin.emWinDispStringAt("Keypad",x0+xPosOffset+90,listPos+yPosOffset+14);
        break;
        case 2:
          System.emWin.emWinDispStringAt("Remote",x0+xPosOffset+90,listPos+yPosOffset+14);
        break;
        case 3:
          System.emWin.emWinDispStringAt("Zone",x0+xPosOffset+90,listPos+yPosOffset+14);
        break;
        default:
          break;
      }  
      GUI_SetFont(UpdateDisplay.List[count].Font);
      switch(buffer[8])
      {
        case 1:
          System.emWin.emWinDispStringAt("Disarm",x0+xPosOffset,listPos+yPosOffset);
        break;
        case 2:
          System.emWin.emWinDispStringAt("Arm",x0+xPosOffset,listPos+yPosOffset);
        break;
        case 3:
          System.emWin.emWinDispStringAt("Stay",x0+xPosOffset,listPos+yPosOffset);
          break;
        case 4:
          GUI_DispDecAt (buffer[9],x0+xPosOffset,listPos+yPosOffset,2);
          System.emWin.emWinDispStringAt("Zone",x0+xPosOffset+20,listPos+yPosOffset); 
          break;
        case 5:
          GUI_DispDecAt (buffer[9],x0+xPosOffset,listPos+yPosOffset,2);
          System.emWin.emWinDispStringAt("Remote",x0+xPosOffset+20,listPos+yPosOffset);
          break;
        case 6:
          System.emWin.emWinDispStringAt("Keypad",x0+xPosOffset,listPos+yPosOffset); 
          break;
        default:
          break;
      }       
     }
     else//没有记录
     {
       GUI_SetFont(UpdateDisplay.List[count].Font);
       System.emWin.emWinDispStringAt("Empty",x0+xPosOffset,listPos+yPosOffset);	
     }        
		/*-------------------------------------当前选择项------------------------------------------------*/
		listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
		System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
		ICON.x0=UpdateDisplay.List[count].ListXY[0]+xIconPosOffset;
		ICON.y0=listPos+yIconPosOffset;
		path=UpdateDisplay.List[count].Path[UpdateDisplay.List[count].SelectedIndexes];
    ImageDrawAlpah(ICON.x0,ICON.y0,path);
//        BmpDecode((u8*)path,ICON.x0,ICON.y0,26,26,0,0);		
        
    GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);
        
    System.Device.Storage.Parameter.StorageReadData(NumAddr,(u8*)&buffer[0],1);
    if(buffer[0]>UpdateDisplay.List[count].SelectedIndexes)
    {
      LogAddrIndex = buffer[0] - UpdateDisplay.List[count].SelectedIndexes - 1;
    }
    else
    {
      LogAddrIndex = 30 + buffer[0] - UpdateDisplay.List[count].SelectedIndexes - 1;
    }
    System.Device.Storage.Parameter.StorageReadData(LogAddr+LogAddrIndex*10,(u8*)buffer,10);
      
      
      
//        System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].IconAdd[UpdateDisplay.List[count].SelectedIndexes],(u8*)buffer,10);    
    if(0==buffer[7])//有记录
    {
      year=buffer[0];
      year<<=8;
      year|=buffer[1];
            
      GUI_SetFont(GUI_FONT_8X13_1);
      GUI_DispDecAt (year,x0+xPosOffset+140,listPos+yPosOffset-5,4);

      System.emWin.emWinDispStringAt(Months[buffer[2]-1],x0+xPosOffset+90,listPos+yPosOffset-5);

      GUI_DispDecAt (buffer[3],x0+xPosOffset+120,listPos+yPosOffset-5,2);

      GUI_DispDecAt (buffer[4],x0+xPosOffset+190,listPos+yPosOffset-5,2);

      System.emWin.emWinDispStringAt(":",x0+xPosOffset+210,listPos+yPosOffset-5);

      GUI_DispDecAt (buffer[5],x0+xPosOffset+220,listPos+yPosOffset-5,2);
      
      switch(buffer[6])
      {
        case 1:
          System.emWin.emWinDispStringAt("Keypad",x0+xPosOffset+90,listPos+yPosOffset+14);
          break;
        case 2:
          System.emWin.emWinDispStringAt("Remote",x0+xPosOffset+90,listPos+yPosOffset+14);
          break;
        case 3:
          System.emWin.emWinDispStringAt("Zone",x0+xPosOffset+90,listPos+yPosOffset+14);
          break;
        case 4:
          System.emWin.emWinDispStringAt("Call",x0+xPosOffset+90,listPos+yPosOffset+14);
          break;
        default:
          break;
      }
      GUI_SetFont(UpdateDisplay.List[count].Font);
      switch(buffer[8])
      {
        case 1:
          System.emWin.emWinDispStringAt("Disarm",x0+xPosOffset,listPos+yPosOffset);
          break;
        case 2:
          System.emWin.emWinDispStringAt("Arm",x0+xPosOffset,listPos+yPosOffset);
          break;
        case 3:
          System.emWin.emWinDispStringAt("Stay",x0+xPosOffset,listPos+yPosOffset);
          break;
        case 4:
          GUI_DispDecAt (buffer[9],x0+xPosOffset,listPos+yPosOffset,2);
          System.emWin.emWinDispStringAt("Zone",x0+xPosOffset+20,listPos+yPosOffset);
          break;
        case 5:
          GUI_DispDecAt (buffer[9],x0+xPosOffset,listPos+yPosOffset,2);
          System.emWin.emWinDispStringAt("Remote",x0+xPosOffset+20,listPos+yPosOffset); 
          break;
        case 6:
          System.emWin.emWinDispStringAt("Keypad",x0+xPosOffset,listPos+yPosOffset);
          break;
        default:
          break;
      }       
    }
    else//没有记录
    {
      GUI_SetFont(UpdateDisplay.List[count].Font);
      System.emWin.emWinDispStringAt("Empty",x0+xPosOffset,listPos+yPosOffset);	
    } 
	}
	else if(Redraws==UpdateDisplay.List[count].Redraw)
  {
    for(i=0;i<UpdateDisplay.List[count].ItemsPerpage;i++)
    {
 			if(i==UpdateDisplay.List[count].selectedItem)
			{
				listPos=UpdateDisplay.List[count].selectedItem*UpdateDisplay.List[count].ItemsHeight+y0;
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].SelectedBackColor);
				GUI_SetColor(UpdateDisplay.List[count].SelectedPaintColor);																											
			}
			else
			{
				listPos=i*UpdateDisplay.List[count].ItemsHeight+y0;
				System.Device.Lcd.LCDColorFill(x0,listPos,x1,listPos+UpdateDisplay.List[count].ItemsHeight,UpdateDisplay.List[count].BackColor);	
				GUI_SetColor(UpdateDisplay.List[count].PaintColor);	
			}     
			ICON.x0=x0+xIconPosOffset;
			ICON.y0=listPos+yIconPosOffset;
			path=UpdateDisplay.List[count].Path[i+UpdateDisplay.List[count].TopIndexes];
      ImageDrawAlpah(ICON.x0,ICON.y0,path);
//            BmpDecode((u8*)path,ICON.x0,ICON.y0,26,26,0,0);    
            //GUI_DispDecAt (I32 v, I16P x, I16P y, U8 Len);
      
      System.Device.Storage.Parameter.StorageReadData(NumAddr,(u8*)&buffer[0],1);
      if(buffer[0]>(i+UpdateDisplay.List[count].TopIndexes))
      {
        LogAddrIndex = buffer[0] - UpdateDisplay.List[count].TopIndexes - i - 1;
      }
      else
      {
        LogAddrIndex = 30 + buffer[0] - UpdateDisplay.List[count].TopIndexes - i - 1;
      }
      System.Device.Storage.Parameter.StorageReadData(LogAddr+LogAddrIndex*10,(u8*)buffer,10); 
//      System.Device.Storage.Parameter.StorageReadData(*UpdateDisplay.List[count].IconAdd[i+UpdateDisplay.List[count].TopIndexes],(u8*)buffer,10);    
      if(0==buffer[7])//有记录
      {
        year=buffer[0];
        year<<=8;
        year|=buffer[1];
                
        GUI_SetFont(GUI_FONT_8X13_1);
        GUI_DispDecAt (year,x0+xPosOffset+140,listPos+yPosOffset-5,4);
                
        System.emWin.emWinDispStringAt(Months[buffer[2]-1],x0+xPosOffset+90,listPos+yPosOffset-5);
                
        GUI_DispDecAt (buffer[3],x0+xPosOffset+120,listPos+yPosOffset-5,2);
                
        GUI_DispDecAt (buffer[4],x0+xPosOffset+190,listPos+yPosOffset-5,2);
                
        System.emWin.emWinDispStringAt(":",x0+xPosOffset+210,listPos+yPosOffset-5);
                
        GUI_DispDecAt (buffer[5],x0+xPosOffset+220,listPos+yPosOffset-5,2);
        
        switch(buffer[6])
        {
          case 1:
            System.emWin.emWinDispStringAt("Keypad",x0+xPosOffset+90,listPos+yPosOffset+14);
          break;
          case 2:
            System.emWin.emWinDispStringAt("Remote",x0+xPosOffset+90,listPos+yPosOffset+14);
          break;
          case 3:
            System.emWin.emWinDispStringAt("Zone",x0+xPosOffset+90,listPos+yPosOffset+14);
          break;
          case 4:
            System.emWin.emWinDispStringAt("Call",x0+xPosOffset+90,listPos+yPosOffset+14);
            GUI_SetFont(UpdateDisplay.List[count].Font);
          break;
          default:
            break;
        }
        GUI_SetFont(UpdateDisplay.List[count].Font);
        switch(buffer[8])
        {
          case 1:
            System.emWin.emWinDispStringAt("Disarm",x0+xPosOffset,listPos+yPosOffset);
          break;
          case 2:
            System.emWin.emWinDispStringAt("Arm",x0+xPosOffset,listPos+yPosOffset);
          break;
          case 3:
            System.emWin.emWinDispStringAt("Stay",x0+xPosOffset,listPos+yPosOffset);
          break;
          case 4:
            GUI_DispDecAt (buffer[9],x0+xPosOffset,listPos+yPosOffset,2);
            System.emWin.emWinDispStringAt("Zone",x0+xPosOffset+20,listPos+yPosOffset); 
          break;
          case 5:
            GUI_DispDecAt (buffer[9],x0+xPosOffset,listPos+yPosOffset,2);
            System.emWin.emWinDispStringAt("Remote",x0+xPosOffset+20,listPos+yPosOffset); 
          break;
          case 6:
            System.emWin.emWinDispStringAt("Keypad",x0+xPosOffset,listPos+yPosOffset);
          break;
          default:
            break;
        }                                  
      }
      else//没有记录
      {
        GUI_SetFont(UpdateDisplay.List[count].Font);
        System.emWin.emWinDispStringAt("Empty",x0+xPosOffset,listPos+yPosOffset);	
      }
    }
  }
}
/****************************************************************************************************************************************



*****************************************************************************************************************************************/
static void ListBoxStaticUpdate(void)
{
	static u16 	indexes;//记录上次的索引
	byte 		i,count;
	byte 		update;
	u16 		x0,x1,y0,y1;


	
	if(FocusFormPointer->ListPointer==NULL) return;
	for(count=0;count<FocusFormPointer->ListCount;count++)
	{
		update=UpdateDisable;
		x0=UpdateDisplay.List[count].ListXY[0];
		x1=UpdateDisplay.List[count].Size[0];
		y0=UpdateDisplay.List[count].ListXY[1];
		y1=UpdateDisplay.List[count].Size[1];	
		if(ListBoxID[count]!=UpdateDisplay.List[count].ID)
		{
			update=UpdateEnable;
			ListBoxID[count]=UpdateDisplay.List[count].ID;
			for(i=0;i<FocusFormPointer->ListPointer->TotalItems;i++)
				CheckState[i]=UpdateDisplay.List[count].State[i];		
			System.Device.Lcd.LCDColorFill(x0,y0,x1,y1,UpdateDisplay.List[count].BackColor);
			UpdateDisplay.List[count].Redraw=Redraws;
			if(0xFF000000!=UpdateDisplay.List[count].RimColor)
			{
				LCDDrawRectangle(x0-1,y0-1,x1-x0+2,y1-y0+2,UpdateDisplay.List[count].RimColor);
				LCDDrawRectangle(x0-2,y0-2,x1-x0+4,y1-y0+4,UpdateDisplay.List[count].RimColor);
			}
		}
		else if(indexes!=UpdateDisplay.List[count].SelectedIndexes)
				update=UpdateEnable;
			 else if((UpdateDisplay.List[count].Type==CheckList)||(UpdateDisplay.List[count].Type==SwitchList))
			 {		
				for(i=0;i<FocusFormPointer->ListPointer->TotalItems;i++)
				{
					if(CheckState[i]!=UpdateDisplay.List[count].State[i])
					{
 						CheckState[i]=UpdateDisplay.List[count].State[i];
						update=UpdateEnable;
					}
				}
			 }	
		if(update)
		{
			switch(UpdateDisplay.List[count].Type)
			{
				case IconList:
					ICONListUpdate(count,x0,y0,x1,y1);
					break;
				case CheckList:
					 CheckListUpdate(count,x0,y0,x1,y1);
					break;
				case TextList:
					TextListUpdate(count,x0,y0,x1,y1);
					break;
        case SwitchList:
           SwitchListUpdate(count,x0,y0,x1,y1);
           break;
        case LogStateList:
           LogListUpdate(count,x0,y0,x1,y1);
           break;
			}
			indexes=UpdateDisplay.List[count].SelectedIndexes;
		}
	}
}

/*************************************************************************************************
*描述		：  在当前列表中找查索引号为  indexes 的list项
*输入参数	：	listx,要查找的列表指针
				index,要查找的列表项索引号
*输出参数	：	返回找到的list项的指针  或是 NULL(未找到)
*************************************************************************************************/
ListboxList * ListSearch(ListboxList *listx,u16 index)
{
	u16 icount=0;
 	ListboxList * listtemp=listx;	

	while(listtemp->Prevlist!=NULL)
		listtemp=listtemp->Prevlist;//追溯到最开始的list
	while(1)//寻找编号为index的list
	{
		if(index==icount)break;	 
		if(listtemp->Nextlist!=NULL)
		{
			listtemp=listtemp->Nextlist;    
			icount++;//计数增加
		}else return NULL;//未找到编号为index的list
	} 
	return listtemp;//返回找到的list指针
} 
/*********************************************************************************************************
*描述       	：  对列表框进行解析
*输入参数	  	：  无  
*输出参数		：	无
*********************************************************************************************************/
static void ParseListbox(void)
{
	ListboxList *listx;
	ListBox *listbox;
	byte i=0;
	byte j=0;
	listbox=FocusFormPointer->ListPointer;
	while(listbox!=NULL)
	{
		EnterCritical();										    

		listx=listbox->List;
		UpdateDisplay.List[j].BackColor=listbox->BackColor;
		UpdateDisplay.List[j].Font=listbox->Font;
		
		UpdateDisplay.List[j].ItemsPerpage=listbox->ItemsPerpage;
		UpdateDisplay.List[j].ID=listbox->ID;
		UpdateDisplay.List[j].ListXY[xPos]=listbox->x0;
		UpdateDisplay.List[j].ListXY[yPos]=listbox->y0;
		UpdateDisplay.List[j].Size[xPos]=listbox->x1;
		UpdateDisplay.List[j].Size[yPos]=listbox->y1;
		UpdateDisplay.List[j].PaintColor=listbox->PaintColor;
		UpdateDisplay.List[j].RimColor=listbox->RimColor;
		UpdateDisplay.List[j].selectedItem=listbox->selectedItem;
		UpdateDisplay.List[j].SelectedBackColor=listbox->SelectedBackColor;
		UpdateDisplay.List[j].SelectedIndexes=listbox->SelectedIndexes;
		UpdateDisplay.List[j].SelectedPaintColor=listbox->SelectedPaintColor;
		UpdateDisplay.List[j].TextMode=listbox->TextMode;
		UpdateDisplay.List[j].TopItem=listbox->TopItem;
		UpdateDisplay.List[j].TopIndexes=listbox->TopIndexes;
		UpdateDisplay.List[j].TotalItems=listbox->TotalItems;
		UpdateDisplay.List[j].LastTimeSelected=listbox->LastTimeSelected;
		UpdateDisplay.List[j].Redraw=listbox->Redraw;
		UpdateDisplay.List[j].LastTimeIndexes=listbox->LastTimeIndexes;
		UpdateDisplay.List[j].Type=listbox->Type;
		UpdateDisplay.List[j].CheckSize[xPos]=listbox->xSize;
		UpdateDisplay.List[j].CheckSize[yPos]=listbox->ySize;
		UpdateDisplay.List[j].Select=listbox->Select;
		UpdateDisplay.List[j].ItemsHeight=listbox->ItemsHeight;

		listx=ListSearch(listx,0);
		while(listx!=0)
		{
			if(listx!=0)
			{	//printf("Error\n\n");
				UpdateDisplay.List[j].IconAdd[i]=listx->IconAdd;
				UpdateDisplay.List[j].FlashAddr[i]=listx->FlashAddr;
				UpdateDisplay.List[j].NullPointer[i]=listx->NullPointer;
        UpdateDisplay.List[j].Path[i]=listx->Path;
				UpdateDisplay.List[j].Name[i]=listx->Name;	
				UpdateDisplay.List[j].State[i]=listx->State;	
				i++;
			}
			listx=listx->Nextlist;
		}
		i=0;		
		j++;
    ExitCritical();
		listbox=listbox->NextList;
	}			
}

static void ParseAutoDigit(void)
{
  AutoDigit *digitPointer;
	u8 count=0;
  digitPointer=FocusFormPointer->AutoDigitPointer;
  while(NULL!=digitPointer)
  {
    UpdateDisplay.AutoDigit[count].x=digitPointer->x;
    UpdateDisplay.AutoDigit[count].y=digitPointer->y;
    UpdateDisplay.AutoDigit[count].xSize=digitPointer->xSize;
    UpdateDisplay.AutoDigit[count].ySize=digitPointer->ySize;
    UpdateDisplay.AutoDigit[count].ID=digitPointer->ID;
    UpdateDisplay.AutoDigit[count].Value=digitPointer->Value;
    UpdateDisplay.AutoDigit[count].Font=digitPointer->Font;//
    UpdateDisplay.AutoDigit[count].BackColor=digitPointer->BackColor;//
    UpdateDisplay.AutoDigit[count].PaintColor=digitPointer->PaintColor;//
    UpdateDisplay.AutoDigit[count].TextMode=digitPointer->TextMode;//
		UpdateDisplay.AutoDigit[count].bit=digitPointer->bit;
    digitPointer=digitPointer->NextPointer;
		count++;
  }     
}

/**************************************************************************
*
*描述        ：更新文本框
*输入参数   ： 无
*输出参数   ： 无
*
*************************************************************************/
static void UpdateTextBox(void)
{
	u8 i,update=0;
	u16 type,lenght=1;
	u16 updateData[6]; 
	static u16 originalData[6];
	
	for(i=0;i<FocusFormPointer->TextBoxCount;i++)
	{
		if(UpdateDisplay.TextBox[i].DataPointer!=null)
		{
			switch(UpdateDisplay.TextBox[i].Type)
			{
				case GuiDataTypeByteDec:
					 updateData[i]=*(u8*)(UpdateDisplay.TextBox[i].DataPointer);
					break;
				case GuiDataTypeString:
					break;
				case GuiDataTypeUintDec:
					updateData[i]=*(u16*)(UpdateDisplay.TextBox[i].DataPointer);
					break;
				
			}
		}
		if(TextBoxID[i]!=UpdateDisplay.TextBox[i].ID)//
		{
			update=1;
			TextBoxID[i]=UpdateDisplay.TextBox[i].ID;
			originalData[i]=updateData[i];
			TextMode[i]=UpdateDisplay.TextBox[i].TextMode;
		}else if((originalData[i]!=updateData[i])||(TextMode[i]!=UpdateDisplay.TextBox[i].TextMode))//µ±ÎÄ×Ö¸Ä±äµÄÊ±ºò
		{
			update=2;
			originalData[i]=updateData[i];
			TextMode[i]=UpdateDisplay.TextBox[i].TextMode;
		}
		if(update)
		{
			GUI_SetTextMode(UpdateDisplay.TextBox[i].TextMode);
			GUI_SetFont(UpdateDisplay.TextBox[i].Font);
			GUI_SetColor(UpdateDisplay.TextBox[i].PaintColor);
			GUI_SetBkColor(UpdateDisplay.TextBox[i].BackColor);

			if(0x01==UpdateDisplay.TextBox[i].RimType)
			{
				System.Device.Lcd.LCDColorFill(UpdateDisplay.TextBox[i].RectWrap.x0,UpdateDisplay.TextBox[i].RectWrap.y0+26,
								UpdateDisplay.TextBox[i].RectWrap.x1,UpdateDisplay.TextBox[i].RectWrap.y1+26,FocusFormPointer->Color);	
				LCDDrawRectangle(UpdateDisplay.TextBox[i].x-3,UpdateDisplay.TextBox[i].y+1,279,59,UpdateDisplay.TextBox[i].PaintColor);
				LCDDrawRectangle(UpdateDisplay.TextBox[i].x-4,UpdateDisplay.TextBox[i].y,281,61,UpdateDisplay.TextBox[i].PaintColor);
				GUI_DispStringAt(UpdateDisplay.TextBox[i].DataPointer,UpdateDisplay.TextBox[i].x,UpdateDisplay.TextBox[i].y+26);
				update=0;
				return;
			}
			type=UpdateDisplay.TextBox[i].Type;				
			System.Device.Lcd.LCDColorFill(UpdateDisplay.TextBox[i].RectWrap.x0,UpdateDisplay.TextBox[i].RectWrap.y0,
											UpdateDisplay.TextBox[i].RectWrap.x1,UpdateDisplay.TextBox[i].RectWrap.y1,FocusFormPointer->Color);			
			if(GuiDataTypeString!=type)
			{
				if(originalData[i]>=10)
					lenght=2;
				if(originalData[i]>=100) 
					lenght=3;
				if(originalData[i]>=1000)
					lenght=4;
				if(UpdateDisplay.TextBox[i].Lenght)
					GUI_DispDecAt(originalData[i],UpdateDisplay.TextBox[i].x,UpdateDisplay.TextBox[i].y,UpdateDisplay.TextBox[i].Lenght);
				else
					GUI_DispDecAt(originalData[i],UpdateDisplay.TextBox[i].x,UpdateDisplay.TextBox[i].y,lenght);
				lenght=1;
			}
			else
			{
				GUI_DispStringAt(UpdateDisplay.TextBox[i].DataPointer,UpdateDisplay.TextBox[i].x,UpdateDisplay.TextBox[i].y);
				if(App.Menu.FocusFormPointer==&App.Menu.AddPhoneForm)
				{
					App.Menu.FocusFormPointer->TextBoxPointer->DataAddr=0;
					memset(App.Menu.FocusFormPointer->TextBoxPointer->DataPointer,0,30);
				}
			}
			update=0;
		}
	}
}
void GsmSignalTab(u16 x,u16 y,u8 val)
{
    u8 i;
    
    for(i=0;i<4;i++)
    {
        GUI_ClearRect(x+(i*8),y+(3*(i+1)), 3+(i*7), y+20);
    }
}
static void UpdateGsmSignal(void)
{
	static u8 ID;
  GUI_SetBkColor(GUI_WHITE);
  GsmSignalTab(30,0,4);
  if((FocusFormPointer!=&App.Menu.AlarmForm)&&(FocusFormPointer!=&App.Menu.WaitForm))
  {
    if(App.Data.GsmSimCardState==1)
    {
      if((record!=App.Data.GsmCSQ)||(ID!=FocusFormPointer->FormID))
      {
        LcdFillArea(0,0,80,28,BLACK);
        GUI_SetBkColor(GUI_WHITE);
        GsmSignalTab(30,0,4);
        record=App.Data.GsmCSQ;
        ID=FocusFormPointer->FormID;
        if((record<=8)&&(record>0))
        {
          GUI_SetBkColor(GUI_WHITE);
          GUI_ClearRect(5, 20, 7, 20);
                
          GUI_SetBkColor(GUI_BLACK);
          GUI_ClearRect(12, 18, 14, 26);
          GUI_ClearRect(19, 16, 23, 26);
          GUI_ClearRect(26, 14, 28, 26);				
        }
        if((record<=16)&&(record>8))
        {
          GUI_SetBkColor(GUI_WHITE);
          GUI_ClearRect(5, 20, 7, 26);
          GUI_ClearRect(12, 18, 14,26);
          GUI_SetBkColor(GUI_BLACK);
          GUI_ClearRect(19, 16, 21, 26);
          GUI_ClearRect(26, 14, 28, 26);	
        }
        if((record<=24)&&(record>16))
        {
          GUI_SetBkColor(GUI_WHITE);
          GUI_ClearRect(5, 20, 7, 26);
          GUI_ClearRect(12, 18, 14, 26);
          GUI_ClearRect(19, 16, 21, 26);
                
          GUI_SetBkColor(GUI_BLACK);
          GUI_ClearRect(26, 14, 28, 26);	
        }
        if((record>24)&&(record<32))
        {
          GUI_SetBkColor(GUI_WHITE);
          GUI_ClearRect(5, 20, 7, 26);
          GUI_ClearRect(12, 18, 14, 26);
          GUI_ClearRect(19, 16, 21, 26);
          GUI_ClearRect(26, 14, 28, 26);	
        } 
       }
//     if(App.Data.OperatorUpdate)
//     {
//         GUI_SetColor(GUI_WHITE);
//         GUI_SetBkColor(GUI_BLACK);
//         GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
//         GUI_SetFont(GUI_FONT_8X13_1);
//         GUI_DispStringAt((const char*)App.Data.OperatorName,38,16);  
//         App.Data.OperatorUpdate=0;
//     } 
    }
    if(App.Data.OperatorUpdate&&(App.Data.GsmSimCardState==2))
    {
      LcdFillArea(0,0,80,28,BLACK);
      GUI_SetColor(GUI_WHITE);
      GUI_SetBkColor(GUI_BLACK);
      GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
      GUI_SetFont(GUI_FONT_8X13_1);
      GUI_DispStringAt("SEARCHING",5,16);  
      App.Data.OperatorUpdate=0;
    }
    if(App.Data.OperatorUpdate&&(!App.Data.GsmSimCardState))
    {
      LcdFillArea(0,0,80,28,BLACK);
      GUI_SetColor(GUI_WHITE);
      GUI_SetBkColor(GUI_BLACK);
      GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
      GUI_SetFont(GUI_FONT_8X13_1);
      GUI_DispStringAt("NO SIM",5,16);  
      App.Data.OperatorUpdate=0;
    }
  }
  else
  {
    ID=FocusFormPointer->FormID;
  } 
}
/***********************************=***********************************
*
*描述		：更新数字显示
*输入参数	：无
*输出参数	：无
*
*
**********************************************************************/
void UpdateAutoDigit(void)
{
    u8        	update=0; 
    u16       	x0,x1,y0,y1;
	u8			i;
    AutoDigit 	*digitPointer;
    
    digitPointer=FocusFormPointer->AutoDigitPointer;
	for(i=0;i<App.Menu.FocusFormPointer->AutoDigitCount;i++)
	{
		if(digitPointer!=null)
		{
			if(Rectime[i]!=UpdateDisplay.AutoDigit[i].Value)
			{
				Rectime[i]=UpdateDisplay.AutoDigit[i].Value;
				update=1;
				x0=UpdateDisplay.AutoDigit[i].x;
				y0=UpdateDisplay.AutoDigit[i].y;
				x1=UpdateDisplay.AutoDigit[i].x+UpdateDisplay.AutoDigit[i].xSize;
				y1=UpdateDisplay.AutoDigit[i].y+UpdateDisplay.AutoDigit[i].ySize;
			}
			if(update)
			{
				GUI_SetColor(UpdateDisplay.AutoDigit[i].BackColor);
				GUI_FillRect(x0,y0,x1,y1);
				GUI_SetColor(UpdateDisplay.AutoDigit[i].PaintColor);
				GUI_SetBkColor(UpdateDisplay.AutoDigit[i].BackColor);
				GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
				GUI_SetFont(UpdateDisplay.AutoDigit[i].Font);
				if(App.Menu.FocusFormPointer->AutoDigitCount==1)
					GUI_DispDecAt(UpdateDisplay.AutoDigit[i].Value,x0+8,y0+4,UpdateDisplay.AutoDigit[i].bit);
				else
					GUI_DispDecAt(UpdateDisplay.AutoDigit[i].Value,x0,y0,UpdateDisplay.AutoDigit[i].bit);
				update=0;
			}
		}
	}
}


extern void TimeDisplay(void);
extern void RecTimeUpdate(void);

void TimeDisplay(void)
{
	static u8 i=0;
	char *strPointer;
  if(UpdateDisplay.Sec!=App.Data.Sec)
  {
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_SetFont(GUI_FONT_8X16X2X2);
		if(i==0)
		{
			ImageLocalDraw(150,60,10,30,(u8*)gImage_desktop);
			if(App.Data.DelayArmFlag)
			{
				if((App.Data.SystemMode==SystemStayMode)||(SystemDisarmMode==App.Data.SystemMode))
					ImageLocalDraw(42,150,50,50,(u8*)gImage_desktop);
				else if(App.Data.SystemMode==SystemArmMode)
					ImageLocalDraw(132,150,50,50,(u8*)gImage_desktop);
			}
			i++;
		}
		else
		{
			GUI_DispStringAt(":",150,60);
			if((App.Data.SystemMode==SystemStayMode)||(SystemDisarmMode==App.Data.SystemMode))
				ImageDrawAlpah(42,150,(u8*)gImage_Arm);
			else if(App.Data.SystemMode==SystemArmMode)
				ImageDrawAlpah(132,150,(u8*)gImage_Arm);
			i=0;
		}
		if(App.Data.Hour!=Calendar.Hour)
			ImageLocalDraw(120,60,40,30,(u8*)gImage_desktop);  
		GUI_DispDecAt(Calendar.Hour,120,60,2);
		if(App.Data.Min!=Calendar.Min)
			ImageLocalDraw(165,60,40,30,(u8*)gImage_desktop);   
		GUI_DispDecAt(Calendar.Min,165,60,2);

		GUI_SetFont(GUI_FONT_24_1);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		
		if(App.Data.Day!=Calendar.Date)
			ImageLocalDraw(200,100,24,30,(u8*)gImage_desktop); 
		GUI_DispDecAt(Calendar.Date,200,100,2);
		if(App.Data.Month!=Calendar.Month)
			ImageLocalDraw(150,100,30,30,(u8*)gImage_desktop);
		GUI_DispStringAt(Months[Calendar.Month-1],150,100);
		if(App.Data.Year!=Calendar.Year)
			ImageLocalDraw(240,100,40,30,(u8*)gImage_desktop);
		GUI_DispDecAt(Calendar.Year,240,100,4);
		switch(Calendar.Week)
		{
			case 1:
				strPointer="Monday";
				break;
			case 2:
				strPointer="Tuesday";
				break;
			case 3:
				strPointer="Wednesday";
				break;
			case 4:
				strPointer="Thursday";
				break;
			case 5:
				strPointer="Friday";
				break;
			case 6:
				strPointer="Saturday";
				break;
			case 0:
				strPointer="Sunday";
				break;
		}
		if(App.Data.Week!=Calendar.Week)
			ImageLocalDraw(30,100,120,30,(u8*)gImage_desktop);
		GUI_DispStringAt(strPointer,30,100);
		
		App.Data.Min=Calendar.Min;
		App.Data.Hour=Calendar.Hour;

		App.Data.Day=Calendar.Date;
		App.Data.Month=Calendar.Month;
		App.Data.Year=Calendar.Year;
		App.Data.Week=Calendar.Week;
    }
	 if(App.Data.SystemMode!=SystemAlarmMode)
	 {
		System.Device.Storage.Parameter.StorageReadData(SystemModeAddr,&App.Data.SystemMode,1);
		if(App.Data.SystemMode!= App.Data.SystemModeUpdate)
		{
			switch(App.Data.SystemMode)
			{
				case SystemArmMode:
					//ImageLocalDraw(132,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(132,150,(u8*)gImage_Arm);

					//ImageLocalDraw(42,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(42,150,(u8*)gImage_Unlock);

					//ImageLocalDraw(224,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(224,150,(u8*)gImage_home);

					App.Data.SystemModeUpdate=App.Data.SystemMode;
					break;
				case SystemDisarmMode:
					//           ImageLocalDraw(132,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(132,150,(u8*)gImage_Unlock);

					//ImageLocalDraw(42,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(42,150,(u8*)gImage_Arm);

					//ImageLocalDraw(224,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(224,150,(u8*)gImage_home);

					App.Data.SystemModeUpdate=App.Data.SystemMode;
					//BmpDecode("0:/ICON/bigICon/Lock.bmp",132,150,70,90,0,0);
					App.Data.SystemModeUpdate=App.Data.SystemMode;
					return;
				case SystemStayMode:
					//ImageLocalDraw(132,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(132,150,(u8*)gImage_home);

					//ImageLocalDraw(42,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(42,150,(u8*)gImage_Arm);

					//ImageLocalDraw(224,150,50,50,(u8*)gImage_wait);
					ImageDrawAlpah(224,150,(u8*)gImage_Unlock);

					App.Data.SystemModeUpdate=App.Data.SystemMode;
					break;
				case SystemAlarmMode:
					//strPointer="Alarm";
					break;
			}
			App.Data.SystemModeUpdate=App.Data.SystemMode;
		}
	}  
}
void BatteryDisplay(u16 x,u16 y)
{
	static u32 count=0;
	static u8 bx;
	static u8 voltageTemp=0;
	u8	volt;
	
	if((FocusFormPointer!=&App.Menu.AlarmForm)&&(FocusFormPointer!=&App.Menu.WaitForm))
	{
		LCDDrawRectangle(x,y,30,13,WHITE);
		System.Device.Lcd.LCDColorFill(x+31,y+3,x+31+2,y+2+7,WHITE);
		if(0==DcDetPort)
		{
			voltageTemp=0;
			if(++count>=500) 
			{
				if(++bx>27)
				{
					bx=0;
					volt=(u8)(App.Data.Voltage*10);
					switch(volt)
					{
						case 41:
							bx=27;
							break;
						case 40:
							bx=23;
							break;
						case 39:
							bx=19;
							break;
						case 38:
							bx=15;
							break;
						case 37:
							bx=11;
							break;
						case 36:
							bx=3;
							break;
						case 35:
							bx=0;
							break;
					}
					System.Device.Lcd.LCDColorFill(x+1+bx,y+1,x+29,y+12,BLACK);
				}
				count=0;
			}
			System.Device.Lcd.LCDColorFill(x+2,y+2,bx+2+x,y+11,GREEN);
		}
		else 
		{
			volt=(u8)(App.Data.Voltage*10);
			if(voltageTemp!=volt)
			{
				voltageTemp=volt;
				System.Device.Lcd.LCDColorFill(x+1,y+1,x+29,y+12,BLACK);
				switch(voltageTemp)
				{
					case 41:
						bx=27;
						break;
					case 40:
						bx=23;
						break;
					case 39:
						bx=19;
						break;
					case 38:
						bx=15;
						break;
					case 37:
						bx=11;
						break;
					case 36:
						bx=3;
						break;
					case 35:
						bx=0;
						break;
				}
			}
			System.Device.Lcd.LCDColorFill(x+2,y+2,bx+2+x,y+11,GREEN);
		}
		
	}
}


u8 UserUpdataVal = 0xFF;
extern const unsigned char gImage_recordeIco[][2600];
extern const u8* RecordListTab[];
void RecordFormUpdata(u8 sel)
{
  GUI_SetFont(GUI_FONT_16B_1);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  GUI_SetColor(GUI_BLACK);
  EnterCritical();
  switch(sel)
  {
    case 1://录音
      LcdFillArea(160,160,240,220,WHITE);
      ImageDrawAlpah(202,160,(u8*)gImage_recordeIco[3]);
      GUI_DispStringHCenterAt((const char *)RecordListTab[2],202+36/2,200);//停止播放
    
      LcdFillArea(40,160,160,220,WHITE);
      ImageDrawAlpah(83,160,(u8*)gImage_recordeIco);
      GUI_DispStringHCenterAt((const char *)RecordListTab[3],83+36/2,200);
      break;
    case 2://停止录音
      LcdFillArea(40,160,160,220,WHITE);
      ImageDrawAlpah(83,160,(u8*)gImage_recordeIco[2]);
      GUI_DispStringHCenterAt((const char *)RecordListTab[1],83+36/2,200);
      break;
    case 3://播放
      LcdFillArea(40,160,160,220,WHITE);
      ImageDrawAlpah(83,160,(u8*)gImage_recordeIco[2]);
      GUI_DispStringHCenterAt((const char *)RecordListTab[1],83+36/2,200);//停止录音
    
      LcdFillArea(160,160,240,220,WHITE);
      ImageDrawAlpah(202,160,(u8*)gImage_recordeIco[1]);
      GUI_DispStringHCenterAt((const char *)RecordListTab[4],202+36/2,200);
      break;
    case 4://停止播放
      LcdFillArea(160,160,240,220,WHITE);
      ImageDrawAlpah(202,160,(u8*)gImage_recordeIco[3]);
      GUI_DispStringHCenterAt((const char *)RecordListTab[2],202+36/2,200);
      break;
    default:
      break;
  }
  UserUpdataVal = 0xff;
  ExitCritical();
}
void UserUpdate()
{
  RecordFormUpdata(UserUpdataVal);
}
/*******************************************************************************
* 描述	    : LCD屏数据更新，发现显示数据有变化，按行更新LCD屏内容
*******************************************************************************/
static void Update(void)
{	
	u16 x0,y0,x1,y1;
  static u8 update=0;
	BatteryDisplay(280,10);
  x0=FocusFormPointer->RectWrap.x0;
  y0=FocusFormPointer->RectWrap.y0;
  x1=FocusFormPointer->RectWrap.x1;
  y1=FocusFormPointer->RectWrap.y1;
	if(FormID!=FocusFormPointer->FormID)
	{
        
		FormID=FocusFormPointer->FormID;
		if(FocusFormPointer->ListPointer==NULL)
		{
      if(FocusFormPointer==&App.Menu.StandbyForm)  
      {
        EnterCritical();
				UpdateDisplay.Sec=0xFF;
        ImageDraw(0,30,(u8*)gImage_desktop);
        ExitCritical();
        update=1;
      }
      else
      {
        update=0;
        GUI_SetColor(FocusFormPointer->Color);
        EnterCritical();
        System.Device.Lcd.LCDColorFill(x0,y0,x1,y1,FocusFormPointer->Color);
        ExitCritical();                
      }
		} 

		memset(Rectime,0xFF,2);
		memset(LabelID,0xFF,20);
    memset(ArCheckID,0xFF,7);
		memset(IconID,0xFF,10);
		memset(FillAreaID,0xFF,10);
		memset(ListBoxID,0xFF,2);
		memset(TextBoxID,0xFF,6);
		memset(TextMode,0xffff,6);
		memset(CheckState,0xFF,30);
	}
    UpdateFillArea();

    UpdateLabel();

    ListBoxStaticUpdate();

    UpdateIcon();

    UpdateTextBox();

    UpdateGsmSignal();

    UpdateArCheckBox();

    UpdateAutoDigit();

    RecTimeUpdate();
  
    UserUpdate();
        
	if(update)
  {
    TimeDisplay(); 
  }
}

void ParseTime(void)
{
	if(App.Menu.FocusFormPointer==&App.Menu.StandbyForm)
	{
		if(UpdateDisplay.Sec!=App.Data.Sec)
    {
			UpdateDisplay.Sec=App.Data.Sec;
    }
	}
}
/*******************************************************************************
*
* 描述	    : 解析页面
*
*******************************************************************************/
static void Parse(Form * formPointer)
{           
//	Assert(formPointer != (Form *)0);

	FocusFormPointer = formPointer;

	ParseIcon();

	ParseLabel();

	ParseFillArea();
	
	ParseTextBox();

	ParseListbox();

	ParseArCheckBox();
    
  ParseAutoDigit();
	
	ParseTime();

	Update();
		
}
/*******************************************************************************
* 描述	    : 初始化页面
* 输入参数  : formPointer :窗体Form指针
*******************************************************************************/
static void InitForm(Form *formPointer)
{
 	formPointer->IconCount=0;
	formPointer->LabelCount=0;
	formPointer->FillAreaCount=0;
	formPointer->ListCount=0;
	formPointer->TextBoxCount=0;
	formPointer->LabelUpdate=0;
    
  formPointer->LabelPointer = (Label *)0;
	formPointer->FocusTextBoxPointer = (TextBox *)0;
	formPointer->ListPointer=(ListBox *)0;
	formPointer->TextBoxPointer = (void *)0;
	formPointer->IconPointer=(Icon *)0;
	formPointer->ListAddr=0;
	formPointer->LabelUpdate=0;
	formPointer->FillAreaPointer=(FillArea *)0;
  formPointer->AutoDigitPointer=(AutoDigit *)0;
	formPointer->FormID=0;
}
/********************************************************************************
*描述	：添加复框
********************************************************************************/
static void AddCheckBox(Form *formPointer,CheckBox *checkBoxPointer)
{
	checkBoxPointer->x=0;
	checkBoxPointer->y=0;
	checkBoxPointer->xSize=0;
	checkBoxPointer->ySize=0;
	checkBoxPointer->State=0;
	checkBoxPointer->SelectedIndexes=0;
}
/********************************************************************************************
* 描述	    : 窗体Form加载ListBox，加载时清空ListBox内容，加载后，需要对ListBox初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : ListBoxPointer: 控件ListBox指针
********************************************************************************************/
static void AddListBox(Form *formPointer,ListBox *listBoxPointer)
{
	listBoxPointer->x0=0;
	listBoxPointer->y0=0;
	listBoxPointer->x1=0;
	listBoxPointer->y1=0;
	listBoxPointer->Font=(const GUI_FONT GUI_UNI_PTR *)0;
	listBoxPointer->List=(ListboxList*)0;
	listBoxPointer->ID=0;
	listBoxPointer->ItemsPerpage=0;
	listBoxPointer->TopItem=0;
	listBoxPointer->TotalItems=0;
	listBoxPointer->TopIndexes=0;
	listBoxPointer->SelectedIndexes=0;
	listBoxPointer->selectedItem=0;
	listBoxPointer->Redraw=1;
	listBoxPointer->LastTimeSelected=0;
	listBoxPointer->LastTimeIndexes=0;
	listBoxPointer->TopIndexes=0;
	listBoxPointer->TopItem=0;
	listBoxPointer->Select=0;
	listBoxPointer->RimColor=0xFF000000;
	listBoxPointer->ItemsHeight=30;
	listBoxPointer->Addr=0;

	listBoxPointer->NextList=formPointer->ListPointer;
	formPointer->ListPointer=listBoxPointer;
}
/********************************************************************************************
* 描述	    : 窗体Form加载ArCheck
* 输入参数  : formPointer :窗体Form指针
*           : ListBoxPointer: 控件ListBox指针
********************************************************************************************/
static void AddArCheck(Form *formPointer,ArCheck *arCheckPointer)
{
  arCheckPointer->ID=0;
	arCheckPointer->x=0;
	arCheckPointer->y=0;
	arCheckPointer->Size=0;
  arCheckPointer->stringCount=0;
	arCheckPointer->Font=(const GUI_FONT GUI_UNI_PTR *)0;
	arCheckPointer->BackColor=0;
  arCheckPointer->PaintColor=0;
  arCheckPointer->TextMode=0;
  arCheckPointer->SelectedBackColor =0;
  arCheckPointer->SelectedPaintColor =0;
  arCheckPointer->State=0;//这个值需要去读取存储器中的值为准
  arCheckPointer->StringPointer=0;
  arCheckPointer->Addr=0;
  arCheckPointer->Update=1;

	arCheckPointer->NextPointer=formPointer->ArCheckPointer;
	formPointer->ArCheckPointer=arCheckPointer;
}
/***********************************************************************************
*描述     ：给列表框添加项目
*输入参数 ：listBoxPointer,当前的列表框
			name,要添加的字符串
			iconAdd,图标Flash中的地址
			CodeAddr,存放的是防区地址
***********************************************************************************/
static void AddList(ListBox * listBoxPointer,string name,u32 *iconAdd,u32 *codeAddr,u32 *nullPointer,u8 *path)
{
	ListboxList *listx;
	ListboxList *listTemp;

	listx=malloc(sizeof(ListboxList));
	if(listx==null) return;
	listx->State=0xFF;
	listx->Name=name;
	listx->IconAdd=iconAdd;
	listx->NullPointer=nullPointer;
	listx->FlashAddr=codeAddr;
  listx->Path=path;
	listx->Nextlist=null;
	if(listBoxPointer->List==null)
	{
		listx->Prevlist=null;
		listBoxPointer->List=listx;
	}
	else
	{
		listTemp=listBoxPointer->List;
		while(listTemp->Nextlist!=null)
		{
			listTemp=listTemp->Nextlist;
		}
		listx->Prevlist=listTemp;
		listTemp->Nextlist=listx;
	}
	listBoxPointer->TotalItems++;//记录总条目前
}
/************************** *****************************************************
* 描述	    : 添加列表项
* 输入参数  : list :要添加的列表名称
			  name[],列表项名称
			  number,总的条数
			  iconAddOrCheckBox,图标或者是复选框
*           : 
*******************************************************************************/
static void AddLists(ListBox *list,string name[] ,u8 usedIconum,u32 *iconAddOrCheckBox,u32 *codeAddr,u32 *nullPointer,u8 **path,u8 curIcoNum)
{
	u8 i,count=usedIconum;
	list->Change=0;
	list->TotalItems=0;
	for(i=0;i<count;i++)
  {
    if(curIcoNum<usedIconum)
    {
      AddList(list,name[i],&iconAddOrCheckBox[i],&codeAddr[i],&nullPointer[i],(u8*)path);	
    }
    else
    {
      AddList(list,name[i],&iconAddOrCheckBox[i],&codeAddr[i],&nullPointer[i],(u8*)(path+i*202));
    }
  }
}
/************************** *****************************************************
* 描述	    : 窗体Form加载Label，加载时清空Label内容，加载后，需要对Label初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : labelPointer: 控件Label指针
*******************************************************************************/
static void AddLabel(Form * formPointer, Label *labelPointer)
{
	//labelPointer->DataPointer = null;
	labelPointer->StringBlockPointer = (void *)0;	
	labelPointer->ID=0;
	labelPointer->Addr=0;
	labelPointer->Type=0;
  labelPointer->Update=1;
  labelPointer->DataPointer=NULL;
	labelPointer->NextLabelPointer = formPointer->LabelPointer;
	formPointer->LabelPointer = labelPointer;
}

/********************************************************************************
* 描述	    : 窗体Form加载Label，加载时清空FillArea内容，加载后，需要对FillArea初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : FillAreaPointer: 控件FillArea指针
********************************************************************************/
static void AddFillArea(Form * formPointer, FillArea *FillAreaPointer)
{
	FillAreaPointer->ID=0;
	FillAreaPointer->NextFillAreaPointer=formPointer->FillAreaPointer; 
	formPointer->FillAreaPointer=FillAreaPointer;
}
/*******************************************************************************
* 描述	    : 窗体Form加载IconLabel，加载时清空Label内容，加载后，需要对Label初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : labelPointer: 控件Label指针
*******************************************************************************/
static void AddIcon(Form * formPointer,Icon * iconPointer)
{
	iconPointer->StringPointer=(void *)0;
	iconPointer->IconPointer=(void *)0;
//	iconPointer->Font=(const GUI_FONT GUI_UNI_PTR *)0;
	iconPointer->NextIconPointer=formPointer->IconPointer;//指向下一个节点
	formPointer->IconPointer=iconPointer;//当前节点	
}
/*******************************************************************************
* 描述	    : 窗体Form加载TextBox，加载时清空TextBox内容，加载后，需要对TextBox初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : textBoxPointer: 控件TextBox指针
*******************************************************************************/
static void AddTextBox(Form * formPointer, TextBox *textBoxPointer)
{
	
	textBoxPointer->DataPointer = null;
	textBoxPointer->Font=null;
	textBoxPointer->ID=0;
	textBoxPointer->KeyCount=0;
	textBoxPointer->Leght=0;
	textBoxPointer->DataAddr=0;
	textBoxPointer->RimType=0;
	textBoxPointer->StringPointer = (const string *)0;
	textBoxPointer->Font=(const GUI_FONT GUI_UNI_PTR *)0;	

	textBoxPointer->NextTextBoxPointer = formPointer->TextBoxPointer;
	formPointer->TextBoxPointer = textBoxPointer;
}
/**************************************************************************************
*
* 描述        ：在当前页面添加焦点控件
*
***************************************************************************************/
static void AddFocus(Form *formPointer,FocusSturct *focusPointer)
{
    focusPointer->BoxType=0;
    focusPointer->FocusPointer=null;
    focusPointer->NextPointer=NULL;
    focusPointer->ParentPointer=NULL;
    
    focusPointer->NextPointer=formPointer->FocusPointer;
    formPointer->FocusPointer=focusPointer;
}
/***************************************************************************************
*
*描述     ：
*
***************************************************************************************/
static void AddAutoDigit(Form *formPointer,AutoDigit *autoDigitPointer)
{
    autoDigitPointer->x=0;
    autoDigitPointer->y=0;
    autoDigitPointer->xSize=0;
    autoDigitPointer->ySize=0;     
    autoDigitPointer->NextPointer=formPointer->AutoDigitPointer;
    formPointer->AutoDigitPointer=autoDigitPointer;
}
/*******************************************************************************
*
* 描述	    : 文本控件焦点切换处理程序，切换文本控件焦点
*
* 
*******************************************************************************/
static void SwitchTextBoxFocusUp(void)
{
    TextBox *textBoxPointer;
    Label   *labelPointer;
    ArCheck *checkBoxPointer;
    u8       i=1;

    if(FocusFormPointer->FocusPointer!=NULL)
    {
        
        switch(FocusFormPointer->FocusPointer->BoxType)
        {
            case TextBoxType:
                 textBoxPointer=(TextBox*)FocusFormPointer->FocusPointer->FocusPointer;
                 textBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
                break;
            case LabelBoxType:
                labelPointer=(Label*)FocusFormPointer->FocusPointer->FocusPointer;
                i=labelPointer->Update;
                i++;
                if(i==255) i=1;
                labelPointer->BackColor=GUI_WHITE;
                labelPointer->TextMode=GUI_TEXTMODE_NORMAL;
                labelPointer->Update=i;
                break;
            case ArCheckBoxType:
                checkBoxPointer=(ArCheck*)FocusFormPointer->FocusPointer->FocusPointer;
                i=checkBoxPointer->Update;
                i++;
                if(i==255) i=1;
                checkBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
                checkBoxPointer->PaintColor=GUI_BLACK;
                checkBoxPointer->BackColor=GUI_WHITE;
                checkBoxPointer->Update=i++;
                break;
        }
        FocusFormPointer->FocusPointer=FocusFormPointer->FocusPointer->ParentPointer;
        switch(FocusFormPointer->FocusPointer->BoxType)
        {
            
            case TextBoxType:
                 textBoxPointer=(TextBox*)FocusFormPointer->FocusPointer->FocusPointer;
                 textBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
                 FocusFormPointer->FocusTextBoxPointer=textBoxPointer;
                break;
            case LabelBoxType:
                labelPointer=(Label*)FocusFormPointer->FocusPointer->FocusPointer;
                i=labelPointer->Update;
                i++;
                if(i==255) i=1;
                labelPointer->TextMode=GUI_TEXTMODE_NORMAL;
                labelPointer->BackColor=GUI_BLUE;
                labelPointer->Update=i;
                break;
            case ArCheckBoxType:
                checkBoxPointer=(ArCheck*)FocusFormPointer->FocusPointer->FocusPointer;
                i=checkBoxPointer->Update;
                i++;
                if(i==255) i=1;
                checkBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
                checkBoxPointer->PaintColor=GUI_BLUE;
                checkBoxPointer->BackColor=GUI_WHITE;
                checkBoxPointer->Update=i;
                break;
        }       
        //FocusFormPointer->LabelUpdate=0xFE;//更新所有的標签
        return;
    }
	FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_TRANS;    
    FocusFormPointer->FocusTextBoxPointer = FocusFormPointer->FocusTextBoxPointer->NextTextBoxPointer;
    if(FocusFormPointer->FocusTextBoxPointer == null)
    {
        FocusFormPointer->FocusTextBoxPointer = FocusFormPointer->TextBoxPointer;
		FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
        return;
    }
	FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
}
/*********************************************************************************************************
*
*描述		：切换焦点控件（向下）
*输入参数	：无
*输出参数	：无
* 
*********************************************************************************************************/
static void SwitchTextBoxFocusDown(void)
{
    TextBox *textBoxPointer;
    Label   *labelPointer;
    ArCheck *checkBoxPointer;
    u8       i=1;
    
    
    if(App.Menu.FocusFormPointer->FocusPointer!=NULL)
    {
        
        switch(App.Menu.FocusFormPointer->FocusPointer->BoxType)
        {
            case TextBoxType://文本控件
                 textBoxPointer=(TextBox*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                 textBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
                break;
            case LabelBoxType://标签控件
                labelPointer=(Label*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                i=labelPointer->Update;
                i++;
                labelPointer->BackColor=GUI_WHITE;
                labelPointer->TextMode=GUI_TEXTMODE_NORMAL;
                labelPointer->Update=i;
                break;
            case ArCheckBoxType://复选框控件
                checkBoxPointer=(ArCheck*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                i=checkBoxPointer->Update;
                i++;
                checkBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
                checkBoxPointer->PaintColor=GUI_BLACK;
                checkBoxPointer->BackColor=GUI_WHITE;
                checkBoxPointer->Update=i++;
                break;
        }
        App.Menu.FocusFormPointer->FocusPointer=App.Menu.FocusFormPointer->FocusPointer->NextPointer;
        switch(App.Menu.FocusFormPointer->FocusPointer->BoxType)
        {
            
            case TextBoxType:
                 textBoxPointer=(TextBox*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                 textBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
                 App.Menu.FocusFormPointer->FocusTextBoxPointer=textBoxPointer;
                break;
            case LabelBoxType:
                labelPointer=(Label*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                i=labelPointer->Update;
                i++;
                labelPointer->TextMode=GUI_TEXTMODE_NORMAL;
                labelPointer->BackColor=GUI_BLUE;
                labelPointer->Update=i;
                break;
            case ArCheckBoxType:
                checkBoxPointer=(ArCheck*)App.Menu.FocusFormPointer->FocusPointer->FocusPointer;
                i=checkBoxPointer->Update;
                i++;
                checkBoxPointer->TextMode=GUI_TEXTMODE_TRANS;
                checkBoxPointer->PaintColor=GUI_BLUE;
                checkBoxPointer->BackColor=GUI_WHITE;
                checkBoxPointer->Update=i;
                break;
        }       
        //FocusFormPointer->LabelUpdate=0xFE;//更新所有的標签
        return;
    }
	App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_TRANS;    
	App.Menu.FocusFormPointer->FocusTextBoxPointer = App.Menu.FocusFormPointer->FocusTextBoxPointer->ParentTextBoxPointer;
	if(App.Menu.FocusFormPointer->FocusTextBoxPointer == null)
	{
		App.Menu.FocusFormPointer->FocusTextBoxPointer = App.Menu.FocusFormPointer->TextBoxPointer;
		App.Menu.FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
		return;
	}
    //else if(FocusFormPointer->FocusTextBoxPointer != null)
    //{
        //FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_TRANS;    
       // FocusFormPointer->FocusTextBoxPointer = FocusFormPointer->FocusTextBoxPointer->ParentTextBoxPointer;
    //}
	FocusFormPointer->FocusTextBoxPointer->TextMode=GUI_TEXTMODE_NORMAL;
}


/*************************************************************************************************************
*
*描述		：编辑文本
*输入参数	：key,按键值
*输出参数	：无
*
*************************************************************************************************************/
static void ModifyTextBoxData(u8 key)
{
	TextBox * textBoxPointer;
  void 	* dataPointer;
	u8		dataType;
	uint	s32;
	uint 	s32Max;
  uint 	s32Min;
	
	textBoxPointer = FocusFormPointer->FocusTextBoxPointer;
	if (textBoxPointer == null) 
  {
    return;
  }
  if((FocusFormPointer==&App.Menu.AutoSetupForm)||(FocusFormPointer==&App.Menu.ContactIDForm)) 
  {
    if(FocusFormPointer->FocusPointer->BoxType!=TextBoxType) 
    {
      return;
    }
  }    
        
	dataPointer = textBoxPointer->DataPointer;
  dataType = textBoxPointer->Type;
	if((KeyZero<=key)&&(KeyNine>=key))//0-9的按键
	{
		switch(dataType)
		{
			case GuiDataTypeByteDec:
				s32=*(u8*)(textBoxPointer->DataPointer);
				s32Max=(u8)(textBoxPointer->MaxDataPointer);
				s32Min=(u8)(textBoxPointer->MinDataPointer);		
				break;
			case GuiDataTypeShortDec:
				s32=*(u8*)(textBoxPointer->DataPointer);
				s32Max=(short)(textBoxPointer->MaxDataPointer);
				s32Min=(short)(textBoxPointer->MinDataPointer);			
				break;
			case GuiDataTypeUintHex:
				s32=*(u8*)(textBoxPointer->DataPointer);
				s32Max=(uint)(textBoxPointer->MaxDataPointer);
				s32Min=(uint)(textBoxPointer->MinDataPointer);	
				break;	
			case GuiDataTypeUintDec:
				s32=*(u16*)(textBoxPointer->DataPointer);
				s32Max=(u16)(textBoxPointer->MaxDataPointer);
				s32Min=(u16)(textBoxPointer->MinDataPointer);	
				break;
			case GuiDataTypeString:
				s32Max=(u8)(textBoxPointer->MaxDataPointer);
				s32Min=(u8)(textBoxPointer->MinDataPointer);				
				break;
		}
		switch(dataType)
		{
			case GuiDataTypeByteDec:
				{
					if((textBoxPointer->KeyCount>=3)&&(s32Max>100))
						textBoxPointer->KeyCount=0;
					if(textBoxPointer->KeyCount>=2)
						if((s32Max>10)&&(s32Max<100))
							textBoxPointer->KeyCount=0;
					if(s32Max<10) textBoxPointer->KeyCount=0;
					if(textBoxPointer->KeyCount==0)
					{
						s32=key-0x30;
						*(u8 *)dataPointer=s32;
						if(s32>s32Max)//超出最大
							*(u8 *)dataPointer=s32Max;
						if(s32<s32Min)
							*(u8 *)dataPointer=s32Min;
					}
					if(textBoxPointer->KeyCount>0)
					{
						s32*=10;
						s32+=key-0x30  ;
						if(s32>s32Max)//超出最大
							*(u8 *)dataPointer=s32Max;
						else if(s32<=s32Min)//超出最小
							*(u8 *)dataPointer=s32Min;
						else
							*(u8 *)dataPointer=s32;
					}
					textBoxPointer->KeyCount++;

				}break;
			case GuiDataTypeUintDec:
				{
					if(textBoxPointer->KeyCount>=4) 
						textBoxPointer->KeyCount=0;
					if(textBoxPointer->KeyCount==0)
					{
						s32=key-0x30;
						*(u16 *)dataPointer=s32;
					}
					if(textBoxPointer->KeyCount>0)
					{
						s32*=10;
						s32+=key-0x30  ;
						if(s32>s32Max)//超出最大
							*(u16 *)dataPointer=s32Max;
						else
							*(u16 *)dataPointer=s32;
					}
					textBoxPointer->KeyCount++;
				}break;
			case GuiDataTypeString:
				if(textBoxPointer->KeyCount>=50)
        {
					return;
        }
				else
				{
					if((textBoxPointer->KeyCount>=s32Max)||(textBoxPointer->KeyCount==0))
					{
						textBoxPointer->KeyCount=0;
						memset((u8*)dataPointer,NULL,30);
					}
					*((u8*)dataPointer+textBoxPointer->KeyCount)=key;
					textBoxPointer->KeyCount++;
					textBoxPointer->ID=~textBoxPointer->ID;
				}
				break;
		}
		textBoxPointer->Edit=0xFE;

	}
}
/*--------------------------------------------------------------------------------------------------
*
*描述		：初始化GUI各函数数端口
*输入参数	：无
*输出参数	：无
*
-------------------------------------------------------------------------------------------------*/
void InitGui(void)
{
	System.Gui.Parse = Parse;

	System.Gui.Form.AddIcon=AddIcon;

	System.Gui.Form.AddLabel = AddLabel;

	System.Gui.Form.AddList=AddList;
	
	System.Gui.Form.AddLists=AddLists;

	System.Gui.Form.AddListBox=AddListBox;

	System.Gui.Form.AddFillArea=AddFillArea;

	System.Gui.Form.AddTextBox = AddTextBox;

	System.Gui.Form.AddCheckBox=AddCheckBox;

	System.Gui.Form.Init = InitForm;    

  System.Gui.Form.SwitchTextBoxFocusDown 	= SwitchTextBoxFocusDown;
	
	System.Gui.Form.SwitchTextBoxFocusUp	= SwitchTextBoxFocusUp;

	System.Gui.Form.ModifyTextBoxData       = ModifyTextBoxData;
	
	System.Gui.Form.ListSearch=ListSearch;
	
  System.Gui.Form.AddArCheck=AddArCheck;
    
  System.Gui.Form.AddFocus = AddFocus;
    
  System.Gui.Form.AddAutoDigit=AddAutoDigit;
}

















