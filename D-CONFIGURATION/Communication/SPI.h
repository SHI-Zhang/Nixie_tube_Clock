
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
 #ifndef __SPI_H
 #define __SPI_H
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ˵�� ��ʼ   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*--------------------------------  ˵�� ����   -------------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 #include <gd32f30x.h>
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #DEFINE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

 /*-----------------------------  #DEFINE   END   -----------------------------*/
  
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE FUNCTION BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
  u8 SPI_MY_Init(void);
  void SPI1_SEND_Byte(u8 data_byte);
//  void SPI1_SEND_Word(u16 data_word);
 void DMA_SPI1_Init(u32 memory_Adress,u16 buffer_size);
 /*-------------------------  DECLARE FUNCTION  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~ DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*------------------------- DECLARE  VARIABLE  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 #endif
 