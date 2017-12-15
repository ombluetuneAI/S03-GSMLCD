#ifndef __BMP_H_
#define __BMP_H_
#include "sys.h"

#define BmpBufferSize			2048	//定义bmp解码数组的大小(最少应为LCD宽度*3)
#define BmpFormatError		    0x27	//格式错误
#define BmpSizeError			0x28	//图片尺寸错误
#define WindowError				0x29	//窗口设定错误
#define MemoryError				0x11	//内存错误
/*---------------------------------图象数据压缩的类型------------------------------*/
#define BmpRGB	 			0  //没有压缩.RGB 5,5,5.
#define BmpRLE8 			1  //每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；
#define BmpRLE4 			2  //每个象素4比特的RLE压缩编码，压缩格式由2字节组成
#define BmpBITFIELDS 	    3  //每个象素的比特由指定的掩码决定。  

/*----------------BMP头文件-----------------*/
typedef __packed struct
{
		u16  BmpfType ;     //文件标志.只对'BM',用来识别BMP位图类型
		u16  BmpReserved1 ;	//保留
		u16  BmpReserved2 ;	//保留
		u32  BmpSize ;	  	//文件大小,占四个字节

		u32  BmpOffset ;  	//从文件开始到位图数据(bitmap data)开始之间的的偏移量
}BmpFileHeader;


/*-----------------------------------BMP信息头--------------------------------------*/
typedef __packed struct
{
		u32 	BmpSize ;		   			//说明BmpInfoHead结构所需要的字数。
		long  BmpWidth ;		   		//说明图象的宽度，以象素为单位 
		long  BmpHeight ;	   			//说明图象的高度，以象素为单位 	
		u16  	BmpPlanes ;	   			//为目标设备说明位面数，其值将总是被设为1 
		u16  	BmpBitCount ;	   		//说明比特数/象素，其值为1、4、8、16、24、或32
		u32 	BmpCompression ;  	//说明图象数据压缩的类型。其值可以是下述值之一：
		u32 	BmpSizeImage ;			//说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0  
		long  BmpXPelsPerMeter ;	//说明水平分辨率，用象素/米表示
		long  BmpYPelsPerMeter ;	//说明垂直分辨率，用象素/米表示	
	
		u32 	BmpClrUsed ;	  		//说明位图实际使用的彩色表中的颜色索引数
		u32 	BmpClrImportant ; 	//说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。
}BmpInfoHeader;
/*---------------------------彩色表----------------------------*/
typedef __packed struct 
{
    u8 RGBBlue ;    //指定蓝色强度
    u8 RGBGreen ;		//指定绿色强度 
    u8 RGBRed ;	  	//指定红色强度 
    u8 RGBReserved ;//保留，设置为0 
}RGBTab;
/*---------------------------位图信息头------------------------*/
typedef __packed struct
{ 
		BmpFileHeader BmpFileHead;
		BmpInfoHeader BmpInfoHead;  
		u32 					RGBMask[3];			//调色板用于存放RGB掩码.
}BmpInfo; 


/*------------------------------图像信息---------------------------*/
typedef __packed struct
{		
		u16 LcdWidth;	//LCD的宽度
		u16 LcdHeight;	//LCD的高度
		u32 ImageWidth; 	//图像的实际宽度和高度
		u32 ImageHeight;

		u32 ZoomFact;  	//缩放系数 (扩大了8192倍的)
		
		u32 SetHeight; 	//设定的高度和宽度
		u32 SetWidth;
		
		u32	Xoffset;	  	//x轴和y轴的偏移量
		u32 Yoffset;

		u32 StaticX; 	//当前显示到的ｘｙ坐标
		u32 StaticY;																 	
}ImageInfo;


typedef RGBTab	*RGBQuad;//彩色表  



u8 BmpDecode(u8 *fileName,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode);
#endif








