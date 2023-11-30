#include "REMOTE.h"
#include "Parameter.h"
#include "GPIO.h"


#ifdef _Remote_Function


switch(key)
	{
	  case 0:
	  {

	  }break;
	  case 162:      //power
	  {

	  }break;
	  case 226:      //MENU
	  {

	  }break;
	  case 34:    //test
	  {

	  }break;
	  case 2:        //+
	  {

	  }break;
	  case 194:      //返回
	  {

	  }break;
	  case 224:      // <<-
	  {

	  }break;
	  case 168:      // >
	  {

	  }break;
	  case 104:      // 0
	  {

	  }break;
	  case 152:      // -
	  {

	  }break;
	  case 176:      // C
	  {

	  }break;
	  case 144:      // ->>
	  {

	  }break;
	  case 48:    //1
	  {
		key=key;
	  }break;
	  case 24:    //2
	  {

	  }break;
	  case 122:    //3
	  {

	  }break;
	  case 16:    //4
	  {

	  }break;
	  case 56:    //5
	  {

	  }break;
	  case 90:    //6
	  {

	  }break;
	  case 66:    //7
	  {

	  }break;
	  case 74:    //8
	  {

	  }break;
	  case 82:    //9
	  {

	  }break;

	  default:
	  {

	  }break;
	}

/*********************************************************************
 * @fn       : <Remote_Receive_Init>
 * @brief    : <用于初始化tim2定时器，准备开始进行PWm输入捕获>
 * @Input    : <param>
 * @return   : <param>
 * @author   : <lzm>
 * @date     : 2023年3月25日
 ********************************************************************/
void Remote_Receive_Init(void)
{
	// 1.定义变量
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM2_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// 2.开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟

	// 3.配置io口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // PA0 下拉   更改--01

	// 4.定时器的初始化
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseStructure.TIM_Period = 10000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	// 5.输入捕获的初始化
	// 那如何确保某一个io口正好就是某一个输入捕获的接头呢?
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1;			 // 这个好像只有1-4个通道
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 设置为上升沿捕获？
																 // 这里我不是很懂唉  ，好像是“输入捕获映射关系”，一般就是这个了，那个是2映射到1,1映射到2
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM2, &TIM2_ICInitStructure); // 注意这里的初始化函数也是要用到TIM2的

	// 6.定时器中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 更改--02
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 7.允许中断更新，
	// 使能定时器的几个中断
	TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE); // 使能更新中断，和捕获中断

	// 8.使能TIM2
	TIM_Cmd(TIM2, ENABLE); //
}

/*********************************************************************
 * @fn       : <TIM2_IRQHandler>
 * @brief    : <用于现实捕捉电平，以及电平的一些时间长度等信息>
 * @Input    : <param>
 * @return   : <param>
 * @author   : <网上当的>
 * @date     : 2023年3月25日
 ********************************************************************/
