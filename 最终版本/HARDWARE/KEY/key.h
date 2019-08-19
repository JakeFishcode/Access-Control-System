#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP

#define KEY0  PCin(10) //读取按键0
#define KEY1  PCin(11)//读取按键1
#define KEY2  PCin(12)//读取按键2 
#define KEY3  PDin(2)//读取按键2 
#define KEY4  PBin(3)//读取按键2 

#define	WorkMode  101
#define AddUser  102
#define DelUser  103
#define RootLogin  104
#define InitSystem 105
#define RootOut  106
#define MakerInfo 107
#define SystemInfo 108
#define RootInit 109



#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define KEY2_PRES	3	//KEY2按下
#define KEY3_PRES	5	//KEY2按下
#define KEY4_PRES	6	//KEY2按下
#define KEY5_PRES 99
#define KEY6_PRES 98
void showp1();
void showp2();
void meanu_mode(u8* pblash);
void do_key(u8 key,u8 *blash);
void show_mode(u8 ml,u8 *blash);
u8 scan(void);
void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif
