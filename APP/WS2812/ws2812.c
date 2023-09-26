#include "ws2812.h"


/**
* WS2812 Init
*/
void WS2812_Init(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( WS2812_RCC, ENABLE);        // enable led port clk
  GPIO_InitStructure.GPIO_Pin = WS2812_PIN;	         // select pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // set output mode : push pull mode      
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(WS2812_PORT, &GPIO_InitStructure);        // init led 
}

void WS2812_Send1Value(){
	WS2812_on();
	delay_n111ns(TIMES_T1H_111NS);
	WS2812_off();
	delay_n111ns(TIMES_T1L_111NS);
}

void WS2812_Send0Value(){
	WS2812_on();
	delay_n111ns(TIMES_T0H_111NS);
	WS2812_off();
	delay_n111ns(TIMES_T0L_111NS);
}

void WS2812_SendReset(){
	WS2812_off();
	delay_us(TIMES_RESET_US);
}


void WS2812_SendOneUnit(COLOR_TYPE * color){
	u8 i ;
	for( i =0;i<8;i++) {
			if( ( (color->G ) >> (7-i)) & 0x01 ) {
					WS2812_Send1Value();
			}else{
					WS2812_Send0Value();
			}
	}
	
	for( i =0;i<8;i++) {
			if( ( (color->R ) >> (7-i)) & 0x01 ) {
					WS2812_Send1Value();
			}else{
					WS2812_Send0Value();
			}
	}
	
	for( i =0;i<8;i++) {
			if( ( (color->B ) >> (7-i)) & 0x01 ) {
					WS2812_Send1Value();
			}else{
					WS2812_Send0Value();
			}
	}
}

void WS2812_SendGroup(COLOR_TYPE * color, u8 len){
	u8 i ;
	if( !color ) return ;
	for( i=0;i<len;i++) {
		WS2812_SendOneUnit(color+ i);
	}
	
	WS2812_SendReset();
}


// area operator
COLOR_TYPE ws2812_map[ WS2812_NUM ];
u8 ws2812_change_max_index = 0;

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

void WS2812_Update(){
	WS2812_SendGroup(ws2812_map,ws2812_change_max_index + 1);
	ws2812_change_max_index = 0;
	
}








