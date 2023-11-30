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
 Ҫ�Ǻ�����ʾ�Ļ�����ô�ֱ�����172X320���������1-25 ������ʾͼ�꣬������ת̬����Ҫ�Ļ���������ʾʱ��
 --------------------------------  ˵�� ����   -------------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 #include "parameter.h"
 #include "LCD_Display.h"
 #include "TIME_Management.h"
 #include "LCD.h"
 #include "Key_Board.h"
 #include "GPIO.h"
 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/

char Virtual_Keyboard_1_row[10]={'0','1','2','3','4','5','6','7','8','9'};
char Virtual_Keyboard_2_row[10]={'Q','W','E','R','T','Y','U','I','O','P'};
char Virtual_Keyboard_3_row[10]={'A','S','D','F','G','H','J','K','L',';'};
char Virtual_Keyboard_4_row[10]={'Z','X','C','V','B','N','M',',','.','/'};
 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/
 extern u8 Wifi_Line[];
 extern u8 Wifi_no_Line[];
 extern struct Key_Struct X_AXIS_Joystick;
 extern struct Key_Struct Y_AXIS_Joystick;
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
void SPI1_DMA_Send(u16 data_size)
{
		SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);     //ʹ��SPI1��DMA���͹���
		DMA_Cmd(DMA1_Channel3,DISABLE);       //�رն�Ӧ��dmaͨ��
		DMA_SetCurrDataCounter(DMA1_Channel3,data_size);     //����DMA�������ز�����DMAͨ����������Ŀ
		DMA_Cmd(DMA1_Channel3, ENABLE);   //��������
}




 /*********************************************************************
 * @fn       : <Refresh_Screen/ˢ����Ļ����ʾ>
 * @brief    : <brief>
 * @note     : <DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET  �������>
 * @param    : <param>
 * @return   : <1==��ʼִ��
								0==�ϴδ�����δ��ɣ��ȴ��С���>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
u8 Refresh_Screen(u16 y_coordinate)
{
	u8 i=0,j=0;
	u16 k=0;
	static u16 run=0;
	for(i=y_coordinate;i<55+y_coordinate;i+=10)
	{
		for(j=0;j<10;j++)
		{
			for (k=0;k<320;k++)
			{
//				if(color==0xffff)
//					color=0x0000;
//				IPS_LCD_Cahe_Data[j][k]=((u16)gImage_3445[(i-y_coordinate+1)*(k+1)*2-2]<<8)+((u16)gImage_3445[(i-y_coordinate+1)*(k+1)*2-1]);
			}
		}
		LCD_Address_Set(0,i,319,i+9);  //����λ�� 
		CS_L;
		DATA_ORDER;        //��ΪҪ�������ݣ�����Ҫ��RS��CS������Ӧ�����á�
		SPI1_DMA_Send(10*320*2);
		while(DMA_GetFlagStatus(DMA1_FLAG_TC3)==RESET)
		{
		}
		DMA_ClearFlag(DMA1_FLAG_TC3);
		run++;
	}
		
	return 0;  
}
 
/*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <������ʾͼ���>
 * @note     : <���������1-24�У�ÿ��ͼ����24*24 �ģ�����24*12
				��Ҫ��ʾ��Ԫ���У�ʱ�䣬   wifi����������Ժ���ԵĻ������е�����>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void Status_bar(void )
{
//	LCD_Fill(0,0,320,24,LGRAY);
	//time hour
	if(Refresh_Display_time_flg)
	{
		Refresh_Display_time_flg=0;
		
		LCD_ShowChar(12,0,0x30+g_Hour/10,BLACK,LGRAY,24,0);
		LCD_ShowChar(24,0,0x30+g_Hour%10,BLACK,LGRAY,24,0);
		//:
		LCD_ShowChar(36,0,':',BLACK,LGRAY,24,0);
		//minute
		LCD_ShowChar(48,0,0x30+g_Minute/10,BLACK,LGRAY,24,0);
		LCD_ShowChar(60,0,0x30+g_Minute%10,BLACK,LGRAY,24,0);
		
		//:
		LCD_ShowChar(72,0,':',BLACK,LGRAY,24,0);
		//second
		LCD_ShowChar(84,0,0x30+g_Second/10,BLACK,LGRAY,24,0);
		LCD_ShowChar(96,0,0x30+g_Second%10,BLACK,LGRAY,24,0);
		
		if(g_Wifi_line)
		{
			LCD_Show_ICON(272,0,Wifi_Line,BLACK,LGRAY,24,0);
		}
		else
		{
			LCD_Show_ICON(272,0,Wifi_no_Line,BLACK,LGRAY,24,0);
		}
	}		
	
}

/*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <brief>
 * @note     : <ҡ�˵Ŀռ�Ļ�������100X100
				ҡ�˾���1��20X20��Բ���>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void Show_Rocker(u16 fc,u16 bc)
{
	u8 i=0,j=0;
	u8 x=0,y=0;
	x=(u8)(X_AXIS_Joystick.Analog_quantity*80/4095);
	y=(u8)(Y_AXIS_Joystick.Analog_quantity*80/4095);
	
	LCD_Address_Set(0,72,100-1,172-1);  //���ù��λ��
	for(i=0;i<100;i++)
	{
		for(j=0;j<100;j++)
		{
			if((j>=(x-10))&&(j<=(x+10))&&(i>=(y-10))&&(i<=(y+10)))
			{
				LCD_Send_Word_Data(fc);
			}
			else
			{
				LCD_Send_Word_Data(bc);
			}
			
		}
	}
}
/*********************************************************************
 * @fn       : <LCD_Show_Virtual_Key/������>
 * @brief    : <��ʾһ�����ⰴ��>
 * @note     : <����������ʾһ�������Ŀ�Ȼ������������ʾ��ĸ>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void LCD_Show_Virtual_Key(u16 x,u16 y,u8 num,u16 fc,u16 bc,u16 shadow,u8 sizey,u8 mode)
{
	
//	u16 i=0,j=0;
	
	//1-����ʾģ��
	
//	LCD_Address_Set(x,y,x+sizey+4-1,y+sizey+4-1);  //����ǰ���λ�� 
//	
//	for(i=0;i<(sizey+4-1);i++)
//	{
//		for (j=0;j<(sizey+4-1);j++)
//		{
//			if((i>=0)&&(i<sizey-1)&&(j>=0)&&(j<sizey-1))
//			{
//				LCD_Send_Word_Data(bc);
//			}
//			else if((i>=sizey-1)&&(i<sizey+4-1)&&(j>=0)&&(j<i-sizey+1))
//			{
//				LCD_Send_Word_Data(bc);
//			}
//			else if((i>=0)&&(i<4-1)&&(j>=i+sizey-1)&&(j<sizey-1+4))
//			{
//				LCD_Send_Word_Data(bc);
//			}
//			else
//			{
//				LCD_Send_Word_Data(shadow);
//			}
//		}

//	}
//	LCD_ShowChar(x+sizey/4,y,num,fc,bc,sizey,mode);
}

/*********************************************************************
 * @fn       : <function_name/������>
 * @brief    : <brief>
 * @note     : <������̣�һ������24X24 �ɣ��м�ճ���
				�������ּ�10����Ȼ����3�еİ���>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void Virtual_Keyboard(u8 flg,u8 row,u8 cllow)
{
//	u8 i=0,j=0;
//	if(flg)
//	{
//		//��һ�ţ�
//		for(j=0;j<1;j++)
//		{
////			LCD_ShowChar(12+j*26,30,*(Virtual_Keyboard_1_row+j),BLACK,LGRAYBLUE,24,0);
////			LCD_Show_Virtual_Key(12+j*26,30,*(Virtual_Keyboard_1_row+j),BLACK,GRAY,24,0);
//			LCD_Show_Virtual_Key(12+j*28,30,*(Virtual_Keyboard_1_row+j),BLACK,LIGHTGREEN,BLACK,24,1);
//		}

		
//		//�ڶ���
//		for(j=0;j<10;j++)
//		{
//			LCD_ShowChar(24+j*26,56,*(Virtual_Keyboard_2_row+j),BLACK,LGRAYBLUE,24,0);
//		}
//		//������
//		for(j=0;j<10;j++)
//		{
//			LCD_ShowChar(36+j*26,82,*(Virtual_Keyboard_3_row+j),BLACK,LGRAYBLUE,24,0);
//		}
//		//��4��
//		for(j=0;j<10;j++)
//		{
//			LCD_ShowChar(48+j*26,110,*(Virtual_Keyboard_4_row+j),BLACK,LGRAYBLUE,24,0);
////		}

//	}
}
	
