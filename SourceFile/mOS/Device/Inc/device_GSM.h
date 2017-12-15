
#ifndef __DEVICE_GSM_H
#define __DEVICE_GSM_H

#include "sys.h"



#define SimCheck 0x01
#define CSQCheck 0x02
#define Operator 0x03

typedef enum
{
	GsmDTMFMessage		= 0xFF00,//DTMF����
	GsmSMSMessage		= 0xFE00,//SMS֪ͨ
	GsmRingMessage		= 0xFD00,//����
	GsmCallMessage		= 0xFC00,//ͨ��״̬
	GsmDateMessage		= 0xFB00,//����
	GsmCSQMessage		= 0xFA00,//�ź���
	GsmReadyMessage		= 0xF900,//GSM����
	GsmTSMSINFOMessage	= 0xF800,//������Ϣ������δ������Ϣ��
	GsmSMSRead			= 0xF700,//�յ�����Ϣ���Ѷ�ȡ������
	GsmSMSContent		= 0xF600,
	GsmSMSSendOK		= 0xF500,
  GsmMessageFull      = 0xF400,
  GsmMessageCEND      = 0xF300,
  GsmMessageCONN      = 0xF200,
	
	GsmNotMessage		= 0xF100,//����Ҫ������Ϣ
  GsmMessageOperator  = 0xEF00, 
	GsmBusy				= 0xEE00,
	GsmSimCardQuery		= 0xED00,
	GsmSingQuery		= 0xEC00,
	GsmOperatorQuery	= 0xEB00,
	MessageContactID	= 0xEA00,
  
  GSMSimRemoved      = 0xE900,
  GsmDiscovery       = 0xE800,
  
  GsmNormalMessage   = 0xE700,
	
}GsmMessageEnum;

//��ǰͨ����״̬
typedef enum
{
	GsmCallStopOr1	= 0,//��ǰͨ����ֹ�����ٻ�����һ�������ֵ�ͨ��
	GsmActiveCall	= 1,//��ʼ��������绰
	GsmCallFailure	= 2,//����ʧ��
	GsmCallRing		= 3,//��������绰���Է���������
	GsmIntoCallWait	= 4,//ͨ����ת����еȴ�
	GsmCallConnect	= 5,//�������н�ͨ
	GsmCallStop		= 6,//ͨ����ȫ��ֹ
	GsmCallHangUp	= 7,//ͨ��������
	GsmEmergencyCall= 8,//�ڽ���ģʽ�²���绰���ǽ�������
	GsmCallNoRes	= 9,//��������绰���Է�û��Ӧ
	GsmCallBusy		= 10,//�����绰�Է�æ
}GsmCallStateEnum;
typedef enum
{
	Activation=0xFF,
	StayIn=0xFE,
	Dialing=0xFD,
	Ringing=0xFC,
	Waiting=0xFB,
}GsmCallEnum;
#define GsmPower		PBout(1)//��Դ����


//#define SimcomReset		PCout(9)

#endif

