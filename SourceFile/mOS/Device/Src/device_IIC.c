#include "device_IIC.h"
#include "delay.h"
#include "Device_GSM.h"

/***********************************************************************************

��ʼ��IIC
***********************************************************************************/
void IICInit(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTBʱ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	
//    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9); 
    
 
	IICSCL1=1;
	IICSDA1=1;
}
/***************************************************************************************
����IIC��ʼ�ź�
***************************************************************************************/
void IICStart(u8 number)
{
    if(number==1)
    {
        SDAOUT1();     //sda�����
        IICSDA1=1;	  	  
        IICSCL1=1;
        DelayUs(4);
        IICSDA1=0;//START:when CLK is high,DATA change form high to low 
        DelayUs(4);
        IICSCL1=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
    }
    else
    {
        SDAOUT2();     //sda�����
        IICSDA2=1;	  	  
        IICSCL2=1;
        DelayUs(4);
        IICSDA2=0;//START:when CLK is high,DATA change form high to low 
        DelayUs(4);
        IICSCL2=0;//ǯסI2C���ߣ�׼�����ͻ��������         
    }
}	  
/***************************************************************************************

����IICֹͣ�ź�

***************************************************************************************/
void IICStop(u8 number)
{
    if(number==1)
    {
        SDAOUT1();//sda�����
        IICSCL1=0;
        IICSDA1=0;//STOP:when CLK is high DATA change form low to high
        DelayUs(4);
        IICSCL1=1; 
        IICSDA1=1;//����I2C���߽����ź�
        DelayUs(4);	
    }
    else
    {
        SDAOUT2();//sda�����
        IICSCL2=0;
        IICSDA2=0;
        DelayUs(4);
        IICSCL2=1; 
        IICSDA2=1;//����I2C���߽����ź�
        DelayUs(4);	       
    }
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IICWaitAck(u8 number)
{
	u16 ucErrTime=0;
    if(number==1)
    {
    	SDAIN1();      //SDA����Ϊ����  
    	IICSDA1=1;
        DelayUs(1);	   
    	IICSCL1=1;
        DelayUs(1);	 
    	while(READSDA1)
    	{
    		ucErrTime++;
    		if(ucErrTime>250)
    		{
    			IICStop(1);
    			return 1;
    		}
    	}
    	IICSCL1=0;//ʱ�����0 	
    }
    else
    {
        SDAIN2();      //SDA����Ϊ����  
        IICSDA2=1;
        DelayUs(1);       
        IICSCL2=1;
        DelayUs(1);     
        while(READSDA2)
        {
            ucErrTime++;
            if(ucErrTime>250)
            {
                IICStop(2);
                return 1;
            }
        }    
        IICSCL2=0;//ʱ�����0 
    }   
	return 0;  
} 
//����ACKӦ��
void IICAck(u8 number)
{
    if(number==1)
    {
    	IICSCL1=0;
    	SDAOUT1();
    	IICSDA1=0;
    	DelayUs(2);
    	IICSCL1=1;
    	DelayUs(2);
    	IICSCL1=0;
		IICSDA1=1;
    }
    else
    {
        IICSCL2=0;
        SDAOUT2();
        IICSDA2=0;
        DelayUs(2);
        IICSCL2=1;
        DelayUs(2);
        IICSCL2=0;    
		IICSDA2=1;		
    }
}
	
/*************************************************************************
*

*************************************************************************/
void IICNAck(u8 number)
{
    if(number)
    {
        IICSCL1=0;
        SDAOUT1();
        IICSDA1=1;
        DelayUs(2);
        IICSCL1=1;
        DelayUs(2);
        IICSCL1=0;
    }
    else
    {
        IICSCL2=0;
        SDAOUT2();
        IICSDA2=1;
        DelayUs(2);
        IICSCL2=1;
        DelayUs(2);
        IICSCL2=0;
    }
}				 				     

/***************************************************************************
*����		:	IIC����һ���ֽ�
*�������	:	txd,Ҫ���͵��ֽ�
*�������	��	1����Ӧ��;0����Ӧ��	
***************************************************************************/
void IICSendByte(u8 txd,u8 number)
{                        
    u8 t;   
    if(number==1)
    {
        SDAOUT1();       
        IICSCL1=0;//����ʱ�ӿ�ʼ���ݴ���
        for(t=0;t<8;t++)
        {              
            IICSDA1=(txd&0x80)>>7;
            txd<<=1;      
            DelayUs(2);   
            IICSCL1=1;
            DelayUs(2); 
            IICSCL1=0;   
            DelayUs(2);
        }
    }
    else
    {
        SDAOUT2();       
        IICSCL2=0;//����ʱ�ӿ�ʼ���ݴ���
        for(t=0;t<8;t++)
        {              
            IICSDA2=(txd&0x80)>>7;
            txd<<=1;      
            DelayUs(2);   
            IICSCL2=1;
            DelayUs(2); 
            IICSCL2=0;   
            DelayUs(2);
        }
    }	 
} 
/*********************************************************************

*����		:	��1���ֽ� 
*�������	:	��ack=1ʱ������ACK��ack=0������nACK   
*�������	��	���ض���������
*********************************************************************/

u8 IICReadByte(u8 ack,u8 number)
{
	unsigned char i,receive=0;

    if(number)
    {
    	SDAIN1();//SDA����Ϊ����
        for(i=0;i<8;i++ )
    	{
            IICSCL1=0; 
            DelayUs(2);
    		IICSCL1=1;
            receive<<=1;
            if(READSDA1)
                receive++;   
    		DelayUs(1); 
        }					 
        if (!ack)
            IICNAck(1);//����nACK
        else
            IICAck(1); //����ACK   
    }
    else
    {
        SDAIN2();//SDA����Ϊ����
        for(i=0;i<8;i++ )
        {
            IICSCL2=0; 
            DelayUs(2);
            IICSCL2=1;
            receive<<=1;
            if(READSDA2)
                receive++;   
            DelayUs(1); 
        }                    
        if (!ack)
            IICNAck(2);//����nACK
        else
            IICAck(2); //����ACK   
    }
    return receive;
}



























