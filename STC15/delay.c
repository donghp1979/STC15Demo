
/*---------------------------------------------------------------------*/
/* --- STC MCU International Limited ----------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ���˺꾧�Ƽ������ϼ�����   */
/*---------------------------------------------------------------------*/


/*************	����˵��	**************

���ļ�ΪSTC15xxxϵ�е���ʱ����,�û��������Բ��޸��������.

******************************************/

#include	"delay.h"

//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
	unsigned int i;
	do {
		i = MAIN_Fosc / 13000;
		while(--i);   //14T per loop
	} while(--ms);
}

// 1us��ȷ��ʱ����
#if MAIN_Fosc==11059200L
void Delay1us()		//@11.0592MHz
{
	_nop_();
	_nop_();
	_nop_();
}
#elif MAIN_Fosc==12000000L
void Delay1us()		//@12.000MHz
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
#elif MAIN_Fosc==22118400L
void Delay1us()		//@22.1184MHz
{
	unsigned char i;

	i = 3;
	while (--i);
}
#elif MAIN_Fosc==24000000L
void Delay1us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}
#else
	#error "Undefined Delay1us"
#endif


// 5us��ȷ��ʱ����
#if MAIN_Fosc==11059200L
void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 11;
	while (--i);
}

#elif MAIN_Fosc==12000000L
void Delay5us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 12;
	while (--i);
}

#elif MAIN_Fosc==22118400L
void Delay5us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

#elif MAIN_Fosc==24000000L
void Delay5us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}
#else
	#error "Undefined Delay1us"
#endif

// 10us��ȷ��ʱ����
#if MAIN_Fosc==11059200L
void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}


#elif MAIN_Fosc==12000000L
void Delay10us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}

#elif MAIN_Fosc==22118400L
void Delay10us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 52;
	while (--i);
}


#elif MAIN_Fosc==24000000L
void Delay10us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 57;
	while (--i);
}

#else
	#error "Undefined Delay1us"
#endif


// 100us��ȷ��ʱ����
#if MAIN_Fosc==11059200L
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
#elif MAIN_Fosc==12000000L
void Delay100us()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 39;
	do
	{
		while (--j);
	} while (--i);
}
#elif MAIN_Fosc==22118400L
void Delay100us()		//@22.1184MHz
{
	unsigned char i, j;

	i = 3;
	j = 35;
	do
	{
		while (--j);
	} while (--i);
}
#elif MAIN_Fosc==24000000L
void Delay100us()		//@24.000MHz
{
	unsigned char i, j;

	i = 3;
	j = 82;
	do
	{
		while (--j);
	} while (--i);
}
#else
	#error "Undefined Delay1us"
#endif
