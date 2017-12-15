/*
********************************************************************************

********************************************************************************
*文件名     : device_usart.c
*作用       : 串口设备
*原理       : 发送采用Buffer发送，接收采用App注册函数方式接收
*******************************************************************************/

#include "device_usart.h"


typedef __packed  struct 
{
    byte    * Start;            //指向队列开始 
    byte    * End;              //指向队列结束 
    byte    * In;               //插入一个消息  
    byte    * Out;              //取出一个消息  
    ushort    Entries;          //消息长度      
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
* 描述	    : 串口1发送一个字节
* 输入参数  : data 字节类型，发送参数
* 返回参数  : bool 类型
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
* 描述	    : 串口1发送一串数据
* 输入参数  : dataPointer 数据指针，sum 数据总数
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
            if (*dataPointer == '\n')       //换行符
                WriteByteToUsart2('\r');    //增加回车符
            WriteByteToUsart2(*dataPointer++);
        }
    }
}
/*******************************************************************************
* 描述	    : Keil编译器支持printf函数，需要调用此函数，注意这个需要MicorLib支持
* 输入参数  : ch 实为byte类型，实际发送的数据
*           : f 设备，不需要处理
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    if (ch == '\n')                         //换行符
        WriteByteToUsart2('\r');            //增加回车符

    WriteByteToUsart2((byte)ch);

    return ch;
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序发送缓冲区数据
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
//GPIO端口设置
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

//NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	
  
//USART 初始化设置
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

//使能串口 

    InitUsartQueue();

    System.Device.Usart1.Open = OpenUsart2;
    System.Device.Usart1.Close = CloseUsart2;
    System.Device.Usart1.WriteByte = WriteByteToUsart2;
    System.Device.Usart1.Write = WriteToUsart2;
}

