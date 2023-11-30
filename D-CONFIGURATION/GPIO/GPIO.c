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
/*计划所有的gpio初始化，都放到这个文件中，然后在main函数中统一调用*/
/*--------------------------------  说明 结束   -------------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "gd32f30x_timer.h"
#include "gd32f30x.h"
#include "Configur.h"
#include "GPIO.h"
//#include "Key_Board.h"
/*-----------------------------  #INCLUDE   END   ----------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
struct PIN_Var USART0_CK_struct = {USART0_CK_pin};   // PA8
struct PIN_Var USART0_TX_struct = {USART0_TX_pin};   // PA9
struct PIN_Var USART0_RX_struct = {USART0_RX_pin};   // PA10
struct PIN_Var USART0_CTS_struct = {USART0_CTS_pin}; // PA11
struct PIN_Var USART0_RTS_struct = {USART0_RTS_pin}; // PA12


struct PIN_Var OLED_D_C_struct= {OLED_D_C_pin }; // PA3
struct PIN_Var SPI0_NSS_struct= {SPI0_NSS_pin }; // PA4
struct PIN_Var SPI0_SCK_struct= {SPI0_SCK_pin }; // PA5
struct PIN_Var SPI0_MISO_struct={SPI0_MISO_pin}; // PA6
struct PIN_Var SPI0_MOSI_struct={SPI0_MOSI_pin}; // PA7
struct PIN_Var LCD_RES_struct={LCD_RES_pin}; // Pc4

struct PIN_Var X_struct={X_pin};
struct PIN_Var Y_struct={Y_pin};
struct PIN_Var Enter_struct={Enter_pin};

//SDIO
struct PIN_Var SDIO_D0_struct={SDIO_D0_Pin}; 		//pc8
struct PIN_Var SDIO_D1_struct={SDIO_D1_Pin}; 		//pc9
struct PIN_Var SDIO_D2_struct={SDIO_D2_Pin}; 		//pc10
struct PIN_Var SDIO_D3_struct={SDIO_D3_Pin}; 		//pc11
struct PIN_Var SDIO_SCk_struct={SDIO_SCk_Pin}; 		//pc12
struct PIN_Var SDIO_CMD_struct={SDIO_CMD_Pin}; 		//pd2
/*-------------------------  DECLARE  VARIABLE  END   ------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 extern struct Key_Struct X_AXIS_Joystick; 
 extern struct Key_Struct Y_AXIS_Joystick;
/*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/

void GPIO_Init_GD32(GPIO_InitTypeDef_GD32* Data)
{
	gpio_init(Data->GPIO_GROUP,Data->GPIO_MODE,Data->GPIO_SPEED,Data->GPIO_PIN);
}



/*********************************************************************
 * @fn       : <IO_Init  /  整体的GPIO初始化函数，用于main函数中>
 * @brief    : <brief>
 * @param    : <param>
 * @return   : <param>
 * @author   : <editor>
 * @date     :  2023年10月9日
 ********************************************************************/
void IO_Init(void)
{
    UART_GPIO_Init();
#ifdef _1_47_IPS_LCD
	SPI1_GPIO_Init();
#endif  /*_1_47_IPS_LCD*/
	KeyBoard_Init();
	
	SDIO_GPIO_Init();
	

}

