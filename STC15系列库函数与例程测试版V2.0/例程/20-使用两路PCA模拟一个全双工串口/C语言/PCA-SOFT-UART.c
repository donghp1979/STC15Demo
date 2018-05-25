/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
/*---------------------------------------------------------------------*/

/*************	����˵��	**************

ʹ��STC15ϵ�е�PCA0��PCA1����ģ�⴮��. PCA0����(P2.5), PCA1����(P2.6).

�ٶ�����оƬ�Ĺ���Ƶ��Ϊ22118400Hz. ʱ��Ϊ5.5296MHZ ~ 35MHZ. 

�����ʸߣ���ʱ��ҲҪѡ��, ����ʹ�� 22.1184MHZ, 11.0592MHZ.

���Է���: ��λ����������,MCU�յ����ݺ�ԭ������.

���ڹ̶�����: 1λ��ʼλ, 8λ����λ, 1λֹͣλ,  �������� 600 ~ 57600 bps.

1200 ~ 57600 bps @ 33.1776MHZ
 600 ~ 57600 bps @ 22.1184MHZ
 600 ~ 38400 bps @ 18.4320MHZ
 300 ~ 28800 bps @ 11.0592MHZ
 150 ~ 14400 bps @  5.5296MHZ

******************************************/

#include	<reg52.h>

#define MAIN_Fosc		22118400UL	//������ʱ��
#define	UART3_Baudrate	57600UL		//���岨����
#define	RX_Lenth		16			//���ճ���

#define	PCA_P12_P11_P10_P37	(0<<4)
#define	PCA_P34_P35_P36_P37	(1<<4)
#define	PCA_P24_P25_P26_P27	(2<<4)
#define	PCA_Mode_Capture	0
#define	PCA_Mode_SoftTimer	0x48
#define	PCA_Clock_1T		(4<<1)
#define	PCA_Clock_2T		(1<<1)
#define	PCA_Clock_4T		(5<<1)
#define	PCA_Clock_6T		(6<<1)
#define	PCA_Clock_8T		(7<<1)
#define	PCA_Clock_12T		(0<<1)
#define	PCA_Clock_ECI		(3<<1)
#define	PCA_Rise_Active		(1<<5)
#define	PCA_Fall_Active		(1<<4)
#define	PCA_PWM_8bit		(0<<6)
#define	PCA_PWM_7bit		(1<<6)
#define	PCA_PWM_6bit		(2<<6)

#define	UART3_BitTime	(MAIN_Fosc / UART3_Baudrate)

#define		ENABLE		1
#define		DISABLE		0

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

sfr AUXR1 = 0xA2;
sfr CCON = 0xD8;
sfr CMOD = 0xD9;
sfr CCAPM0 = 0xDA;		//PCAģ��0�Ĺ���ģʽ�Ĵ�����
sfr CCAPM1 = 0xDB;		//PCAģ��1�Ĺ���ģʽ�Ĵ�����
sfr CCAPM2 = 0xDC;		//PCAģ��2�Ĺ���ģʽ�Ĵ�����

sfr CL     = 0xE9;
sfr CCAP0L = 0xEA;		//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1L = 0xEB;		//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP2L = 0xEC;		//PCAģ��2�Ĳ�׽/�ȽϼĴ�����8λ��

sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1H = 0xFB;		//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP2H = 0xFC;		//PCAģ��2�Ĳ�׽/�ȽϼĴ�����8λ��

sbit CCF0  = CCON^0;	//PCA ģ��0�жϱ�־����Ӳ����λ�������������0��
sbit CCF1  = CCON^1;	//PCA ģ��1�жϱ�־����Ӳ����λ�������������0��
sbit CCF2  = CCON^2;	//PCA ģ��2�жϱ�־����Ӳ����λ�������������0��
sbit CR    = CCON^6;	//1: ����PCA������������0: ��ֹ������
sbit CF    = CCON^7;	//PCA�����������CH��CL��FFFFH��Ϊ0000H����־��PCA�������������Ӳ����λ�������������0��
sbit PPCA  = IP^7;		//PCA �ж� ���ȼ��趨λ

u16		CCAP0_tmp;
u16		CCAP1_tmp;

u8	Tx3_read;			//���Ͷ�ָ��
u8	Rx3_write;			//����дָ��
u8 	idata	buf3[RX_Lenth];	//���ջ���

//===================== ģ�⴮�����===========================
sbit P_RX3 = P2^5;	//����ģ�⴮�ڽ���IO
sbit P_TX3 = P2^6;	//����ģ�⴮�ڷ���IO

u8  Tx3_DAT;		// ������λ����, �û����ɼ�
u8  Rx3_DAT;		// ������λ����, �û����ɼ�
u8  Tx3_BitCnt;		// �������ݵ�λ������, �û����ɼ�
u8  Rx3_BitCnt;		// �������ݵ�λ������, �û����ɼ�
u8	Rx3_BUF;		// ���յ����ֽ�, �û���ȡ
u8	Tx3_BUF;		// Ҫ���͵��ֽ�, �û�д��
bit	Rx3_Ring;		// ���ڽ��ձ�־, �Ͳ����ʹ��, �û����򲻿ɼ�
bit	Tx3_Ting;		// ���ڷ��ͱ�־, �û���1������, �ײ㷢�������0
bit	RX3_End;		// ���յ�һ���ֽ�, �û���ѯ ����0
//=============================================================

void	PCA_Init(void);


