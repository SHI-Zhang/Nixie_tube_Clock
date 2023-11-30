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
 
  
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ˵�� ��ʼ   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ���ƹ�������ϵͳʱ��ģ�����ʵ��ʱ�䡢ϵͳ�δ�ʱ�䡢�ȵ�
 --------------------------------  ˵�� ����   -------------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 #include <gd32f30x.h>
 #include "TIME_Management.h"
// #include "lvgl.h"			//����LVGL��ͷ�ļ�������lvgl������
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 uint8_t g_Hour=15;
 uint8_t g_Minute=18;
 uint8_t g_Second=00;
 uint8_t g_Wifi_line=0;
 uint8_t Refresh_Display_time_flg=0;			//0==����ˢ��ʱ����ʾ��1==ˢ��ʱ����ʾ
 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 
 
 
/*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <1msһ�ģ���ʱ��Ϊlvgl������>
 * @note     : <>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void TIMER2_MY_Init(void)
{
 //@1:�������
  timer_parameter_struct TIM_TimeBaseInitStruct={0};
  NVIC_InitTypeDef_GD32 NVIC_InitStructure;
 //@2;ʹ�ܶ�ʱ��ʱ��
	rcu_periph_clock_enable(RCU_TIMER2);		// ʱ��ʹ�ܣ����ҿɵ�TIMER2���ص���APB1ʱ���ߡ�
	
	timer_deinit(TIMER2);
	
 //@3:���ö�ʱ������
  TIM_TimeBaseInitStruct.counterdirection=TIMER_COUNTER_UP;// TIM���ϼ���ģʽ
  TIM_TimeBaseInitStruct.period=10000-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,�����˶��һ���ж�
  TIM_TimeBaseInitStruct.prescaler=12000-1;// ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ,����������Ƶ�ʡ�8M/(7999+1)=1k
      //��˵���������������Ը߼���ʱ�����ã�����csdn���ղ������������
      //  TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;  //�������о��������.
      /*�������о��������.������ʱ�ӷ�Ƶϵ���������Ƶϵ��Ϊ1��ʱ��ʵ��ֵ��0������ʾʱ�Ӳ��䣬���Ϊ2��������Ƕ�ʱ��ʱ�ӱ�Ϊ1/2
      Ҳ���Ǳ���������1s��Ȼ��ͱ����2s*/
  TIM_TimeBaseInitStruct.clockdivision=TIMER_CKDIV_DIV1;
	TIM_TimeBaseInitStruct.alignedmode=TIMER_COUNTER_EDGE;		//����ģʽ
	TIM_TimeBaseInitStruct.repetitioncounter=0;			//�Զ���װֵ��ÿ�δ����жϺ󣬴����ֵ��ʼ�����������涨��period��
  timer_init(TIMER2,&TIM_TimeBaseInitStruct);// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	
	timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);	// ���TIMx���жϴ�����λ:TIMER2 �ж�Դ ���·��֣�Ҫ�ڿ����ж�ǰ��һ���жϱ�־λ������ͻ�һ���жϾͽ����жϡ�


	//@4:ʹ�ܻ���ʧ��ָ����TIM�ж�   ����ź����ǲ��Ǻõ㣿
  timer_interrupt_enable(TIMER2,TIMER_INT_UP);
	
	
 //@5:�ж����ȼ� NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = TIMER2_IRQn;        // TIMER2�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    // ��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      // �����ȼ�3��
	nvic_irq_enable(NVIC_InitStructure.NVIC_IRQChannel,\
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority,\
	NVIC_InitStructure.NVIC_IRQChannelSubPriority);// ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	

 //@6:ʹ��TIMx����
  timer_enable(TIMER2);                    // ʹ��TIMx���裬ʹ�ܶ�ʱ��
}



/*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <TIMER2�жϴ�����>
 * @note     : <>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/

void TIMER2_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) != RESET)       // ���ָ����TIM�жϷ��������ָ����TIM�ж�Դ 
    {
      timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);     // ���TIMx���жϴ�����λ:TIMER2 �ж�Դ 
//      /*�жϴ�����룬���û���д*/
			Refresh_Display_time_flg=1;
			g_Second++;
			if(g_Second>=60)
			{
				g_Second=0;
				g_Minute++;
				if(g_Minute>=60)
				{
					g_Minute=0;
					g_Hour++;
					
					if(g_Hour>=24)
					{
						g_Hour=0;
					}
				}
			}
			
// 			lv_tick_inc(1);			//input:* == *ms
				/*�жϴ�����룬���û���д*/
    }
	
}



/*********************************************************************
 * @fn       : <TIM4_PWM_Init/������>
 * @brief    : <TIM4���PWM���ĳ�ʼ������>
 * @note     : <>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/

//void TIM4_PWM_Init()
//{
//    // 1-�������
//    TIM_OCInitTypeDef TIM_OCInitStruct = {0};
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct = {0};
//    // 2-������Ӧʱ��
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    TIM_TimeBaseStruct.TIM_Period = LED_Per - 1;
//    TIM_TimeBaseStruct.TIM_Prescaler = 1 - 1;
//    TIM_TimeBaseStruct.TIM_ClockDivision = 0;
//    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);

//    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC1Init(TIM4, &TIM_OCInitStruct);
//    TIM_OC2Init(TIM4, &TIM_OCInitStruct);

//    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
//    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
//    TIM_Cmd(TIM4, ENABLE);
//}
