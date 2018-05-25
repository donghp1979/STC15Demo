
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


/*************	����˵��	**************

����1ȫ˫���жϷ�ʽ�շ�ͨѶ���򡣱�����ʹ��22.1184MHZʱ�ӣ���Ҫ�ı䣬���޸������"������ʱ��"��ֵ�����±��롣

��������Ϊ��115200,8,n,1.

ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/

/*************	���س�������	**************/
#define MAIN_Fosc		22118400L	//������ʱ��
#define	RX1_Lenth		32			//���ڽ��ջ��峤��
#define	BaudRate1		115200UL	//ѡ������


#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 ��װֵ�� ��Ӧ300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 ��װֵ�� ��Ӧ300KHZ

#include	"STC15Fxxxx.H"


/*************	���ر�������	**************/
u8	idata RX1_Buffer[RX1_Lenth];	//���ջ���
u8	TX1_Cnt;	//���ͼ���
u8	RX1_Cnt;	//���ռ���
bit	B_TX1_Busy;	//����æ��־


/*************	���غ�������	**************/



/**********************************************/
void main(void)
{
	B_TX1_Busy = 0;
	RX1_Cnt = 0;
	TX1_Cnt = 0;

	S1_8bit();				//8λ����
	S1_USE_P30P31();		//UART1 ʹ��P30 P31��	Ĭ��
//	S1_USE_P36P37();		//UART1 ʹ��P36 P37��
//	S1_USE_P16P17();		//UART1 ʹ��P16 P17��

/*
	TR1 = 0;			//������ʹ��Timer1����
	AUXR &= ~0x01;		//S1 BRT Use Timer1;
	AUXR |=  (1<<6);	//Timer1 set as 1T mode
	TH1 = (u8)(Timer1_Reload >> 8);
	TL1 = (u8)Timer1_Reload;
	TR1  = 1;
*/

	AUXR &= ~(1<<4);	//Timer stop		������ʹ��Timer2����
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (u8)(Timer2_Reload >> 8);
	TL2 = (u8)Timer2_Reload;
	AUXR |=  (1<<4);	//Timer run enable

	REN = 1;	//�������
	ES  = 1;	//�����ж�

	EA = 1;		//����ȫ���ж�
	

	while (1)
	{
		if(TX1_Cnt != RX1_Cnt)		//�յ�������
		{
			if(!B_TX1_Busy)		//���Ϳ���
			{
				B_TX1_Busy = 1;		//��־����æ
				SBUF = RX1_Buffer[TX1_Cnt];	//��һ���ֽ�
				if(++TX1_Cnt >= RX1_Lenth)	TX1_Cnt = 0;	//�����������
			}
		}
	}
}

/********************* UART1�жϺ���************************/
void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		RX1_Buffer[RX1_Cnt] = SBUF;		//����һ���ֽ�
		if(++RX1_Cnt >= RX1_Lenth)	RX1_Cnt = 0;	//�����������
	}

	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//�������æ��־
	}
}



