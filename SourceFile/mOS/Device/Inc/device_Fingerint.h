

#ifndef __DEVICE_FINGERINT_H
#define __DEVICE_FINGERINT_H

/*---------------头文件---------------*/
#include "stm32F10x.h"
#include "sys.h"


/*----------------------------------常数-------------------------------------*/







/*----------------------------------宏定义-----------------------------------*/




/*-----------------------------------变量------------------------------------*/



/*-----------------------------------类型------------------------------------*/


typedef enum
{
	NotParameter=0,
	HaveParameter=1,
}EnumBit;
//指令参数
typedef enum
{
    PacketStart=0xef01,//包标识
    
	Connection=0x13,//握手信号
	ReadFlash=0x16,//读Flash内容
	BufferImage=0x01,//从传感器上读入图像存于缓存中
	FirstImage=0x0a,//上传原始图像
	FirstToFeature=0x02,//根据原始图像生成指纹特征存于CharBuffer1中
	MergerFeature=0x05,//合并CharBuffer1和CharBuffer2的特征文件
	FeatureToFlash=0x06,//将特征缓存文件放到Flash的指纹库中
	SearchFlash=0x1B,//高速搜索Flash
	AccurateJudge=0x03,//精确对比CharBuffer1和CharBuffer2的特征文件
	ReadNotepadCmd=0x19,//读记事本
	WriteNotepadCmd=0x18,//写记事本
	SamplingRandomCmd=0x14,//采样随机数
	EffectiveTemplateCmd=0x1D,//读有效模版
	DeleteCmd=0x0C,//删除指纹库中的一个指纹特征文件
	ClearCmd=0x0D//清除所有指纹特征
}EnumInstruct;




typedef enum
{
	CommandMark=1,
	DataMark=2,
	EndMark=8
}EnumMark;

/*-----------------------------------函数------------------------------------*/
#endif 




















