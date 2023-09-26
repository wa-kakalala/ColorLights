#include "nrf24g.h"

/**
* PA1-> NRF IRQ
*/
void NRF_IRQ_Init(){
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);           // AFIO Enable
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_1;	   //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //��������  
	GPIO_Init(GPIOA,&GPIO_InitStructure);		     /* ��ʼ��GPIO */
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);   //ѡ��GPIO�ܽ������ⲿ�ж���·
	
	//EXTI0 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void NRF24G_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI2_Init();
	NRF_IRQ_Init();
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); // set SPI2 speed 9M, nrf24l01->max 10M
	
  RCC_APB2PeriphClockCmd( NRF_CE_RCC, ENABLE);        // enable led port clk
  GPIO_InitStructure.GPIO_Pin = NRF_CE_PIN;	          // select pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // set output mode : push pull mode      
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(NRF_CE_PORT, &GPIO_InitStructure);        // init led 
	
	RCC_APB2PeriphClockCmd( NRF_CS_RCC, ENABLE);        // enable led port clk
  GPIO_InitStructure.GPIO_Pin = NRF_CS_PIN;	          // select pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // set output mode : push pull mode      
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(NRF_CS_PORT, &GPIO_InitStructure);        // init led 
	// set CE to 0
	NRF_CE_LOW();
	
	// set CS to 1
	NRF_CS_HIGH();
}

// Check NRF24G's Status
// 0 --> ok
// 1 --> error
u8 NRF24G_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i = 0;
	NRF_CS_LOW();	 
	SPI2_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	SPI2_Write_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	NRF_CS_HIGH();
	for(i=0;i<5;i++) if(buf[i]!=0XA5) return 1;	 							   
	return 0;		 
}	


const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};

/**
* RX Mode
* 0x0f: 0000 1111 
* 0x3f: 0011 1111 RX Intruption only
*/
void NRF24L01_RX_Mode()
{
	NRF_CE_LOW();  
	NRF_CS_LOW();	
  SPI2_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
  SPI2_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  SPI2_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  SPI2_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  SPI2_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  SPI2_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��   
  SPI2_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  NRF_CS_HIGH();
	NRF_CE_HIGH();
}	

/**
* TX Mode with no auto resend
* 0x0e: 0000 1110
* 0x5e: 0101 1110  TX Intruption only
*/
void NRF24L01_TX_Mode()
{														 
	NRF_CE_LOW();  
	NRF_CS_LOW();	    
  SPI2_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
  SPI2_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  SPI2_Write_Reg(NRF_WRITE_REG+EN_AA,0x00);     // disable 
  SPI2_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  SPI2_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x00);// disable
  SPI2_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  SPI2_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  SPI2_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF_CS_HIGH();
	NRF_CE_HIGH();
}


void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)==1)
	{
		#ifdef DEBUG_MODE
		printf("EXTI 1 is get...\r\n");
		#endif
		
		
		
		
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}




