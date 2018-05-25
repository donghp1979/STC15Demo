#ifndef __INCLUDE_STC_CONFIG_H_
#define __INCLUDE_STC_CONFIG_H_

typedef unsigned char uchar;
typedef unsigned int uint;

/* ����ʹ�õ�ģ�� */
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


/* ����ʱ��Ƶ�� */
#define FOSC 11059200L

#include <intrins.h>

/* ����STC�ͺ�����ͷ�ļ� */
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

/************* ģ�鶨��� ***************/
// ����ӳ�ģ��
#ifdef USE_MOD_DELAY
	// �ӳ�1us
	#if defined(USE_MOD_DELAY1US_NOP)
		#define Delay1us() _nop_()
	#elif !defined(Delay1us)
		extern void _Delay1us();
		#define Delay1us() _Delay1us()
	#endif
	
	// �ӳ�6us
	#if defined(USE_MOD_DELAY6US)
		extern void Delay6us();
	#endif
	// �ӳ�10us
	#if defined(USE_MOD_DELAY10US)
		extern void Delay10us();
	#endif
	// �ӳ�100us
	#if defined(USE_MOD_DELAY100US)
		extern void Delay100us();
	#endif
	// �ӳ�1ms
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

// ��ʼ������
extern void System_Init();
extern void Timer0_Init();

#endif
