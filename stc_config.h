#ifndef __INCLUDE_STC_CONFIG_H_
#define __INCLUDE_STC_CONFIG_H_

typedef unsigned char uchar;
typedef unsigned int uint;

/* 定义使用的模块 */
//#define USE_STC8
#define USE_STC15
//#define USE_STC89C5xRC
//#define USE_STC90C5xAD
//#define USE_STC12C5A60S2
//#define USE_STC12C2052AD
//#define USE_STC12C2052AD
//#define USE_8052

#define USE_MOD_DELAY
	//#define USE_MOD_DELAY1US_NOP
	#define USE_MOD_DELAY6US
	#define USE_MOD_DELAY10US
	#define USE_MOD_DELAY100US
	#define USE_MOD_DELAY1MS


/* 定义时钟频率 */
#define FOSC 11059200L

#include <intrins.h>

/* 根据STC型号引入头文件 */
#if defined(USE_STC8)
	#include <STC8.H>
#elif defined(USE_STC15)
	#include <STC15.h>
#elif defined(USE_STC89C5xRC)
	#include <STC89C5xRC.H>
#elif defined(USE_STC90C5xAD)
	#include <STC90C5xAD.H>
#elif defined(USE_STC12C5A60S2)
	#include <STC90C5xAD.H>
#elif defined(USE_STC12C2052AD)
	#include <STC12C2052AD.H>
#elif defined(USE_8052)
  #include <reg52.h>
#else
	#if !defined(USE_8051)
		#define USE_8051 1
	#endif
	#if defined(USE_8051)
		#include <reg51.h>
	#endif
#endif

/************* 模块定义宏 ***************/
// 软件延迟模块
#ifdef USE_MOD_DELAY
	// 延迟1us
	#if defined(USE_MOD_DELAY1US_NOP)
		#define Delay1us() _nop_()
	#elif !defined(Delay1us)
		extern void _Delay1us();
		#define Delay1us() _Delay1us()
	#endif
	
	// 延迟6us
	#if defined(USE_MOD_DELAY6US)
		extern void Delay6us();
	#endif
	// 延迟10us
	#if defined(USE_MOD_DELAY10US)
		extern void Delay10us();
	#endif
	// 延迟100us
	#if defined(USE_MOD_DELAY100US)
		extern void Delay100us();
	#endif
	// 延迟1ms
	#if defined(USE_MOD_DELAY1MS)
		extern void Delay1ms(uint y);
	#endif
#endif

#if defined(USE_STC8)||defined(USE_STC15)
	#define CLR_EA() _nop_();EA=0;_nop_()
	#define SETB_EA() _nop_();EA=1;_nop_()
#else
	#define CLR_EA() EA=0
	#define SETB_EA() EA-1
#endif

// 初始化函数
extern void System_Init();
extern void Timer0_Init();

#endif
