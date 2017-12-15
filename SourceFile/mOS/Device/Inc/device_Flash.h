
#ifndef __DEVICE_FLASH_H
#define __DEVICE_FLASH_H


#include "sys.h"
#define AutoBase    40

//�û������Լ�����Ҫ����
#define STM32Capacity 		768	 		//��ѡSTM32��FLASH������С(��λΪK)
#define FlashWriteEnable 	1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define LogAlarmAddr        0x0807E000//��־��ʼ��ַ 
#define LogDisarmAddr       0x0807E1E0
#define LogArmAddr          0x0807E3C0

#define Auto1Addr           0x0807E5A0
#define Auto2Addr           Auto1Addr+(AutoBase*1)
#define Auto3Addr           Auto1Addr+(AutoBase*2)
#define Auto4Addr           Auto1Addr+(AutoBase*3)
#define Auto5Addr           Auto1Addr+(AutoBase*4)
#define Auto6Addr           Auto1Addr+(AutoBase*5)
#define Auto7Addr           Auto1Addr+(AutoBase*6)
#define Auto8Addr           Auto1Addr+(AutoBase*7)

#define Auto1State          Auto8Addr+60
#define Auto2State          Auto1State+2
#define Auto3State          Auto1State+4
#define Auto4State          Auto1State+6
#define Auto5State          Auto1State+8
#define Auto6State          Auto1State+10
#define Auto7State          Auto1State+12
#define Auto8State          Auto1State+14


/***************************************************************
��־��ʽ��	״̬ + �� + �� + �� + ʱ + �� + ������ʽ + ����
             1   + 1  + 1  + 1  + 1  + 1  +	1        +  1  =  8byte 

�洢50��
***************************************************************/

#define AdminPasswordAddr	0x0807F800//����Ա����
#define UserPasswordAddr	0x0807F80A//�û�����



#define CIDArmUploadAddr		0x0807FDC0
#define CIDDisarmUploadAddr	0x0807FDC2
#define CIDStayUploadAddr		0x0807FDC4//-----------------********************-------------------------------
#define CIDUserCodeAddr			0x0807FDC6

#define LogAlarmTotalAddr   0x0807FDBA
#define LogArmTotalAddr     0x0807FDBC
#define LogDisarmTotalAddr  0x0807FDBE

#define ResetAddr			      0x0807F814//GSM
#define VoiceVolumeAddr		  0x0807FC96
#define AlarmVolumeAddr		  0x0807F816//����
#define ArmDelayAddr		    0x0807F818//��ʱ��������
#define AlarmDelayAddr		  0x0807F81A//��ʱ����ʱ��

#define SirenDelayAddr		  0x0807F81C//��������ʱ��
#define RingTimesAddr		    0x0807F81E//�������

#define SMSReplyAddr		    0x0807F820//���Żظ�
#define SecurityCenterAddr	0x0807F822//�������ϴ�����
#define SMSNoticeAddr		    0x0807F826//���������Źرջ��߿���

#define WirelessSirenAddr	  0x0807F828//���߾��ſ������߹ر�

#define PhoneMember1Addr	0x0807F000	
#define PhoneMember2Addr	0x0807F01E
#define PhoneMember3Addr	0x0807F03C
#define PhoneMember4Addr	0x0807F05A
#define PhoneMember5Addr	0x0807F078
#define PhoneMember6Addr	0x0807F096
#define PhoneMember7Addr	0x0807F0B4
#define PhoneMember8Addr	0x0807F0D2

#define PhoneName1Addr		0x0807F12E
#define PhoneName2Addr		0x0807F142
#define PhoneName3Addr		0x0807F156
#define PhoneName4Addr		0x0807F16A
#define PhoneName5Addr		0x0807F17E
#define PhoneName6Addr		0x0807F192
#define PhoneName7Addr		0x0807F1A6
#define PhoneName8Addr		0x0807F1BA

#define Phone1TypeAddr		PhoneName1Addr-4
#define Phone2TypeAddr		PhoneName2Addr-4
#define Phone3TypeAddr		PhoneName3Addr-4
#define Phone4TypeAddr		PhoneName4Addr-4
#define Phone5TypeAddr		PhoneName5Addr-4
#define Phone6TypeAddr		PhoneName6Addr-4
#define Phone7TypeAddr		PhoneName7Addr-4
#define Phone8TypeAddr		PhoneName8Addr-4

#define IllegalArmAddr		0x0807F8DE//�Ƿ�����

/**********************************************************************************************************
ң��
��17��-----�����ң���ǲ��Ǵ���
��18��-----��ң����û�п�����
**********************************************************************************************************/
//ң��ID�惦
#define RemoteBaseAddr  0x0807F8EE

#define Remote1Addr			RemoteBaseAddr  //0x0807F8EE
#define Remote2Addr 		Remote1Addr+4   //0x0807F8F2
#define Remote3Addr			Remote2Addr+4   //0x0807F8F6
#define Remote4Addr			Remote3Addr+4   //0x0807F8FA
#define Remote5Addr			Remote4Addr+4   //0x0807F8FE
#define Remote6Addr			Remote5Addr+4   //0x0807F902
#define Remote7Addr			Remote6Addr+4   //0x0807F906
#define Remote8Addr			Remote7Addr+4   //0x0807F90A


//ң�����ִ洢
#define Remote1NameAddr		0x0807FCDC	
#define Remote2NameAddr		0x0807FCF0
#define Remote3NameAddr		0x0807FD04
#define Remote4NameAddr		0x0807FD18	
#define Remote5NameAddr		0x0807FD2C
#define Remote6NameAddr		0x0807FD40
#define Remote7NameAddr		0x0807FD54
#define Remote8NameAddr		0x0807FD68

