/*
********************************************************************************

********************************************************************************
*�ļ���     : device_usart.c
*����       : �����豸
*ԭ��       : ���Ͳ���Buffer���ͣ����ղ���Appע�ắ����ʽ����
*******************************************************************************/

#include "device_usart.h"


typedef __packed  struct 
{
    byte    * Start;            //ָ����п�ʼ 
    byte    * End;              //ָ����н��� 
    byte    * In;               //����һ����Ϣ  
    byte    * Out;              //ȡ��һ����Ϣ  
    ushort    Entries;          //��Ϣ����      
}QueueStruct;

#define Txd1BufferSum 1000
static QueueStruct Txd1Queue;
static byte Txd1Buffer[Txd1BufferSum];

static void InitUsartQueue(void)
{
    Txd1Queue.Start = Txd1Buffer;
    Txd1Queue.End = Txd1Buffer + Txd1BufferSum - 1;
    Txd1Queue.In = Txd1Queue.Start;
    Txd1Queue.Out = Txd1Queue.Start;
    Txd1Queue.Entries = 0;
}

/*******************************************************************************
* ����	    : ����1����һ���ֽ�
* �������  : data �ֽ����ͣ����Ͳ���
* ���ز���  : bool ����
*******************************************************************************/
static bool WriteByteToUsart2(byte data) 
{
    if (Txd1Queue.Entries >= Txd1BufferSum) return (false);
    
    Txd1Queue.Entries++;
    
    *Txd1Queue.In++ = data;	
    
    if (Txd1Queue.In > Txd1Queue.End) 
        Txd1Queue.In = Txd1Queue.Start;
    
    return (true);
}

/*******************************************************************************
* ����	    : ����1����һ������
* �������  : dataPointer ����ָ�룬sum ��������
*******************************************************************************/
static void WriteToUsart2(byte * dataPointer, int sum)
{
    if (sum)
    {
        while (sum--) 
            WriteByteToUsart2(*dataPointer++);
    }
    else
    {
        while (*dataPointer) 
        {
            if (*dataPointer == '\n')       //���з�
                WriteByteToUsart2('\r');    //���ӻس���
            WriteByteToUsart2(*dataPointer++);
        }
    }
}
/*******************************************************************************
* ����	    : Keil������֧��printf��������Ҫ���ô˺�����ע�������ҪMicorLib֧��
* �������  : ch ʵΪbyte���ͣ�ʵ�ʷ��͵�����
*           : f �豸������Ҫ����
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    if (ch == '\n')                         //���з�
        WriteByteToUsart2('\r');            //���ӻس���

    WriteByteToUsart2((byte)ch);

    return ch;
}

/*******************************************************************************
* ����	    : ϵͳ����1000/S,��1mSһ�ε��ô˷�������ͻ���������
*******************************************************************************/
void UsartSystick1000Routine(void) 
{
    if (Txd1Queue.Entries == 0) return;

    if ((USART2->SR&0X40)==0) return;

    USART2->DR = *Txd1Queue.Out++;
    Txd1Queue.Entries--;
    
    if (Txd1Queue.Out > Txd1Queue.End) 
        Txd1Queue.Out = Txd1Queue.Start;
}


static void OpenUsart2(void)
{
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

static void CloseUsart2(void)
{
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    USART_Cmd(USART2, DISABLE);
}
     
void InitUsart(void) 
{
//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	
	USART_DeInit(USART2);
	//TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

//NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	
  
//USART ��ʼ������
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

	//USART_ITConfig(USART2,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  
    //USART_ITConfig(USART2,USART_IT_IDLE,ENABLE); 
   
	//USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USART2, ENABLE);  

//ʹ�ܴ��� 

    InitUsartQueue();

    System.Device.Usart1.Open = OpenUsart2;
    System.Device.Usart1.Close = CloseUsart2;
    System.Device.Usart1.WriteByte = WriteByteToUsart2;
    System.Device.Usart1.Write = WriteToUsart2;
}

