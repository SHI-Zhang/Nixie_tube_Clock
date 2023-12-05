

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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "gd32f30x.h"
#include "delay.h"
#include "Configur.h"
#include <stdio.h>
//	#include "LED.h"
//	#include "REMOTE.h"
//	#include "Configur.h"
//	#include "OLED.h"
#include "SPI.h"
#include "LCD.h"
#include "LCD_Display.h"
#include "TIME_Management.h"
//	#include "Key_Board.h"
//	#include "lvgl.h"
//	#include "LVGL_GUI.h"
#include "SDIO.h"
#include "SD_Card.h"
/*-----------------------------  #INCLUDE   END   ----------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~   DEFINE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------   DEFINE  VARIABLE  END   ------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~   DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~*/
extern struct Key_Struct X_AXIS_Joystick;
extern struct Key_Struct Y_AXIS_Joystick;
/*--------------------------- DECLARE  VARIABLE  END   -----------------------*/

int main(void)
{
	/*��ʼ��������������*/
	nvic_priority_group_set(NVIC_PRIGROUP_PRE0_SUB4); // ��4  �Ǿ���4λ��ռ���ȼ���0λ��Ӧ���ȼ�
	delay_init();
	IO_Init();
	TIMER2_MY_Init();
	//	TIM3_MY_Init();
	//	TIM4_PWM_Init();
	//	ADC1_Init_User();
	//

	/*��ʼ��������������*/

	/*��ʼ���߼����ܺͲ�������*/
#ifdef _Remote_Function
	Remote_Receive_Init();
	REMOTE_Init();
#endif /*_Remote_Function*/

#ifdef _0_96_OLED_DISPALY
	OLED_Init();
	OLED_full();
#endif /*_0_96_OLED_DISPALY*/

#ifdef _1_47_IPS_LCD
	SPI_MY_Init();
	//	DMA_SPI1_Init((u32)IPS_LCD_Cahe_Data, 6400);
	// LVGL_SET();
	LCD_Init();
#endif /*_1_47_IPS_LCD*/

	delay_ms(10);

#ifdef _SD_CARD
	sd_error_enum sd_error;
	uint16_t i = 5;
	/* initialize the card */
	do
	{
		sd_error = sd_io_init();
	} while ((SD_OK != sd_error) && (--i));

	if (i)
	{
		Print_my("\r\n Card init success!\r\n",1);
	}
	else
	{
		Print_my("\r\n Card init failed!\r\n",1);
	}
	
//	card_info_get();
#endif /*_SD_CARD*/
		/*����ִ��*/
		while (1)
		{
			// print_figure(0,30,3456785,7,24);
			Print_my_2(0,30,"the figure is:%d.",3456,4,2);
			delay_ms(5);
			//		lv_timer_handler();
		}
	}
