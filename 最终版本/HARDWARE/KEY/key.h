#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP

#define KEY0  PCin(10) //��ȡ����0
#define KEY1  PCin(11)//��ȡ����1
#define KEY2  PCin(12)//��ȡ����2 
#define KEY3  PDin(2)//��ȡ����2 
#define KEY4  PBin(3)//��ȡ����2 

#define	WorkMode  101
#define AddUser  102
#define DelUser  103
#define RootLogin  104
#define InitSystem 105
#define RootOut  106
#define MakerInfo 107
#define SystemInfo 108
#define RootInit 109



#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define KEY3_PRES	5	//KEY2����
#define KEY4_PRES	6	//KEY2����
#define KEY5_PRES 99
#define KEY6_PRES 98
void showp1();
void showp2();
void meanu_mode(u8* pblash);
void do_key(u8 key,u8 *blash);
void show_mode(u8 ml,u8 *blash);
u8 scan(void);
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
