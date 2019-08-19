#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
extern u8 Res;
int main(void)
 {		
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	usart_init();        
 	while(1)
	{
		
		   printf("%s ",SYN_Receive_Char(USART1));
	}
}
 
