
/*--------------------------------------------???-------------------------------------*/
#include "device_key.h"
#include "device_IIC.h"
#include "device_Voice.h"
#include "App.h"
/*---------------------------------------------??--------------------------------------*/


/*--------------------------------------------???-------------------------------------*/

#define KeyLongThreshold 				80  	//??????
#define KeyBatterThreshold 	  			100   	//??????
#define KeyValueNULL 					0xFF  	// ??,?????,???? 


#define TS20Add							0xF4    //TS20??
#define TS20Sensitivity					0xFF	//?????

/*---------------------------------------------??--------------------------------------*/

static KeyValueStruct KeyStruct;

/*---------------------------------------------??--------------------------------------*/


/*---------------------------------------------??--------------------------------------*/
static void KeyPortInit(void)
{
	
}



/*-----------------------------------------------------------------------------------------------
--??      :     ????????
--??      :     ?
--???    :     ?
-----------------------------------------------------------------------------------------------*/
void TouchKeyInit(void)
{
	KeyStruct.KeyState=0;
	IICInit();
	DelayMs(100);
	IICStart(1);
	IICSendByte(TS20Add,1);
	IICWaitAck(1);
	IICStop(1);
	DelayUs(10);
	
	IICStart(1);
	IICSendByte(TS20Add,1);
	IICWaitAck(1);
	
//?????
	IICSendByte(Channel1to2,1);
	IICWaitAck(1);
	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	

	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	

	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	

	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	
	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);

	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	
	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	
	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	
	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	
	IICSendByte(TS20Sensitivity,1);
	IICWaitAck(1);
	
	IICStop(1);
	DelayUs(10);
	
//??	
	IICStart(1);
	IICSendByte(TS20Add,1);
	IICWaitAck(1);
	
	IICSendByte(GeneralCTRL1,1);
	IICWaitAck(1);
	IICSendByte(0x42,1);
	IICWaitAck(1);
	
	IICStop(1);
	DelayUs(10);
	
//?????????
	IICStart(1);
	IICSendByte(TS20Add,1);
	IICWaitAck(1);
	
	IICSendByte(PortCTRL1,1);
	IICWaitAck(1);
	IICSendByte(0x00,1);
	IICWaitAck(1);
	
	IICSendByte(0x00,1);
	IICWaitAck(1);
	
	IICSendByte(0x00,1);
	IICWaitAck(1);
	
	IICSendByte(0x00,1);
	IICWaitAck(1);
	
	IICSendByte(0x00,1);
	IICWaitAck(1);
	
	IICSendByte(0x00,1);
	IICWaitAck(1);
	IICStop(1);
	DelayUs(20);
	
	
//??
	IICStart(1);
	IICSendByte(TS20Add,1);
	IICWaitAck(1);
	IICSendByte(CalHold1,1);
	IICWaitAck(1);
	
	IICSendByte(0x00,1);
	IICWaitAck(1);
	
	IICSendByte(0x00,1);
	IICWaitAck(1);

	IICSendByte(0x00,1);
	IICWaitAck(1);
	
	IICStop(1);
	DelayUs(10);
	
	IICStart(1);
	IICSendByte(TS20Add,1);
	IICWaitAck(1);
	IICSendByte(GeneralCTRL2,1); 
	IICWaitAck(1);

	IICSendByte(0x12,1);
	IICWaitAck(1);
	IICStop(1);
	DelayUs(10);
}
/*-----------------------------------------------------------------------------------------------
--??      :     ?????????
--??      :     Addr: ?????
--???    :     ??????
-----------------------------------------------------------------------------------------------*/
static uint8_t TouchKeyRead(uint8_t Addr)
{
	uint8_t res;
	
	IICStart(1);
	IICSendByte(TS20Add,1);
	IICWaitAck(1);
	
	IICSendByte(Addr,1);
	IICWaitAck(1);
	IICStop(1);
	DelayUs(10);

	IICStart(1);
	IICSendByte(TS20Add|0x01,1);
	IICWaitAck(1);
	res=IICReadByte(0x00,1);
	IICStop(1);
	DelayUs(10);
	
	return res;
}

