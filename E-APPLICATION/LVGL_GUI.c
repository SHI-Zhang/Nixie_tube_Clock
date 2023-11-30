
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
 #include "LVGL_GUI.h"
 #include "stm32f10x.h"
 #include "parameter.h"
 //LVGL_Start
	#include "lvgl.h"
	#include "lv_port_disp_template.h"
	#include "lv_port_indev_template.h"
	#include "lv_demo_stress.h"
//LVGL_End
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 	extern lv_indev_t * indev_keypad;
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 
#ifdef _LVGL_GUI
 
	void LVGL_SET(void)
	{
		lv_init();   
		lv_port_disp_init();   //	LCD_Init();
		lv_port_indev_init();
	 
		//���
		lv_obj_t* Ta = lv_textarea_create(lv_scr_act());
	  lv_obj_align(Ta,LV_ALIGN_TOP_MID,0,0);
		lv_textarea_set_text(Ta, "");
	 
		//����
		lv_obj_t* keyboard_01 =lv_keyboard_create(lv_scr_act());			//��������ͼ��

		lv_keyboard_set_textarea(keyboard_01,Ta);
		
		//��GROUP
		lv_group_t *g = lv_group_create();
		//	lv_group_set_default(g);			//����Ĭ����
		lv_group_add_obj(g,keyboard_01);//��Ӳ������飬�����obj������ָ�Լ����Ĳ���
	 
		lv_indev_set_group(indev_keypad,g);		//������������   indev_keypad �ǰ�������̵Ĵ���g����ĵĴ���
		 
 } 
	 
#endif  /*_LVGL_GUI*/


	