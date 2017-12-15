#ifndef __WIRELESSPROCESS_H
#define __WIRELESSPROCESS_H


#include "sys.h"

#include "TaskLogic.h"
#include "system.h"
#include "GUI.h"
#include "BSP.h"

#define KeyUnlock1527            0X02  //遥控撤防	锁＝01
#define KeyAlarm1527             0X08  //遥控报警	锁＝04
#define KeyAlawy1527             0X04  //遥控布防
#define KeyHome1527              0X01  //遥控在家布防

#define KeyDoorOpen1527		  	   0x0E	//开门提示
#define KeyDoorClose1527         0X07  //门磁

#define KeyRemove1527			       0X0B  //拆除（防拆）

#define KeyHongWai1527           0X0B  //红外
#define KeyYanGan1527            0X0A  //烟感
#define KeyQiGan1527             0X05  //气感
#define Key_Help1527             0X0F  //求助	
#define Key_pale1527             0X09  //栅栏	





#define KeyAlawy2262             0XC0  //布防
#define KeyHome2262              0X03  //在家布防
#define KeyUnLock2262			       0X0C  //撤防
#define KeyAlarm2262             0X30  //遥控报警
#define KeyDoor2262              0X50  //门磁
#define KeyYanGan2262            0Xfc  //烟感 
#define KeyQiGan2262             0X74  //气感
#define KeyHongWai2262           0X5c  //红外码
#define KeyWindows2262           0X44  //窗磁
#define KeyHelp2262              0X34  //求助	
#define KeyPale2262              0X54  //栅栏	

#endif



