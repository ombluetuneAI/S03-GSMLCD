

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
  * @������ SPI_FLASH_Init
  * @����   ��ʼ�����ⲿSPI FLASH�ӿڵ���������
  *         Initializes the peripherals used by the SPI FLASH driver.
  * @����   ��
  * @����ֵ ��
***----------------------------------------------------------------*/
void SPIFlashInit(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

    /*��ʼ��Ƭѡ�źŹܽ�: /CS*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*����Ƭѡ�ź�*/
    SPI_FLASH_CS_HIGH();

    /* SPI1��ʼ������*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  //����ģʽѡ��SCK�ź�����������
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //SPIͨ�����ݵĴ�С
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ʱ�Ӽ���
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;   //ʱ����λ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	  //Ƭѡ�źţ�Ӳ��ģʽ�������ģʽ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//SPI��Ƶֵ����Ƶ���ֵΪSCK��ʱ��Ƶ��
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ѡ��SPIͨ��ʱ���Ǹ�λ������ǰ���ǵ�λ������ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//��CRCУ���й�
    SPI_Init(SPI2, &SPI_InitStructure);

    /*ʹ��SPI1 */
    SPI_Cmd(SPI2, ENABLE);
	
	/*����SPI FLASH*/
    SPIFlash_WAKEUP();
}

/**-----------------------------------------------------------------
  * @������ SPI_FLASH_SectorErase
  * @����   ����SPI FLASHһ����������������
  *         Erases the specified FLASH sector.
  * @����   SectorAddr: ������ַ address of the sector to erase.
  * @����ֵ ��
***----------------------------------------------------------------*/
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
    SPI_FLASH_WriteEnable();

    /* ����Ƭѡ�ź�*/
    SPI_FLASH_CS_LOW();
    /* Send Sector Erase instruction */
    SPI_FLASH_SendByte(W25X_SectorErase);
    /* Send SectorAddr high nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    SPI_FLASH_SendByte(SectorAddr & 0xFF);

    /* ����Ƭѡ�ź� */
    SPI_FLASH_CS_HIGH();

    /* �ȴ�д��SPI�Ĳ������*/
    SPI_FLASH_WaitForWriteEnd();
}

/**-----------------------------------------------------------------
  * @������ SPI_FLASH_ChipErase
  * @����   ��������SPI FLASH����������
  *         Erases the entire FLASH.
  * @����   ��
  * @����ֵ ��
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
  * @������ SPI_FLASH_PageWrite
  * @����   ����д����д�����һ���ֽڶ�С�ڵ���һҳ��С������
  *         Writes more than one byte to the FLASH with a single 
  *         WRITE cycle(Page WRITE sequence). The number of byte 
  *         can't exceed the FLASH page size.
  * @����   - pBuffer : ָ�����д�����ݻ������ĵ�ַָ��
  *             pointer to the buffer  containing the data to be
  *             written to the FLASH.
  *         - WriteAddr : flash��д���ַ
  *             FLASH's internal address to write to.
  *         - NumByteToWrite : д����ֽ���
  *             number of bytes to write to the FLASH, must be
  *             equal or less than "SPI_FLASH_PageSize" value.
  * @����ֵ ��
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
  * @������ SPI_FLASH_BufferWrite
  * @����   ��SPI FLASHд��һ�����ݣ�д����ֽ������Դ���һҳ����
  *         Writes block of data to the FLASH. In this function,
  *         the number of WRITE cycles are reduced,
  *         using Page WRITE sequence.
  * @����   - pBuffer : ָ�����д�����ݻ������ĵ�ַָ��
  *             pointer to the buffer  containing the data to be
  *             written to the FLASH.
  *         - WriteAddr : flash��д���ַ
  *             FLASH's internal address to write to.
  *         - NumByteToWrite : д����ֽ���
  *             number of bytes to write to the FLASH.
  * @����ֵ ��
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
  * @������ SPI_FLASH_BufferRead
  * @����   ��SPI FLASH����һ�����ݣ�д����ֽ������Դ���һҳ����
  *         Reads a block of data from the FLASH.
  * @����   - pBuffer : ָ�����д�����ݻ������ĵ�ַָ��
  *             pointer to the buffer that receives the data read
  *             from the FLASH.
  *         - ReadAddr : flash�Ķ���ʼ��ַ
  *             FLASH's internal address to read from.
  *         - NumByteToWrite : �������ֽ���
  *             number of bytes to read from the FLASH.
  * @����ֵ ��
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
  * @������ SPI_FLASH_ReadID
  * @����   ��ȡSPI FLASH����ID���豸ID(�豸ID�������ͺ�����)
  *         Reads Manufacturer ID and two Device ID bytes
  * @����   ��
  * @����ֵ 24bit���ߵ�������Ϊ����ID�����ͺ�����
***----------------------------------------------------------------*/
u32 SPI_FLASH_ReadID(void)
{
    u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /*����SPI_FLASHƬѡ�ź���*/
    SPI_FLASH_CS_LOW();

    /* ��ѭ��ȡID ʱ�򣬷�������*/
    SPI_FLASH_SendByte(W25X_JedecDeviceID);
    Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
    Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
    Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

     /*����SPI_FLASHƬѡ�ź���*/
    SPI_FLASH_CS_HIGH();

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

/**-----------------------------------------------------------------
  * @������ SPI_FLASH_ReadDeviceID
  * @����   ��ȡSPI FLASH�豸ID
  *         Read one Device ID bytes
  * @����   ��
  * @����ֵ һ���ֽڵ�Device ID
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
  * @������ SPI_FLASH_StartReadSequence
  * @����   ����һ����ȡSPI FLASH�ķ��ʣ��������Ͷ��������ʼ��ַ
  *         Initiates a read data byte (READ) sequence from the Flash.
  *         This is done by driving the /CS line low to select the device,
  *         then the READ instruction is transmitted followed by 3 bytes
  *         address. This function exit and keep the /CS line low, so the
  *         Flash still being selected. With this technique the whole
  *         content of the Flash is read with a single READ instruction.
  *         Read one Device ID bytes
  * @����   ReadAddr FLASH�ķ��ʵ�ַ
  *                  FLASH's internal address to read from.
  * @����ֵ ��
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
  * @������ SPI_FLASH_ReadByte
  * @����   ��ȡSPI FLASH��һ���ֽڣ�δ�������Ͷ��������ʼ��ַ
  * @����   ��
  * @����ֵ ��SPI_FLASH��ȡ��һ���ֽ�
***----------------------------------------------------------------*/
u8 SPI_FLASH_ReadByte(void)
{
    return (SPI_FLASH_SendByte(Dummy_Byte));
}


/**-----------------------------------------------------------------
  * @������ SPI_FLASH_SendByte
  * @����   ͨ��SPI���߷���һ���ֽ�����(˳�����һ���ֽ�����)
  *         Sends a byte through the SPI interface and return the byte
  *         received from the SPI bus.
  * @����   Ҫд���һ���ֽ�����
  * @����ֵ �ڷ�����ʱ��MISO�ź����Ͻ��յ�һ���ֽ�
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
  * @������ SPI_FLASH_SendHalfWord
  * @����   ͨ��SPI���߷���һ������(16bit=2���ֽ�����)(˳���������)
  *         Sends a Half Word through the SPI interface and return the
  *         Half Word received from the SPI bus.
  * @����   Ҫд���һ����������(16bit)
  * @����ֵ �ڷ�����ʱ��MISO�ź����Ͻ��յ�һ����������(16bit)
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
  * @������ SPI_FLASH_WriteEnable
  * @����   SPI FLASHдʹ��
  *         Enables the write access to the FLASH.
  * @����   ��
  * @����ֵ ��
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
  * @������ SPI_FLASH_WaitForWriteEnd
  * @����   ͨ��������ȡSPI FLASH��״̬�Ĵ����ж�д���Ƿ�ִ�н���
  *         Polls the status of the Write In Progress (WIP) flag in the
  *         FLASH's status  register  and  loop  until write  opertaion
  *         has completed.
  * @����   ��
  * @����ֵ ��
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
  * @������ SPI_Flash_PowerDown
  * @����   SPI FLASH�������ģʽ
  * @����   ��
  * @����ֵ ��
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
  * @������ SPI_Flash_WAKEUP
  * @����   ����SPI FLASH
  * @����   ��
  * @����ֵ ��
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
  * @������ Buffercmp
  * @����   �Ƚ������������������ȵ������Ƿ���ͬ�ĺ���
  * @����1  pBuffer1 �� pBuffer2 ��Ҫ�Ƚ�������������ʼ��ַ
  * @����2  BufferLength �����ıȽϳ��ȣ��ֽ�Ϊ��λ
  * @����ֵ PASSED: �������������ȵ�������ͬ
  *         FAILED: �������������ȵ����ݲ���ͬ
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
  * @������ SPI_FLASH_Test
  * @����   SPI FLASH��д����
  * @����   ��
  * @����ֵ ��
***----------------------------------------------------------------*/
void SPI_FLASH_Test(void)
{
	__IO uint32_t FlashID = 0;
	__IO uint32_t DeviceID = 0;

    /*��ȡSPI_Flash��ID*/
    FlashID = SPI_FLASH_ReadID();

//    printf("\r\n оƬ��ID�� 0x%X", FlashID);

    /* �ж϶�ȡ��SPI_Flash��ID�Ƿ���ȷ */
    if ((FlashID == W25Q16_FLASH_ID))
    {
        /* ����SPI FLASHһ������ */
        SPI_FLASH_SectorErase(FLASH_SectorToErase);


        /* дTx_Buffer���ݵ�SPI FLASH�� */
        SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);

        /* ��SPI FLASH�ж�ȡ����*/
        SPI_FLASH_BufferRead(Rx_Buffer, FLASH_WriteAddress, BufferSize);
			//  printf("\r\n д��W25x16 SPI FLASH��������: %s", Rx_Buffer);

        /* ���д��ȥ�Ͷ������������Ƿ�һ�� */
        TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

        if(PASSED == TransferStatus1)
        {
            //    printf("\r\n ����������д������һ��!\n\r");
        }
        else
        {
             //   printf("\r\n ����-->����������д�����ݲ�һ��!\n\r");
        }
        
//		/* ����SPI FLASHһ������ */    
//        SPI_FLASH_SectorErase(FLASH_SectorToErase);

//         /* ��SPI FLASH�ж�ȡ����*/
//        SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);

//       /* ���������������Ƿ���ȷ */
//        for (Index = 0; Index < BufferSize; Index++)
//        {
//          if (Rx_Buffer[Index] != 0xFF)
//          {
//            TransferStatus2 = FAILED;
//          }
//        }

//        if(PASSED == TransferStatus2)
//        {
//                printf("\r\n �������������Ϊ0xFF����ȷ!\n\r");
//        }
//        else
//        {
//                printf("\r\n ����-->������������ݲ�Ϊ0xFF!\n\r");
//        }

//        if((PASSED == TransferStatus1) && (PASSED == TransferStatus2))
//        {
//            printf("\r\n W25x16��дʵ��ɹ�!\n\r");
//        }
//        else
//        {
//            printf("\r\n W25x16��дʵ��ʧ��!\n\r");
//        }
    }

//    else
//    {
//        printf("\r\n W25x16��дʵ��ʧ��!\n\r");
//    }

    /*SPI FLASH�������ģʽ*/
    SPIFlash_PowerDown();  
}

/******************* (C) COPYRIGHT 2010 www.armjishu.com *****END OF FILE****/
