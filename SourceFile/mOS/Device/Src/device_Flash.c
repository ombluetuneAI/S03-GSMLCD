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
*����		����ȡָ����ַ�İ���(16λ����)
*�������	��faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
*			  NumToWrite:����(16λ)��  
*�������	�����ض�Ӧ����.
*********************************************************************************************************/
u16 FlashReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if FlashWriteEnable 	//���ʹ����д   

/*********************************************************************************************************
*����		��������д��
*�������	��pBuffer:����ָ��
*			  NumToWrite:����(16λ)��  
*�������	����
*********************************************************************************************************/
static void FlashWriteNoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 

/*********************************************************************************************************
*����		����ָ����ַ��ʼ����ָ�����ȵ�����
*�������	��ReadAddr,��ʼ��ַ
*			  NumToWrite,����(16λ)��
*�������	����
*********************************************************************************************************/
static void FlashRead(u32 readAddr,u16 *bufferPointer,u16 numToRead)   	
{
	u16 i;
	for(i=0;i<numToRead;i++)
	{
		bufferPointer[i]=FlashReadHalfWord(readAddr);//��ȡ2���ֽ�.
		readAddr+=2;//ƫ��2���ֽ�.	
	}
}

/*********************************************************************************************************
*����		����ָ����ַ��ʼд��ָ�����ȵ�����
*�������	��WriteAddr����ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
*			  pBuffer������ָ��
			  NumToWrite������(16λ)��(����Ҫд���16λ���ݵĸ���.)
�������	����
*********************************************************************************************************/
#if STM32Capacity<256
#define SectorSize 1024 //�ֽ�
#else 
#define SectorSize	2048
#endif		 
u16 SectorBuffer[SectorSize];//�����2K�ֽ�
void FlashWrite(u32 writeAddr,u16 *pBuffer,u16 numToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(writeAddr<STM32FlashBASE||(writeAddr>=(STM32FlashBASE+1024*STM32Capacity)))return;//�Ƿ���ַ
	FlashUnlock();						//����
	offaddr=writeAddr-STM32FlashBASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/SectorSize;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%SectorSize)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=SectorSize/2-secoff;		//����ʣ��ռ��С   
	if(numToWrite<=secremain)secremain=numToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		FlashRead(secpos*SectorSize+STM32FlashBASE,SectorBuffer,SectorSize/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SectorBuffer[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*SectorSize+STM32FlashBASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				SectorBuffer[i+secoff]=pBuffer[i];	  
			}
			FlashWriteNoCheck(secpos*SectorSize+STM32FlashBASE,SectorBuffer,SectorSize/2);//д����������  
		}else FlashWriteNoCheck(writeAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(numToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		 	pBuffer+=secremain;  	//ָ��ƫ��
			writeAddr+=secremain;	//д��ַƫ��	   
		  numToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(numToWrite>(SectorSize/2))secremain=SectorSize/2;//��һ����������д����
			else secremain=numToWrite;//��һ����������д����
		}	 
	};	
	FlashLock();//����
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
*����		�� �����ӿڳ�ʼ��
***************************************************************************************************/
void InitFlash(void)
{
	System.Device.Storage.Parameter.StorageWriteData=FlashWrite;
	System.Device.Storage.Parameter.StorageReadData=FlashReadData;
  System.Device.Storage.ReadData=FlashReadData;
  System.Device.Storage.WriteData=FlashWrite;
}

