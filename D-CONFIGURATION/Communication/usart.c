#include "Configur.h"
#define INITIAL_STA 0x00
#define RECEIVING_Data 0x01
#define RECEIVED_Data 0x02

u8 USART_Receive_num = 0; // 用于接收的一个序号，0-9
u8 USART_Check_num = 0;	  // 用于查询的一个序号，0-9


#ifdef _WIFI_Function
	u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; // 接收缓冲,最大USART1_MAX_RECV_LEN个字节.
	u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; // 发送缓冲,最大USART1_MAX_SEND_LEN字节
	// struct USART_RX_struct USART_RECEIVE_Data_01={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_02={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_03={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_04={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_05={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_06={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_07={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_08={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_09={0,0,0,0};
	// struct USART_RX_struct USART_RECEIVE_Data_10={0,0,0,0};
	struct USART_RX_struct USART_RECEIVE_Data[10] = {0};
	// 通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
	// 如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
	// 任何数据,则表示此次接收完毕.
	// 接收到的数据状态
	//[15]:0,没有接收到数据;1,接收到了一批数据.
	//[14:0]:接收到的数据长度
	vu16 USART1_RX_STA = 0; // 单独就是用来表示接收装态的，==0：处理完毕 =1:数据接收中   。==2：数据接收完成，但是未处理
							//==3，数据超长
	vu16 USART1_RX_Len = 0; // 这个长度是实时变化的，根据每次收到字节的数量进行累加的。
	vu16 USART1_TX_Len = 0;


// 串口接收缓存区

u8 num = 0;




void UART0_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// UART_8266_IO_Init(); // GPIO 的初识化函数

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_DeInit(USART1); // 复位串口

	USART_InitStructure.USART_BaudRate = bound;										// 波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式

	// 设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器

	USART_Init(USART1, &USART_InitStructure); // 初始化串口	1

	USART_Cmd(USART1, ENABLE); // 使能串口

	// 使能接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启中断

	//	TIM7_Int_Init(1000-1,7200-1);		//10ms中断
	USART1_RX_STA = 0; // 清零
					   //	TIM_Cmd(TIM7,DISABLE);			//关闭定时器7

	USART_RECEIVE_Data[0].Last_Addr = &(USART_RECEIVE_Data[9]);
	USART_RECEIVE_Data[1].Last_Addr = &(USART_RECEIVE_Data[0]);
	USART_RECEIVE_Data[2].Last_Addr = &(USART_RECEIVE_Data[1]);
	USART_RECEIVE_Data[3].Last_Addr = &(USART_RECEIVE_Data[2]);
	USART_RECEIVE_Data[4].Last_Addr = &(USART_RECEIVE_Data[3]);
	USART_RECEIVE_Data[5].Last_Addr = &(USART_RECEIVE_Data[4]);
	USART_RECEIVE_Data[6].Last_Addr = &(USART_RECEIVE_Data[5]);
	USART_RECEIVE_Data[7].Last_Addr = &(USART_RECEIVE_Data[6]);
	USART_RECEIVE_Data[8].Last_Addr = &(USART_RECEIVE_Data[7]);
	USART_RECEIVE_Data[9].Last_Addr = &(USART_RECEIVE_Data[8]);
}




/*********************************************************************
 * @fn       : <function_name>
 * @brief    : <应该是usart的接收函数>
 * @Input    : <param>
 * @return   : <param>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
void USART1_IRQHandler(void)
{
	u8 res;
	u8 i = 0;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收到数据
	{
		res = USART_ReceiveData(USART1);

		if (Usart_Rec_Arr.Receive_Data_sat == RECEIVED_Data)
		{
			// if (Usart_Rec_Arr.Data_len != 0) // clear buf
			// {
				for (i = 0; i <= Usart_Rec_Arr.Data_len; i++)
				{
					Usart_Rec_Arr.RX_Data[i] = 0;
				}
				Usart_Rec_Arr.Receive_Data_sat = INITIAL_STA;
				Usart_Rec_Arr.Data_len = 0;
			// }
		}

		if (Usart_Rec_Arr.Data_len == 0) // 算是第一次进入这个接收吧
		{
			if ((res != 0x0A) && (res != 0x0D))
			{
				Usart_Rec_Arr.RX_Data[Usart_Rec_Arr.Data_len] = res;
				Usart_Rec_Arr.Data_len++;
				Usart_Rec_Arr.Receive_Data_sat = RECEIVING_Data;
			}
		}
		else
		{
			if ((res != 0x0A) && (res != 0x0D))
			{

				Usart_Rec_Arr.RX_Data[Usart_Rec_Arr.Data_len] = res;
				Usart_Rec_Arr.Data_len++;
			}
			else // 数据接收完毕
			{
				Usart_Rec_Arr.Receive_Data_sat = RECEIVED_Data;
				USART_Receive_num++;
				if (USART_Receive_num == 8)
				{
					USART1_RX_BUF[0] = res;
				}
			}
		}

		if (USART_Receive_num > 9)
		{
			USART_Receive_num = 0;
		}
	}
}

/*********************************************************************
 * @fn       : <function_name>
 * @brief    : <窗口的发送函数>
 * @Input    : <param>
 * @return   : <param>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
void MY_UART_Send(u8 *Msg, u8 Len)
{
	u8 i = 0;
	for (i = 0; i < Len; i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			; // 循环发送,直到发送完毕
		USART_SendData(USART1, *(Msg + i));
	}
}

/*********************************************************************
 * @fn       : <MY_UART_Send2>
 * @brief    : <相比于上一个函数，这个是一个不定长发送的>
 * @Input    : <param>
 * @return   : <param>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
void MY_UART_Send_2(u8 *Msg)
{
	u8 i = 0, Len = 255;
	for (i = 0; (i < Len) && (*(Msg + i) != 0x00); i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			; // 循环发送,直到发送完毕
		USART_SendData(USART1, *(Msg + i));
	}
}
#endif /*_WIFI_Function*/ 

