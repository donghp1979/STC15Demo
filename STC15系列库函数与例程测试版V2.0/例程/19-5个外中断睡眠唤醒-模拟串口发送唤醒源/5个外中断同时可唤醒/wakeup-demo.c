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

��ʾ5������Դ��MCU���ѡ�
�Ӵ���������(ASCII)��9600,8,N,1.

******************************************/


/*************	�û�ϵͳ����	**************/

#define MAIN_Fosc		22118400L	//������ʱ��, ģ�⴮�ںͺ���ʱ���Զ���Ӧ��5~35MHZ



/*************	���º궨���û������޸�	**************/
#include	<reg52.H>
#include 	<intrins.h>

#define	uchar	unsigned char
#define uint	unsigned int

/******************************************/

sfr INT_CLKO = 0x8F;

//sbit INT2 = P3^4;	//Only for STC15F/L10xW
//sbit INT3 = P3^5;	//Only for STC15F/L10xW
sbit INT2 = P3^6;
sbit INT3 = P3^7;
sbit INT4 = P3^0;


sbit	P_TXD1 = P3^1;
void	Tx1Send(unsigned char dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);

uchar	WakeUpSource;
uchar	WakeUpCnt;


void main(void)
{
	P3 = 0xff;
	delay_ms(2);	//delay 2ms


	PrintString("*** STC15Fϵ��5������Դ���Գ��� ***\r\n");
	
	while(1)
	{
		while(!INT0);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT1);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT2);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT3);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT4);	//�ȴ����ж�Ϊ�ߵ�ƽ
		delay_ms(10);	//delay 10ms

		while(!INT0);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT1);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT2);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT3);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT4);	//�ȴ����ж�Ϊ�ߵ�ƽ
		delay_ms(10);	//delay 10ms


		IE1  = 0;	//���ж�1��־λ
		IE0  = 0;	//���ж�0��־λ
		INT_CLKO |= (1 << 6);	//INT4 Enable	
		INT_CLKO |= (1 << 5);	//INT3 Enable	
		INT_CLKO |= (1 << 4);	//INT2 Enable	
		EX1 = 1;				//INT1 Enable
		EX0 = 1;				//INT0 Enable

		IT0 = 1;		//INT0 �½����ж�		
		IT1 = 1;		//INT1 �½����ж�		
		EA  = 1;		//Enable all interrupt

		WakeUpSource = 0;

		PCON |= 0x02;	//Sleep
		_nop_();
		_nop_();
		_nop_();
		delay_ms(1);	//delay 1ms
		
		if(WakeUpSource == 1)	PrintString("���ж�INT0����  ");
		if(WakeUpSource == 2)	PrintString("���ж�INT1����  ");
		if(WakeUpSource == 3)	PrintString("���ж�INT2����  ");
		if(WakeUpSource == 4)	PrintString("���ж�INT3����  ");
		if(WakeUpSource == 5)	PrintString("���ж�INT4����  ");
		WakeUpSource = 0;
		
		WakeUpCnt++;
		Tx1Send(WakeUpCnt/100+'0');
		Tx1Send(WakeUpCnt%100/10+'0');
		Tx1Send(WakeUpCnt%10+'0');
		PrintString("�λ���\r\n");
	}

}


/********************* INT0�жϺ��� *************************/
void INT0_int (void) interrupt 0		//���ж�ʱ�Ѿ������־
{
	WakeUpSource = 1;	//���INT0����
	EX0 = 0;			//INT0 Disable
	IE0  = 0;			//���ж�0��־λ
}

/********************* INT1�жϺ��� *************************/
void INT1_int (void) interrupt 2		//���ж�ʱ�Ѿ������־
{
	WakeUpSource = 2;	//���INT1����
	EX1 = 0;			//INT1 Disable
	IE1  = 0;			//���ж�1��־λ
}

/********************* INT2�жϺ��� *************************/
void INT2_int (void) interrupt 10
{
	WakeUpSource = 3;		//���INT2����
	INT_CLKO &= ~(1 << 4);	//INT2 Disable	
}

/********************* INT3�жϺ��� *************************/
void INT3_int (void) interrupt 11
{
	WakeUpSource = 4;		//���INT3����
	INT_CLKO &= ~(1 << 5);	//INT3 Disable	
}

/********************* INT4�жϺ��� *************************/
void INT4_int (void) interrupt 16
{
	WakeUpSource = 5;		//���INT4����
	INT_CLKO &= ~(1 << 6);	//INT4 Disable	
}


//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��.
// ����: none.
// �汾: VER1.0
// ����: 2010-12-15
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do
	 {
		i = MAIN_Fosc / 13000L;	//1T
		while(--i)	;   //13T per loop
     }while(--ms);
}

/********************** ģ�⴮����غ���************************/

void	BitTime(void)	//λʱ�亯�� 9600bps
{
	unsigned int i;
	i = ((MAIN_Fosc / 100) * 104) / 130000L - 1;	//������ʱ��������λʱ�� 1T
	while(--i)	;   //13T per loop
}

//ģ�⴮�ڷ���
void	Tx1Send(unsigned char dat)		//9600��N��8��1		����һ���ֽ�
{
	unsigned char	i;
	P_TXD1 = 0;
	BitTime();
	i = 8;
	do
	{
		dat >>= 1;
		P_TXD1 = CY;
		BitTime();
	}while(--i);
	P_TXD1 = 1;
	BitTime();	//stop bit
	BitTime();	//stop bit
}

void PrintString(unsigned char code *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//����ֹͣ��0����
}

