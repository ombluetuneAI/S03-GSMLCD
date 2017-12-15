#ifndef __T9INPUT_H_
#define __T9INPUT_H_


#include "stm32F10x.h"
#include "sys.h"
#include "system.h"


typedef struct 
{
	u16 x;
	u16 y;
	
	u16 WordRecord;//
	u16 PressCount;//按下次数
	u16 Time;//计时
}InputStruct;


#endif

