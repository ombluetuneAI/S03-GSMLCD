#ifndef __SYSTEMGUI_H
#define __SYSTEMGUI_H


/*--------------------------------------------头文件-------------------------------------*/
#include "stm32F10x.h"
#include "sys.h"
#include "System.h"
/*---------------------------------------------常数--------------------------------------*/
#define LCDXSize      320
#define LCDYSize      240
//#define  listHeight				30  //默认列表高度为20
#define  xPosOffset				40
#define  yPosOffset				10
#define  yIconPosOffset			10
#define  xIconPosOffset			10

#define  IconList				0xff
#define	 CheckList				0xef
#define	 CheckListState			0xCF
#define	 TextList				0xDF
#define  SwitchList             0xBF
#define  LogStateList           0xAF

#define  Image2Lcd				0xFE

#define CheckBoxSelect			1
#define	CheckBoxNotSelect		0

#define  Redraws				1
#define  NotRedraw				0

#define DefaultLabel            0
#define CarryLabel              0xFE    //预先带地址
#define NumberLabel             0xFB    //纯数字Label
#define ChangeCharLabel         0xF8
#define indirectLabel           0xF9    //间接转换地址

#define IconNotName				0x0F
#define ListFont                GUI_FONT_20_1
#define ListNameFont            GUI_FONT_24_1
/*--------------------------------------------宏定义-------------------------------------*/

#define GuiFont		const GUI_FONT GUI_UNI_PTR*

#define GuiRECT		GUI_RECT
#define LcdColor	LCD_COLOR
/*---------------------------------------------变量--------------------------------------*/

typedef enum
{
	chaeck1=0,
}CheckEnum;

/*---------------------------------------------类型--------------------------------------*/


/*---------------------------------------------函数--------------------------------------*/


//list结构体.链表结构
typedef __packed struct 
{
	string 	Name;
  u8      *Path;
	u8		  State;
	u32   	*IconAdd;
	u32		  *FlashAddr;
	u32		  *NullPointer;
	u32 	  ID;
	void 	  * Prevlist;
	void 	  * Nextlist;
}ListboxList;




typedef __packed struct Listbox
{
	byte 	Redraw;
	byte 	LastTimeSelected;//记录上次选项的屏幕位置
	byte 	LastTimeIndexes;//记录上次索引号
	byte 	Type;	//[bit7]:1,需要画滚动条出来(条件是totalitems>itemsperpage);0,不需要画出来.(此位由软件自动控制)
	byte	Change;
	byte	Select;//Select=1 则是该列表复选框只能选择一个 ,当列表为文字时 为2 则是有后缀
	byte	ItemsHeight;//一个选择项的高度
							//[bit6:0]:保留	
	//byte State;//listbox状态,[bit7]:滑动标志;[bit6]:编号有效的标志;[bit5:0]:第一次按下的编号.	
	byte 	ID;
	byte 	selectedItem;
	u16 	x0;
	u16 	y0;
	u16 	x1;
	u16 	y1;
	u16 	xSize;//复选择框的尺寸或者是 纯文本列表的偏移量
	u16 	ySize;
	u8	 	ItemsPerpage;//页显示条目
	u8	 	TopItem;//顶端的条目
	u8	 	TopIndexes;
	u8	 	TotalItems;//总条目
	
	uint 	BackColor;//背景颜色
	uint 	PaintColor;//画笔颜色
	int  	TextMode;//文本模式
	GuiFont Font;//字体
	
	uint 	SelectedBackColor;//选中的背景颜色
	uint 	SelectedPaintColor;//选中的画笔颜色
	byte 	SelectedIndexes;//选择的索引
	u32		Addr;
	uint 	RimColor;		
	ListboxList *List;
	struct Listbox *NextList;
}ListBox;

typedef __packed  struct CheckBox
{
	u16  x;
	u16  y;
	u16  xSize;
	u16  ySize;
	u8   ID;
	u32  RimColor;//画笔颜色
	u32	 FillColor;
	uint SelectedBackColor;//选中的背景颜色
	uint SelectedIndexes;
	uint State;
	struct CheckBox *NextCheckBox;
}CheckBox;

typedef enum
{
	ChartTypeSignal=0,
	ChartTypeBatteAmount,
}ChartType;



typedef enum 
{
    GuiDataTypeByteDec,
    GuiDataTypeShortDec,
    GuiDataTypeUshortDec,
    GuiDataTypeIntDec,
    GuiDataTypeUintDec,
    GuiDataTypeFloatDec,
    GuiDataTypeUshortHex,
    GuiDataTypeUintHex,
    GuiDataTypeString,              // 直接显示字符串
    GuiDataTypeSnString             // 序号显示字符串(与字符串数组配套)
}GuiDataType;


typedef __packed struct FillAreaSelf
{
	byte  		ID;
//	u16 		  x;
//	u16 		  y;
	u32  		  PaintColor;
	GuiRECT	  RectWrap;
	struct FillAreaSelf * NextFillAreaPointer;
}FillArea;

