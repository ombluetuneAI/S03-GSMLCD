#include "spi.h"
#include "App.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//SPI 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/13 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI2的初始化

//SPI1初始化
#define SPI2_DR_Address    0x4000380C
extern u8 tx[4];

void InitSPITxDMA(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
///	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE );
  DMA_DeInit(DMA1_Channel5);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SPI2_DR_Address;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)tx;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = 0;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器

	
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
// 	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority =  3; 
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
// 	NVIC_Init(&NVIC_InitStructure);
}
void InitSPIRxDMA(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	DMA_DeInit(DMA1_Channel4);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;  //DMA外设ADC基地址
	//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)tx;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设到内存
	DMA_InitStructure.DMA_BufferSize =2;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE); 
	DMA_Cmd(DMA1_Channel4, DISABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =  2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

void DMA1_Channel4_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC4);
	DMA_ClearITPendingBit(DMA1_IT_TE4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA1_Channel4->CNDTR = 3;
	DMA_Cmd(DMA1_Channel4, ENABLE);
}
void SPI2CmdSend(DMA_Channel_TypeDef* DMA_CHx,u16 length)
{
	SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE); //使能串口1的DMA发送 
	
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道  
	DMA_ClearFlag(DMA1_FLAG_TC5);   
	DMA_SetCurrDataCounter(DMA_CHx,length);//DMA通道的DMA缓存的大小
	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
	
	while(DMA_CHx->CNDTR!=0);	//等待通道7传输完成   
	
	SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,DISABLE); //禁止串口1的DMA发送 
}


void SPI1_Init(void)
{	    
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );//PORTB，SPI2时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //PA5.6.7复用推挽  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7上拉
			
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	SPI2_ReadWriteByte(0xEB);
	SPI2_ReadWriteByte(0xff);//启动传输	 
} 
//SPI 速度设置函数
//SpeedSet:0~7
//SPI速度=fAPB2/2^(SpeedSet+1)
//APB2时钟一般为72Mhz
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet;
	SPI_Cmd(SPI1,ENABLE);	   
} 
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{				   			 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
				  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
					    
}



void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPI2_ReadWriteByte(0xff);//启动传输	 
	
	InitSPITxDMA();
	InitSPIRxDMA();

}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI2->CR1&=0XFFC7; 
	SPI2->CR1|=SpeedSet;
	SPI_Cmd(SPI2,ENABLE); 
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while(!(SPI2->SR&0x02))//(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}		
  SPI2->DR = TxData;  
//	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while(!(SPI2->SR&0x01)) //(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI2->DR;//SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}

























