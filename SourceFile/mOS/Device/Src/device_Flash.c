#include "device_Flash.h"
#include "system.h"

static void FlashLock(void)
{
	FLASH_Lock();
}

static void FlashUnlock(void)
{
	FLASH_Unlock();	
}
 
/*********************************************************************************************************
*描述		：读取指定地址的半字(16位数据)
*输入参数	：faddr:读地址(此地址必须为2的倍数!!)
*			  NumToWrite:半字(16位)数  
*输出参数	：返回对应数据.
*********************************************************************************************************/
u16 FlashReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if FlashWriteEnable 	//如果使能了写   

/*********************************************************************************************************
*描述		：不检查的写入
*输入参数	：pBuffer:数据指针
*			  NumToWrite:半字(16位)数  
*输出参数	：无
*********************************************************************************************************/
static void FlashWriteNoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 

/*********************************************************************************************************
*描述		：从指定地址开始读出指定长度的数据
*输入参数	：ReadAddr,起始地址
*			  NumToWrite,半字(16位)数
*输出参数	：无
*********************************************************************************************************/
static void FlashRead(u32 readAddr,u16 *bufferPointer,u16 numToRead)   	
{
	u16 i;
	for(i=0;i<numToRead;i++)
	{
		bufferPointer[i]=FlashReadHalfWord(readAddr);//读取2个字节.
		readAddr+=2;//偏移2个字节.	
	}
}

/*********************************************************************************************************
*描述		：从指定地址开始写入指定长度的数据
*输入参数	：WriteAddr，起始地址(此地址必须为2的倍数!!)
*			  pBuffer，数据指针
			  NumToWrite，半字(16位)数(就是要写入的16位数据的个数.)
输出参数	：无
*********************************************************************************************************/
#if STM32Capacity<256
#define SectorSize 1024 //字节
#else 
#define SectorSize	2048
#endif		 
u16 SectorBuffer[SectorSize];//最多是2K字节
void FlashWrite(u32 writeAddr,u16 *pBuffer,u16 numToWrite)	
{
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	if(writeAddr<STM32FlashBASE||(writeAddr>=(STM32FlashBASE+1024*STM32Capacity)))return;//非法地址
	FlashUnlock();						//解锁
	offaddr=writeAddr-STM32FlashBASE;		//实际偏移地址.
	secpos=offaddr/SectorSize;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%SectorSize)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=SectorSize/2-secoff;		//扇区剩余空间大小   
	if(numToWrite<=secremain)secremain=numToWrite;//不大于该扇区范围
	while(1) 
	{	
		FlashRead(secpos*SectorSize+STM32FlashBASE,SectorBuffer,SectorSize/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SectorBuffer[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secpos*SectorSize+STM32FlashBASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				SectorBuffer[i+secoff]=pBuffer[i];	  
			}
			FlashWriteNoCheck(secpos*SectorSize+STM32FlashBASE,SectorBuffer,SectorSize/2);//写入整个扇区  
		}else FlashWriteNoCheck(writeAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(numToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		 	pBuffer+=secremain;  	//指针偏移
			writeAddr+=secremain;	//写地址偏移	   
		  numToWrite-=secremain;	//字节(16位)数递减
			if(numToWrite>(SectorSize/2))secremain=SectorSize/2;//下一个扇区还是写不完
			else secremain=numToWrite;//下一个扇区可以写完了
		}	 
	};	
	FlashLock();//上锁
}
#endif
/***************************************************************************************************

***************************************************************************************************/
void FlashReadData(u32 addr , u8 *bufferPoiner , u16 size)
{
	while(size--)
	{
		*(bufferPoiner++)=*((u8*)addr++);
	}
}

/***************************************************************************************************
*描述		： 函數接口初始化
***************************************************************************************************/
void InitFlash(void)
{
	System.Device.Storage.Parameter.StorageWriteData=FlashWrite;
	System.Device.Storage.Parameter.StorageReadData=FlashReadData;
  System.Device.Storage.ReadData=FlashReadData;
  System.Device.Storage.WriteData=FlashWrite;
}

