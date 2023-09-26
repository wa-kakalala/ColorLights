/**************************************
* filename    :  main.c
* author      :  wkk
* update time :  2023/8/21
*********************************************************/	
#include "led.h"
#include "delay.h"
#include "usart3.h"
#include "system.h"

void SYS_Init(void){
	SystemInit();	    // config sys clk to 72M 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	SysTick_Init(72);
	LED_Init();       // Led Port Init
	USART3_Init(115200);

}

int main(void)
{
	u32 led_counter = 0;
	while (1)
	{

		if( led_counter == 300000 ) {
			 LED_on();
		}else if( led_counter == 600000 ) {
			 LED_off();
			 led_counter =0;
		}else {
		}
		led_counter ++;

	}
}




