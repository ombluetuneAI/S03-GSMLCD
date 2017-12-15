
#include "bmp.h"
#include "string.h"
#include "system.h"
#include "ff.h"
#include "device_lcd.h"
extern void LcdSetWindow(u16 sx,u16 sy,u16 width,u16 height);
ImageInfo PictureInfo;//图像信息
//快速ALPHA BLENDING算法.
//src:源颜色
//dst:目标颜色
//alpha:透明程度(0~32)
//返回值:混合后的颜色.
u16 pic(u16 src,u16 dst,u8 alpha)
{
	u32 src2;
	u32 dst2;	 
	//Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
	src2=((src<<16)|src)&0x07E0F81F;
	dst2=((dst<<16)|dst)&0x07E0F81F;   
	//Perform blending R:G:B with alpha in range 0..32
	//Note that the reason that alpha may not exceed 32 is that there are only
	//5bits of space between each R:G:B value, any higher value will overflow
	//into the next component and deliver ugly result.
	dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
	return (dst2>>16)|dst2;  
}
//小尺寸的bmp解码,解码filename这个BMP文件		
//filename:包含路径的文件名
//x,y,width,height:开窗大小
//acolor:附加的alphablend的颜色(这个仅对32位色bmp有效!!!)
//mode:模式(除了bit5,其他的均只对32位色bmp有效!!!)
//     bit[7:6]:0,仅使用图片本身和底色alphablend;
//              1,仅图片和acolor进行alphablend,并且不适用附加的透明度;
//              2,底色,acolor,图片,一起进行alphablend;
//	   bit5:保留
//     bit4~0:0~31,使用附加alphablend的透明程度 	      	  			  
//返回值:0,成功;
//    其他,错误码.

