#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
extern u8 Res=0;
extern u8 BUF[40];
 
void usart_init(void)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStrue; 
  USART_InitTypeDef USART_InitStrue;	
	NVIC_InitTypeDef NVIC_InitStrue; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//USART1_TX   GPIOA.9
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStrue);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrue);//��ʼ��GPIOA.10  
//USART ��ʼ������
	USART_InitStrue.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStrue.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStrue.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStrue.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStrue.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(USART1, &USART_InitStrue); //��ʼ������1
  USART_Cmd(USART1, ENABLE); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�

	//���ȼ�������
  NVIC_InitStrue.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1   ;//��ռ���ȼ�3
	NVIC_InitStrue.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStrue);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
//�жϷ�����  
void USART1_IRQHandler(void) 
{
	u8 Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		Res=USART_ReceiveData(USART1);	
		USART_SendData(USART1,Res); 
	}

}

unsigned char SYN_Receive_Char(USART_TypeDef* USARTx)    //????
{
  unsigned char Dat;
	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)!=SET);
  Dat=USART_ReceiveData(USARTx);	//????????
  return Dat;
}
//���´���֧��printf����������Ҫѡ��usb microlib  
#if 1
#pragma import(__use_no_semihosting)             
               
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
  
_sys_exit(int x) 
{ 
	x = x; 
} 
//���¶���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif

//void USART1_IRQHandler(void)                	//����1�жϷ������
//	{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//		{
//		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
//		
//		if((USART_RX_STA&0x8000)==0)//����δ���
//			{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//				}
//			else //��û�յ�0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//					}		 
//				}
//			}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 