//文字
typedef __packed struct LabelSelf
{
	byte 	  ID;
	byte 	  Count;
  byte    Update;
  u16 	  X;
  u16	    Y;
	u32		  BackColor;
	u32		  PaintColor;
	u32 	  Addr;
	u8		  Type;
  void    *DataPointer;
	int     TextMode;
  int     TextAlign;
	GuiFont Font;
	
  const string * StringBlockPointer;
  struct LabelSelf * NextLabelPointer;	
}Label;


//图标标签
typedef __packed struct IconSelf
{
  byte 				      ID;
  u16 				      x;
	u16 				      y;
	u8 				        *IconPointer;
	u8 				        *StringPointer;
	u32		    		    Add;
	struct IconSelf 	* NextIconPointer;
	byte 				      Type;
//	u16 				      x;
//	u16 				      y;
//	u16 				      xSize;
//	u16               ySize;
//	u16 				      RectX0;
//	u16 				      RectY0;
//	u16 				      RectX1;
//	u16 				      RectY1;
//	void 				      *BitmapPointer;
//	//字体
//	GuiFont			      Font;
//	LCD_COLOR         PaintColor;
//	int         		  TextMode;	
//	void 				      *StringPointer;
//	uint	 			      BackColor;
//	u32		    		    Add;
//	struct IconSelf 	* NextIconPointer;
//	byte 				      ID;
//	byte 				      Type;
}Icon;




//文本标签
typedef __packed struct TextBoxSelf
{
  u16 				x;
  u16 				y;
	byte 				ID;
	byte				Type;
	byte				RimType;
	byte				Edit;
	byte				Leght;
	byte				KeyCount;
	uint	 			BackColor;//底色
	LcdColor   	PaintColor;//画笔
	u16					MaxDataPointer;
	u16					MinDataPointer;
	u16         TextMode;//显示模式
	u32					DataAddr;
	GuiFont			Font;//字体
	GuiRECT  		RectWrap;
  void				*DataPointer;
  const  string 		*StringPointer;//字符指针
  struct TextBoxSelf 	* NextTextBoxPointer;//下一个链表
	struct TextBoxSelf 	* ParentTextBoxPointer;//上一个链表
	
}TextBox;

typedef __packed struct FoucsSturct
{
  u8      BoxType;
  void    * FocusPointer;
  struct  FoucsSturct	*ParentPointer;//上一个链表
	struct  FoucsSturct *NextPointer;
}FocusSturct;

typedef enum
{
    TextBoxType=0x0A,
    LabelBoxType,
    ArCheckBoxType,   
}FocusType;
/***********************************************************************************

*描述   ：任意一个复选框，带文字的

***********************************************************************************/
typedef __packed  struct ArCheck
{
  u8      ID;
  u16     x;
  u16     y;
	u16		  xString;
	u16 	  yString;
  u8      Size;//尺寸，长宽是一样的
  u8      stringCount;
    
	uint 	  BackColor;//背景颜色
	uint 	  PaintColor;//画笔颜色
	int  	  TextMode;//文本模式
	GuiFont Font;//字体
  const   string *StringPointer;
  void    *Addr;
	u32		  StateAddr;
  u8      Update;
	
	uint 	  SelectedBackColor;//选中的背景颜色
	uint 	  SelectedPaintColor;//选中的画笔颜色   
  u8      Week;
  u8      State;//选择中的星期，该值为1时表示选择是该星期，为0时则是不选择
  struct  ArCheck *NextPointer;
}ArCheck;

//多变数字
typedef __packed  struct autoDigit
{
  u8      ID;
  u8      Value;
	u8		  bit;
  u16     x;
  u16     y;
  u16     xSize;
  u16     ySize;
  GuiFont Font;//字体
  uint 	  BackColor;//背景颜色
  uint 	  PaintColor;//画笔颜色
  int  	  TextMode;//文本模式
  struct  autoDigit *NextPointer;
}AutoDigit;

//样式
typedef __packed  struct
{
	byte	  	  Update;
	byte	 	    FormID;
  byte        ArCheckCount;
	byte	  	  IconCount;
	byte	  	  LabelCount;
	byte		    AutoDigitCount;
	byte      	FillAreaCount;
	byte      	ListCount;
	byte		    TextBoxCount;
	byte 		    LabelUpdate;
	int         TextMode;
	u32			    ListAddr;
	GuiRECT  	  RectWrap;
	GuiFont		  Font;
	LcdColor   	Color;	
	Icon      	* IconPointer;//图标	
	Label     	* LabelPointer;//标签		
	TextBox   	* TextBoxPointer;//文本标签
	ListBox   	* ListPointer;
	FillArea  	* FillAreaPointer;		
	TextBox   	* FocusTextBoxPointer;
	TextBox		  * DefaultFocusTextBoxPointer;
  ArCheck     * ArCheckPointer;
  AutoDigit   * AutoDigitPointer; 
    
  FocusSturct * FocusPointer;
  void        * DefaultFocus;
}Form;


extern void InitGui(void);


#endif