u8 BmpDecode(u8 *fileName,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode)
{
	FIL				*fileBmp;
	u8 				*dataBuffer; 		//数据读取存
	u8 				alphabend=0xff;		//代表透明色为0，完全不透明
	u8				res;
	u8				colorByte;
	u8 				bmpCompression=0;	//记录压缩方式
	u8 				*bmpBuffer;			//数据解码地址
	u16				rowCount;			//一次读取的行数
	u16 			rowLenght;	  		 	//水平方向字节数  
	u16 			rowpix=0;			//水平方向像素数	  
	u8 				rowaFillBytes;				//每行填充字节数
	u16 			tx,ty,color;	 
	u16				byteRead;
	u8 				alphamode=mode>>6;	//得到模式值,0/1/2
	u16				i,j;
	u16 			tempColor;
//	u16				*updateBuffer;
	u32				count=0;

	u16				readLenght=BmpBufferSize;//一次从SD卡读取的字节数长度,不能小于LCD宽度*3!!!
	BmpInfo		    *bmpPointer;
	PictureInfo.SetHeight=height;
	PictureInfo.SetWidth=width;
	
	dataBuffer=(u8*)malloc(readLenght);
	if(NULL==dataBuffer)
		return MemoryError;
	fileBmp=(FIL*)malloc(sizeof(FIL));
	if(NULL==fileBmp)
	{
		free(dataBuffer);
		return MemoryError;
	}
	
	res=f_open(fileBmp,(const TCHAR*)fileName,FA_READ);//打开文件	 		
	if(NULL==res)
	{
		f_read(fileBmp,dataBuffer,sizeof(BmpInfo),(UINT*)&byteRead);//读出BmpInfo信息
		bmpPointer=(BmpInfo*)dataBuffer;//得到BMP的头部信息  
		colorByte=bmpPointer->BmpInfoHead.BmpBitCount/8;//彩色位 16/24/32  
		bmpCompression=bmpPointer->BmpInfoHead.BmpCompression;//压缩方式
		PictureInfo.ImageHeight=bmpPointer->BmpInfoHead.BmpHeight;//得到图片高度
		PictureInfo.ImageWidth=bmpPointer->BmpInfoHead.BmpWidth;	//得到图片宽度   
//		updateBuffer=(u16*)malloc(PictureInfo.ImageHeight*PictureInfo.ImageWidth);
		//水平像素必须是4的倍数
		if((PictureInfo.ImageWidth*colorByte)%4)
			rowLenght=(((PictureInfo.ImageWidth*colorByte)>>2)+1)<<2;
		else 
			rowLenght=PictureInfo.ImageWidth*colorByte;
		rowaFillBytes=rowLenght-PictureInfo.ImageWidth*colorByte;	//每行填充字节数		
		//开始解码BMP   
		color=0;//颜色清空	 													 
		tx=0 ;
		ty=PictureInfo.ImageHeight-1;
		if(PictureInfo.ImageWidth<=PictureInfo.SetWidth&&PictureInfo.ImageHeight<=PictureInfo.SetHeight)
		{
			rowCount=readLenght/rowLenght;						//一次读取的行数
			readLenght=rowCount*rowLenght;						//一次读取的字节数
			rowpix=PictureInfo.ImageWidth;			//水平像素数就是宽度 		
			f_lseek(fileBmp,bmpPointer->BmpFileHead.BmpOffset);	//偏移到数据起始位置 	 	
			count=0;
			while(1)
			{
				res=f_read(fileBmp,dataBuffer,readLenght,(UINT *)&byteRead);	//读出readlen个字节
				bmpBuffer=dataBuffer;//数据首地址  
				if(byteRead!=readLenght)rowCount=byteRead/rowLenght;//最后剩下的行数
				switch(colorByte)
				{
					case 2:
						for(j=0;j<rowCount;j++)//每次读到的行数
						{
							if(bmpCompression==BmpRGB)//RGB:5,5,5
							{
								for(i=0;i<rowpix;i++)
								{
									color=((u16)*bmpBuffer&0X1F);			//R
									color+=(((u16)*bmpBuffer++)&0XE0)<<1; 	//G
									color+=((u16)*bmpBuffer++)<<9;  	    //R,G	 
									//*(updateBuffer+count)=color;
                  System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//显示图片	
                  tx++;
									count++;	
								}
							}else  //RGB 565
							{
								for(i=0;i<rowpix;i++)
								{											 
									color=*bmpBuffer++;  			//G,B
									color+=((u16)*bmpBuffer++)<<8;	//R,G	 
									//*(updateBuffer+count)=color;
                  System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//显示图片	
                  tx++;
									count++;	
								}
							}
							bmpBuffer+=rowaFillBytes;//跳过填充区
              tx=0;
              ty--;
						}	
						break;
					case 3:
							for(i=0;i<rowpix;i++)//写一行像素
							{
								color=(*bmpBuffer++)>>3;		   		//B
								color+=((u16)(*bmpBuffer++)<<3)&0X07E0;	//G
								color+=(((u16)*bmpBuffer++)<<8)&0XF800;	//R
                tx++;
								//*(updateBuffer+count)=color;
                System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//显示图片	
								count++;
							}
							bmpBuffer+=rowaFillBytes;//跳过填充区
              tx=0;
              ty--;

						break;
					case 4:
						for(j=0;j<rowCount;j++)	//每次读到的行数
						{
							for(i=0;i<rowpix;i++)
							{
								color=(*bmpBuffer++)>>3;		//B
								color+=((u16)(*bmpBuffer++)<<3)&0X07E0;	//G
								color+=(((u16)*bmpBuffer++)<<8)&0XF800;	//R
								alphabend=*bmpBuffer++;	//ALPHA通道
								if(1!=alphamode) 		//需要读取底色
								{
									tempColor=System.Device.Lcd.LcdReadPoint(x+tx,y+ty);//读取颜色		   
									if(2==alphamode)//需要附加的alphablend
                  {
										tempColor=pic(tempColor,acolor,mode&0X1F);	//与指定颜色进行blend		 
                  }
									color=pic(tempColor,color,alphabend>>3); 		//和底色进行alphablend
								}else 
                {
									tempColor=pic(alphabend,color,alphabend>>3);	//与指定颜色进行blend
                }
								System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//显示图片	
                tx++;
								//*(updateBuffer+count)=color;
								count++;  
							}
							bmpBuffer+=rowaFillBytes;//跳过填充区
              tx=0;
              ty--;
						}
						break;
				}
				if(byteRead!=readLenght||res)break;
			}
		}
		f_close(fileBmp);//关闭文件   
//        count=(PictureInfo.ImageWidth)*(PictureInfo.ImageHeight);
//        LcdSetWindow(x,y,PictureInfo.ImageWidth,PictureInfo.ImageHeight);
//        while(count--)
//                LCDAdd->LCDRAMData=*(updateBuffer+count);
//        free(updateBuffer);
	
//        LcdSetWindow(0,0,320,240);
	}
    free(dataBuffer);
    free(fileBmp);
	return 0;
}
void ImagetoLcdDraw(u16 x,u16 y,u8 *filename)
{
  FIL	*filePic;
  u16 xEnd,yEnd;
  u16 colorTemp;
  u8 	tmp_R0,tmp_G0,tmp_B0,tmp_A,tmp_R1,tmp_G1,tmp_B1;
  u32 index=0;     
  u32 filesize;
  u8  grayLevel;//????
  u16 xSize,ySize;
  u32 usOldRGB;
  u8 *dataBuffer;
  u8 *dataRead;
  u8 res;
  u16	byteRead;
  u16 readLen;
  u16 readCnt;
  u8 tmp_i;
  readLen = BmpBufferSize;
  dataBuffer=(u8*)malloc(BmpBufferSize);
	if(NULL==dataBuffer)
  {
    free(dataBuffer);
		return;
  }
	filePic=(FIL*)malloc(sizeof(FIL));
	if(NULL==filePic)
	{
		free(filePic);
		return;
	}
  res = f_open(filePic,(const TCHAR*)filename,FA_READ);
  if(res==NULL)
  {
    f_read(filePic,dataBuffer,8,(UINT*)&byteRead);
    grayLevel = dataBuffer[1];
    xEnd = dataBuffer[2];
    xEnd<<=8;
    xEnd|=dataBuffer[3];
    
    yEnd=dataBuffer[4];
    yEnd<<=8;
    yEnd|=dataBuffer[5];
    
    filesize = filePic->fsize - 8;
    readCnt = filesize/BmpBufferSize + ((filesize%BmpBufferSize)?1:0);
    switch(grayLevel)
    {
      case 0x10:
        {
          LCDSetCursor(x,y);
          LCDWriteRAMPrepare();
          LcdSetWindow(x,y,xEnd,yEnd);
          
          for(tmp_i=0;tmp_i<readCnt;tmp_i++)
          {
            f_read(filePic,dataBuffer,BmpBufferSize,(UINT*)&byteRead);
            dataRead = dataBuffer;
            if(byteRead!=BmpBufferSize)
            {
              readLen = byteRead;
            }
            for(index=0;index<readLen/2;index++)
            {
              colorTemp = (*dataRead++)<<8;
              colorTemp|=*dataRead;
              LCDAdd->LCDRAMData=colorTemp;
              dataRead++;
            }
          }
        }
        break;
      case 0x20:
        {
          LCDSetCursor(x,y);
          LCDWriteRAMPrepare();
          LcdSetWindow(x,y,xEnd,yEnd);
          
          for(tmp_i=0;tmp_i<readCnt;tmp_i++)
          {
            f_read(filePic,dataBuffer,BmpBufferSize,(UINT*)&byteRead);
            dataRead = dataBuffer;
            if(byteRead!=BmpBufferSize)
            {
              readLen = byteRead;
            }
            for(index=0;index<readLen/4;index++)
            {
              tmp_R0 = *dataRead;
              tmp_G0 = *dataRead++;
              tmp_B0 = *dataRead++;
              tmp_A = *dataRead++;
              
              colorTemp = RGB(tmp_R1, tmp_G1, tmp_B1);
              LCDAdd->LCDRAMData=colorTemp;
              dataRead++;
            }
          }
        }
        
        break;
      default:
        break;
    }
//    while(1)
//    {
//      
//      
//      f_read(filePic,dataBuffer,BmpBufferSize,(UINT*)&byteRead);
//      
//    }
  }
  LcdSetWindow(0,0,320,240);
  f_close(filePic);
  free(dataBuffer);
  free(filePic);
  
//    grayLevel=color[1];

//    xEnd=color[2];
//    xEnd<<=8;
//    xEnd|=color[3];

//    yEnd=color[4];
//    yEnd<<=8;
//    yEnd|=color[5];
//    
//	totalpoint=xEnd*yEnd;
//    color+=8;
//// 	EnterCritical();
//    switch(grayLevel)
//    {
//    	case 16:
//			LCDSetCursor(x,y);	//?????? 
//			LCDWriteRAMPrepare(); //????GRAM
//			LcdSetWindow(x,y,xEnd,yEnd);	
//			for(index=totalpoint;index>0;index--)
//			{
//		    colorTemp=ImageGetColor(1,color);       
//				LCDAdd->LCDRAMData=colorTemp;
//		    color+=2;
//			}
//		break;
//		case 32:
//			for(ySize=0;ySize<yEnd;ySize++)
//			{
//				for(xSize=0;xSize<xEnd;xSize++)
//				{
//					tmp_R1 =  *color;
//					tmp_G1 =  *color++;
//					tmp_B1 =  *color++;
//					tmp_A  =  *color++;	/* Alpha ?(???),0-255, 0????,1?????, ???????? */	
////					if(A==0)
////					{
////						;//????
////					}				
////					else if(A==0xFF)//?????
////					{
////						colorTemp = RGB(R1, G1, B1);
////						LCDDrawPoint(xSize+x,ySize+y, colorTemp);
////					}
////					else//???
//					{
//						usOldRGB = LCDReadPointRGB(x+xSize,y+ySize);
//					//	color=(*bmpBuffer++)>>3;		//B
//					//	color+=((u16)(*bmpBuffer++)<<3)&0X07E0;	//G
//					//	color+=(((u16)*bmpBuffer++)<<8)&0XF800;	//R

//						tmp_R0 = RGB565_R(usOldRGB);
//						tmp_G0 = RGB565_G(usOldRGB);
//						tmp_B0 = RGB565_B(usOldRGB);

//						tmp_R1 = (tmp_R1 * tmp_A) / 255 + tmp_R0 * (255 - tmp_A) / 255;
//						tmp_G1 = (tmp_G1 * tmp_A) / 255 + tmp_G0 * (255 - tmp_A) / 255;
//						tmp_B1 = (tmp_B1 * tmp_A) / 255 + tmp_B0 * (255 - tmp_A) / 255;
//						colorTemp = RGB(tmp_R1, tmp_G1, tmp_B1);
//						LCDDrawPoint(xSize+x,ySize+y, colorTemp);					
//					}
//					color++;
//				}
//			}	
//		break;
//	}
////	ExitCritical();
//    LcdSetWindow(0,0,320,240);
}











