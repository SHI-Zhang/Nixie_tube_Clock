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
 #include "Key_Board.h"
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 struct Key_Struct X_AXIS_Joystick={0}; 
 struct Key_Struct Y_AXIS_Joystick={0};
 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 

 
 /*********************************************************************
 * @fn     : 
 * @brief  : ADC初始化
 * @param  : <输入参数>
 * @return : <输出参数>
 * @editor : <作者/编辑史>
 ********************************************************************/
void ADC1_Init_User(void)
{
//01 定义变量
  ADC_InitTypeDef ADC_InitStructure;
  
//02 复位ADC1，
  ADC_DeInit(ADC1);
  
//03 使能需要用到的gpio口&adc时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  
//04 设置ADC的时钟分频因  这里频率不能超过14MHZ
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);  // 72?/8=9
  

  
//06 配置ADC参数    下面的参数目前就是固定这样吧，具体以后再实验吧
  ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;    //ADC独立模式
  ADC_InitStructure.ADC_ScanConvMode=DISABLE;    //单通道模式
  ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;     //单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;    //转换由软件启动，而不是由外部触发启动
  ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;    //数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel=1;      //顺序进行规制转换的adc通道数目    该参数意义尚不明确
  ADC_Init(ADC1,&ADC_InitStructure);
  
//07 使能ADC
  ADC_Cmd(ADC1,ENABLE);

//08 开启复位校准
  ADC_ResetCalibration(ADC1);

//09 等待复位校准结束
  while(ADC_GetResetCalibrationStatus(ADC1));

//10 开启AD校准
  ADC_StartCalibration(ADC1);

//11 等待AD校准结束
  while(ADC_GetCalibrationStatus(ADC1));
  
}

/*********************************************************************
 * @fn     : <Get_Adc1>
 * @brief  : <获取adc1的ch通道的adc值>
 * @param  : <ch：具体是哪个通道>
 * @return : <adc采集后的值>
 * @editor : <editor>
 ********************************************************************/
u16 Get_Adc1(u8 ch)
{
  ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);   //通道ch,规则采样顺序值为1，采样时间为239.5周期，
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);      //使能指定的adc1的软件转换功能
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));      //等待转换结束
  return ADC_GetConversionValue(ADC1);        //返回最近一次ADC1规则组的转换结果。
}



/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void Key_Board_Refresh(void)
{
	//模拟量按键-摇杆等
	X_AXIS_Joystick.Analog_quantity=Get_Adc1(ADC_Channel_1);
	Y_AXIS_Joystick.Analog_quantity=Get_Adc1(ADC_Channel_2);
	
	
	
	//数字量按键-按键
	
	
}



/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
u8 Digit_Key_Return(struct Key_Struct* key)
{
//	if(!X_AXIS_Joystick.action_executed_flg)            //可能表示的是第一次按下按键，
//	{
//		X_AXIS_Joystick.action_executed_flg=1;
//		;//动作执行一次
//	}
//	if((!X_AXIS_Joystick.acceleration_mode_flg)/*&&(Sys_Time_5ms>=(Prss_Start_Time+100))*/)    //按下的时间较长了
//	{
//	   //
//	}
//	if(X_AXIS_Joystick.acceleration_mode_flg/*&&(Sys_Time_5ms%4==0)&&(Sys_Time_5ms_Pre!=Sys_Time_5ms)*/)		//这个执行也得有个时间间隔啊
//	{
//	   //在加速模式下执行动作
////				Sys_Time_5ms_Pre=Sys_Time_5ms;
//	}	
	return 1;
}





