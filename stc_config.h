#ifndef __INCLUDE_STC_CONFIG_H_
#define __INCLUDE_STC_CONFIG_H_

typedef unsigned char uchar;
typedef unsigned int uint;

#include  "config.h"
#include  "delay.h"
#include	"STC15Fxxxx.H"

/************* 模块定义宏 ***************/

#if defined(USE_STC8)||defined(USE_STC15)
	#define CLR_EA() _nop_();EA=0;_nop_()
	#define SETB_EA() _nop_();EA=1;_nop_()
#else
	#define CLR_EA() EA=0
	#define SETB_EA() EA-1
#endif

// 初始化函数
extern void GPIO_Init();
extern void Timer0_Init();

#endif
