/*--------------------------------------------头文件-------------------------------------*/
#include "BSP.h"
#include "System.h"
#include "boot.h"
#include "emWinGUI.h"

/*--------------------------------------------宏定义-------------------------------------*/

/*---------------------------------------------变量--------------------------------------*/



/*---------------------------------------------类型--------------------------------------*/



/*---------------------------------------------函数--------------------------------------*/
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
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//悬浮输入
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}
/*----------------------------------------------------------------------------------------
--描述      :     系统初始化
`
----------------------------------------------------------------------------------------*/
extern void LoadDefaultPara(void);
void SystemAllInit(void)
{
  u16 tmp;
  SPIFlashInit();
	SystemGUIInit();  //GUi以及开机界面
	SystemDeviceInit();
  tmp = BKP_ReadBackupRegister(BKP_DR2);
  if (BKP_ReadBackupRegister(BKP_DR2) != DefaultPara)//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	 
    LoadDefaultPara();    
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 	
		BKP_WriteBackupRegister(BKP_DR2, DefaultPara);	//向指定的后备寄存器中写入用户程序数据
	}
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_SetPriority(SysTick_IRQn, 0xFE);   //次优先级
	NVIC_SetPriority(PendSV_IRQn, 0xFF);    //最低优先级
}
/*----------------------------------------------------------------------------------------
--描述      : 系统的硬件初始化
--输入参数	：无
--输出参数	：无
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
--描述      :     GUI初始化

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

