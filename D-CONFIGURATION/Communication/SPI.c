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
 * @fn       : <function_name/函数名>
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
	// 2 使能SPI1时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //使能 SPI1 时钟
	 rcu_periph_clock_enable(RCU_SPI0);
	// 3 初始化spi结构体变量
	 SPI_InitStruct.device_mode=SPI_MASTER;		//设置 SPI 工作模式，设置为主模式
	SPI_InitStruct.device_mode = SPI_TRANSMODE_FULLDUPLEX;   //设置 SPI 单向或者双向的数据模式
	SPI_InitStruct.frame_size = SPI_FRAMESIZE_8BIT;           //设置 SPI 的数据大小:SPI 发送接收 8 位帧结构
	SPI_InitStruct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;             //串行同步时钟的空闲状态为高电平
             //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStruct.nss = SPI_NSS_SOFT;               //（使用 SSI 位）管理:内部 NSS 信号有 SSI 位控制
	SPI_InitStruct.prescale = SPI_PSC_2; //定义波特率预分频的值:波特率预分频值为 256
	SPI_InitStruct.endian = SPI_ENDIAN_MSB;           //指定数据传输从 MSB 位还是 LSB 位开始:数据传输从 MSB 位开始
	SPI_InitStruct.prescale = SPI_PSC_2;
	spi_init(SPI1, &SPI_InitStruct);    //
	// 5 使能SPI
	spi_enable(SPI0);
	return 0; 
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
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <对于两字节的数据发送，采用的是两个单字节的函数，因为spi初始化那里就已经给卡死了>
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
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <memory_Adress:DMA输出数据在Flash中的首地址>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void DMA_SPI1_Init(u32 memory_Adress,u16 buffer_size)
{
	//1-建立结构体变量
	dma_parameter_struct  DMA0_Init;
	//2-使能DMA1时钟  因为SPI1是DMA通道1的3号通道
	rcu_periph_clock_enable(RCU_DMA0);
	//3
	dma_deinit(DMA0,DMA_CH2);           //将DMA的通道2(stm32的3)寄存器重设为缺省值
	DMA0_Init.periph_addr=SPI_DATA(SPI0);     //启动传输前装入外设的地址
	DMA0_Init.periph_inc=DMA_PERIPH_INCREASE_DISABLE;//外设地址寄存器不变
	DMA0_Init.periph_width=DMA_PERIPHERAL_WIDTH_8BIT;//数据宽度为8位
	
	DMA0_Init.memory_addr=(u32)memory_Adress;              //待发送数据的首地址 
	DMA0_Init.memory_inc=DMA_MEMORY_INCREASE_ENABLE;
	DMA0_Init.memory_width=DMA_MEMORY_WIDTH_8BIT;//数据宽度为8位
	
	DMA0_Init.direction=DMA_MEMORY_TO_PERIPHERAL;    //数据传输方向，从内存读取发送到外设
	DMA0_Init.number=
	DMA0_Init.priority=	DMA_PRIORITY_HIGH;
	
	dma_init(DMA0,DMA_CH2,&DMA0_Init);
	
//    DMA1_Init.DMA_PeripheralBaseAddr = (u32)(&SPI1->DR);          //启动传输前装入外设的地址，表述方法基本雷同
//    DMA1_Init.DMA_MemoryBaseAddr = (u32)memory_Adress;              //待发送数据的首地址 
//	DMA1_Init.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
    //DMA1_Init.DMA_BufferSize =buffer_size;   // DMA通道的DMA缓存的大小（需传输的数据实际字节数）
//    DMA1_Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址寄存器不变
//    DMA1_Init.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内存地址寄存器递增
//    DMA1_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
//    DMA1_Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //数据宽度为8位
    //DMA1_Init.DMA_Mode = DMA_Mode_Normal;                           //工作在正常模式
//    DMA1_Init.DMA_Priority = DMA_Priority_High;                     // DMA通道 x拥有高优先级
    //DMA1_Init.DMA_M2M = DMA_M2M_Disable;                            // DMA通道x没有设置为内存到内存传输
		
    //DMA_Init(DMA1_Channel3, &DMA1_Init);                            //对DMA通道3进行初始化
} 



