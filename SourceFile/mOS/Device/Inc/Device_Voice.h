#ifndef __DEVICE_VOICE_H
#define __DEVICE_VOICE_H

/*--------------------------------------------ͷ�ļ�-------------------------------------*/
#include "stm32F10x.h"
#include "sys.h"
/*---------------------------------------------����--------------------------------------*/

/*--------------------------------------------�궨��-------------------------------------*/


#define MicPowerCtrl 	      PCout(7)
#define SirenCtrl           PAout(1)


#define VoiceEN			        PDout(13)
#define IsLoudSpenkerOpened PDin(13)

#define RecordData		      PEout(4)
#define PlayData            PEout(5)


#define WTclk			          PCout(10)
#define WTcs			          PCout(11)
#define WTdata			        PCout(12)

#define CD4053Sa		PAout(5)
#define CD4053Sb		PAout(6)
#define CD4053Sc		PAout(7)

#define CloseSpeaker()	    VoiceEN	=0
#define OpenSpeaker()	      VoiceEN	=1 

#define OpenSiren()         SirenCtrl=0
#define CloseSiren()        SirenCtrl=1

#define WT5880DVolume   0xE0
/*---------------------------------------------����--------------------------------------*/


/*---------------------------------------------����--------------------------------------*/
typedef enum
{
	StartV				=0x7E,	    //��ʼ����

	RecordFileName 		=0xA0,		//��ʼָ���ļ���¼��
	RecordNumber		=0xA1,		//��ʼ���¼��
	VoicePlayName		=0xA2,		//���ļ�����������
	VoicePlayNumber		=0xA3,		//����Ų�������
	VoiceVolume			=0xA4,		//��������00H~08H
	RecordOrPlayStop	=0xA5,		//ֹͣ¼��
	DeleteFileName		=0xA6,		//ָ���ļ���ɾ���ļ�
	DeleteFileNumber	=0xA7,		//ָ���ļ����ɾ���ļ�
	Format				=0xA8,		//��ʽ��
	Suspend				=0xA9,		//��ͣ
	ProceedPlay			=0xAA,		//��������
	NoSleep				=0xAB,		//������
	Sleep				=0xAC		//����
}VoiceEnum;

//typedef enum 
//{
//  Volume1 = 0xE0,
//  Volume2,
//  Volume3,
//  Volume4,
//  Volume5,
//  Volume6,
//  Volume7,
//  Volume8,
//}PlayVolume;
/*---------------------------------------------����--------------------------------------*/

void SendThreeLines(u8  addr) ;
void LineDataToWTR050( u8 data);

#endif



