#ifndef __DEVICE_KEY_H
#define __DEVICE_KEY_H

/*--------------------------------------------???-------------------------------------*/
#include "stm32F10x.h"
#include "sys.h"
#include "System.h"
/*---------------------------------------------??--------------------------------------*/

/*--------------------------------------------???-------------------------------------*/

/*---------------------------------------------??--------------------------------------*/


/*---------------------------------------------??--------------------------------------*/
//??????
typedef enum
{
	KeyStateInit      		=0, //?????
	KeyStateShake     		=1, //????
	KeyStatePress     		=2, //????
	KeyStateLong      		=3, //????
	KeyStateRelease   		=4  //??
		
}KeyState;






//?????????
typedef enum
{
      KeyReturnDown     		=0x200, //??
      KeyReturnLong     		=0x100, //??

}KeyReturnState;

//?????
typedef __packed struct
{
	uint16_t KeyState; 				//???????
	uint16_t KeyTimeCount;		  	//????  ??????
	uint16_t LastKey;	 			//???????  ????????
	uint16_t KeyTemp ; 				//?????????

}KeyValueStruct;

typedef enum
{
	
	KeyUP=0x18,
	KeyEnter=0x0D,
	KeyDown=0x19,
	KeyReturn=0x1B,

	KeyOne=0x31,
	KeyTwo=0x32,
	KeyThree=0x33,
	KeyFour=0x34,
	KeyFive=0x35,
	KeySix=0x36,
	KeySeven=0x37,
	KeyEight=0x38,
	KeyNine=0x39,
	KeyStar=0x2A,
	KeyZero=0x30,
	KeyWell=0x23,

	KeyARM=		0xE5,
	KeyDisarm=	0xE6,
	KeyHome=	0xE7,
	KeySOS=		0xE8,
	KeyCall=	0xE9,
	KeyRing=	0xEA,
	KeyMenu=	0xEB,
  
  KeyValueNULL = 0xFF,
}EnumTouchKey;

typedef enum
{
	Key1   	=	0x00000001,
	Key2  	=	0x00200000,
	Key3	=	0x00000100,
	Key4	=	0x00000800,

	Key5	=	0x00000200,
	Key6	=	0x00080000,
	Key7    =	0x00040000,
	Key8	=	0x00002000,
	
	Key9    =	0x00400000,
	Key10	=	0x00020000,
	Key11	=	0x00100000,
	Key12	=	0x00008000,
	
	Key13	=	0x00010000,
	Key14	=	0x00000400,
	Key15	=	0x00001000,
	Key16	=	0x00004000,

}TouchKeyNumber;
//????????,00H?0AH,?????????????
typedef enum
{
	Channel1to2=0x00,
	Channel3to4,
	Channel5to6,
	Channel7to8,
	Channel9to10,
	Channel11to12,
	Channel13to14,
	Channel15to16,
	Channel17to18,
	Channel19to20
}SensitivityRegister;

//??????
typedef enum
{
	GeneralCTRL1=0x0B,
	GeneralCTRL2,
	
}GeneralRegister;
//???????
typedef enum
{
	CalibrationCTRL=0x0D
}SpeedRegister;

//?????????
typedef enum
{
	PortCTRL1=0x0e,
	PortCTRL2,
	PortCTRL3,
	PortCTRL4,
	PortCTRL5,
	PortCTRL6
}PortsRegister;

//???????
typedef enum
{
	CalHold1=0x14,
	CalHold2,
	CalHold3,
}ChannelCalibration;

//???????   
typedef enum
{
	KeyOut1=0x20,
	KeyOut2,
	KeyOut3,
}OutputRegsiter;
	
//????
typedef enum 
{
	Noise=0x17,
}NoiseControl;
/*---------------------------------------------??--------------------------------------*/




#endif

