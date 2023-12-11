#include "ws2812.h"

// area operator
WS2812_TYPE ws2812_map[ WS2812_NUM * 3 + 1];
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
	DMA_InitTypeDef	DMA_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);// enable GPIOA clk
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE) ;// enable TIM3 clk
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE) ;// enable DMA1
	
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

	// dma config
	DMA_DeInit(&DMA1_Channel1_BASE);
	DMA_InitStructure.DMA_PeripheralBaseAddr = 1 ;     // DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = 2     ;     // DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // 数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize =    // DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc =  // 外设地址存储器
	DMA_InitStructure.DMA_MemoryInc =      // 内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize // 外设数据位宽
	DMA_InitStructure.DMA_MemoryDataSize     // 内存数据位宽
	DMA_InitStructure.DMA_Mode =             // 工作模式
	DMA_InitStructure.DMA_Priority =         // 优先级
	DMA_InitStructure.DMA_M2M 	             // 是否为内存到内存模式
	DMA_Init(&DMA1_Channel2_BASE,&DMA_InitStructure);
	
	// reset signal
	ws2812_map[WS2812_NUM * 3] = 0x0000;
}

void WS2812_Update(){
	DMA_Cmd();
	DMA_SetCurrDataCounter();
	DMA_Cmd();
}

void WS2812_AllinOneColor(COLOR_TYPE * color){
	unsigned int i;
	if( !color ) return ;
	for( i =0 ;i< WS2812_NUM*3;i+=3) {
		*(ws2812_map + i)   = color->B;
		*(ws2812_map + i+1) = color->G;
		*(ws2812_map + i+2) = color->R;
	}
}

// x from 0 to WS2812_WIDTH  - 1
// y from 0 to WS2812_HEIGHT - 1
char WS2812_SetUnitColor(COLOR_TYPE * color,u8 x, u8 y ) {
	u8 change_index;
	if( x >= WS2812_WIDTH  ||
   		y >= WS2812_HEIGHT ||
	    !color 
    ) return 1;
	if( x % 2 == 0) {
		*(ws2812_map + (y * WS2812_WIDTH + x)*3     ) = color->B;
		*(ws2812_map + (y * WS2812_WIDTH + x)*3 + 1 ) = color->G;
		*(ws2812_map + (y * WS2812_WIDTH + x)*3 + 2 ) = color->R;
	}else{
		*(ws2812_map + (y * WS2812_WIDTH + WS2812_WIDTH - x - 1 )*3     ) = color->B;
		*(ws2812_map + (y * WS2812_WIDTH + WS2812_WIDTH - x - 1 )*3 + 1 ) = color->G;
		*(ws2812_map + (y * WS2812_WIDTH + WS2812_WIDTH - x - 1 )*3 + 2 ) = color->R;
	}
	return 0;
}














