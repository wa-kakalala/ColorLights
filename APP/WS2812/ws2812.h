#ifndef _WS2812_H_
#define _WS2812_H_
#include "system.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#define WS2812_ZERO  21
#define WS2812_ONE   62

typedef struct COLOR_TYPE{
	u8 R;
	u8 G;
	u8 B;
}COLOR_TYPE;

// user operator
#define WS2812_WIDTH   6
#define WS2812_HEIGHT  1
#define WS2812_NUM     (WS2812_WIDTH * WS2812_HEIGHT)

// just the function of "WS2812_Update" can show the color actually
void WS2812_Init(void);


#endif

