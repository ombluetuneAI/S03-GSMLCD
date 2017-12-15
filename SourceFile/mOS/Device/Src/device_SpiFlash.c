

/* Includes ------------------------------------------------------------------*/
//#include "usart.h"
#include "device_SpiFlash.h"

#define SPI_FLASH_SPI                   SPI2
#define SPI_FLASH_SPI_CLK               RCC_APB2Periph_SPI2
#define SPI_FLASH_SPI_SCK_PIN           GPIO_Pin_13              /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT     GPIOB
#define SPI_FLASH_SPI_SCK_GPIO_CLK      RCC_APB2Periph_GPIOB
#define SPI_FLASH_SPI_MISO_PIN          GPIO_Pin_14              /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT    GPIOB
#define SPI_FLASH_SPI_MISO_GPIO_CLK     RCC_APB2Periph_GPIOB
#define SPI_FLASH_SPI_MOSI_PIN          GPIO_Pin_15             /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT    GPIOB
#define SPI_FLASH_SPI_MOSI_GPIO_CLK     RCC_APB2Periph_GPIOB
#define SPI_FLASH_CS_PIN_NUM            12                       /* PB.09 */
#define SPI_FLASH_CS_PIN                GPIO_Pin_12  
#define SPI_FLASH_CS_GPIO_PORT          GPIOB
#define SPI_FLASH_CS_GPIO_CLK           RCC_APB2Periph_GPIOB


/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable                0x06 
#define W25X_WriteDisable               0x04 
#define W25X_ReadStatusReg              0x05 
#define W25X_WriteStatusReg             0x01 
#define W25X_ReadData                   0x03 
#define W25X_FastReadData               0x0B 
#define W25X_FastReadDual               0x3B 
#define W25X_PageProgram                0x02 
#define W25X_BlockErase                 0xD8 
#define W25X_SectorErase                0x20 
#define W25X_ChipErase                  0xC7 
#define W25X_PowerDown                  0xB9 
#define W25X_ReleasePowerDown           0xAB 
#define W25X_DeviceID                   0xAB 
#define W25X_ManufactDeviceID           0x90 
#define W25X_JedecDeviceID              0x9F 

#define WIP_FlagMask                    0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                      0xA5


/* Private variables ---------------------------------------------------------*/
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;

#define  BufferSize (countof(Tx_Buffer)-1)
#define countof(a) (sizeof(a) / sizeof(*(a)))

uint8_t Tx_Buffer[] = "\n\r www.armjishu.com STM32F10x SPI Flash Test Example: \n\r communication with an Winbond W25*16 SPI FLASH";
uint8_t Index, Rx_Buffer[BufferSize];

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_Init
  * @功能   初始化与外部SPI FLASH接口的驱动函数
  *         Initializes the peripherals used by the SPI FLASH driver.
  * @参数   无
  * @返回值 无
