#include "sonar_rx.h"

// SONAR RX Init 
// PB0 and PC14 
void SONAR_RX_Init(){
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);           // AFIO Enable
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0;	   //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;  //��������  
	GPIO_Init(GPIOB,&GPIO_InitStructure);		     /* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;  //��������  
	GPIO_Init(GPIOC,&GPIO_InitStructure);		     /* ��ʼ��GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);   //ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14);  //ѡ��GPIO�ܽ������ⲿ�ж���·
	
	//EXTI0 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC
	
	//EXTI14 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   //EXTI15-10�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		   //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                       //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line14; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}


void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)
	{
		#ifdef DEBUG_MODE
		printf("EXTI 0 is get...\r\n");
		#endif
		
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14)==1)
	{
		#ifdef DEBUG_MODE
		printf("EXTI 14 is get...\r\n");
		#endif
	}
	EXTI_ClearITPendingBit(EXTI_Line14);
}



