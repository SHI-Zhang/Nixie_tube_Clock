#ifndef __GPIO_H
#define __GPIO_H
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
*                             佛祖保佑，万事顺遂，不出BUG
*                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  说明 开始   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*计划所有的gpio初始化，都放到这个文件中，然后在main函数中统一调用*/
/*--------------------------------  说明 结束   -------------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "gd32f30x.h"
/*-----------------------------  #INCLUDE   END   ----------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #DEFINE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Light_Blue_pin GPIOB, GPIO_PIN_6   // 蓝色LED控制IO口
#define Lighr_Yellow_pin GPIOB, GPIO_PIN_7 // 暖白LED控制IO口

#define LIGHT_up_pin GPIOB, GPIO_PIN_5   // 控制LED变亮的输入IO口
#define LIGHT_down_pin GPIOB, GPIO_PIN_4 // 控制LED变暗的输入IO口
#define Mode_Pin GPIOB, GPIO_PIN_3
#define HONGWAI_Pin GPIOA, GPIO_PIN_0
#define MIC_Pin GPIOA, GPIO_PIN_1

#define LIGHT_up !GPIO_ReadInputDataBit(LIGHT_up_pin)     // 读取控制LED变亮的输入
#define LIGHT_down !GPIO_ReadInputDataBit(LIGHT_down_pin) // 读取控制LED变暗的输入
#define MODE_key !GPIO_ReadInputDataBit(Mode_Pin)
#define Remote_pin_sat GPIO_ReadInputDataBit(HONGWAI_Pin)

#define USART0_CK_pin GPIOA, GPIO_PIN_8   // PA8
#define USART0_TX_pin GPIOA, GPIO_PIN_9   // PA9
#define USART0_RX_pin GPIOA, GPIO_PIN_10  // PA10
#define USART0_CTS_pin GPIOA, GPIO_PIN_11 // PA11
#define USART0_RTS_pin GPIOA, GPIO_PIN_12 // PA12

//OLED
#define OLED_D_C_pin GPIOA, GPIO_PIN_3   // PA3  1==data,0==cmd
#define SPI0_NSS_pin GPIOA, GPIO_PIN_4   // PA4
#define SPI0_SCK_pin GPIOA, GPIO_PIN_5   // PA5
#define SPI0_MISO_pin GPIOA, GPIO_PIN_6   // PA6
#define SPI0_MOSI_pin GPIOA, GPIO_PIN_7   // PA7
#define LCD_RES_pin GPIOC, GPIO_PIN_4			//pc4

#define DATA_ORDER gpio_bit_set(OLED_D_C_pin)
#define CMD_ORDER gpio_bit_reset(OLED_D_C_pin)
#define CS_H gpio_bit_set(SPI0_NSS_pin)
#define CS_L gpio_bit_reset(SPI0_NSS_pin)
#define RES_H gpio_bit_set(LCD_RES_pin)
#define RES_L gpio_bit_reset(LCD_RES_pin)

//KEY   //1、c9 c8 a9 a8
#define X_pin GPIOA, GPIO_PIN_1   // PA3
#define Y_pin GPIOA, GPIO_PIN_2
#define Enter_pin GPIOA, GPIO_PIN_10

//SDIO
#define SDIO_D0_Pin		GPIOC, GPIO_PIN_8			//pc8
#define SDIO_D1_Pin		GPIOC, GPIO_PIN_9			//pc9
#define SDIO_D2_Pin		GPIOC, GPIO_PIN_10		//pc10
#define SDIO_D3_Pin		GPIOC, GPIO_PIN_11		//pc11
#define SDIO_SCk_Pin	GPIOC, GPIO_PIN_12		//pc12
#define SDIO_CMD_Pin	GPIOD, GPIO_PIN_2			//pd2
/*-----------------------------  #DEFINE   END   -----------------------------*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE FUNCTION BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
void GPIO_Init_GD32(GPIO_InitTypeDef_GD32* Data);
void IO_Init(void);
void PIN_Init(void);
void UART_GPIO_Init(void);
u8 SPI1_GPIO_Init(void);
 void KeyBoard_Init(void);
u8 F1_KEY_STATUS(void);
 void SDIO_GPIO_Init(void);
/*-------------------------  DECLARE FUNCTION  END   -------------------------*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~ DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*------------------------- DECLARE  VARIABLE  END   -------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/

/*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/

struct PIN_Var
{
//    GPIO_TypeDef *group;
	u32 group;
  uint16_t PIN;
};





#endif
