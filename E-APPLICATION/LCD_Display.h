
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
 
 /*--------------------------------  说明 结束   -------------------------------*/
#ifndef __LCD_DISPLAY_H
#define __LCD_DISPLAY_H
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  #include "parameter.h"
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #DEFINE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*-----------------------------  #DEFINE   END   -----------------------------*/
  
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE FUNCTION BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 void Status_bar(void );
 void Show_Rocker(u16 fc,u16 bc);
 void SPI1_DMA_Send(u16 data_size);
 u8 Refresh_Screen(u16 y_coordinate);
 void Virtual_Keyboard(u8 flg,u8 row,u8 cllow);
 void LCD_Show_Virtual_Key(u16 x,u16 y,u8 num,u16 fc,u16 bc,u16 shadow,u8 sizey,u8 mode);
 /*-------------------------  DECLARE FUNCTION  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~ DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*------------------------- DECLARE  VARIABLE  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 
#endif

