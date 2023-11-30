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
 * @brief  : ADC��ʼ��
 * @param  : <�������>
 * @return : <�������>
 * @editor : <����/�༭ʷ>
 ********************************************************************/
void ADC1_Init_User(void)
{
//01 �������
  ADC_InitTypeDef ADC_InitStructure;
  
//02 ��λADC1��
  ADC_DeInit(ADC1);
  
//03 ʹ����Ҫ�õ���gpio��&adcʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  
//04 ����ADC��ʱ�ӷ�Ƶ��  ����Ƶ�ʲ��ܳ���14MHZ
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);  // 72?/8=9
  

  
//06 ����ADC����    ����Ĳ���Ŀǰ���ǹ̶������ɣ������Ժ���ʵ���
  ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;    //ADC����ģʽ
  ADC_InitStructure.ADC_ScanConvMode=DISABLE;    //��ͨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;     //����ת��ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;    //ת����������������������ⲿ��������
  ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;    //�����Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel=1;      //˳����й���ת����adcͨ����Ŀ    �ò��������в���ȷ
  ADC_Init(ADC1,&ADC_InitStructure);
  
//07 ʹ��ADC
  ADC_Cmd(ADC1,ENABLE);

//08 ������λУ׼
  ADC_ResetCalibration(ADC1);

//09 �ȴ���λУ׼����
  while(ADC_GetResetCalibrationStatus(ADC1));

//10 ����ADУ׼
  ADC_StartCalibration(ADC1);

//11 �ȴ�ADУ׼����
  while(ADC_GetCalibrationStatus(ADC1));
  
}

/*********************************************************************
 * @fn     : <Get_Adc1>
 * @brief  : <��ȡadc1��chͨ����adcֵ>
 * @param  : <ch���������ĸ�ͨ��>
 * @return : <adc�ɼ����ֵ>
 * @editor : <editor>
 ********************************************************************/
u16 Get_Adc1(u8 ch)
{
  ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);   //ͨ��ch,�������˳��ֵΪ1������ʱ��Ϊ239.5���ڣ�
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);      //ʹ��ָ����adc1�����ת������
  while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));      //�ȴ�ת������
  return ADC_GetConversionValue(ADC1);        //�������һ��ADC1�������ת�������
}



/*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void Key_Board_Refresh(void)
{
	//ģ��������-ҡ�˵�
	X_AXIS_Joystick.Analog_quantity=Get_Adc1(ADC_Channel_1);
	Y_AXIS_Joystick.Analog_quantity=Get_Adc1(ADC_Channel_2);
	
	
	
	//����������-����
	
	
}



/*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
u8 Digit_Key_Return(struct Key_Struct* key)
{
//	if(!X_AXIS_Joystick.action_executed_flg)            //���ܱ�ʾ���ǵ�һ�ΰ��°�����
//	{
//		X_AXIS_Joystick.action_executed_flg=1;
//		;//����ִ��һ��
//	}
//	if((!X_AXIS_Joystick.acceleration_mode_flg)/*&&(Sys_Time_5ms>=(Prss_Start_Time+100))*/)    //���µ�ʱ��ϳ���
//	{
//	   //
//	}
//	if(X_AXIS_Joystick.acceleration_mode_flg/*&&(Sys_Time_5ms%4==0)&&(Sys_Time_5ms_Pre!=Sys_Time_5ms)*/)		//���ִ��Ҳ���и�ʱ������
//	{
//	   //�ڼ���ģʽ��ִ�ж���
////				Sys_Time_5ms_Pre=Sys_Time_5ms;
//	}	
	return 1;
}





