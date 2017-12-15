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
	u16 PressCount;//���´���
	u16 Time;//��ʱ
}InputStruct;


#endif

