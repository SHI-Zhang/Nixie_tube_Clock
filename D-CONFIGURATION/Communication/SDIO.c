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
 #include <gd32f30x.h>
 #include "SDIO.h"
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~   DEFINE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*-------------------------   DEFINE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~   DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*--------------------------- DECLARE  VARIABLE  END   -----------------------*/
 
 
 /*********************************************************************
 * @fn       : <MY_SDIO_Init/������>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
 void MY_SDIO_Init(void)
 {
	 /*1-SDIO �Ĵ�����ʼ��*/
	 sdio_deinit();			//SDIO �Ĵ�����ʼ��
	 /*2-��ʼ���жϣ���Ȼ���ж��ò���*/
	 nvic_irq_enable(SDIO_IRQn,0,0);			//��ʼ���жϣ���Ȼ���ж��ò���
	 
	 /*3-����ʱ��*/
	 rcu_periph_clock_enable(RCU_SDIO);
   rcu_periph_clock_enable(RCU_DMA1);
	 
	//  /*3-ʱ�����á������߿�Ⱥ�Ӳ����(��stm32�Ŀ⺯���У�����һ��������������)*/
	//  /*HCLK = 120MHz, SDIOCLK = 120MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz*/
	//  sdio_clock_config(SDIO_SDIOCLKEDGE_RISING,SDIO_CLOCKBYPASS_DISABLE,SDIO_CLOCKPWRSAVE_DISABLE,SDIO_INIT_CLK_DIV);
	//  sdio_bus_mode_set(SDIO_BUSMODE_1BIT);
	//  sdio_hardware_clock_disable();
	 
	//  /*4-�����ϵ�״̬*/
	//  sdio_power_state_set(SDIO_POWER_ON);
	 
	//  /*5-����SDIO-CKʱ�ӣ�ʱ��ʹ��*/
	//  sdio_clock_enable();
 }
 
 
 /*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <����STM32 ��д����������SDIO����ĺ���>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void SDIO_SendCommand(SDIO_SMD_Send_struct* data)
{
	/*1-���üĴ�����ֵ*/
	sdio_command_response_config(data->index,data->argument,data->response_type);
	/*2-���õȴ�����  ��*/
	sdio_wait_type_set(data->wait_type);
	/*3-ʹ�� CSM*/
	sdio_csm_enable();
}

