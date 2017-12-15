

#ifndef __DEVICE_FINGERINT_H
#define __DEVICE_FINGERINT_H

/*---------------ͷ�ļ�---------------*/
#include "stm32F10x.h"
#include "sys.h"


/*----------------------------------����-------------------------------------*/







/*----------------------------------�궨��-----------------------------------*/




/*-----------------------------------����------------------------------------*/



/*-----------------------------------����------------------------------------*/


typedef enum
{
	NotParameter=0,
	HaveParameter=1,
}EnumBit;
//ָ�����
typedef enum
{
    PacketStart=0xef01,//����ʶ
    
	Connection=0x13,//�����ź�
	ReadFlash=0x16,//��Flash����
	BufferImage=0x01,//�Ӵ������϶���ͼ����ڻ�����
	FirstImage=0x0a,//�ϴ�ԭʼͼ��
	FirstToFeature=0x02,//����ԭʼͼ������ָ����������CharBuffer1��
	MergerFeature=0x05,//�ϲ�CharBuffer1��CharBuffer2�������ļ�
	FeatureToFlash=0x06,//�����������ļ��ŵ�Flash��ָ�ƿ���
	SearchFlash=0x1B,//��������Flash
	AccurateJudge=0x03,//��ȷ�Ա�CharBuffer1��CharBuffer2�������ļ�
	ReadNotepadCmd=0x19,//�����±�
	WriteNotepadCmd=0x18,//д���±�
	SamplingRandomCmd=0x14,//���������
	EffectiveTemplateCmd=0x1D,//����Чģ��
	DeleteCmd=0x0C,//ɾ��ָ�ƿ��е�һ��ָ�������ļ�
	ClearCmd=0x0D//�������ָ������
}EnumInstruct;




typedef enum
{
	CommandMark=1,
	DataMark=2,
	EndMark=8
}EnumMark;

/*-----------------------------------����------------------------------------*/
#endif 




















