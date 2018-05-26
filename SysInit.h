#ifndef __INCLUDE_STC_CONFIG_H_
#define __INCLUDE_STC_CONFIG_H_

#define CLR_EA() _nop_();EA=0;_nop_()
#define SETB_EA() _nop_();EA=1;_nop_()

// ³õÊ¼»¯º¯Êý
extern void GPIO_Init();
extern void Timer0_Init(unsigned int ms);
extern void UART_Init();

#endif