/*-----------------------------------------------------------------------------------------------
--??      :     ?????????
--??      :     Addr: ?????
--???    :     ?????
-----------------------------------------------------------------------------------------------*/
static uint8_t TouchGetKey(void)
{
	uint32_t res=0;
	uint8_t keyValue;
	
	
    keyValue=TouchKeyRead(KeyOut3);
    if(0x20!=(keyValue&0x20))
    {
    	keyValue=TouchKeyRead(KeyOut1);//??1-7
    	res=keyValue;
    	res<<=8;
    	keyValue=TouchKeyRead(KeyOut2);//??8-15
    	res|=keyValue;
    	res<<=8;
    	keyValue=TouchKeyRead(KeyOut3);//??16-20 ????
    	res|=keyValue;
		switch(res)
		{
			case Key1	: 
				return KeyOne;
			case Key2	: 
				return KeyTwo;
			case Key3	: 
				return KeyThree;
			case Key4	: 
				if(&App.Menu.StandbyForm==App.Menu.FocusFormPointer)
					return KeyARM;
				else
					return KeyUP;			
			case Key5	: 
				return KeyFour;
			case Key6	: 
				return KeyFive;
			case Key7	: 
				return KeySix;	
			case Key8	:
				if(&App.Menu.StandbyForm==App.Menu.FocusFormPointer)
					return KeyHome;
				else	
					return KeyDown;
			case Key9	: 
				return KeySeven;
			case Key10	: 
				return KeyEight;
			case Key11	: 
				return KeyNine;
			case Key12	: 
				return KeyDisarm;						
			case Key13	: 
				return KeyMenu;
			case Key14	: 
				return KeyZero;
			case Key15	: 
				return KeyReturn;	
			case Key16	: 
				return KeyEnter;			
			default: return KeyValueNULL;
		}
			
    }

    return KeyValueNULL;

}




/*-----------------------------------------------------------------------------------------------
--??      :     ??????(??????????),???????????????????
--??      :     ?
--???    :     ????????
-----------------------------------------------------------------------------------------------*/
static uint16_t KeyGetStateAndValue(void)
{
	KeyStruct.KeyTemp=TouchGetKey();	

	switch(KeyStruct.KeyState)
	{
		case KeyStateInit:  
		{
			if(KeyValueNULL!=(KeyStruct.KeyTemp))
			{
				KeyStruct.KeyState = KeyStateShake;
        KeyStruct.LastKey = KeyStruct.KeyTemp;
        KeyStruct.KeyTemp = KeyValueNULL;
			}

		} break;

		case KeyStateShake: 
		{
      if(KeyStruct.LastKey==KeyStruct.KeyTemp)
      {
        KeyStruct.KeyState = KeyStatePress; 
      }
      else 
      {
        KeyStruct.KeyState = KeyStateInit;
      }
      KeyStruct.KeyTemp = KeyValueNULL;
		} break;

		case KeyStatePress: 
		{
			if(KeyValueNULL!=(KeyStruct.KeyTemp))
			{
				KeyStruct.KeyTemp |= KeyReturnDown;
        KeyStruct.LastKey = KeyStruct.KeyTemp;
				KeyStruct.KeyState = KeyStateLong;       
			}
			else
			{
				KeyStruct.KeyState = KeyStateInit; 
			}
      KeyStruct.KeyTemp = KeyValueNULL;
		} break;

		case KeyStateLong:	
		{
			if(KeyValueNULL!=(KeyStruct.KeyTemp)) 
			{
				if(++KeyStruct.KeyTimeCount > KeyLongThreshold) 
				{
					KeyStruct.KeyTimeCount = 0 ; 
					KeyStruct.KeyTemp |= KeyReturnLong; 
          KeyStruct.LastKey = KeyStruct.KeyTemp;
          KeyStruct.KeyState = KeyStateRelease;
				}
			}
			else
			{
				KeyStruct.KeyTimeCount = 0 ;
				KeyStruct.KeyState = KeyStateRelease; 
			}
      KeyStruct.KeyTemp = KeyValueNULL;
		} break;
    
		case KeyStateRelease :  //??????????
		{
			KeyStruct.KeyTemp = KeyStruct.LastKey ;
			KeyStruct.KeyState = KeyStateInit;
		} break;
		default :    
			return(KeyValueNULL); 
	}
	return (KeyStruct.KeyTemp);

}


/*-------------------------------------------------------------------------------
--??	    : ????100/S,?10mS?????????,????????,
			  ??????????LogicTask???????
-------------------------------------------------------------------------------*/
void KeySystick100Routine(void) 
{
	u16 scan;
	u8 	temp;
	
	scan=KeyGetStateAndValue();
	temp=Byte0(scan);
	if(temp!=KeyValueNULL)
  {
		PostMessage(MessageKeyType, scan);
  }

}




























