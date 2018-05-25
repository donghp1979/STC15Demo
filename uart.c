#include "uart.h"

bit Enable_DEBUG=1;

void UART_Init(void)		//115200bps@11.0592MHz
{
	COMx_InitDefine com1;
	
	com1.UART_Mode = UART_8bit_BRTx;
	com1.UART_BRT_Use = BRT_Timer1;
	com1.UART_BaudRate = 115200L;
	com1.Morecommunicate = ENABLE;
	com1.UART_RxEnable = ENABLE;
	com1.BaudRateDouble = DISABLE;
	com1.UART_Interrupt = ENABLE;
	com1.UART_Polity = PolityHigh;
	com1.UART_P_SW = UART1_SW_P30_P31;
	com1.UART_RXD_TXD_Short = DISABLE;
	
	USART_Configuration(USART1, &com1);
}
