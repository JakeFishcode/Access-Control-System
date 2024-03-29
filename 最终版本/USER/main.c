 #include "delay.h"
#include "sys.h"
#include "usart.h"
#include "rc522.h"
#include "stmflash.h"
#include "led.h"
#include "key.h"
#include "nokia_5110.h"
#include "remote.h"
#include "userinfo.h"
/**
*   连线说明：
*   1--SDA  <----->PA4
*   2--SCK  <----->PA5
*   3--MOSI <----->PA7
*   4--MISO <----->PA6
*   5--悬空
*   6--GND <----->GND
*   7--RST <----->PB0
*   8--VCC <----->VCC
**/


extern unsigned char CT[2];
extern unsigned char SN[4]; //卡号
extern unsigned char card_1[4];



int main(void)
{
		u8 key,key1,blash=2;
		u8 i=0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
#if DEBUG
		uart_init(115200);	 	//串口初始化为115200	 
		LED_Init();	 

#else
			 			//初始化与LED连接的硬件接�	
#endif
		delay_init();	    	 //延时函数初始�


		KEY_Init();					//初始化按键
		RC522_Init();       //初始化射频卡模块
		LCD_IO_Init();
		LCD_init();
		LCD_clear();
	
	
		Door_IO_init();
		i=0;
//		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&i,1);
//		STMFLASH_Write(FlASH_ROOT_NUM_DATA,(u16*)&i,1);
		Remote_Init();
		LCD_write_english_string(0,0,"Loading..");
		information_init();
		

		Door1=0;
		Door2=0;
		LED1=0;
		LED0=0;
    while(1)
    {
			
			switch(blash)
			{
				case 1:write_mode();break;
				case 2:read_mode();break;
				case 3:clear_mode();blash=2;break;
				case 4:root_mode(&blash);break;
				case SystemInfo:show_mode(SystemInfo,&blash);break;
				case MakerInfo:show_mode(MakerInfo,&blash);break;
			}
			key=KEY_Scan(0);
			key1=Remote_Scan();
			switch(key1)
			{
					case 2:meanu_mode(&key);break;//>>|
//				case 48:key=KEY0_PRES;break;
//				case 24:key=KEY1_PRES;break;
//				case 122:key=KEY2_PRES;break;	
//				case 16:key=KEY3_PRES;break;
//				case 56:key=KEY5_PRES;break;	
//				case 90:key=KEY6_PRES;break;
			}
			
			do_key(key,&blash);
   
		} 
}



