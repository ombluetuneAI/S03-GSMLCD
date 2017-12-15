#ifndef __SYSTEMGUI_H
#define __SYSTEMGUI_H


/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "stm32F10x.h"
#include "sys.h"
#include "System.h"
/*---------------------------------------------����--------------------------------------*/
#define LCDXSize      320
#define LCDYSize      240
//#define  listHeight				30  //Ĭ���б�߶�Ϊ20
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
#define CarryLabel              0xFE    //Ԥ�ȴ���ַ
#define NumberLabel             0xFB    //������Label
#define ChangeCharLabel         0xF8
#define indirectLabel           0xF9    //���ת����ַ

#define IconNotName				0x0F
#define ListFont                GUI_FONT_20_1
#define ListNameFont            GUI_FONT_24_1
/*--------------------------------------------�궨��-------------------------------------*/

#define GuiFont		const GUI_FONT GUI_UNI_PTR*

#define GuiRECT		GUI_RECT
#define LcdColor	LCD_COLOR
/*---------------------------------------------����--------------------------------------*/

typedef enum
{
	chaeck1=0,
}CheckEnum;

/*---------------------------------------------����--------------------------------------*/


/*---------------------------------------------����--------------------------------------*/


//list�ṹ��.����ṹ
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
	byte 	LastTimeSelected;//��¼�ϴ�ѡ�����Ļλ��
	byte 	LastTimeIndexes;//��¼�ϴ�������
	byte 	Type;	//[bit7]:1,��Ҫ������������(������totalitems>itemsperpage);0,����Ҫ������.(��λ������Զ�����)
	byte	Change;
	byte	Select;//Select=1 ���Ǹ��б�ѡ��ֻ��ѡ��һ�� ,���б�Ϊ����ʱ Ϊ2 �����к�׺
	byte	ItemsHeight;//һ��ѡ����ĸ߶�
							//[bit6:0]:����	
	//byte State;//listbox״̬,[bit7]:������־;[bit6]:�����Ч�ı�־;[bit5:0]:��һ�ΰ��µı��.	
	byte 	ID;
	byte 	selectedItem;
	u16 	x0;
	u16 	y0;
	u16 	x1;
	u16 	y1;
	u16 	xSize;//��ѡ���ĳߴ������ ���ı��б��ƫ����
	u16 	ySize;
	u8	 	ItemsPerpage;//ҳ��ʾ��Ŀ
	u8	 	TopItem;//���˵���Ŀ
	u8	 	TopIndexes;
	u8	 	TotalItems;//����Ŀ
	
	uint 	BackColor;//������ɫ
	uint 	PaintColor;//������ɫ
	int  	TextMode;//�ı�ģʽ
	GuiFont Font;//����
	
	uint 	SelectedBackColor;//ѡ�еı�����ɫ
	uint 	SelectedPaintColor;//ѡ�еĻ�����ɫ
	byte 	SelectedIndexes;//ѡ�������
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
	u32  RimColor;//������ɫ
	u32	 FillColor;
	uint SelectedBackColor;//ѡ�еı�����ɫ
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
    GuiDataTypeString,              // ֱ����ʾ�ַ���
    GuiDataTypeSnString             // �����ʾ�ַ���(���ַ�����������)
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

//����
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


//ͼ���ǩ
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
//	//����
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




//�ı���ǩ
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
	uint	 			BackColor;//��ɫ
	LcdColor   	PaintColor;//����
	u16					MaxDataPointer;
	u16					MinDataPointer;
	u16         TextMode;//��ʾģʽ
	u32					DataAddr;
	GuiFont			Font;//����
	GuiRECT  		RectWrap;
  void				*DataPointer;
  const  string 		*StringPointer;//�ַ�ָ��
  struct TextBoxSelf 	* NextTextBoxPointer;//��һ������
	struct TextBoxSelf 	* ParentTextBoxPointer;//��һ������
	
}TextBox;

typedef __packed struct FoucsSturct
{
  u8      BoxType;
  void    * FocusPointer;
  struct  FoucsSturct	*ParentPointer;//��һ������
	struct  FoucsSturct *NextPointer;
}FocusSturct;

typedef enum
{
    TextBoxType=0x0A,
    LabelBoxType,
    ArCheckBoxType,   
}FocusType;
/***********************************************************************************

*����   ������һ����ѡ�򣬴����ֵ�

***********************************************************************************/
typedef __packed  struct ArCheck
{
  u8      ID;
  u16     x;
  u16     y;
	u16		  xString;
	u16 	  yString;
  u8      Size;//�ߴ磬������һ����
  u8      stringCount;
    
	uint 	  BackColor;//������ɫ
	uint 	  PaintColor;//������ɫ
	int  	  TextMode;//�ı�ģʽ
	GuiFont Font;//����
  const   string *StringPointer;
  void    *Addr;
	u32		  StateAddr;
  u8      Update;
	
	uint 	  SelectedBackColor;//ѡ�еı�����ɫ
	uint 	  SelectedPaintColor;//ѡ�еĻ�����ɫ   
  u8      Week;
  u8      State;//ѡ���е����ڣ���ֵΪ1ʱ��ʾѡ���Ǹ����ڣ�Ϊ0ʱ���ǲ�ѡ��
  struct  ArCheck *NextPointer;
}ArCheck;

//�������
typedef __packed  struct autoDigit
{
  u8      ID;
  u8      Value;
	u8		  bit;
  u16     x;
  u16     y;
  u16     xSize;
  u16     ySize;
  GuiFont Font;//����
  uint 	  BackColor;//������ɫ
  uint 	  PaintColor;//������ɫ
  int  	  TextMode;//�ı�ģʽ
  struct  autoDigit *NextPointer;
}AutoDigit;

//��ʽ
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
	Icon      	* IconPointer;//ͼ��	
	Label     	* LabelPointer;//��ǩ		
	TextBox   	* TextBoxPointer;//�ı���ǩ
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