//ң�����ʹ洢
#define Remote1TypeAddr		0x0807FCEE
#define Remote2TypeAddr 	0x0807FD02
#define Remote3TypeAddr		0x0807FD16
#define Remote4TypeAddr		0x0807FD2A
#define Remote5TypeAddr		0x0807FD3E
#define Remote6TypeAddr		0x0807FD52
#define Remote7TypeAddr		0x0807FD66
#define Remote8TypeAddr		0x0807FD7A



//�����о��ŵ�״̬�洢
#define Zone1SirenAddr		0x0807FCDC
#define Zone30SirenAddr		0x0807F9DA

//����״̬�洢
#define RemoteTotalAddr		0x0807F9DC//ң������
#define SystemModeAddr		0x0807F9DE//
#define DetectorTotalAddr	0x0807FDA0//
#define SirenStateAddr		0x0807FDB2//

#define Alarm3GAddr         0x0807FDB4
#define AlarmPSTNAddr       0x0807FDB6
#define Alarm3gAndPSTNAddr  0x0807FDB8

#define AttributeNormalAddr		0x0807FDA6
#define AttributeStayAddr		0x0807FDA8
#define AttributeDoorbellAddr	0x0807FDAA
#define AttributeUrgentAddr		0x0807FDAC
#define AttributeHomeAddr		0x0807FDAE
#define AttributeCloseAddr		0x0807FDB0

//��������
#define Zone1NameAddr		0x0807FA18
#define Zone2NameAddr		0x0807FA2C
#define Zone3NameAddr		0x0807FA40
#define Zone4NameAddr		0x0807FA54
#define Zone5NameAddr		0x0807FA68
#define Zone6NameAddr		0x0807FA7C
#define Zone7NameAddr		0x0807FA90
#define Zone8NameAddr		0x0807FAA4
#define Zone9NameAddr		0x0807FAB8
#define Zone10NameAddr		0x0807FACC
#define Zone11NameAddr		0x0807FAE0
#define Zone12NameAddr		0x0807FAF4
#define Zone13NameAddr		0x0807FB08
#define Zone14NameAddr		0x0807FB1C
#define Zone15NameAddr		0x0807FB30
#define Zone16NameAddr		0x0807FB44
#define Zone17NameAddr		0x0807FB58
#define Zone18NameAddr		0x0807FB6C
#define Zone19NameAddr		0x0807FB80
#define Zone20NameAddr		0x0807FB94
#define Zone21NameAddr		0x0807FBA8
#define Zone22NameAddr		0x0807FBBC
#define Zone23NameAddr		0x0807FBD0
#define Zone24NameAddr		0x0807FBE4
#define Zone25NameAddr		0x0807FBF8
#define Zone26NameAddr		0x0807FC0C
#define Zone27NameAddr		0x0807FC20
#define Zone28NameAddr		0x0807FC34
#define Zone29NameAddr		0x0807FC48
#define Zone30NameAddr		0x0807FC5C


#define YearAddr			0x0807FC86
#define MonthAddr			0x0807FC8A
#define DayAddr				0x0807FC8C
#define HourAddr			0x0807FC8E
#define MinAddr				0x0807FC90
#define SecAddr				0x0807FC94

#define KeyToneStateAddr	0x0807FC98
#define ScreensaverTimeAddr	0x0807FC9A
#define SMSReportAddr		0x0807FC9C

#define ArmSMSNoticeAddr	0x0807FC9E
#define DisarmSMSNoticeAddr	0x0807FCA0
#define HomeSMSNoticeAddr	0x0807FCA2

#define Screen30SecAddr		0x0807FCA4
#define Screen1MinAddr		0x0807FCA6
#define Screen2MinAddr		0x0807FCA8
#define Screen5MinAddr		0x0807FCAA
#define Screen10MinAddr		0x0807FCAC
#define ScreenNeverAddr		0x0807FCAE

#define PowerOFFVoiceAddr	0x0807FCB0
#define PowerOFFSirenAddr	0x0807FCB2
#define PowerOFFSMSAddr		0x0807FCB4
#define PowerOFFCallAddr	0x0807FCB6

#define PowerONVoiceAddr	0x0807FCB8
#define PowerONSirenAddr	0x0807FCBA
#define PowerONSMSAddr		0x0807FCBC
#define PowerONCallAddr		0x0807FCBE


#define PowerLowVoiceAddr	0x0807FCC0
#define PowerLowSirenAddr	0x0807FCC2
#define PowerLowSMSAddr		0x0807FCC4
#define PowerLowCallAddr	0x0807FCC6


#define DetectorVoiceAddr	0x0807FCC8
#define DetectorSrienAddr	0x0807FCCA
#define DetectorSMSAddr		0x0807FCCC
#define DetectorCallAddr	0x0807FCCE

#define DeleteRemoterAddr	0x0807FCD0
#define DeleteSensorsAddr	0x0807FCD2
#define DeletePhoneAddr		0x0807FCD4
#define DeleteRestAddr		0x0807FCD6

#define LanguageSetAddr		0x0807FCD8
#define OutputWithalarmAddr	0x0807FCDA



#define ZoneDetectorAddr	0x0807D000
//FLASH��ʼ��ַ
#define STM32FlashBASE 		0x08000000 	//STM32 FLASH����ʼ��ַ


#endif

