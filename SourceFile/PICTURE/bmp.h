#ifndef __BMP_H_
#define __BMP_H_
#include "sys.h"

#define BmpBufferSize			2048	//����bmp��������Ĵ�С(����ӦΪLCD���*3)
#define BmpFormatError		    0x27	//��ʽ����
#define BmpSizeError			0x28	//ͼƬ�ߴ����
#define WindowError				0x29	//�����趨����
#define MemoryError				0x11	//�ڴ����
/*---------------------------------ͼ������ѹ��������------------------------------*/
#define BmpRGB	 			0  //û��ѹ��.RGB 5,5,5.
#define BmpRLE8 			1  //ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��
#define BmpRLE4 			2  //ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
#define BmpBITFIELDS 	    3  //ÿ�����صı�����ָ�������������  

/*----------------BMPͷ�ļ�-----------------*/
typedef __packed struct
{
		u16  BmpfType ;     //�ļ���־.ֻ��'BM',����ʶ��BMPλͼ����
		u16  BmpReserved1 ;	//����
		u16  BmpReserved2 ;	//����
		u32  BmpSize ;	  	//�ļ���С,ռ�ĸ��ֽ�

		u32  BmpOffset ;  	//���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ����
}BmpFileHeader;


/*-----------------------------------BMP��Ϣͷ--------------------------------------*/
typedef __packed struct
{
		u32 	BmpSize ;		   			//˵��BmpInfoHead�ṹ����Ҫ��������
		long  BmpWidth ;		   		//˵��ͼ��Ŀ�ȣ�������Ϊ��λ 
		long  BmpHeight ;	   			//˵��ͼ��ĸ߶ȣ�������Ϊ��λ 	
		u16  	BmpPlanes ;	   			//ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1 
		u16  	BmpBitCount ;	   		//˵��������/���أ���ֵΪ1��4��8��16��24����32
		u32 	BmpCompression ;  	//˵��ͼ������ѹ�������͡���ֵ����������ֵ֮һ��
		u32 	BmpSizeImage ;			//˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0  
		long  BmpXPelsPerMeter ;	//˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
		long  BmpYPelsPerMeter ;	//˵����ֱ�ֱ��ʣ�������/�ױ�ʾ	
	
		u32 	BmpClrUsed ;	  		//˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
		u32 	BmpClrImportant ; 	//˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��
}BmpInfoHeader;
/*---------------------------��ɫ��----------------------------*/
typedef __packed struct 
{
    u8 RGBBlue ;    //ָ����ɫǿ��
    u8 RGBGreen ;		//ָ����ɫǿ�� 
    u8 RGBRed ;	  	//ָ����ɫǿ�� 
    u8 RGBReserved ;//����������Ϊ0 
}RGBTab;
/*---------------------------λͼ��Ϣͷ------------------------*/
typedef __packed struct
{ 
		BmpFileHeader BmpFileHead;
		BmpInfoHeader BmpInfoHead;  
		u32 					RGBMask[3];			//��ɫ�����ڴ��RGB����.
}BmpInfo; 


/*------------------------------ͼ����Ϣ---------------------------*/
typedef __packed struct
{		
		u16 LcdWidth;	//LCD�Ŀ��
		u16 LcdHeight;	//LCD�ĸ߶�
		u32 ImageWidth; 	//ͼ���ʵ�ʿ�Ⱥ͸߶�
		u32 ImageHeight;

		u32 ZoomFact;  	//����ϵ�� (������8192����)
		
		u32 SetHeight; 	//�趨�ĸ߶ȺͿ��
		u32 SetWidth;
		
		u32	Xoffset;	  	//x���y���ƫ����
		u32 Yoffset;

		u32 StaticX; 	//��ǰ��ʾ���ģ�������
		u32 StaticY;																 	
}ImageInfo;


typedef RGBTab	*RGBQuad;//��ɫ��  



u8 BmpDecode(u8 *fileName,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode);
#endif








