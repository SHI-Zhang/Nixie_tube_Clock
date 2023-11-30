#include "Configur.h"
#define INITIAL_STA 0x00
#define RECEIVING_Data 0x01
#define RECEIVED_Data 0x02

u8 USART_Receive_num = 0; // ���ڽ��յ�һ����ţ�0-9
u8 USART_Check_num = 0;	  // ���ڲ�ѯ��һ����ţ�0-9


#ifdef _WIFI_Function
	u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; // ���ջ���,���USART1_MAX_RECV_LEN���ֽ�.
	u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; // ���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
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
	// ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
	// ���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
	// �κ�����,���ʾ�˴ν������.
	// ���յ�������״̬
	//[15]:0,û�н��յ�����;1,���յ���һ������.
	//[14:0]:���յ������ݳ���
	vu16 USART1_RX_STA = 0; // ��������������ʾ����װ̬�ģ�==0��������� =1:���ݽ�����   ��==2�����ݽ�����ɣ�����δ����
							//==3�����ݳ���
	vu16 USART1_RX_Len = 0; // ���������ʵʱ�仯�ģ�����ÿ���յ��ֽڵ����������ۼӵġ�
	vu16 USART1_TX_Len = 0;


// ���ڽ��ջ�����

u8 num = 0;




void UART0_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// UART_8266_IO_Init(); // GPIO �ĳ�ʶ������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_DeInit(USART1); // ��λ����

	USART_InitStructure.USART_BaudRate = bound;										// ������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ

	// �����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_Init(USART1, &USART_InitStructure); // ��ʼ������	1

	USART_Cmd(USART1, ENABLE); // ʹ�ܴ���

	// ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // �����ж�

	//	TIM7_Int_Init(1000-1,7200-1);		//10ms�ж�
	USART1_RX_STA = 0; // ����
					   //	TIM_Cmd(TIM7,DISABLE);			//�رն�ʱ��7

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
 * @brief    : <Ӧ����usart�Ľ��պ���>
 * @Input    : <param>
 * @return   : <param>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
void USART1_IRQHandler(void)
{
	u8 res;
	u8 i = 0;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // ���յ�����
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

		if (Usart_Rec_Arr.Data_len == 0) // ���ǵ�һ�ν���������հ�
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
			else // ���ݽ������
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
 * @brief    : <���ڵķ��ͺ���>
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
			; // ѭ������,ֱ���������
		USART_SendData(USART1, *(Msg + i));
	}
}

/*********************************************************************
 * @fn       : <MY_UART_Send2>
 * @brief    : <�������һ�������������һ�����������͵�>
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
			; // ѭ������,ֱ���������
		USART_SendData(USART1, *(Msg + i));
	}
}
#endif /*_WIFI_Function*/ 

