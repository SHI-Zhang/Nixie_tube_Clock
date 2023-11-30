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
 #include "GPIO.h"
 #include "SPI.h"
 #include "LCD.h"
 #include "delay.h"
 #include "parameter.h"

 /*-----------------------------  #INCLUDE   END   ----------------------------*/
 
 /*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/

 /*-------------------------  DECLARE  VARIABLE  END   ------------------------*/
 
 
 /*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/

 /*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/



#ifdef _1_47_IPS_LCD




 void LCD_Send_Cmd(u8 order)
 {
	CS_L;
	CMD_ORDER;
    SPI1_SEND_Byte(order);
    CS_H;
 }
 
 void LCD_Send_Data(u8 data)
 {
	CS_L;
	DATA_ORDER;
    SPI1_SEND_Byte(data);
    CS_H;
 }
 
  void LCD_Send_Word_Data(u16 data)
 {
	CS_L;
	DATA_ORDER;
	//2023年11月2日  在LCD屏这里，两字节的数据，应该是连用两次单字节的数据发送函数。
	//否则会出现问题，但问题不明
	//SPI1_SEND_Word(data);
	 LCD_Send_Data(data>>8);
	 LCD_Send_Data(data);
    CS_H;
 }
 

void LCD_Init(void)
{
	RES_L;//LCD_RES_Clr();//复位
	delay_ms(30);
	RES_H;//LCD_RES_Set();
	delay_ms(100);
	delay_ms(100);
	LCD_Send_Cmd(0x11);  
	LCD_Send_Cmd(0x36); 
	
	if(USE_HORIZONTAL==0)LCD_Send_Data(0x00);
	else if(USE_HORIZONTAL==1)LCD_Send_Data(0xC0);
	else if(USE_HORIZONTAL==2)LCD_Send_Data(0x70);
	else LCD_Send_Data(0xA0);
	
	LCD_Send_Cmd(0x3A);
	LCD_Send_Data(0x05);
	LCD_Send_Cmd(0xB2);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x0C);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x33);
	LCD_Send_Data(0x33); 
	LCD_Send_Cmd(0xB7); 
	LCD_Send_Data(0x35);  
	LCD_Send_Cmd(0xBB);
	LCD_Send_Data(0x35);
	LCD_Send_Cmd(0xC0);
	LCD_Send_Data(0x2C);
	LCD_Send_Cmd(0xC2);
	LCD_Send_Data(0x01);
	LCD_Send_Cmd(0xC3);
	LCD_Send_Data(0x13);   
	LCD_Send_Cmd(0xC4);
	LCD_Send_Data(0x20);  
	LCD_Send_Cmd(0xC6); 
	LCD_Send_Data(0x0F);    
	LCD_Send_Cmd(0xD0); 
	LCD_Send_Data(0xA4);
	LCD_Send_Data(0xA1);
	LCD_Send_Cmd(0xD6); 
	LCD_Send_Data(0xA1);
	LCD_Send_Cmd(0xE0);
	LCD_Send_Data(0xF0);
	LCD_Send_Data(0x00);
	LCD_Send_Data(0x04);
	LCD_Send_Data(0x04);
	LCD_Send_Data(0x04);
	LCD_Send_Data(0x05);
	LCD_Send_Data(0x29);
	LCD_Send_Data(0x33);
	LCD_Send_Data(0x3E);
	LCD_Send_Data(0x38);
	LCD_Send_Data(0x12);
	LCD_Send_Data(0x12);
	LCD_Send_Data(0x28);
	LCD_Send_Data(0x30);
	LCD_Send_Cmd(0xE1);
	LCD_Send_Data(0xF0);
	LCD_Send_Data(0x07);
	LCD_Send_Data(0x0A);
	LCD_Send_Data(0x0D);
	LCD_Send_Data(0x0B);
	LCD_Send_Data(0x07);
	LCD_Send_Data(0x28);
	LCD_Send_Data(0x33);
	LCD_Send_Data(0x3E);
	LCD_Send_Data(0x36);
	LCD_Send_Data(0x14);
	LCD_Send_Data(0x14);
	LCD_Send_Data(0x29);
	LCD_Send_Data(0x32);
	LCD_Send_Cmd(0x21); 
	LCD_Send_Cmd(0x11);
	delay_ms(120);	
	LCD_Send_Cmd(0x29); 
	delay_ms(10);
	LCD_Fill(0,0,320,172,LGRAY);
}


 
/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_Send_Cmd(0x2a);//列地址设置
		LCD_Send_Word_Data(x1+34);
		LCD_Send_Word_Data(x2+34);

		LCD_Send_Cmd(0x2b);//行地址设置
		LCD_Send_Word_Data(y1);
		LCD_Send_Word_Data(y2);

		LCD_Send_Cmd(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_Send_Cmd(0x2a);//列地址设置
		LCD_Send_Word_Data(x1+34);
		LCD_Send_Word_Data(x2+34);
		LCD_Send_Cmd(0x2b);//行地址设置
		LCD_Send_Word_Data(y1);
		LCD_Send_Word_Data(y2);
		LCD_Send_Cmd(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_Send_Cmd(0x2a);//列地址设置
		LCD_Send_Word_Data(x1);
		LCD_Send_Word_Data(x2);
		LCD_Send_Cmd(0x2b);//行地址设置
		LCD_Send_Word_Data(y1+34);
		LCD_Send_Word_Data(y2+34);
		LCD_Send_Cmd(0x2c);//储存器写
	}
	else
	{
		LCD_Send_Cmd(0x2a);//列地址设置
		LCD_Send_Word_Data(x1);
		LCD_Send_Word_Data(x2);
		LCD_Send_Cmd(0x2b);//行地址设置
		LCD_Send_Word_Data(y1+34);
		LCD_Send_Word_Data(y2+34);
		LCD_Send_Cmd(0x2c);//储存器写
	}
}

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend);//设置显示范围
	for(i=ysta;i<yend+1;i++)		//lvgl 中已经将xend和yend进行-1了，所以这里不需要剪1，但是这个循环要么加1，要么就是<=
	{													   	 	
		for(j=xsta;j<xend+1;j++)
		{
			LCD_Send_Word_Data(color);
		}
	} 					  	     
}


/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <专用于lvgl的画点函数，经过一夜的失眠，最终好像想出来了，
								LCD_Fill 这个函数是全部填充一种颜色，所以我的代码会出现只有框，没有内容>
 * @param    : <param>
 * @return   : <return>
 * @author   : <SHI长>
 * @date     : 
 ********************************************************************/
 void LCD_Fill_Lvgl(u16 xsta,u16 ysta,u16 xend,u16 yend,u16* color)
 {
		u16 i,j; 
	u16* color_p=color;
		LCD_Address_Set(xsta,ysta,xend,yend);//设置显示范围
		for(i=ysta;i<yend+1;i++)		//lvgl 中已经将xend和yend进行-1了，所以这里不需要剪1，但是这个循环要么加1，要么就是<=
		{													   	 	
			for(j=xsta;j<xend+1;j++)
			{
				LCD_Send_Word_Data(*color_p++);		//可能输入的color仅仅是颜色数组的开始。
			}
		} 					  	    
 }

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_Send_Word_Data(color);
} 




/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//一个字符所占字节大小
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)
			temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)
			temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)
			temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)
			temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))
				{
					LCD_Send_Word_Data(fc);
				}
				else 
				{
					LCD_Send_Word_Data(bc);
				}
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))
				{
					LCD_DrawPoint(x,y,fc);//画一个点
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
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

void LCD_Show_ICON(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 size,u8 mode)
{
	u32 temp=0;
	u16 i,j;//一个字符所占字节大小

	LCD_Address_Set(x,y,x+size-1,y+size-1);  //设置光标位置 
	for(i=0;i<size*(size/8);i++)
	{ 
		temp=*(p+i);		 //调用16x32字体
		for(j=0;j<8;j++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<j))
				{
					LCD_Send_Word_Data(fc);
				}
				else 
				{
					LCD_Send_Word_Data(bc);
				}

			}
			else//叠加模式
			{
				if(temp&(0x01<<j))
				{
					LCD_DrawPoint(x,y,fc);//画一个点
				}
			}
		}
	}   	 	  
}


#endif  /*_1_47_IPS_LCD*/






