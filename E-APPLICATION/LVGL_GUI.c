
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
	 
		//输出
		lv_obj_t* Ta = lv_textarea_create(lv_scr_act());
	  lv_obj_align(Ta,LV_ALIGN_TOP_MID,0,0);
		lv_textarea_set_text(Ta, "");
	 
		//输入
		lv_obj_t* keyboard_01 =lv_keyboard_create(lv_scr_act());			//创建键盘图标

		lv_keyboard_set_textarea(keyboard_01,Ta);
		
		//绑定GROUP
		lv_group_t *g = lv_group_create();
		//	lv_group_set_default(g);			//设置默认组
		lv_group_add_obj(g,keyboard_01);//添加部件到组，这里的obj可能是指自己建的部件
	 
		lv_indev_set_group(indev_keypad,g);		//关联按键和组   indev_keypad 是按键或键盘的代表，g是组的的代表
		 
 } 
	 
#endif  /*_LVGL_GUI*/


	