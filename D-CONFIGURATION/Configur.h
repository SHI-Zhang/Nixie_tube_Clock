 /*

 *                                     _oo0oo_
 *                                    o8888888o
 *                                    88" . "88
 *                                    (| -_- |)
 *                                    0\  =  /0
 *                                  ___/`---'\___
 *                                .' \\|     |// '.
 *                               / \\|||  :  |||// \
 *                              / _||||| -:- |||||- \
 *                             |   | \\\  - /// |   |
 *                             | \_|  ''\---/''  |_/ |
 *                             \  .-\__  '-'  ___/-. /
 *                           ___'. .'  /--.--\  `. .'___
 *                        ."" '<  `.___\_<|>_/___.' >' "".
 *                       | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *                       \  \ `_.   \_ __\ /__ _/   .-` /  /
 *                   =====`-.____`.___ \_____/___.-`___.-'=====
 *                                     `=---='
 *              
 *                             ���汣�ӣ�����˳�죬����BUG
 *                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ˵�� ��ʼ   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*--------------------------------  ˵�� ����   -------------------------------*/
 
#ifndef __PARAMETER_H
#define __PARAMETER_H
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 #include "gd32f30x.h"
 #include <delay.h>
 #include "GPIO.h"

 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #DEFINE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



 /*-------��������ѡ��ʼ---------*/
 
 //#define _Remote_Function
 //#define _0_96_OLED_DISPALY
// #define _1_47_IPS_LCD
 
// #define _WIFI_Function

// #define _LVGL_GUI
 /*-------��������ѡ�����---------*/


 /*-------�������ÿ�ʼ---------*/
#ifdef _0_96_OLED_DISPALY
	#define OLED_COLUMN_NUMBER 128
	#define OLED_LINE_NUMBER 64
	#define OLED_COLUMN_OFFSET 2
	#define OLED_PAGE_NUMBER OLED_LINE_NUMBER/8
#endif	/*_0_96_OLED_DISPALY*/


#ifdef _1_47_IPS_LCD
	#define LCD_Display_Row 172
	#define LCD_Display_Collow 320

	#define LCD_Display_Buffer_Row 10
	#define LCD_Display_Buffer_Collow 320
	
//	#define _Touchpad_Mode
//	#define _Mouse_Mode
	#define _Keypad_Mode
//	#define _Encoder_Mode
//	#define _Button_Mode
	
#endif	/*_1_47_IPS_LCD*/

#ifdef _WIFI_Function
	#define USART1_MAX_RECV_LEN 500
	#define USART1_MAX_SEND_LEN 500
	#define Usart_Rec_Arr USART_RECEIVE_Data[USART_Receive_num]
	
	struct USART_RX_struct
	{
	u8 RX_Data[150];	 // ���յ�������
	u8 Data_len;		 // ���յ������ݵ�ʵ�ʳ���
	u8 Receive_Data_sat; // ����װ̬
	struct USART_RX_struct* Last_Addr;
	};
	
#endif /*_WIFI_Function*/ 

 #define LED_Per 2000

 /*-------�������ý���---------*/	



 /*-----------------------------  #DEFINE   END   -----------------------------*/
  
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE FUNCTION BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*-------------------------  DECLARE FUNCTION  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~ DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*------------------------- DECLARE  VARIABLE  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 extern GPIO_InitTypeDef_GD32 GPIO_InitStruct;
 extern uint16_t power;
 extern uint16_t dir ;
 extern struct PIN_Var YELLOW_PIN;
 extern struct PIN_Var BLUE_PIN;
 extern u8 TIM2CH1_CAPTURE_STA;                                                  // ���벶��״̬
 extern u16 TIM2CH1_CAPTURE_VAL;                                                 // ���벶��ֵ
 extern u32 RmtRec,RmtSta,RmtCnt,Dval;
 extern u8  Rmt_Key_val;
 extern uint16_t    Led_Blue_Bright_u16;                                              //��ɫLED�Ƶ�����
 extern uint16_t    Led_Yellow_Bright_u16;                                            //ů��ɫLED������
 extern uint8_t Led_Mode;                                                             //  ��������LED����ģʽ��==1�����⣬==2��ů�׹⣬==3��һ��==0������
 extern u16 Key_up_st;                                                                 //up���ĵ�ǰװ̬��==1����ǰ�����ˣ�==0����ǰ������
 extern u16 Key_down_st;                                                                 //down���ĵ�ǰװ̬��==1����ǰ�����ˣ�==0����ǰ������
 extern u16 Mode_key_st;
 extern u16 LED_Bright_Tab[];
 // ����˿�
 extern struct PIN_Var BLUE_PIN;
 extern struct PIN_Var YELLOW_PIN;
 
  // ����˿�
 extern struct PIN_Var LIGHT_UP;
 extern struct PIN_Var LIGHT_DOWN ;
 extern struct PIN_Var MODE;
 extern struct PIN_Var HONGWAI;
 extern struct PIN_Var MIC;
 //LCD & OLED
#ifdef _1_47_IPS_LCD
 extern u8 USE_HORIZONTAL;
 extern const unsigned char ascii_1206[][12];
 extern const unsigned char ascii_1608[][16];
 extern const unsigned char ascii_2412[][48];
 extern const unsigned char ascii_3216[][64];
 extern u16 IPS_LCD_Cahe_Data[10][320];			//�����Դ�
 extern const unsigned char Stone_Ficture[1250];
 extern const unsigned char Sea_Ficture[300];
 extern const unsigned char Grass_Ficture[25*12*2];
// extern const unsigned char gImage_3445[35200];
 //ͼ��
 extern u8 Wifi_Line[];
 extern u8 Wifi_no_Line[];
 
#endif	/*_1_47_IPS_LCD*/
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/













#endif

