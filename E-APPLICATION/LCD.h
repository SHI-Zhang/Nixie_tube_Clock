
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
 #ifndef __LCD_H
 #define __LCD_H
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ˵�� ��ʼ   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*--------------------------------  ˵�� ����   -------------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 #include "parameter.h"
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #DEFINE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 


#ifdef _1_47_IPS_LCD
	//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#endif  /*_1_47_IPS_LCD*/
 
 /*-----------------------------  #DEFINE   END   -----------------------------*/
  
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE FUNCTION BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
   
   void LCD_Init(void);
   void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);
   void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
   void LCD_Send_Cmd(u8 order);
   void LCD_Send_Data(u8 data);
   void LCD_Send_Word_Data(u16 data);
   void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);
   void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);
   void LCD_Show_ICON(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 size,u8 mode);
   void LCD_Fill_Lvgl(u16 xsta,u16 ysta,u16 xend,u16 yend,u16* color);
 /*-------------------------  DECLARE FUNCTION  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~ DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*------------------------- DECLARE  VARIABLE  END   -------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 

#endif 
 
 
