#include "usart3.h"

/*******************************************************************************
* usart3 --> debug
* PB10 -> USART3_TX
* PB11 -> USART3_RX
*******************************************************************************/ 
void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX		PB10
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	   
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX			PB11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//USART3
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_Cmd(USART3, ENABLE); 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//Usart3 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
}

/*******************************************************************************
* USART3_IRQHandler
*******************************************************************************/ 
void USART3_IRQHandler(void)                	
{
	//u8 r;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		//r =USART_ReceiveData(USART3);//(USART1->DR)

	} 
}

int fputc(int ch,FILE *p)  
{
	USART_SendData(USART3,(u8)ch);	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	return ch;
}

