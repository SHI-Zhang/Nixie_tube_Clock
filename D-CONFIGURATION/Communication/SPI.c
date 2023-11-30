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
 #include "gd32f30x.h"
 #include "SPI.h"
 #include "GPIO.h"
 #include "delay.h"
// #include "LCD.h"
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
 
 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 
 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/
 
 
 /*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
 u8 SPI_MY_Init(void)
 {
	spi_parameter_struct SPI_InitStruct;
	// 2 ʹ��SPI1ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //ʹ�� SPI1 ʱ��
	 rcu_periph_clock_enable(RCU_SPI0);
	// 3 ��ʼ��spi�ṹ�����
	 SPI_InitStruct.device_mode=SPI_MASTER;		//���� SPI ����ģʽ������Ϊ��ģʽ
	SPI_InitStruct.device_mode = SPI_TRANSMODE_FULLDUPLEX;   //���� SPI �������˫�������ģʽ
	SPI_InitStruct.frame_size = SPI_FRAMESIZE_8BIT;           //���� SPI �����ݴ�С:SPI ���ͽ��� 8 λ֡�ṹ
	SPI_InitStruct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;             //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
             //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStruct.nss = SPI_NSS_SOFT;               //��ʹ�� SSI λ������:�ڲ� NSS �ź��� SSI λ����
	SPI_InitStruct.prescale = SPI_PSC_2; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ 256
	SPI_InitStruct.endian = SPI_ENDIAN_MSB;           //ָ�����ݴ���� MSB λ���� LSB λ��ʼ:���ݴ���� MSB λ��ʼ
	SPI_InitStruct.prescale = SPI_PSC_2;
	spi_init(SPI1, &SPI_InitStruct);    //
	// 5 ʹ��SPI
	spi_enable(SPI0);
	return 0; 
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
 void SPI0_SEND_Byte(u8 data_byte)
 {
	u8 retry=0;
	while (spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET)
	{
	  retry++;
	  if(retry>200)
	  {
		break;
	  }
	}
	spi_i2s_data_transmit(SPI0, data_byte);
	while (spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET){}
 }

 
 
 /*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <brief>
 * @note     : <�������ֽڵ����ݷ��ͣ����õ����������ֽڵĺ�������Ϊspi��ʼ��������Ѿ���������>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
//void SPI1_SEND_Word(u16 data_word)
// {
//	u8 retry=0;
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
//	{
//	  retry++;
//	  if(retry>200)
//	  {
//		break;
//	  }
//	}
//	SPI_I2S_SendData(SPI1, data_word);
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
// }

 
 
 
 /*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <memory_Adress:DMA���������Flash�е��׵�ַ>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void DMA_SPI1_Init(u32 memory_Adress,u16 buffer_size)
{
	//1-�����ṹ�����
	dma_parameter_struct  DMA0_Init;
	//2-ʹ��DMA1ʱ��  ��ΪSPI1��DMAͨ��1��3��ͨ��
	rcu_periph_clock_enable(RCU_DMA0);
	//3
	dma_deinit(DMA0,DMA_CH2);           //��DMA��ͨ��2(stm32��3)�Ĵ�������Ϊȱʡֵ
	DMA0_Init.periph_addr=SPI_DATA(SPI0);     //��������ǰװ������ĵ�ַ
	DMA0_Init.periph_inc=DMA_PERIPH_INCREASE_DISABLE;//�����ַ�Ĵ�������
	DMA0_Init.periph_width=DMA_PERIPHERAL_WIDTH_8BIT;//���ݿ��Ϊ8λ
	
	DMA0_Init.memory_addr=(u32)memory_Adress;              //���������ݵ��׵�ַ 
	DMA0_Init.memory_inc=DMA_MEMORY_INCREASE_ENABLE;
	DMA0_Init.memory_width=DMA_MEMORY_WIDTH_8BIT;//���ݿ��Ϊ8λ
	
	DMA0_Init.direction=DMA_MEMORY_TO_PERIPHERAL;    //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA0_Init.number=
	DMA0_Init.priority=	DMA_PRIORITY_HIGH;
	
	dma_init(DMA0,DMA_CH2,&DMA0_Init);
	
//    DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&SPI1->DR);          //��������ǰװ������ĵ�ַ����������������ͬ
//    DMA1_Init.DMA_MemoryBaseAddr = (u32)memory_Adress;              //���������ݵ��׵�ַ 
//	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
    //DMA1_Init.DMA_BufferSize =buffer_size;   // DMAͨ����DMA����Ĵ�С���贫�������ʵ���ֽ�����
//    DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //�����ַ�Ĵ�������
//    DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�ڴ��ַ�Ĵ�������
//    DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
//    DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //���ݿ��Ϊ8λ
    //DMA1_Init.DMA_Mode = DMA_Mode_Normal;                           //����������ģʽ
//    DMA1_Init.DMA_Priority = DMA_Priority_High;                     // DMAͨ�� xӵ�и����ȼ�
    //DMA1_Init.DMA_M2M = DMA_M2M_Disable;                            // DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		
    //DMA_Init(DMA1_Channel3, &DMA1_Init);                            //��DMAͨ��3���г�ʼ��
} 