***----------------------------------------------------------------*/
void SPIFlashInit(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI2, ENABLE); //使能SPI外设

    /*初始化片选信号管脚: /CS*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*拉高片选信号*/
    SPI_FLASH_CS_HIGH();

    /* SPI1初始化配置*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  //主从模式选择，SCK信号有主机产生
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //SPI通信数据的大小
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//时钟极性
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;   //时钟相位
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	  //片选信号，硬件模式还是软件模式
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//SPI分频值，分频后的值为SCK的时钟频率
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//选择SPI通信时，是高位数据在前还是低位数据在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//与CRC校验有关
    SPI_Init(SPI2, &SPI_InitStructure);

    /*使能SPI1 */
    SPI_Cmd(SPI2, ENABLE);
	
	/*唤醒SPI FLASH*/
    SPIFlash_WAKEUP();
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_SectorErase
  * @功能   擦除SPI FLASH一个扇区的驱动函数
  *         Erases the specified FLASH sector.
  * @参数   SectorAddr: 扇区地址 address of the sector to erase.
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
    SPI_FLASH_WriteEnable();

    /* 拉低片选信号*/
    SPI_FLASH_CS_LOW();
    /* Send Sector Erase instruction */
    SPI_FLASH_SendByte(W25X_SectorErase);
    /* Send SectorAddr high nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    SPI_FLASH_SendByte(SectorAddr & 0xFF);

    /* 拉高片选信号 */
    SPI_FLASH_CS_HIGH();

    /* 等待写入SPI的操作完成*/
    SPI_FLASH_WaitForWriteEnd();
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_ChipErase
  * @功能   擦除整个SPI FLASH的驱动函数
  *         Erases the entire FLASH.
  * @参数   无
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_ChipErase(void)
{
    /* Send write enable instruction */
    SPI_FLASH_WriteEnable();

    /* Bulk Erase */
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    /* Send Bulk Erase instruction  */
    SPI_FLASH_SendByte(W25X_ChipErase);
    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_PageWrite
  * @功能   单个写周期写入大于一个字节而小于等于一页大小的数据
  *         Writes more than one byte to the FLASH with a single 
  *         WRITE cycle(Page WRITE sequence). The number of byte 
  *         can't exceed the FLASH page size.
  * @参数   - pBuffer : 指向包含写入数据缓冲器的地址指针
  *             pointer to the buffer  containing the data to be
  *             written to the FLASH.
  *         - WriteAddr : flash的写入地址
  *             FLASH's internal address to write to.
  *         - NumByteToWrite : 写入的字节数
  *             number of bytes to write to the FLASH, must be
  *             equal or less than "SPI_FLASH_PageSize" value.
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    SPI_FLASH_WriteEnable();

    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    /* Send "Write to Memory " instruction */
    SPI_FLASH_SendByte(W25X_PageProgram);
    /* Send WriteAddr high nibble address byte to write to */
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /* Send WriteAddr medium nibble address byte to write to */
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /* Send WriteAddr low nibble address byte to write to */
    SPI_FLASH_SendByte(WriteAddr & 0xFF);

    if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
    }

    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        SPI_FLASH_SendByte(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_BufferWrite
  * @功能   向SPI FLASH写入一堆数据，写入的字节数可以大于一页容量
  *         Writes block of data to the FLASH. In this function,
  *         the number of WRITE cycles are reduced,
  *         using Page WRITE sequence.
  * @参数   - pBuffer : 指向包含写入数据缓冲器的地址指针
  *             pointer to the buffer  containing the data to be
  *             written to the FLASH.
  *         - WriteAddr : flash的写入地址
  *             FLASH's internal address to write to.
  *         - NumByteToWrite : 写入的字节数
  *             number of bytes to write to the FLASH.
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % SPI_FLASH_PageSize;
    count = SPI_FLASH_PageSize - Addr;
    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
            {
                temp = NumOfSingle - count;

                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            else
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if (NumOfSingle != 0)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_BufferRead
  * @功能   从SPI FLASH读出一段数据，写入的字节数可以大于一页容量
  *         Reads a block of data from the FLASH.
  * @参数   - pBuffer : 指向包含写入数据缓冲器的地址指针
  *             pointer to the buffer that receives the data read
  *             from the FLASH.
  *         - ReadAddr : flash的读起始地址
  *             FLASH's internal address to read from.
  *         - NumByteToWrite : 读出的字节数
  *             number of bytes to read from the FLASH.
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_ReadID
  * @功能   读取SPI FLASH厂商ID和设备ID(设备ID包含类型和容量)
  *         Reads Manufacturer ID and two Device ID bytes
  * @参数   无
  * @返回值 24bit，高到底依次为厂商ID、类型和容量
***----------------------------------------------------------------*/
u32 SPI_FLASH_ReadID(void)
{
    u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /*拉低SPI_FLASH片选信号线*/
    SPI_FLASH_CS_LOW();

    /* 遵循读取ID 时序，发送命令*/
    SPI_FLASH_SendByte(W25X_JedecDeviceID);
    Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
    Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
    Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

     /*拉高SPI_FLASH片选信号线*/
    SPI_FLASH_CS_HIGH();

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_ReadDeviceID
  * @功能   读取SPI FLASH设备ID
  *         Read one Device ID bytes
  * @参数   无
  * @返回值 一个字节的Device ID
***----------------------------------------------------------------*/
u32 SPI_FLASH_ReadDeviceID(void)
{
    u32 Temp = 0;

    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "RDID " instruction */
    SPI_FLASH_SendByte(W25X_DeviceID);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp = SPI_FLASH_SendByte(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    return Temp;
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_StartReadSequence
  * @功能   发起一个读取SPI FLASH的访问，包括发送读命令和起始地址
  *         Initiates a read data byte (READ) sequence from the Flash.
  *         This is done by driving the /CS line low to select the device,
  *         then the READ instruction is transmitted followed by 3 bytes
  *         address. This function exit and keep the /CS line low, so the
  *         Flash still being selected. With this technique the whole
  *         content of the Flash is read with a single READ instruction.
  *         Read one Device ID bytes
  * @参数   ReadAddr FLASH的访问地址
  *                  FLASH's internal address to read from.
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "Read from Memory " instruction */
    SPI_FLASH_SendByte(W25X_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_ReadByte
  * @功能   读取SPI FLASH的一个字节，未包含发送读命令和起始地址
  * @参数   无
  * @返回值 从SPI_FLASH读取的一个字节
***----------------------------------------------------------------*/
u8 SPI_FLASH_ReadByte(void)
{
    return (SPI_FLASH_SendByte(Dummy_Byte));
}


/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_SendByte
  * @功能   通过SPI总线发送一个字节数据(顺便接收一个字节数据)
  *         Sends a byte through the SPI interface and return the byte
  *         received from the SPI bus.
  * @参数   要写入的一个字节数据
  * @返回值 在发数据时，MISO信号线上接收的一个字节
***----------------------------------------------------------------*/
u8 SPI_FLASH_SendByte(u8 byte)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI2, byte);

    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_SendHalfWord
  * @功能   通过SPI总线发送一个半字(16bit=2个字节数据)(顺便接收数据)
  *         Sends a Half Word through the SPI interface and return the
  *         Half Word received from the SPI bus.
  * @参数   要写入的一个半字数据(16bit)
  * @返回值 在发数据时，MISO信号线上接收的一个半字数据(16bit)
***----------------------------------------------------------------*/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

    /* Send Half Word through the SPI1 peripheral */
    SPI_I2S_SendData(SPI2, HalfWord);

    /* Wait to receive a Half Word */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the Half Word read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_WriteEnable
  * @功能   SPI FLASH写使能
  *         Enables the write access to the FLASH.
  * @参数   无
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_WaitForWriteEnd
  * @功能   通过反复读取SPI FLASH的状态寄存器判断写入是否执行结束
  *         Polls the status of the Write In Progress (WIP) flag in the
  *         FLASH's status  register  and  loop  until write  opertaion
  *         has completed.
  * @参数   无
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  }
  while ((FLASH_Status & WIP_FlagMask) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/**-----------------------------------------------------------------
  * @函数名 SPI_Flash_PowerDown
  * @功能   SPI FLASH进入掉电模式
  * @参数   无
  * @返回值 无
***----------------------------------------------------------------*/
void SPIFlash_PowerDown(void)   
{ 
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}   

/**-----------------------------------------------------------------
  * @函数名 SPI_Flash_WAKEUP
  * @功能   唤醒SPI FLASH
  * @参数   无
  * @返回值 无
***----------------------------------------------------------------*/
void SPIFlash_WAKEUP(void)   
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}   

/**-------------------------------------------------------
  * @函数名 Buffercmp
  * @功能   比较两个缓冲区给定长度的内容是否相同的函数
  * @参数1  pBuffer1 和 pBuffer2 需要比较两个缓冲区起始地址
  * @参数2  BufferLength 给定的比较长度，字节为单位
  * @返回值 PASSED: 缓冲器给定长度的内容相同
  *         FAILED: 缓冲器给定长度的内容不相同
***------------------------------------------------------*/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

/**-----------------------------------------------------------------
  * @函数名 SPI_FLASH_Test
  * @功能   SPI FLASH读写测试
  * @参数   无
  * @返回值 无
***----------------------------------------------------------------*/
void SPI_FLASH_Test(void)
{
	__IO uint32_t FlashID = 0;
	__IO uint32_t DeviceID = 0;

    /*读取SPI_Flash的ID*/
    FlashID = SPI_FLASH_ReadID();

//    printf("\r\n 芯片的ID是 0x%X", FlashID);

    /* 判断读取的SPI_Flash的ID是否正确 */
    if ((FlashID == W25Q16_FLASH_ID))
    {
        /* 擦除SPI FLASH一个扇区 */
        SPI_FLASH_SectorErase(FLASH_SectorToErase);


        /* 写Tx_Buffer数据到SPI FLASH中 */
        SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);

        /* 从SPI FLASH中读取数据*/
        SPI_FLASH_BufferRead(Rx_Buffer, FLASH_WriteAddress, BufferSize);
			//  printf("\r\n 写入W25x16 SPI FLASH的数据是: %s", Rx_Buffer);

        /* 检查写进去和读出来的数据是否一致 */
        TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

        if(PASSED == TransferStatus1)
        {
            //    printf("\r\n 读出数据与写入数据一致!\n\r");
        }
        else
        {
             //   printf("\r\n 错误-->读出数据与写入数据不一致!\n\r");
        }
        
//		/* 擦除SPI FLASH一个扇区 */    
//        SPI_FLASH_SectorErase(FLASH_SectorToErase);

//         /* 从SPI FLASH中读取数据*/
//        SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);

//       /* 检查读出来的数据是否正确 */
//        for (Index = 0; Index < BufferSize; Index++)
//        {
//          if (Rx_Buffer[Index] != 0xFF)
//          {
//            TransferStatus2 = FAILED;
//          }
//        }

//        if(PASSED == TransferStatus2)
//        {
//                printf("\r\n 擦出后读出数据为0xFF，正确!\n\r");
//        }
//        else
//        {
//                printf("\r\n 错误-->擦出后读出数据不为0xFF!\n\r");
//        }

//        if((PASSED == TransferStatus1) && (PASSED == TransferStatus2))
//        {
//            printf("\r\n W25x16读写实验成功!\n\r");
//        }
//        else
//        {
//            printf("\r\n W25x16读写实验失败!\n\r");
//        }
    }

//    else
//    {
//        printf("\r\n W25x16读写实验失败!\n\r");
//    }

    /*SPI FLASH进入掉电模式*/
    SPIFlash_PowerDown();  
}

/******************* (C) COPYRIGHT 2010 www.armjishu.com *****END OF FILE****/
