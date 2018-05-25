#ifndef __INCLUDE_UART_H_
#define __INCLUDE_UART_H_

#include <stdio.h>

extern void UART_Init();
extern bit Enable_DEBUG;
//#define DEBUG(x) 
#ifndef DEBUG
	#define DEBUG(x) if(Enable_DEBUG) { \
		printf x; \
	}
	#endif

#endif