/*********************************************************************
 * @fn       : <function_name>
 * @brief    : <brief>
 * @param    : <param>
 * @return   : <param>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
//void PIN_Init()
//{
//    //@1定义变量
//    GPIO_InitTypeDef_GD32 GPIO_StructInit = {0};
//    //@2开启时钟
//		rcu_periph_clock_enable(GPIOA|GPIOB);   
//    //@3配置GPIO口模式   YELLOW_PIN    BLUE_PIN  //输出端口
//		GPIO_StructInit.GPIO_GROUP=GPIOA;
//    GPIO_StructInit.GPIO_MODE = GPIO_MODE_AF_PP;
//    GPIO_StructInit.GPIO_PIN = YELLOW_PIN.PIN | BLUE_PIN.PIN;
//    GPIO_StructInit.GPIO_SPEED = GPIO_OSPEED_MAX;
//		GPIO_Init_GD32(&GPIO_StructInit);
//    //@4 IO口的初值设置
//    gpio_bit_reset(YELLOW_PIN.group, YELLOW_PIN.PIN);
//    gpio_bit_reset(BLUE_PIN.group, BLUE_PIN.PIN);

//    //@3配置GPIO口模式   LIGHT_up   LIGHT_DOWN MODE   HONGWAI    MIC  //输入端口
//    GPIO_StructInit.GPIO_MODE = GPIO_MODE_IN_FLOATING;
//    GPIO_StructInit.GPIO_Pin = LIGHT_UP.PIN | LIGHT_DOWN.PIN | MODE.PIN;
//    gpio_init(LIGHT_UP.group, &GPIO_StructInit);
//    GPIO_StructInit.GPIO_Pin = HONGWAI.PIN;
//    gpio_init(HONGWAI.group, &GPIO_StructInit);
//    GPIO_StructInit.GPIO_Pin = MIC.PIN;
//    GPIO_StructInit.GPIO_MODE = GPIO_MODE_AIN;
//    gpio_init(MIC.group, &GPIO_StructInit);
//}

/*********************************************************************
 * @fn       : <串口1的GPIO初始化代码>
 * @brief    : <USART0_CK   PA8
 *              USART0_TX   PA9
 *              USART0_RX   PA10
 *              USART0_CTS  PA11
 *              USART0_RTS PA12>
 * @Input    : <param>
 * @return   : <param>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
	void UART_GPIO_Init(void)
	{
		GPIO_InitTypeDef_GD32	GPIO_InitStructure={0};

		rcu_periph_clock_enable(RCU_GPIOA); //使能GPIOa时钟

		GPIO_InitStructure.GPIO_GROUP=USART0_TX_struct.group;
		GPIO_InitStructure.GPIO_PIN = USART0_TX_struct.PIN;
		GPIO_InitStructure.GPIO_MODE = GPIO_MODE_AF_PP;
		GPIO_InitStructure.GPIO_SPEED = GPIO_OSPEED_MAX;
		GPIO_Init_GD32(&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_GROUP=USART0_RX_struct.group;
		GPIO_InitStructure.GPIO_PIN = USART0_RX_struct.PIN;
		GPIO_InitStructure.GPIO_MODE = GPIO_MODE_IN_FLOATING;
		GPIO_InitStructure.GPIO_SPEED = GPIO_OSPEED_MAX;
		GPIO_Init_GD32(&GPIO_InitStructure);
	}


	/*********************************************************************
	* @fn       : <function_name/函数名>
	* @brief    : <brief>
	* @note     : <SPI0_MISO PA6
				SPI0_NSS PA4
				SPI0_SCK PA5
				SPI0_MOSI PA7
				D/C PA3
				>
	* @param    : <param>
	* @return   : <return>
	* @author   : <editor>
	* @date     : 
	********************************************************************/
	u8 SPI1_GPIO_Init(void)
	{
		GPIO_InitTypeDef_GD32	gpio_initStruct={0};
		
		rcu_periph_clock_enable(RCU_GPIOA); //使能GPIOa时钟
		rcu_periph_clock_enable(RCU_GPIOC); //使能GPIOa时钟//使能GPIOc时钟
		
		gpio_initStruct.GPIO_GROUP=GPIOA;
		gpio_initStruct.GPIO_MODE=GPIO_MODE_OUT_PP;
		gpio_initStruct.GPIO_PIN=OLED_D_C_struct.PIN|SPI0_NSS_struct.PIN;
		gpio_initStruct.GPIO_SPEED=GPIO_OSPEED_MAX;
		GPIO_Init_GD32(&gpio_initStruct);
		
		gpio_initStruct.GPIO_GROUP=LCD_RES_struct.group;
		gpio_initStruct.GPIO_MODE=GPIO_MODE_OUT_PP;
		gpio_initStruct.GPIO_PIN=LCD_RES_struct.PIN;
		gpio_initStruct.GPIO_SPEED=GPIO_OSPEED_MAX;
		GPIO_Init_GD32(&gpio_initStruct);

		gpio_initStruct.GPIO_GROUP=GPIOA;
		gpio_initStruct.GPIO_MODE=GPIO_MODE_AF_PP;
		gpio_initStruct.GPIO_PIN=SPI0_SCK_struct.PIN|SPI0_MOSI_struct.PIN;
		gpio_initStruct.GPIO_SPEED=GPIO_OSPEED_MAX;
		GPIO_Init_GD32(&gpio_initStruct);

		gpio_initStruct.GPIO_GROUP=SPI0_MISO_struct.group;
		gpio_initStruct.GPIO_MODE=GPIO_MODE_IN_FLOATING;
		gpio_initStruct.GPIO_PIN=SPI0_MISO_struct.PIN;
		GPIO_Init_GD32(&gpio_initStruct);

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
 void KeyBoard_Init(void)
 {
	GPIO_InitTypeDef_GD32 gpio_initStruct={0};
	
	//Enter 键
	rcu_periph_clock_enable(RCU_GPIOA);    //使能GPIOa时钟

	gpio_initStruct.GPIO_MODE=GPIO_MODE_IPD;
	gpio_initStruct.GPIO_SPEED=GPIO_OSPEED_MAX;
	gpio_initStruct.GPIO_PIN=Enter_struct.PIN;
	GPIO_Init_GD32(&gpio_initStruct);
	
	//两个模拟输入
	
	gpio_initStruct.GPIO_MODE=GPIO_MODE_AIN;
	gpio_initStruct.GPIO_SPEED=GPIO_OSPEED_MAX;
	gpio_initStruct.GPIO_PIN=X_struct.PIN|Y_struct.PIN;
	GPIO_Init_GD32(&gpio_initStruct);
 }

// 

//u8 F1_KEY_STATUS(void)
//{
//	static u8 up_key=0;
//	Key_Board_Refresh();
//	
//	if(!GPIO_ReadInputDataBit(Enter_pin))
//	{
//		if(up_key==1)
//			{return 0;}
//		else
//		{
//		up_key=1;
//		return 1;
//		}

//	}else if(X_AXIS_Joystick.Analog_quantity<100)
//	{
//			if(up_key==1)
//			{return 0;}
//		else
//		{
//		up_key=1;
//		return 2;
//		}
//	}else if(X_AXIS_Joystick.Analog_quantity>4000)
//	{
//		if(up_key==1)
//			{return 0;}
//		else
//		{
//		up_key=1;
//		return 3;
//		}
//	}else if(Y_AXIS_Joystick.Analog_quantity<100)
//	{
//		if(up_key==1)
//			{return 0;}
//		else
//		{
//		up_key=1;
//		return 4;
//		}
//	}else if(Y_AXIS_Joystick.Analog_quantity>4000)
//	{
//				if(up_key==1)
//			{return 0;}
//		else
//		{
//		up_key=1;
//		return 5;
//		}
//	}
//	else
//	{
//		up_key=0;
//	}
//	
//}
// 
 
/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <SDIO 的 GPIO口初始化函数>
 * @note     : <SDIO_D0_Pin		GPIOC, GPIO_PIN_8			//pc8		复用推挽输出
								SDIO_D1_Pin		GPIOC, GPIO_PIN_9			//pc9		复用推挽输出
								SDIO_D2_Pin		GPIOC, GPIO_PIN_10		//pc10	复用推挽输出
								SDIO_D3_Pin		GPIOC, GPIO_PIN_11		//pc11	复用推挽输出
								SDIO_SCk_Pin	GPIOC, GPIO_PIN_12		//pc12	复用推挽输出  
								SDIO_CMD_Pin	GPIOD, GPIO_PIN_2			//pd2		复用推挽输出
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
 void SDIO_GPIO_Init(void)
 {
	 //#1 建立变量
	 GPIO_InitTypeDef_GD32	gpio_initStruct={0};
	 
	 //#2  开启时钟
	 rcu_periph_clock_enable(RCU_GPIOC);
	 rcu_periph_clock_enable(RCU_GPIOD);
	 rcu_periph_clock_enable(RCU_AF);
	 
	 //#3  配置模式
	 //@1
	 gpio_initStruct.GPIO_GROUP=GPIOC;
	 gpio_initStruct.GPIO_PIN=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
	 gpio_initStruct.GPIO_MODE=GPIO_MODE_AF_PP;
	 gpio_initStruct.GPIO_SPEED=GPIO_OSPEED_MAX;
	 GPIO_Init_GD32(&gpio_initStruct);
	 //@2
	 gpio_initStruct.GPIO_GROUP=GPIOD;
	 gpio_initStruct.GPIO_PIN=GPIO_PIN_2;
//	 gpio_initStruct.GPIO_MODE=GPIO_MODE_AF_PP;
//	 gpio_initStruct.GPIO_SPEED=GPIO_OSPEED_MAX;
	 GPIO_Init_GD32(&gpio_initStruct);
 }
 
 
