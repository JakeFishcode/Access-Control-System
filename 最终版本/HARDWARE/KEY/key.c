#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "rc522.h"
#include "usart.h"
#include "led.h"
#include "stmflash.h"
#include "nokia_5110.h"
#include "lcdlib.h"
#include "remote.h"
#include "userinfo.h"
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
extern u16 NUM;
extern u16 NUM_ROOT;
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4


}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	
	if(mode)key_up=1;  //支持连按		  
	
	if((KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
			
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==0)return KEY4_PRES;

	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}
u8 scan(void)
{
	static u8 blash=0;
	u8 key=0;
	key=KEY_Scan(0);
	if(key==KEY1_PRES)	//KEY1按下,写入STM32 FLASH
	{
		blash=1;
		initSN();
	}
	if(key==KEY0_PRES)	//KEY0按下,读取字符串并显示
	{
		blash=2;
		initSN();	
	}
	if(key==KEY2)
	{
		blash=3;
		initSN();
		
	}
	return blash;
}







void do_key(u8 key,u8 *blash)
{
	static u8 i;
	static u8 sec[4]={0};
	
	//添加 需要有ROOT权限
	if(key==KEY1_PRES||key==AddUser)	//KEY1按下,写入STM32 FLASH
	{
		sec[0]=1;
		i=0;
		if(check_rootkey())
		{
			*blash=1;
			printf("ADD mode\r\n");
			LCD_clear();
			LCD_write_english_string(0,2," Add Mode ");
			LCD_write_english_string(0,3," ADD USERS ");
		}
		else
		{	
			printf("You are not root!\r\n");
			LCD_clear();
			LCD_write_english_string(0,2," Add Mode ");
			LCD_write_english_string(0,3," Limited!");
		}
		LCD_write_english_string(0,0," GDUT  ELC");
		initSN();
	}
	//工作模式
	if(key==KEY0_PRES||key==WorkMode)	//KEY0按下,读取字符串并显示
	{

		i=0;
		printf("Work mode\r\n");
		LCD_clear();
		LCD_write_english_string(0,2," Work Mode ");
		LCD_write_english_string(0,0," GDUT  ELC");
		*blash=2;
		initSN();	
	}
	//删除  需要有ROOT权限
	if(key==KEY2_PRES||key==DelUser)
	{
		if(sec[2]==3)
			sec[1]=2;
		i=0;
		if(check_rootkey())
		{
			printf("Del Mode\r\n");
			LCD_clear();
			LCD_write_english_string(0,2," DEL Mode ");
			*blash=3;
		}
		else
		{
			LCD_clear();		
			LCD_write_english_string(0,2," DEL Mode ");
			printf("Not root\r\n");
			LCD_write_english_string(0,3," Limited！ ");
		}
		
		LCD_write_english_string(0,0," GDUT  ELC");
		delay_ms(200); 
		initSN();
		
	}
	//登录 或 添加ROOT用户
	if(key==KEY3_PRES||key==RootLogin)
	{
		if(sec[0]==1)
			sec[3]=4;
		i=0;

		LCD_clear();
		LCD_write_english_string(0,0," GDUT  ELC");
		LCD_write_english_string(0,2," Root Login ");
		*blash=4;
		initSN();
	}

	if(key==KEY5_PRES||key==InitSystem)
	{
		i=0;
		if(check_rootkey())
		{
			LCD_clear();
			LCD_write_english_string(0,2," All Users ");
			LCD_write_english_string(0,23,"  Clean ");
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&i,1);

			NUM=0;

		}
		else
		{
			LCD_clear();
			LCD_write_english_string(0,1," You're not");

			LCD_write_english_string(0,2,"   Root ");
			LCD_write_english_string(0,3,"  Limited ");
		}
		delay_ms(2000);
	}
	if(key==RootInit)
	{
		i=0;
		if(check_rootkey())
		{
			LCD_clear();
			LCD_write_english_string(0,2," Clean Root ");
			STMFLASH_Write(FlASH_ROOT_NUM_DATA,(u16*)&i,1);
			NUM_ROOT=0;
		}
		else
		{
			LCD_clear();
			LCD_write_english_string(0,1," You're not");

			LCD_write_english_string(0,2,"   Root ");
			LCD_write_english_string(0,3,"  Limited ");
		}
		delay_ms(2000);
	}
	if(key==KEY6_PRES||key==RootOut)
	{
		i=0;
		LCD_clear();
		if(check_rootkey())
			LCD_write_english_string(0,2,"  Logout! ");
		else
			LCD_write_english_string(0,2,"  No Root!");
		logout();
	}
	if(key==MakerInfo)
	{
		*blash=MakerInfo;
	}
	if(key==SystemInfo)
	{
		*blash=SystemInfo;
	}
	
	i++;
	delay_ms(150);  
	
	if(i==5)
	{
		LED0=!LED0;
		LCD_clear();
		LCD_Show_Pic(0, 0, 84, 48, pic_map);
		if(check_rootkey())
			LCD_Show_Pic(0, 0, 12, 12, pic_unlock);
		if(*blash==2) 
			LCD_Show_Pic(36, 0, 12, 12, w_mode);
		else if(*blash==4) 
			LCD_Show_Pic(36, 0, 12, 12, r_mode);
		else if(*blash==1) 
			LCD_Show_Pic(36, 0, 12, 12, a_mode);
		i=0;
	}		   
}