/******************** ������ **************************/
void main(void)
{
	
	PCA_Init();	//PCA��ʼ��
	EA = 1;
	
	Tx3_read  = 0;
	Rx3_write = 0;
	Tx3_Ting  = 0;
	Rx3_Ring  = 0;
	RX3_End   = 0;
	Tx3_BitCnt = 0;

	
	while (1)		//user's function
	{
		if (RX3_End)		// ����Ƿ��յ�һ���ֽ�
		{
			RX3_End = 0;	// �����־
			buf3[Rx3_write] = Rx3_BUF;	// д�뻺��
			if(++Rx3_write >= RX_Lenth)	Rx3_write = 0;	// ָ����һ��λ��,  ������
		}
		if (!Tx3_Ting)		// ����Ƿ��Ϳ���
		{
			if (Tx3_read != Rx3_write)	// ����Ƿ��յ����ַ�
			{
				Tx3_BUF = buf3[Tx3_read];	// �ӻ����һ���ַ�����
				Tx3_Ting = 1;				// ���÷��ͱ�־
				if(++Tx3_read >= RX_Lenth)	Tx3_read = 0;	// ָ����һ��λ��,  ������
			}
		}
	}
}


//========================================================================
// ����: void	PCA_Init(void)
// ����: PCA��ʼ������.
// ����: none
// ����: none.
// �汾: V1.0, 2013-11-22
//========================================================================
void	PCA_Init(void)
{
	CR = 0;
	AUXR1 = (AUXR1 & ~(3<<4)) | PCA_P24_P25_P26_P27;	//�л�IO��
	CCAPM0 = (PCA_Mode_Capture | PCA_Fall_Active | ENABLE);	//16λ�½��ز�׽�ж�ģʽ

	CCAPM1    = PCA_Mode_SoftTimer | ENABLE;
	CCAP1_tmp = UART3_BitTime;
	CCAP1L    = (u8)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
	CCAP1H    = (u8)(CCAP1_tmp >> 8);	//��дCCAP0H

	CH = 0;
	CL = 0;
	CMOD  = (CMOD  & ~(7<<1)) | PCA_Clock_1T;			//ѡ��ʱ��Դ
	PPCA  = 1;	// �����ȼ��ж�
	CR    = 1;	// ����PCA��ʱ��
}
//======================================================================

//========================================================================
// ����: void	PCA_Handler (void) interrupt 7
// ����: PCA�жϴ������.
// ����: None
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
void	PCA_Handler (void) interrupt 7
{
	if(CCF0)		//PCAģ��0�ж�
	{
		CCF0 = 0;			//��PCAģ��0�жϱ�־
		if(Rx3_Ring)		//���յ���ʼλ
		{
			if (--Rx3_BitCnt == 0)		//������һ֡����
			{
				Rx3_Ring = 0;			//ֹͣ����
				Rx3_BUF = Rx3_DAT;		//�洢���ݵ�������
				RX3_End = 1;
				CCAPM0 = (PCA_Mode_Capture | PCA_Fall_Active | ENABLE);	//16λ�½��ز�׽�ж�ģʽ
			}
			else
			{
				Rx3_DAT >>= 1;			  		//�ѽ��յĵ�b���� �ݴ浽 RxShiftReg(���ջ���)
				if(P_RX3) Rx3_DAT |= 0x80;  	//shift RX data to RX buffer
				CCAP0_tmp += UART3_BitTime;		//����λʱ��
				CCAP0L = (u8)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
				CCAP0H = (u8)(CCAP0_tmp >> 8);	//��дCCAP0H
			}
		}
		else
		{
			CCAP0_tmp = ((u16)CCAP0H << 8) + CCAP0L;	//����׽�Ĵ���
			CCAP0_tmp += (UART3_BitTime / 2 + UART3_BitTime);	//��ʼλ + �������λ
			CCAP0L = (u8)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
			CCAP0H = (u8)(CCAP0_tmp >> 8);	//��дCCAP0H
			CCAPM0 = (PCA_Mode_SoftTimer | ENABLE);	//16λ�����ʱ�ж�ģʽ
			Rx3_Ring = 1;					//��־���յ���ʼλ
			Rx3_BitCnt = 9;					//��ʼ�����յ�����λ��(8������λ+1��ֹͣλ)
		}
	}

	if(CCF1)	//PCAģ��1�ж�, 16λ�����ʱ�ж�ģʽ
	{
		CCF1 = 0;						//��PCAģ��1�жϱ�־
		CCAP1_tmp += UART3_BitTime;
		CCAP1L = (u8)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP1H = (u8)(CCAP1_tmp >> 8);	//��дCCAP0H

		if(Tx3_Ting)					// ������, �˳�
		{
			if(Tx3_BitCnt == 0)			//���ͼ�����Ϊ0 �������ֽڷ��ͻ�û��ʼ
			{
				P_TX3 = 0;				//���Ϳ�ʼλ
				Tx3_DAT = Tx3_BUF;		//�ѻ�������ݷŵ����͵�buff
				Tx3_BitCnt = 9;			//��������λ�� (8����λ+1ֹͣλ)
			}
			else						//���ͼ�����Ϊ��0 ���ڷ�������
			{
				if (--Tx3_BitCnt == 0)	//���ͼ�������Ϊ0 �������ֽڷ��ͽ���
				{
					P_TX3 = 1;			//��ֹͣλ����
					Tx3_Ting = 0;		//����ֹͣ
				}
				else
				{
					Tx3_DAT >>= 1;		//�����λ�͵� CY(�洦��־λ)
					P_TX3 = CY;			//����һ��bit����
				}
			}
		}
	}
}



