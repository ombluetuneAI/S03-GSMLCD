/*

*/


#ifndef __DEVICE_H
#define __DEVICE_H



typedef enum 
{
    KeyAdd                  = 0x00,
    KeyLongAdd              = 0x30,
    
    KeySub                  = 0x04,
    KeyLongSub              = 0x34,

    KeyAssistUp             = 0x01,
    KeyLongAssistUp         = 0x31,

    KeyAssistDown           = 0x05,
    KeyLongAssistDown       = 0x35,
    
    KeySave                 = 0x02,
    KeyLongSave             = 0x32,

    KeyTextBoxFocus         = 0x06,
    KeyLongFormFocus        = 0x36,
    
    KeyStart                = 0x03,
    KeyLongStart            = 0x33,
    
    KeyStop                 = 0x07,
    KeyLongStop             = 0x37
}KeyEnum;






typedef enum
{
    Systick10000,
    Systick1000,
    Systick100
}SystickEnum;

#endif

