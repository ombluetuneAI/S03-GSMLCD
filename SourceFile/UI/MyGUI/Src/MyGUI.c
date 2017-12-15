

/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "MyGUI.h"
#include "TFT.h"
#include "ASCII8X16.h"
/*---------------------------------------------����--------------------------------------*/

/*--------------------------------------------�궨��-------------------------------------*/

/*---------------------------------------------����--------------------------------------*/


/*---------------------------------------------����--------------------------------------*/



/*---------------------------------------------����--------------------------------------*/




/*----------------------------------------------------------------------------------------------
--����      :     ����ALPHA BLENDING�㷨
--����      :     SourceColor : Դ��ɫ
                  Targetcolor : Ŀ����ɫ
                  Transparent : ͸���̶�(0~32)
--����ֵ    :     ��Ϻ����ɫ
----------------------------------------------------------------------------------------------*/
uint16_t GUITransparent656(uint16_t SourceColor,uint16_t Targetcolor,uint8_t Transparent)
{
	u32 SourceColor2;
	u32 Targetcolor2;	 
	//Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
	SourceColor2=((SourceColor<<16)|SourceColor)&0x07E0F81F;
	Targetcolor2=((Targetcolor<<16)|Targetcolor)&0x07E0F81F;   

	Targetcolor2=((((Targetcolor2-SourceColor2)*Transparent)>>5)+SourceColor2)&0x07E0F81F;
	return (Targetcolor2>>16)|Targetcolor2;  
}  

/*----------------------------------------------------------------------------------------------
--����      :     ��ָ���������alphablend
--����      :     x,y,width,height:��������
                  color:alphablend����ɫ
                  color:alphablend����ɫ
--����ֵ    :     ��Ϻ����ɫ
----------------------------------------------------------------------------------------------*/
void GUITransparentBlock(u16 X,u16 Y,u16 width,u16 height,u16 color,u8 aval)
{
	u16 i,j;
	u16 tempcolor;
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
                  tempcolor=LCDReadPointRGB(X,Y+j);
                  tempcolor=GUITransparent656(tempcolor,color,aval);
                  LCDDrawPoint(X+i,Y+j,tempcolor);
		}
	}
}
/*----------------------------------------------------------------------------------------------
--����      :     ��ʾһ��ASCII�ַ�
--����      :     X,Y         :     ��ʾ����
                  color       :     �ֵ���ɫ
                  Backlicolor :     �ֱ�������ɫ
                  Backl       :     �Ƿ���Ҫ����ɫ 1 ʹ��  0 ��ʹ��
--����ֵ    :     ��
----------------------------------------------------------------------------------------------*/
void GUICharacter(uint16_t X, uint16_t Y,uint8_t Character, uint16_t Color,uint16_t Backlicolor,uint16_t Backl)
{  
      uint16_t  XStart ,YStart, temp ,YOffset=8,XOffset=0;

      Character-= 0x20;

      for(YStart=0 ; YStart< 16 ; YStart++ )
      {
            if(YStart>=8)
            {
                  YOffset=0;
                  XOffset=8;
            }
            if(YStart+Y<220)
            {
                  temp = nAsciiDot[Character*16+YStart] ;
                  for(XStart=0 ; XStart<8 ; XStart++ )
                  {
                        if(X+XStart<176)
                        {
                              if((temp&(0x80>>(XStart))) == (0x80>>(XStart))  )	//ȡ���� 
                              {
                           
                                    LCDDrawPoint( Y+YStart-XOffset,X+XStart+YOffset,Color) ;		//д�����
                              }
                              else if(Backl)
                              {
                                    LCDDrawPoint(Y+YStart-XOffset,X+XStart+YOffset,Backlicolor) ;  //д�뱳��ɫ
                              }
                        }
                  }
            }
            
      }
}


void GUIString(uint16_t X, uint16_t Y,uint8_t *StringPointer, uint16_t Color,uint16_t Backlicolor,uint16_t Backl)
{
      uint8_t X0=X;
      while((*StringPointer<='~')&&(*StringPointer>=' '))
      {
            if(X>=220)
            {
                  X=X0;
                  Y-=16;
            }
            if(Y>=176)
                  break;
            
            GUICharacter(Y,X,*StringPointer,Color,Backlicolor,Backl);
            X+=8;
            StringPointer++;
      }
}


void GUISelectLabel(uint16_t X,uint16_t Y,uint16_t Color,uint16_t SelectColor)
{
      //LCDDrawLine(uint8_t XStart,uint8_t YStart,uint8_t XEnd,uint8_t YEnd,uint16_t Color)
}
































































































































































































































