/*
********************************************************************************

********************************************************************************
*文件名   : system.h
*作用     : 系统库全局头文件
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define KeyLedID	      1
#define RestTimeID	    2
#define GsmTimer	      3  //定时器ID  用于判断Rxd数据超时
#define CallTimer	      4
#define CallTimer2	    5
#define AlarmTimeID     6
#define ArmTimeID       7
#define LcdLedID	      8
#define SIMCheckID      9
#define AlarmTime       10
#define	DelayProcessID  11
#define CallHangUpTime  12//挂机时间
#define RFOpenTimeID	  13
#define SendTxTimeID	  14
#define SignalQuality	  15
#define LoudSpeaker     0
/*
1、类型定义兼容C#，尽可能用C#标准，常用的为int
2、char在C#中为16bit的unicode，keil C下为8bit
*/
//8bit
//typedef char                sbyte;	    /*char Keil4默认定义为byte，若要带符号，需要更改keil设置*/
typedef unsigned char       byte;       
//16bit
//typedef short             short
typedef unsigned short      ushort;
//32bit
//typedef int               int;
typedef unsigned int        uint;

//字符串
typedef char *              string;

typedef unsigned char       bool;

typedef void (*function)(void);
#define Function(address)   ((function)(address))()

typedef enum
{
	MessageDateType		=	0x0A,
	MessageAutoType		=   0x09,
	MessageCallRing		=	0x08,
    MessageCall         =   0x07,
	MessageCID			=	0x0B,
    MessageSendSMS      =   0x06,
	MessageCallTDMF		=   0x05,
    MessageKeyType      =   0x04,
    MessageTimerType    =   0x02,
	MessageGsmType		=   0x03,
	MessageWireless		=   0x01,
    /*请填充类型*/
}MessageEnum;
 				   
#include "device.h"  
#include "os.h"
#include "gui.h"
#include "Device_Voice.h"
#include "Device_LED.h"
#include "Device_Lcd.h"
#include "device_adc.h"
#include "device_key.h"
#include "device_time.h"
#include "device_GSM.h"
#include "device_RTC.h"
#include "SystemGUI.h"
#include "device_Flash.h"
#include "device_SpiFlash.h"
#include "Device_DMA.h"
#include "ScrollBar_.h"
#include "data.h"
#include "Device_Call.h"
#include "bmp.h"
#include "SEGGER_RTT.h"

#define Open_RTT_printf   1 
#define RTT_printf        SEGGER_RTT_printf

#define true                        1
#define false                       0

#define on                          1
#define off                         0

#define ok                          1
#define yes                         1
#define no                          0

#define null                        ((void *) 0)                    // 空指针
#define invalid                     0xFF                            // 无效值
#define MainClock                   72000000                        // 系统时钟频率
#define IdentifyNumberAddress       0x1FFFF7E8                      // 芯片ID号

#define SetBit(data, offset)        ((data) |= 1U << (offset))      // 置位
#define ResetBit(data, offset)      ((data) &= ~(1U << (offset)))   // 复位
#define GetBit(data, offset)        (((data) >> (offset)) & 0x01)   // 获取位

#define Byte0(data)                 ((byte *)(&(data)))[0]
#define Byte1(data)                 ((byte *)(&(data)))[1]
#define Byte2(data)                 ((byte *)(&(data)))[2]
#define Byte3(data)                 ((byte *)(&(data)))[3]

#define Ushort0(data)               ((ushort *)(&(data)))[0]
#define Ushort1(data)               ((ushort *)(&(data)))[1]

#define Byte(data)                  *((byte *)(&data)) 
#define Ushort(data)                *((ushort *)(&data)) 
#define Uint(data)                  *((uint *)(&data)) 
#define Float(data)                 *(float *)(&data)

#define pByte(address)              *((byte *)(address)) 
#define pUshort(address)            *((ushort *)(address)) 
#define pUint(address)              *((uint *)(address)) 
#define pFloat(address)             *(float *)(address)

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0
 				  
