#ifndef __LED_H
#define __LED_H

 #include <gd32f30x.h>
 #include"GPIO.h"





void Led_Control(u8 mode);
void TIM4_PWM_Init(void);
#endif

