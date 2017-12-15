/*********************************************************************************
*�ļ���     : device_systick.c
*����       : ϵͳ����
*ԭ��       : 
*******************************************************************************/



#include "sys.h"
#include "system.h"

extern void KeySystick100Routine(void);

extern void TimerSystick1000Routine(void);

extern void UsartSystick1000Routine(void);

//extern void AdcSystick10000Routine(void);

extern void WirelessSystick100Routine(void);

extern void SysTick_Handler(void);

extern void InitSystick(void);

extern void DTMFSystick1000Routine(void);

extern void SendMessageSystick100Routine(void);

extern void CallMessageSystick100Routine(void);

extern void CallRingSystick1000Routine(void);

void CidMessageSystick100Routine(void);
	

extern u8 RTCGetTime(void);//����ʱ�� ;
extern void DateSystick1000Routine(void);
extern void Sim800SystickRoutine(void);

extern __IO int32_t OS_TimeMS;

#define Systick10000Sum 2
#define Systick1000Sum  4
#define Systick100Sum   10

static void Dummy(void) {};

static function Systick10000RegisterPointerBlock[Systick10000Sum] =
{
    Dummy, Dummy
};

static function Systick1000RegisterPointerBlock[Systick1000Sum] =
{
    Dummy, Dummy, Dummy, Dummy
};

static function Systick100RegisterPointerBlock[Systick100Sum] =
{
    Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy
};

/*******************************************************************************
* ����	    : ϵͳ����ע�ắ��������ע��ص���������ͬ�ٶȵ�ϵͳ���������С�
* �������  : systemTick ϵͳ�����ٶ����� registerFunction ע��ĺ���ָ��
* ���ز���  : bool���������������Ƿ�ɹ�
*******************************************************************************/
static bool RegisterSystick(SystickEnum type, function registerFunction)
{
    static byte Systick100Counter = 0;
    static byte Systick1000Counter = 0;
    static byte Systick10000Counter = 0;

    switch(type)
    {
        case Systick100:
            if (Systick100Counter == Systick100Sum) return(false);
            Systick100RegisterPointerBlock[Systick100Counter++] = registerFunction;  
            return(true);
        case Systick1000:
            if (Systick1000Counter == Systick1000Sum) return(false);
            Systick1000RegisterPointerBlock[Systick1000Counter++] = registerFunction;   
            return(true);
        case Systick10000:
            if (Systick10000Counter == Systick10000Sum) return(false);
            Systick10000RegisterPointerBlock[Systick10000Counter++] = registerFunction;   
            return(true);
        default:
            return(false);
    }
}
/*******************************************************************************
* ����	    : ϵͳ�����ж���ڣ��ǳ���Ҫ��ÿ��10000�Σ���0.1mSһ��
*           : ϵͳ����ͨ��switchϸ��Ϊÿ��1000�Σ�ÿ��100�ε����н���
*           : ͬ��ֱ�ӵ��ã��߲����ע�����
*******************************************************************************/
u32 SystemRunTime=0;
void SysTick_Handler(void)
{
  byte div;
  static byte Counter = 0;
//	static byte keyTimes=0;
  if(++Counter == 100) Counter = 0;	
//	Sim800SystickRoutine();
	//DateSystick1000Routine();
    //Systick10000RegisterPointerBlock[0]();
    //Systick10000RegisterPointerBlock[1]();
    //OS_TimeMS ++;
	
    div = Counter / 10;
    switch(Counter % 10)
    {
        case 0:
          SystemRunTime++;  
//            AdcSystick10000Routine();
            break;
        case 1:
            Systick1000RegisterPointerBlock[1]();
            break;
        case 2:
            Systick1000RegisterPointerBlock[2]();
            break;
        case 3:
            Systick1000RegisterPointerBlock[3]();
            break;
        case 4:
            UsartSystick1000Routine();
            break;
        case 5:
            TimerSystick1000Routine();
            break;
        case 6:
			CallMessageSystick100Routine();
			CidMessageSystick100Routine();
            break;
        case 7:
//						DateSystick1000Routine();
            SendMessageSystick100Routine();
            break;
        case 8:
            Systick100RegisterPointerBlock[div]();
            break;
        case 9:
		switch(div)//1msִ��һ�Σ������ÿһ��10msִ��һ��
		{
//			case 0:
//				
//				break;
//			case 1:
//				if(++keyTimes>=5)
//				{
//					keyTimes=0;
//					KeySystick100Routine();
//				}
//				break;
			case 0:
				DTMFSystick1000Routine();
				break;
			case 1:
        CallRingSystick1000Routine();
				break;
			case 2:
        Sim800SystickRoutine();
				break;
			default:
				break;
		}
		break;
    }
	//WirelessSystick100Routine();	
}

void InitSystick(void)
{
    System.Device.Systick.Register = RegisterSystick;
    SysTick_Config(SystemCoreClock / 10000);
}



