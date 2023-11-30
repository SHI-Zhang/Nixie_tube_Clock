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
 #include <gd32f30x.h>
 #include "SDIO.h"
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~   DEFINE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*-------------------------   DEFINE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~   DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*--------------------------- DECLARE  VARIABLE  END   -----------------------*/
 
 
 /*********************************************************************
 * @fn       : <MY_SDIO_Init/函数名>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
 void MY_SDIO_Init(void)
 {
	 /*1-SDIO 寄存器初始化*/
	 sdio_deinit();			//SDIO 寄存器初始化
	 /*2-初始化中断，虽然这中断用不上*/
	 nvic_irq_enable(SDIO_IRQn,0,0);			//初始化中断，虽然这中断用不上
	 
	 /*3-开启时钟*/
	 rcu_periph_clock_enable(RCU_SDIO);
   rcu_periph_clock_enable(RCU_DMA1);
	 
	//  /*3-时钟配置、数据线宽度和硬件流(在stm32的库函数中，这是一个函数就设置了)*/
	//  /*HCLK = 120MHz, SDIOCLK = 120MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz*/
	//  sdio_clock_config(SDIO_SDIOCLKEDGE_RISING,SDIO_CLOCKBYPASS_DISABLE,SDIO_CLOCKPWRSAVE_DISABLE,SDIO_INIT_CLK_DIV);
	//  sdio_bus_mode_set(SDIO_BUSMODE_1BIT);
	//  sdio_hardware_clock_disable();
	 
	//  /*4-配置上电状态*/
	//  sdio_power_state_set(SDIO_POWER_ON);
	 
	//  /*5-开启SDIO-CK时钟，时钟使能*/
	//  sdio_clock_enable();
 }
 
 
 /*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <仿照STM32 编写的用来发送SDIO命令的函数>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void SDIO_SendCommand(SDIO_SMD_Send_struct* data)
{
	/*1-设置寄存器内值*/
	sdio_command_response_config(data->index,data->argument,data->response_type);
	/*2-设置等待类型  无*/
	sdio_wait_type_set(data->wait_type);
	/*3-使能 CSM*/
	sdio_csm_enable();
}

