
#include "bmp.h"
#include "string.h"
#include "system.h"
#include "ff.h"
#include "device_lcd.h"
extern void LcdSetWindow(u16 sx,u16 sy,u16 width,u16 height);
ImageInfo PictureInfo;//ͼ����Ϣ
//����ALPHA BLENDING�㷨.
//src:Դ��ɫ
//dst:Ŀ����ɫ
//alpha:͸���̶�(0~32)
//����ֵ:��Ϻ����ɫ.
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
//С�ߴ��bmp����,����filename���BMP�ļ�		
//filename:����·�����ļ���
//x,y,width,height:������С
//acolor:���ӵ�alphablend����ɫ(�������32λɫbmp��Ч!!!)
//mode:ģʽ(����bit5,�����ľ�ֻ��32λɫbmp��Ч!!!)
//     bit[7:6]:0,��ʹ��ͼƬ����͵�ɫalphablend;
//              1,��ͼƬ��acolor����alphablend,���Ҳ����ø��ӵ�͸����;
//              2,��ɫ,acolor,ͼƬ,һ�����alphablend;
//	   bit5:����
//     bit4~0:0~31,ʹ�ø���alphablend��͸���̶� 	      	  			  
//����ֵ:0,�ɹ�;
//    ����,������.

u8 BmpDecode(u8 *fileName,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode)
{
	FIL				*fileBmp;
	u8 				*dataBuffer; 		//���ݶ�ȡ��
	u8 				alphabend=0xff;		//����͸��ɫΪ0����ȫ��͸��
	u8				res;
	u8				colorByte;
	u8 				bmpCompression=0;	//��¼ѹ����ʽ
	u8 				*bmpBuffer;			//���ݽ����ַ
	u16				rowCount;			//һ�ζ�ȡ������
	u16 			rowLenght;	  		 	//ˮƽ�����ֽ���  
	u16 			rowpix=0;			//ˮƽ����������	  
	u8 				rowaFillBytes;				//ÿ������ֽ���
	u16 			tx,ty,color;	 
	u16				byteRead;
	u8 				alphamode=mode>>6;	//�õ�ģʽֵ,0/1/2
	u16				i,j;
	u16 			tempColor;
//	u16				*updateBuffer;
	u32				count=0;

	u16				readLenght=BmpBufferSize;//һ�δ�SD����ȡ���ֽ�������,����С��LCD���*3!!!
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
	
	res=f_open(fileBmp,(const TCHAR*)fileName,FA_READ);//���ļ�	 		
	if(NULL==res)
	{
		f_read(fileBmp,dataBuffer,sizeof(BmpInfo),(UINT*)&byteRead);//����BmpInfo��Ϣ
		bmpPointer=(BmpInfo*)dataBuffer;//�õ�BMP��ͷ����Ϣ  
		colorByte=bmpPointer->BmpInfoHead.BmpBitCount/8;//��ɫλ 16/24/32  
		bmpCompression=bmpPointer->BmpInfoHead.BmpCompression;//ѹ����ʽ
		PictureInfo.ImageHeight=bmpPointer->BmpInfoHead.BmpHeight;//�õ�ͼƬ�߶�
		PictureInfo.ImageWidth=bmpPointer->BmpInfoHead.BmpWidth;	//�õ�ͼƬ���   
//		updateBuffer=(u16*)malloc(PictureInfo.ImageHeight*PictureInfo.ImageWidth);
		//ˮƽ���ر�����4�ı���
		if((PictureInfo.ImageWidth*colorByte)%4)
			rowLenght=(((PictureInfo.ImageWidth*colorByte)>>2)+1)<<2;
		else 
			rowLenght=PictureInfo.ImageWidth*colorByte;
		rowaFillBytes=rowLenght-PictureInfo.ImageWidth*colorByte;	//ÿ������ֽ���		
		//��ʼ����BMP   
		color=0;//��ɫ���	 													 
		tx=0 ;
		ty=PictureInfo.ImageHeight-1;
		if(PictureInfo.ImageWidth<=PictureInfo.SetWidth&&PictureInfo.ImageHeight<=PictureInfo.SetHeight)
		{
			rowCount=readLenght/rowLenght;						//һ�ζ�ȡ������
			readLenght=rowCount*rowLenght;						//һ�ζ�ȡ���ֽ���
			rowpix=PictureInfo.ImageWidth;			//ˮƽ���������ǿ�� 		
			f_lseek(fileBmp,bmpPointer->BmpFileHead.BmpOffset);	//ƫ�Ƶ�������ʼλ�� 	 	
			count=0;
			while(1)
			{
				res=f_read(fileBmp,dataBuffer,readLenght,(UINT *)&byteRead);	//����readlen���ֽ�
				bmpBuffer=dataBuffer;//�����׵�ַ  
				if(byteRead!=readLenght)rowCount=byteRead/rowLenght;//���ʣ�µ�����
				switch(colorByte)
				{
					case 2:
						for(j=0;j<rowCount;j++)//ÿ�ζ���������
						{
							if(bmpCompression==BmpRGB)//RGB:5,5,5
							{
								for(i=0;i<rowpix;i++)
								{
									color=((u16)*bmpBuffer&0X1F);			//R
									color+=(((u16)*bmpBuffer++)&0XE0)<<1; 	//G
									color+=((u16)*bmpBuffer++)<<9;  	    //R,G	 
									//*(updateBuffer+count)=color;
                  System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//��ʾͼƬ	
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
                  System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//��ʾͼƬ	
                  tx++;
									count++;	
								}
							}
							bmpBuffer+=rowaFillBytes;//���������
              tx=0;
              ty--;
						}	
						break;
					case 3:
							for(i=0;i<rowpix;i++)//дһ������
							{
								color=(*bmpBuffer++)>>3;		   		//B
								color+=((u16)(*bmpBuffer++)<<3)&0X07E0;	//G
								color+=(((u16)*bmpBuffer++)<<8)&0XF800;	//R
                tx++;
								//*(updateBuffer+count)=color;
                System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//��ʾͼƬ	
								count++;
							}
							bmpBuffer+=rowaFillBytes;//���������
              tx=0;
              ty--;

						break;
					case 4:
						for(j=0;j<rowCount;j++)	//ÿ�ζ���������
						{
							for(i=0;i<rowpix;i++)
							{
								color=(*bmpBuffer++)>>3;		//B
								color+=((u16)(*bmpBuffer++)<<3)&0X07E0;	//G
								color+=(((u16)*bmpBuffer++)<<8)&0XF800;	//R
								alphabend=*bmpBuffer++;	//ALPHAͨ��
								if(1!=alphamode) 		//��Ҫ��ȡ��ɫ
								{
									tempColor=System.Device.Lcd.LcdReadPoint(x+tx,y+ty);//��ȡ��ɫ		   
									if(2==alphamode)//��Ҫ���ӵ�alphablend
                  {
										tempColor=pic(tempColor,acolor,mode&0X1F);	//��ָ����ɫ����blend		 
                  }
									color=pic(tempColor,color,alphabend>>3); 		//�͵�ɫ����alphablend
								}else 
                {
									tempColor=pic(alphabend,color,alphabend>>3);	//��ָ����ɫ����blend
                }
								System.Device.Lcd.LcdDrawPoint(x+tx,y+ty,color);//��ʾͼƬ	
                tx++;
								//*(updateBuffer+count)=color;
								count++;  
							}
							bmpBuffer+=rowaFillBytes;//���������
              tx=0;
              ty--;
						}
						break;
				}
				if(byteRead!=readLenght||res)break;
			}
		}
		f_close(fileBmp);//�ر��ļ�   
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











