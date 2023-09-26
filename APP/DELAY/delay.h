#ifndef _DELAY_H_
#define _DELAY_H_
#include "system.h"
#include "misc.h"

void SysTick_Init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_n111ns(u32 n111ns);

#endif

