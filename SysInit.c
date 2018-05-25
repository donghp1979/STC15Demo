#include "SysInit.h"

#include "STC15/GPIO.h"
#include "STC15/USART.h"
#include "STC15/timer.h"

void GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitDef;
	
	GPIO_InitDef.Mode = GPIO_PullUp;
	GPIO_InitDef.Pin = GPIO_Pin_All;
	
	GPIO_Inilize(GPIO_P0, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P1, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P2, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P3, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P4, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P5, &GPIO_InitDef);
}

void Timer0_Init(unsigned int ms) {
	TIM_InitTypeDef TIM_Def;
	
	TIM_Def.TIM_Mode = TIM_16BitAutoReload;
	TIM_Def.TIM_Polity = PolityLow;
	TIM_Def.TIM_Interrupt = ENABLE;
	TIM_Def.TIM_ClkSource = TIM_CLOCK_1T;
	TIM_Def.TIM_ClkOut = DISABLE;
	TIM_Def.TIM_Value = 65536-(MAIN_Fosc/(1000/ms));; // 2∫¡√Î@11.0592MHz 1T
	TIM_Def.TIM_Run = ENABLE;
	
	Timer_Inilize(Timer0, &TIM_Def);
}

void UART_Init()		//115200bps@11.0592MHz
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
