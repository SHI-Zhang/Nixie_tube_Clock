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
 
  
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  说明 开始   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 控制管理整个系统时间的，比如实际时间、系统滴答时间、等等
 --------------------------------  说明 结束   -------------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 #include <gd32f30x.h>
 #include "TIME_Management.h"
// #include "lvgl.h"			//包含LVGL的头文件，配置lvgl的心跳
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 uint8_t g_Hour=15;
 uint8_t g_Minute=18;
 uint8_t g_Second=00;
 uint8_t g_Wifi_line=0;
 uint8_t Refresh_Display_time_flg=0;			//0==不用刷新时间显示，1==刷新时间显示
 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 
 
 
/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <1ms一蹦，临时改为lvgl心跳。>
 * @note     : <>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void TIMER2_MY_Init(void)
{
 //@1:定义变量
  timer_parameter_struct TIM_TimeBaseInitStruct={0};
  NVIC_InitTypeDef_GD32 NVIC_InitStructure;
 //@2;使能定时器时钟
	rcu_periph_clock_enable(RCU_TIMER2);		// 时钟使能，并且可得TIMER2搭载的是APB1时钟线、
	
	timer_deinit(TIMER2);
	
 //@3:配置定时器参数
  TIM_TimeBaseInitStruct.counterdirection=TIMER_COUNTER_UP;// TIM向上计数模式
  TIM_TimeBaseInitStruct.period=10000-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值,决定了多久一次中断
  TIM_TimeBaseInitStruct.prescaler=12000-1;// 设置用来作为TIMx时钟频率除数的预分频值,决定计数的频率。8M/(7999+1)=1k
      //据说下面的这个参数仅对高级定时器有用，我在csdn中收藏了这个参数。
      //  TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;  //后面来研究这个参数.
      /*后面来研究这个参数.好像是时钟分频系数，这个分频系数为1的时候（实际值是0），表示时钟不变，如果为2，好像就是定时器时钟变为1/2
      也就是本来定的是1s，然后就变成了2s*/
  TIM_TimeBaseInitStruct.clockdivision=TIMER_CKDIV_DIV1;
	TIM_TimeBaseInitStruct.alignedmode=TIMER_COUNTER_EDGE;		//对齐模式
	TIM_TimeBaseInitStruct.repetitioncounter=0;			//自动重装值，每次触发中断后，从这个值开始数，数到上面定的period。
  timer_init(TIMER2,&TIM_TimeBaseInitStruct);// 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	
	timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);	// 清除TIMx的中断待处理位:TIMER2 中断源 最新发现，要在开启中断前清一下中断标志位，否则就会一开中断就进入中断。


	//@4:使能或者失能指定的TIM中断   这个放后面是不是好点？
  timer_interrupt_enable(TIMER2,TIMER_INT_UP);
	
	
 //@5:中断优先级 NVIC 设置
  NVIC_InitStructure.NVIC_IRQChannel = TIMER2_IRQn;        // TIMER2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    // 先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      // 从优先级3级
	nvic_irq_enable(NVIC_InitStructure.NVIC_IRQChannel,\
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority,\
	NVIC_InitStructure.NVIC_IRQChannelSubPriority);// 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	

 //@6:使能TIMx外设
  timer_enable(TIMER2);                    // 使能TIMx外设，使能定时器
}



/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <TIMER2中断处理函数>
 * @note     : <>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/

void TIMER2_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) != RESET)       // 检查指定的TIM中断发生与否是指定的TIM中断源 
    {
      timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);     // 清除TIMx的中断待处理位:TIMER2 中断源 
//      /*中断处理代码，由用户编写*/
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
				/*中断处理代码，由用户编写*/
    }
	
}



/*********************************************************************
 * @fn       : <TIM4_PWM_Init/函数名>
 * @brief    : <TIM4输出PWM波的初始化函数>
 * @note     : <>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/

//void TIM4_PWM_Init()
//{
//    // 1-定义变量
//    TIM_OCInitTypeDef TIM_OCInitStruct = {0};
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct = {0};
//    // 2-开启相应时钟
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
