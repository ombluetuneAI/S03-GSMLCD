#ifndef __DEVICE_VOICE_H
#define __DEVICE_VOICE_H

/*--------------------------------------------头文件-------------------------------------*/
#include "stm32F10x.h"
#include "sys.h"
/*---------------------------------------------常数--------------------------------------*/

/*--------------------------------------------宏定义-------------------------------------*/


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
/*---------------------------------------------变量--------------------------------------*/


/*---------------------------------------------类型--------------------------------------*/
typedef enum
{
	StartV				=0x7E,	    //开始命令

	RecordFileName 		=0xA0,		//开始指定文件名录音
	RecordNumber		=0xA1,		//开始序号录音
	VoicePlayName		=0xA2,		//按文件名播放语音
	VoicePlayNumber		=0xA3,		//按序号播放语音
	VoiceVolume			=0xA4,		//音量调节00H~08H
	RecordOrPlayStop	=0xA5,		//停止录放
	DeleteFileName		=0xA6,		//指定文件名删除文件
	DeleteFileNumber	=0xA7,		//指定文件序号删除文件
	Format				=0xA8,		//格式化
	Suspend				=0xA9,		//暂停
	ProceedPlay			=0xAA,		//继续播放
	NoSleep				=0xAB,		//不休眠
	Sleep				=0xAC		//休眠
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
/*---------------------------------------------函数--------------------------------------*/

void SendThreeLines(u8  addr) ;
void LineDataToWTR050( u8 data);

#endif



