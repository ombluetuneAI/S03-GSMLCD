/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "BSP.h"
#include "System.h"
#include "boot.h"
#include "emWinGUI.h"

/*--------------------------------------------�궨��-------------------------------------*/

/*---------------------------------------------����--------------------------------------*/



/*---------------------------------------------����--------------------------------------*/



/*---------------------------------------------����--------------------------------------*/
static void SystemDeviceInit(void);
static void SystemGUIInit(void);

extern void LEDInit(void);
extern void LCDInit(void);
extern void WirelessPortInit(void);
extern void TouchKeyInit(void);
extern void VoicePortInit(void);
extern void InitTimer(void);
extern void InitUsart(void);
extern void IconInit(void);
extern void InitStorage(void);
extern u8 	InitRTC(void);
extern void GsmInit(void);
extern void InitFlash(void);
extern void InitAdc(void);
extern void InitHT9200(void);
extern void InitCall(void);
extern void TIM5_Cap_Init(u16 arr,u16 psc);
extern void InitsystemApp(void);
extern void WirelessPortInit(void);
extern void ScreenPointerInit(void);
extern void SendThreeLines(u8 addr);


void DcDetInit(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}
/*----------------------------------------------------------------------------------------
--����      :     ϵͳ��ʼ��
`
----------------------------------------------------------------------------------------*/
extern void LoadDefaultPara(void);
void SystemAllInit(void)
{
  u16 tmp;
  SPIFlashInit();
	SystemGUIInit();  //GUi�Լ���������
	SystemDeviceInit();
  tmp = BKP_ReadBackupRegister(BKP_DR2);
  if (BKP_ReadBackupRegister(BKP_DR2) != DefaultPara)//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{	 
    LoadDefaultPara();    
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
		PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 	
		BKP_WriteBackupRegister(BKP_DR2, DefaultPara);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_SetPriority(SysTick_IRQn, 0xFE);   //�����ȼ�
	NVIC_SetPriority(PendSV_IRQn, 0xFF);    //������ȼ�
}
/*----------------------------------------------------------------------------------------
--����      : ϵͳ��Ӳ����ʼ��
--�������	����
--�������	����
----------------------------------------------------------------------------------------*/

extern void KeySystick100Routine(void) ;
static void SystemDeviceInit(void)
{
  u8 i;
	
	InitSystick();
	
	ScreenPointerInit();
	
	WirelessPortInit();

	VoicePortInit();

	InitTimer();
 
	InitUsart();

	SPIFlashInit();

	//InitStorage();	

	InitRTC();

	InitAdc();

	LEDInit();

	InitFlash();

//	InitCall();

	InitsystemApp();

	//TIM5_Cap_Init(0xFFFF,720-1);
	
	SendThreeLines(22) ;
  
	GsmInit();
  
  System.SystemApp.ScreenSet();
	
	DcDetInit();
	
	TouchKeyInit();
	
	System.Device.Systick.Register(Systick100,KeySystick100Routine);

}
/*----------------------------------------------------------------------------------------
--����      :     GUI��ʼ��

----------------------------------------------------------------------------------------*/
//void ImageDraw(u16 x,u16 y,u8 *color);
//extern const unsigned char gImage_app[12552];
//static void SystemGUIInit(void)
//{
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
//  GUI_Init();
//  LcdLedOn();
//  ImageDraw(50,50,(u8*)gImage_app);
//	LogoDisplay();
//	DelayMs(100);   
//  emWinInit();
//	IconInit();
//}
static void SystemGUIInit(void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  GUI_Init();
	LogoDisplay();
	LcdLedOn();
	DelayMs(100);   
  emWinInit();
//	IconInit();
}