u8 show_meanu(u8 ml,u8* cur,char show_char[][15],u8* base)
{
	u8 i,j;
	char temp[15];
	if(ml==1)
	{
		if(*cur == MEANU_MAX-1)
		{	if ((*base)<MEANU_LONG-MEANU_MAX)(*base)++;}
		else
			(*cur)++;
	}
	else if(ml==0)
	{
		if(*cur == 0)
		{	if((*base)>0) (*base)--;}
		else
			(*cur)--;
	}
	LCD_clear();
	for(i=0;i<MEANU_MAX;i++)
	{

		LCD_write_english_string(0,i,show_char[(*base)+i]);
	}
	for(j=0;j<12;j++)
		temp[j]=show_char[(*cur)+(*base)][j];
	temp[11]='<';
	temp[12]=' ';
	temp[13]=' ';
	LCD_write_english_string(0,*cur,temp);
	LCD_write_english_string(0,4,"\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
	LCD_write_english_string(0,5,"    Meanu");
	
	
	
	return (*base)+(*cur);
}
void meanu_mode(u8* pblash)
{
	char meanu[MEANU_LONG][15]={"Work Mode  ",
									 "Add  User  ",
									 "Del  User  ",
									 "Root Login ",
									 "Root  Out  ",
									 "Root init   ",
									 "Init System  ",		 
									 "System Info  ",
									 "Maker  Info  ",
									 
									};
	u8 num_flag=1,key=0,current=0,sum_current=0,base=0;
	
	show_meanu(2,&current,meanu,&base);
	while(num_flag)
	{
		key=Remote_Scan();
		switch(key)
		{
				    
				case 98:num_flag=0;sum_current=0;break;//ch	    
				case 168:sum_current=show_meanu(1,&current,meanu,&base);break;//+	
				case 226:sum_current=show_meanu(1,&current,meanu,&base);break;//ch+	
				case 2:sum_current=show_meanu(1,&current,meanu,&base);break;//>>|			
				case 194:num_flag=0;break;//>||	  
				case 34:sum_current=show_meanu(0,&current,meanu,&base);break;//|<<			
				case 224:sum_current=show_meanu(0,&current,meanu,&base);break;//-		 
				case 162:sum_current=show_meanu(0,&current,meanu,&base);break;//ch-							
		}

	}
		printf("%d\r\n",sum_current);
		switch(sum_current)
		{
			case 0:*pblash=WorkMode;break;//Work Mode  
			case 1:*pblash=AddUser;break;//Add  User  
			case 2:*pblash=DelUser;break;//Del  User  
			case 3:*pblash=RootLogin;break;//Root Login 
			case 4:*pblash=RootOut;break;
			case 5:*pblash=RootInit;break;//RootInit 
			case 6:*pblash=InitSystem;break;  //Init System
			case 7:*pblash=SystemInfo;break;//System  Info 
			case 8:*pblash=MakerInfo;break;//Maker  Info  
		}
		
									
	
}
void showp1()
{
	LCD_Show_Pic(0, 0, 84, 48, pic_heart);
}
void showp2()
{
	LCD_Show_Pic(0, 0, 84, 48, pic_dianxie);
}
void show_mode(u8 ml,u8 *blash)
{
	char temp[15]={"User Num:     "};
	char temp2[15]={"Root Num:     "};
	u8 t=11,num_flag=1,key=0;
	if(ml==SystemInfo)
	{
		LCD_clear();
		while(num_flag)
		{
			key=Remote_Scan();
			if(key!=0) num_flag=0;
			LCD_write_english_string(0,0,"    Sys Info");

		
			temp[t]=NUM/100+48;
			if(temp[t]==48) temp[t]=' ';
			temp[t+1]=NUM/10%10+48;
			if(temp[t+1]==48) temp[t+1]=' ';
			temp[t+2]=NUM%10+48;
			LCD_write_english_string(0,1,temp);
			//NUM_ROOT
			temp2[t]=NUM_ROOT/100+48;
			if(temp2[t]==48) temp2[t]=' ';
			temp2[t+1]=NUM_ROOT/10%10+48;
			if(temp2[t+1]==48) temp2[t+1]=' ';
			temp2[t+2]=NUM_ROOT%10+48;
			LCD_write_english_string(0,2,temp2);
			LCD_write_english_string(0,3,"Data:2019-8-19");
			LCD_write_english_string(0,4,"Verson:V 1.0");
		}
		*blash=2;
	}
	else if(ml==MakerInfo)
	{
		LCD_clear();
		while(num_flag)
		{
			key=Remote_Scan();
			if(key!=0) num_flag=0;
			LCD_write_english_string(0,0,"  Maker Info  ");
			LCD_write_english_string(0,1,"Maker:Jakefish");
			LCD_write_english_string(0,2,"Ph:15521113661");

		}
		*blash=2;
	}
}