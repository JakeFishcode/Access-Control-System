# Access Control System
## Hardware
1. stm32f103rc
2. RC522 modle
3. LCD5110
4. 38K IRM-3638T
5. MP3 remote controller


## Start
### 1. Ready for the hardware
##### RC522:
*   1--SDA  <----->PA4
*   2--SCK  <----->PA5
*   3--MOSI <----->PA7
*   4--MISO <----->PA6
*   5--NULL<-------> NULL
*   6--GND <----->GND
*   7--RST <----->PB0
*   8--VCC <----->VCC
##### LCD5110:
*  LCD_RST <----->	PB13 
*  LCD_CE <----->	PB12
*  LCD_DC 	<----->PB14
*  SDIN <----->	PC6
*  SCLK <----->	PC7
*  Vcc    <----->  3.3V
*  BL 	 <----->   3.3V
*  GND	 <----->   GND
##### key:
* PC10<----->	key0
* PC11<----->	key1
* PC12<----->	key2
* PD2<----->	key3
* PB3<----->	key4
##### OUT to Door:
PC8  
PC9 
##### LED:

* PA8 <----->	RED LED
* PA9<----->	GREEN LED
##### 38K IRM-3638T:
* PB9
##### BT1 BT2
BT1 <-----> GND  
BT2 <-----> GND
### 2.Ready for the code tool
##### Download Keil5

### 3.Loading the code
##### use the Keil5 to your MCU

### 4. Test
##### Check it work
