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
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStrue; 
  USART_InitTypeDef USART_InitStrue;	
	NVIC_InitTypeDef NVIC_InitStrue; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//USART1_TX   GPIOA.9
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStrue);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrue);//初始化GPIOA.10  
//USART 初始化设置
	USART_InitStrue.USART_BaudRate = 115200;//串口波特率
	USART_InitStrue.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStrue.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStrue.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStrue.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
	USART_Init(USART1, &USART_InitStrue); //初始化串口1
  USART_Cmd(USART1, ENABLE); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断

	//优先级的配置
  NVIC_InitStrue.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1   ;//抢占优先级3
	NVIC_InitStrue.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStrue);	//根据指定的参数初始化VIC寄存器
}
//中断服务函数  
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
//以下代码支持printf函数而不需要选择usb microlib  
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
//重新定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif

//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//		{
//		Res =USART_ReceiveData(USART1);	//读取接收到的数据
//		
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//} 






