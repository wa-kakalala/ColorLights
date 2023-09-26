#ifndef _WS2812_H_
#define _WS2812_H_
#include "system.h"
#include "delay.h"

#define  WS2812_PORT   	GPIOB
#define  WS2812_PIN    	GPIO_Pin_5
#define  WS2812_RCC    	RCC_APB2Periph_GPIOC
#define  WS2812_PIN_NUM	5


#define  WS2812_on()   (PCout(WS2812_PIN_NUM) = 1 )   // ws2812 on   attention !!! here use WS2812_PIN_NUM not WS2812_PIN
#define  WS2812_off()  (PCout(WS2812_PIN_NUM) = 0 )   // ws2812 off

// base time: 111ns
#define TIMES_T0H_111NS  3
#define TIMES_T0L_111NS  6
#define TIMES_T1H_111NS  6
#define TIMES_T1L_111NS  6

// base time: 1us
#define TIMES_RESET_US   300

typedef struct COLOR_TYPE{
	u8 R;
	u8 G;
	u8 B;
}COLOR_TYPE;

// user operator
#define WS2812_WIDTH   6
#define WS2812_HEIGHT  6
#define WS2812_NUM     (WS2812_WIDTH * WS2812_HEIGHT)

// just the function of "WS2812_Update" can show the color actually
void WS2812_AllinOneColor(COLOR_TYPE * color);
void WS2812_SetUnitColor(COLOR_TYPE * color,u8 x, u8 y ) ;
void WS2812_Update(void);

#endif

