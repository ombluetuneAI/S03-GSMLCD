#include "T9Input.h"


InputStruct *InputMethod;
//12������ť��3���ַ�����
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

//�����ż�
const u8 *T9SignTable[5][9]=
{
	{				   
		",",".","?", 
		"!",":","��", 
		";","��","()", 
	},
	{				   
		"+","-","*", 
		"/","=","��", 
		"��","%","��", 
	}, 
	{				   
		"��","��",">", 
		"<","��","��", 
		"��","��","|", 
	},
	{				   
		"��","^","��", 
		"&","����","����", 
		"{}","@","#",   
	},
	{				   
		"��","$","��", 
		"~","��","��", 
		"��","��","��",   
	},
};	

static void T9InputMethod(InputMethod *InputMethodPointer)
{
	
}



