#include "ws2812.h"

// area operator
COLOR_TYPE ws2812_map[ WS2812_NUM ];
u8 ws2812_change_max_index = 0;

/**
* WS2812 Init
* PWM Function -- PA7 TIM3_CH2 
*/
void WS2812_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure; 

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);// enable GPIOA clk
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE) ;// enable TIM3 clk
	
	// port multiplex
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	// set period 800k
	TIM_TimeBaseStructure.TIM_Period = 89;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseStructure);
	
	// pwm config
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity =  TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM3 , &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3 , TIM_OCPreload_Enable);    
}












void WS2812_AllinOneColor(COLOR_TYPE * color){
	u8 i;
	if( !color ) return ;
	for( i =0 ;i<WS2812_NUM;i++) {
		(ws2812_map + i) -> B = color->B;
		(ws2812_map + i) -> G = color->G;
		(ws2812_map + i) -> R = color->R;
	}
	ws2812_change_max_index = WS2812_NUM -1;
}

// x from 0 to WS2812_WIDTH  -  1
// y from 0 to WS2812_HEIGHT - 1
void WS2812_SetUnitColor(COLOR_TYPE * color,u8 x, u8 y ) {
	u8 change_index;
	if( x >= WS2812_WIDTH  ||
   		y >= WS2812_HEIGHT ||
			!color 
    ) return;
	
	(ws2812_map + y * WS2812_WIDTH + x ) -> B = color->B;
	(ws2812_map + y * WS2812_WIDTH + x ) -> G = color->G;
	(ws2812_map + y * WS2812_WIDTH + x ) -> R = color->R;
	
	 change_index  = y * WS2812_WIDTH + x;
	 if( change_index > ws2812_change_max_index )  ws2812_change_max_index = change_index;
}














