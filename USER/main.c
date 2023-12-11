/**************************************
* filename    :  main.c
* author      :  wkk
* update time :  2023/8/21
*********************************************************/	
#include "system.h"
#include "led.h"
#include "delay.h"
#include "usart3.h"
#include "ws2812.h"


void SYS_Init(void){
	SystemInit();  // config sys clk to 72M 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	SysTick_Init(72);
	LED_Init();    // Led Port Init
	USART3_Init(115200);
	WS2812_Init();
}

int main(void)
{
	u32 led_counter = 0;
	COLOR_TYPE color[6] = {{255,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
	SYS_Init();


	//WS2812_Update();
	while (1)
	{

		
		
		
		if( led_counter == 720000 ) {
			 LED_on();
		}else if( led_counter == 600000000 ) {
			 LED_off();
			 led_counter =0;
		}else {
		}
		led_counter ++;
	}
}