/*********************************************************************
 * @fn       : <TIM4_IRQHandler>
 * @brief    : <照着正点原子的代码从新敲一下>
 * @Input    : <param>
 * @return   : <param>
 * @editor   : <editor>
 ********************************************************************/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // 如果触发的中断是更新中断的话-->溢出了
	{
		if (RmtSta & 0x80) // 上次有数据被收到了,其实收收到了引导码
		{
			RmtSta &= ~(1 << 4);		 // 取消上升沿捕获的标志，（可能是要清零从新开始接新的数据），只要溢出一次就费了
			if ((RmtSta & 0x0F) == 0x00) // 低4位没有任何数据 ，先前没有溢出的消息
			{
				RmtSta |= 1 << 6; // 得到了一个按键的所有信息
			}
			if ((RmtSta & 0x0F) < 14) // 两次数据之间要间隔14个周期？
			{
				RmtSta++;
			}
			else
			{
				RmtSta &= ~(1 << 7); // 清空引导码
				RmtSta &= 0xf0;		 // 将低4位全部清空
			}
		}
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) // 如果捕获1发生捕获事件
	{
		if (Remote_pin_sat == 1) // 现在是高电平，那刚刚就是上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Falling); // 设置为下降沿捕获
			TIM_SetCounter(TIM2, 0);							 // 清空定时器,这可以有效的避免定时器溢出问题
			Dval = TIM_GetCounter(TIM2);
			// 记录下时间XX
			RmtSta |= 0x10; // 标记上升沿被捕获
		}
		else // 当前是低电平，表示是下降沿捕获的
		{
			TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising); // 设置为上升沿捕获
			Dval = TIM_GetCounter(TIM2);
			if (RmtSta & 0x10) // 已经完成了一次的上升沿捕获
			{
				if (RmtSta & 0x80) // 得到了引导吗的标志 ，这里是为了确保每一次的数据都是一个完整的，而不是前面的后半部分加上后面的前半部分
				{
					if ((Dval >= 300) && (Dval <= 800)) // 逻辑0
					{
						RmtRec <<= 1; // 左移一位
						RmtRec |= 0;  // 用0补最低位，就是不变
					}
					else if ((Dval >= 1400) && (Dval <= 1800)) // 逻辑1
					{
						RmtRec <<= 1;	// 左移一位
						RmtRec |= 0x01; // 用1补最低位
					}
					else if ((Dval >= 2200) && (Dval <= 2600)) // 按键重复码
					{
						RmtCnt++;
						RmtSta &= 0XF0; // 清空计时器  ????
					}
				}
				else if ((Dval > 4000) && (Dval < 4700)) // 接收到了引导码,引导码是放在信号的开头，表示开启了一段代码
				{
					RmtSta |= 0x80; // 引导吗标志位置1
					RmtCnt = 0;		// 按键次数归零
									// RmtRec=0;
				}
			}
			// RmtSta&=0xEF;    //标记下降沿被捕获
			RmtSta &= ~(1 << 4); // 经典代码啊，将第5位（[4]）变为0.表示接收到的是下降沿
		}
	}

	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_Update); // 清除中断标志位
}

/*********************************************************************
 * @fn       : <Remote_Scan>
 * @brief    : <红外键盘按键处理函数,这个采用的是轮询的方式>
 * @Input    : <param>
 * @return   : <其他 按键的键值
 *         0 没有按键被按下>
 * @editor   : <editor>
 ********************************************************************/
uint8_t Remote_Scan(void)
{
	u8 sta = 0;
	u8 t1, t2;
	if (RmtSta & (1 << 6)) // 如果得到一个按键的所有信息了
	{
		t1 = (RmtRec >> 24);				 // 最开始的8位是地址码
		t2 = (RmtRec >> 16) & 0xff;			 // 得到反地址码，这里不与的话，会有什么问题吗？
		if (t1 == ((u8)~t2) && (t1 == 0x00)) // 地址反码和地址码对应
		{
			t1 = RmtRec >> 8; // 数据码
			t2 = RmtRec;	  // 数据反码
			if (t1 == (u8)(~t2))
			{
				sta = t1; // 键值正确
			}
		}
		if ((sta == 0) || (RmtSta &= 0x80 == 0)) // 校准失败，引导码没收到
		{
			RmtSta &= ~(1 << 6); // 这里是？清楚接收到一个按键所有信息的标志
			RmtCnt = 0;
		}
	}
	return sta;
}

void Remote_Key_Deal(uint8_t VAL)
{
	switch (VAL)
	{
	case 2: //  "+"
	{
		if (Led_Mode == 1)
		{
			if (Led_Blue_Bright_u16 < LED_bright_Tab_scal - 1)
			{
				Led_Blue_Bright_u16++;
			}
		}
		else if (Led_Mode == 2)
		{
			if (Led_Yellow_Bright_u16 < LED_bright_Tab_scal - 1)
			{
				Led_Yellow_Bright_u16++;
			}
		}
	}
	break;

	case 152: //"-"
	{
		if (Led_Mode == 1)
		{
			if (Led_Blue_Bright_u16 > 0)
			{
				Led_Blue_Bright_u16--;
			}
		}
		else if (Led_Mode == 2)
		{
			if (Led_Yellow_Bright_u16 > 0)
			{
				Led_Yellow_Bright_u16--;
			}
		}
	}
	break;
	case 224: //"-"
	{
		if (Led_Mode == 1)
		{
			Led_Mode = 2;
		}
		else if (Led_Mode == 2)
		{
			Led_Mode = 1;
		}
	}
	break;
	case 144: //"-"
	{
		if (Led_Mode == 1)
		{
			Led_Mode = 2;
		}
		else if (Led_Mode == 2)
		{
			Led_Mode = 1;
		}
	}
	break;
	default:
	{
	}
	break;
	}
}

#endif  /*_Remote_Function*/
