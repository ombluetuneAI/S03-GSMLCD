#include "T9Input.h"


InputStruct *InputMethod;
//12个主按钮的3中字符串表
const u8 *T9StringTable[3][10]=
{   		
	{				   
		"abc","def",
		"ghi","jkl","mno",
		"pqrs","tuv","wxyz",
	},
	{				   
		"1","2","3",
		"4","5","6",
		"7","8","9","0", 
	}, 
	{				   
		"ABC","DEF",
		"GHI","JKL","MNO",
		"PQRS","TUV","WXYZ", 
	},
};	

//标点符号集
const u8 *T9SignTable[5][9]=
{
	{				   
		",",".","?", 
		"!",":","、", 
		";","…","()", 
	},
	{				   
		"+","-","*", 
		"/","=","±", 
		"≈","%","‰", 
	}, 
	{				   
		"√","≠",">", 
		"<","≥","≤", 
		"≯","≮","|", 
	},
	{				   
		"‖","^","∑", 
		"&","《》","『』", 
		"{}","@","#",   
	},
	{				   
		"￥","$","℃", 
		"~","β","α", 
		"∵","∴","※",   
	},
};	

static void T9InputMethod(InputMethod *InputMethodPointer)
{
	
}



