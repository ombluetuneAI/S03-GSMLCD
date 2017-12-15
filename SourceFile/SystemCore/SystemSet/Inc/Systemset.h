#ifndef __SYSTEMSET_H
#define __SYSTEMSET_H

#include "stm32F10x.h"
#include "sys.h"
#include "System.h"

#define ArmState			1
#define DisarmState			2
#define StayState			3
#define	Doorbellstate		4

typedef enum
{
	SystemArm			=1,//����
	SysteHome			=2,//�ڼҲ���
	SystemDisarm		=0,//����
	SystemMonitor		=3,//�ز�����
	SystemQuery			=6,//ϵͳ��ѯ
	
	SystemPassWord		=24,//����	
	SystemDate			=20,//ʱ���趨	
	SystemDelay			=17,//��ʱ
	SystemSiren			=22,//����ʱ���趨
	
	SystemVolume		=21,//ϵͳ����
	SystemAuto			=18,//��ʱ������
	SystemAutoQuery		=19,
	SystemRemote		=13,//ң������
	SystemRemoteDel		=16,
	SystemZone			=12,//��������
	SystemRing			=23,//�������
	SystemSMSAck		=31,//���Żظ�����
	SystemUpload		=26,//�������ϴ�����
	SystemSMSReport		=28,//����������֪ͨ
	SystemLangrage		=35,//����ѡ��	
	
	SystemIllegalArm	=33,//�Ƿ�����
	SystemPhoneSet		=10,//���õ绰
	SystemPhoneDel		=11,//��ѯ�绰
	SystemWirelessSiren	=15,//���߾���
	
}SystemEnum;



#endif

