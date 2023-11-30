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
 #include <stm32f10x.h>
 #include "GPIO.h"
 #include "SPI.h"
 #include "OLED.h"
 #include "delay.h"

 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 
#ifdef _0_96_OLED_DISPALY
 
 void OLED_Send_Cmd(u8 order)
 {
	CS_L;
	CMD_ORDER;
    SPI1_SEND_Byte(order);
    CS_H;
 }
 
 void OLED_Send_Data(u8 data)
 {
	CS_L;
	DATA_ORDER;
    SPI1_SEND_Byte(data);
    CS_H;
 }
 
  void OLED_Send_Word_Data(u16 data)
 {
	CS_L;
	DATA_ORDER;
    SPI1_SEND_Word(data);
    CS_H;
 }
 
const unsigned char  OLED_init_cmd[25]=
{
       0xAE,//关闭显示
	
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率

       0xA8,//设置驱动路数
       0X3F,//默认(1/64)
	
       0xD3,//设置显示偏移
       0X00,//默认为0
	
       0x40,//设置显示开始行 [5:0],行数.
	
       0x8D,//电荷泵设置
       0x14,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;  A1
	
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 (C0 翻转显示) C8
	   
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置  
	   
       0x81,//对比度设置
       0x66,//1~255;默认0X7F (亮度设置,越大越亮)
	   
       0xD9,//设置预充电周期
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
	   
       0xDB,//设置VCOMH 电压倍率
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	   
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	   
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示 
       
       0xAF,//开启显示     
};


 /*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
 u8 OLED_Init(void)
 {
	unsigned char i;
    for(i=0;i<25;i++)
      {
        OLED_Send_Cmd(OLED_init_cmd[i]);
      }
	return 0; 
 }
 


 void OLED_full(void)
  {
    unsigned char page,column;
    for(page=0;page<8;page++)             //page loop
      { 
        Page_set(page);
        Column_set(0);	  
	for(column=0;column<128;column++)	//column loop
          {
            OLED_Send_Data(0xff);
          }
      }
  }
 
void Column_set(unsigned char column)
 {
    OLED_Send_Cmd(0x10|(column>>4));    //设置列地址高位
    OLED_Send_Cmd(0x00|(column&0x0f));   //设置列地址低位  
    	 
 }
void Page_set(unsigned char page)
  {
    OLED_Send_Cmd(0xb0+page);
  }
 


#endif   /*_0_96_OLED_DISPALY*/



 
 
