#ifndef  __REMOTE_H
#define	 __REMOTE_H
#include <stm32f10x.h>




void Remote_Receive_Init(void);
void TIM2_IRQHandler(void);
uint8_t Remote_Scan(void);
void Remote_Key_Deal(uint8_t VAL);
#endif