extern uint Empty;
extern void Dummy(void);
extern ushort CriticalNesting;
#define	EnterCritical()		__disable_irq(); CriticalNesting++;
#define ExitCritical()		if(--CriticalNesting == 0){__enable_irq();}

#define EnableIrq()         __enable_irq();
#define DisableIrq()        __disable_irq();

extern uint RomBase;
extern uint RamBase;
extern DataStruct * AppDataPointer;

extern void Delay(int times);

extern void DelayUs(int times);

extern void DelayMs(int times);

extern byte HexToAscii(byte hex);

extern void PostMessage(MessageEnum message, uint data);

extern const unsigned char gImage_desktop[134408];
extern const unsigned char gImage_home[5008];
extern const unsigned char gImage_Arm[5008];
extern const unsigned char gImage_Unlock[5008];

/*******************************************************************************
* 结构体名	: SystemStruct
* 描述	    : msOS中最重要的一个结构体，封装了System层的所有接口
*******************************************************************************/
typedef struct
{
    void (*Init)(void);
    void (*SystemDefaultsParameter)(void);
    struct Device
    {      
		struct KeyLED
		{
			void (*KeyLedConfig)(uint8_t Data);
			void (*KeyCloseLed)(void);
		}KeyLed;

		struct Gsm
		{
			u8 (*GsmATSend)(u8 *strPointer,u8 *ackPointer,u16 waitTime);
			void (*GsmDMAStrin)(char* fmt,...);
			void (*GsmSendSMS)(u8 *number,u8 *text);
			u8 (*GsmCall)(const char *PhoneNumber);
			u8 (*GsmRingTimes)(u8 ringTime,u8* ackPointer,u16 waitTime);
			
		}Gsm;
		struct Lcd
		{
			void (*LCDDrawingBitmap)(Bitmap *BitmapPointer);
			void (*LcdClear)(u16 color);
			u16	 (*RGB888toGRB565)(u32 color);
			void (*LCDColorFill)(uint16_t XStart,uint16_t YStart,uint16_t XEnd,uint16_t YEnd,uint16_t Color);
			void (*LcdDrawPoint)(uint16_t X,uint16_t Y,uint16_t Color);
			void (*LCDDrawRectangle)(uint16_t XSatrt, uint16_t YStart, uint16_t XEnd, uint16_t YEnd,uint16_t Color);
			u16  (*LcdReadPoint)(u16 x,u16 y);
			void (*ImageDraw)(u16 x,u16 y,u8 *filename);
			void (*ImageLocalDraw)(u16 x,u16 y,u16 wide,u16 high,u8 *pointer);
		}Lcd;
		struct Voice
		{
            void (*Siren)(u8 count);
		}Voice;
		struct Adc
		{
			void (*Register)(AdcChannelEnum adcChannel, float * dataPointer);
		}Adc;
        struct Wireless
        {
			u32 (* WirelessDecode)(void);
			void (* WirelessSend)(uint8_t *SendBufferPointer);
        }Wireless;
       
        struct Usart1
        {
            void (*Open)(void);
            void (*Close)(void);
            bool (*WriteByte)(byte data);
            void (*Write)(byte * dataPointer, int sum);
        }Usart1;
        
        struct Timer
        {
            bool (*Start)(int id, TimerHandleModeEnum mode, int times, function registerFunction);
            bool (*Stop)(int id); 
        }Timer;

        struct Storage
        {
            struct Parameter
            {
                void (*ReadAll)(void);
                bool (*Write)(void * dataPointer);  
				void (*StorageReadData)(u32 addr , u8 *bufferPoiner , u16 size);
				void (*StorageWriteData)(u32 addr , u16 *bufferPoiner , u16 size);
            }Parameter;
            struct Log
            {
                void (*Erase)(void);
                char * (*Read)(int sequence);
                bool (*Write)(char *fmt, ...);
            }Log;
            void (*ReadData)(u32 addr , u8 *bufferPoiner , u16 size);
            void (*WriteData)(u32 addr , u16 *bufferPoiner , u16 size);
        }Storage;

        struct Systick
        {
            bool (*Register)(SystickEnum type, function registerFunction);
        }Systick;
		struct 
		{
			u8 (*RTCDateSet)(TimeSetStruct *timeSet);
			u8 (*RTCGetTime)(void);
		}DateSet;
		struct Telephone
		{
			void(*Call)(u8 *number);
		}Telephone;
        
    }Device;

    struct OS
    {
        void (* CreateLogicTask)(function taskPointer);
        void (* CreateMenuTask)(function taskPointer);
        void (* Start)(void);
        bool (* PostMessageQueue)(uint message);
        uint (* PendMessageQueue)(void);
        void (* DelayMs)(int times); 
    }OS;
    struct Gui
    {
        void (*Parse)(Form * formPointer);
		void *IconPointer;
        struct
        {
            void (* AddLabel)(Form * formPointer, Label *labelPointer);
			void (* AddListBox)(Form * formPointer,ListBox *listBoxPointer);
			void (* AddList)(ListBox * listBoxPointer,string name,u32 *IconAdd,u32 *codeAddr,u32 *nullPointer,u8 *path);
			void (* AddLists)(ListBox *list,string name[] ,u8 Size,u32 *iconAdd,u32 *codeAddr,u32 *nullPointer,u8 **path,u8 curIcoNum);
			void (* AddFillArea)(Form *formPointer,FillArea *FillAreaPointer);
			void (* AddCheckBox)(Form *formPointer,CheckBox *checkBoxPointer);
			void (* AddIcon)(Form * formPointer,Icon *IconPointer);
            void (* AddTextBox)(Form * formPointer, TextBox *textBoxPointer);
            void (* Init)(Form *formPointer);
            void (*SwitchTextBoxFocusDown)(void);
			void (*SwitchTextBoxFocusUp)(void);
            void (*SetTextBoxData)(KeyEnum key);
			void (*ModifyTextBoxData)(u8 key);
            void (* AddArCheck)(Form *formPointer,ArCheck *arCheckPointer);
            void (*AddFocus)(Form *formPointer,FocusSturct *focusPointer);
            void (*AddAutoDigit)(Form *formPointer,AutoDigit *autoDigitPointer);
			ListboxList* (*ListSearch)(ListboxList *listx,u16 index);
        }Form;
    }Gui;
	struct emWin
	{
//	    const GUI_FONT GUI_UNI_PTR (*emWinFontSet)(const GUI_FONT GUI_UNI_PTR * pNewFont);
        void (*emWinBackColor)(unsigned long Color);
        void (*emWinPointColor)(unsigned long Color);
        void (*emWinClear)(void);
		void (*emWinDispStringAt)(const char GUI_UNI_PTR * s, int x, int y);	
        void (*emWinDrawBitmap)(const GUI_BITMAP GUI_UNI_PTR * pBM, int x0, int y0);
		void (*emWinIcon)(const GUI_BITMAP GUI_UNI_PTR * pBM);
		U8 	 (*emWinSetPenSize)(U8 Size);
	}emWin;
	struct Icon
	{

		void *IconAbout;
 		void *IconCall;
		void *IconSetup;
		void *IconPhone;
		void *IconSecurity;
		void *IconSensors;
		void *IconJournal;
		void *IconVoice;
		void *IconFingerint;

	}Icon;
 	struct SystemApp
 	{
		void (*SystemSet)(u8 *textPointer);
		void (*DeleteAllNumber)(void);
		void (*DeleteAllRemote)(void);
        void (*DeleteAllDetector)(void);
		void (*ScreenSet)(void);
        void (*BmpDecode)(u8 *fileName,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode);
 	}SystemApp;
}SystemStruct;

extern SystemStruct System;


void InitSystick(void);
#endif 
