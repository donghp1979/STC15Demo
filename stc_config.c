#include "stc_config.h"

#ifdef USE_MOD_DELAY

#if !defined(USE_MOD_DELAY1US_NOP)
void Delay1us()		//@11.0592MHz
{
	_nop_();
	_nop_();
	_nop_();
}

#endif

// 延迟6us
#if defined(USE_MOD_DELAY6US)
void Delay6us()		//@11.0592MHz
{
	unsigned char i;

	i = 14;
	while (--i);
}



#endif
// 延迟10us
#if defined(USE_MOD_DELAY10US)
void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

#endif
// 延迟100us
#if defined(USE_MOD_DELAY100US)
void Delay100us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}


#endif
// 延迟1ms
#ifdef USE_MOD_DELAY1MS
void Delay1ms(unsigned int y)
{
	unsigned char i, j;
	while(y--) {
		_nop_();
		_nop_();
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}
#endif

void System_Init() {
#if defined(USE_STC15)||defined(USE_STC8)
	P0M0=0;
	P0M1=0;
	P1M0=0;
	P1M1=0;
	P2M0=0;
	P2M1=0;
	P3M0=0;
	P3M1=0;

	// 设置定时器0
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	//
	ET0 = 1;    //启用定时器0中断
#endif
}

#endif
