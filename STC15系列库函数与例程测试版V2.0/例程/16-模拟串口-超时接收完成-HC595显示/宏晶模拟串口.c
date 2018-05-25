

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



/*************	��������˵��	**************

				����˵��

	��������ʹ��STCϵ��MCU����ģ�⴮�ڡ�
	
	�û������Լ���ʱ�ӺͲ�����������config.h�����ú�������ء�
	
	ʹ�ô���������MCU�������ݣ�MCU�յ���ԭ�����ظ�PC��
	
	���ڽ��յ�һ�����ݿ�, ���Ҵ��ڿ��г���35��λʱ��(��ʱ)�󣬱�־�������.
	
	������ʹ����Դ: Timer0�ж�.

*/

#include "config.h"


/*************	���س�������	**************/
#define Timer0_Reload	(65536 - MAIN_Fosc / BaudRate / 3)
#define RxLength		32		//���ջ��峤��

u8 code t_display[]={						//��׼�ֿ�
//	 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black	 -     H    J	 K	  L	   N	o   P	 U     t    G    Q    r   M    y
	0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
	0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};	//0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};		//λ��


/*************	���ر�������	**************/

sbit P_RXB = P3^0;			//����ģ�⴮�ڽ���IO
sbit P_TXB = P3^1;			//����ģ�⴮�ڷ���IO

bit  TxBusyFlag;		//���ڷ���һ���ֽ�, �û�����1��ʼ����, �������ϵͳ���Զ�0
bit	B_Rx_OK;	 		//������ı�־λ, �յ����ݿ�ϵͳ����1, �û��������ݺ������0
u8	Rx_Cnt;				//���յ����ֽ���, �û��������ݺ������0
u8  idata RxBuf[RxLength];	//���ջ���
u8	TxSBUF;				//�������ݼĴ���, �û�д��Ҫ���͵�����, ��TxBusyFlag����Ϊ1,ϵͳ�ͻᷢ�ͳ�ȥ, ������ɺ�ϵͳ��TxBusyFlag��0.

//===== ������ЩΪϵͳʹ�õĳ��������, �û����ɼ��������޸� =============

#define	RxBitLenth	9		//8������λ+1��ֹͣλ
#define	TxBitLenth	9		//8������λ+1��ֹͣλ
u8  TxShiftReg,RxShiftReg;	//���� ���� ��λ
u8  TxSample,RxSample;		//���ͺͽ��ռ�� ����������(3�����ʼ��)
u8  TxBitCnt,RxBitCnt;		//���ͺͽ��յ����� λ������
u8	RxTimeOut;		//���ճ�ʱ����
bit	RxStartFlag;			//���ڽ���һ���ֽ�(���յ���ʼλ)

//=====================================================================
sbit	P_HC595_SER   = P3^2;	//pin 14	SER		data input
sbit	P_HC595_RCLK  = P3^4;	//pin 12	RCLk	store (latch) clock
sbit	P_HC595_SRCLK = P3^3;	//pin 11	SRCLK	Shift data clock

u8 	LED8[8];		//��ʾ����
u8	display_index;	//��ʾλ����
u8	cnt_2ms;

/*************	���غ�������	**************/
void	TxByte(u8 dat);
void	TxString(u8 *p);


/*************  �ⲿ�����ͱ������� *****************/



//========================================================================
// ����: void UART_INIT(void)
// ����: UARTģ��ĳ�ʼ����.
// ����: ��.
// ����: ��.
// �汾: V1.0, 2012-10-22
//========================================================================
void UART_INIT(void)
{
	TxBusyFlag = 0;
	RxStartFlag = 0;
	TxSample = 3;
	RxSample = 4;
	RxTimeOut = 0;
	B_Rx_OK = 0;
	Rx_Cnt = 0;
}


//========================================================================
// ����: void main(void)
// ����: ������.
// ����: ��.
// ����: ��.
// �汾: V1.0, 2012-10-22
//========================================================================
void main(void)
{
	u8	i;
	u16	j;

	InternalRAM_enable();
//	ExternalRAM_enable();

	Timer0_1T();
	Timer0_AsTimer();
	Timer0_16bitAutoReload();
	Timer0_Load(Timer0_Reload);
	Timer0_InterruptEnable();
	PT0 = 1;	//��ʱ��0�����ȼ��ж�
	Timer0_Run();
	EA = 1;						//�����ж�					open global interrupt switch

	for(i=0; i<8; i++)	LED8[i] = 0x10;	//�ϵ�����
	LED8[7] = 0;

	UART_INIT();				//UARTģ��ĳ�ʼ����
	TxString("STC15F/Lϵ��MCUģ�⴮����ʾ����\r\n");	//����һ����ʾ�ַ�

	
	while (1)
	{
		if (B_Rx_OK)	//������ı�־λ, �յ����ݿ�ϵͳ����1, �û��������ݺ������0
		{
			if(Rx_Cnt >= 3)	//ȷ��������
			{
				for(i=0; i<Rx_Cnt; i++)	TxByte(RxBuf[i]);	//���յ�������ԭ������,���ڲ���
			}
			if(Rx_Cnt == 2)	//ȷ��������
			{
				j = ((u16)RxBuf[0] << 8) + RxBuf[1];
				LED8[3] = j / 10000;
				LED8[4] = j % 10000 / 1000;
				LED8[5] = j % 1000 / 100;
				LED8[6] = j % 100 / 10;
				LED8[7] = j % 10;
				for(i=3; i<7; i++)
				{
					if(LED8[i] > 0)	break;
					LED8[i] = 0x10;
				}
			//	for(i=0; i<Rx_Cnt; i++)	TxByte(RxBuf[i]);	//���յ�������ԭ������,���ڲ���
			}
			Rx_Cnt  = 0;	//����ֽ���
			B_Rx_OK = 0;	//���������ɱ�־
		}
	}
}


//========================================================================
// ����: void	TxByte(u8 dat)
// ����: ����һ���ֽ�.
// ����: dat: Ҫ���͵�����.
// ����: ��.
// �汾: V1.0, 2012-10-22
//========================================================================
void	TxByte(u8 dat)
{
	TxSBUF = dat;		//Ҫ���͵�����д�뷢�ͻ���
	TxBusyFlag = 1;		//��������
	while(TxBusyFlag);	//�ȴ��������
}

//========================================================================
// ����: void	TxString(u8 *p)
// ����: ����һ�����ݿ�, ����0����.
// ����: *p: ���ݿ��ָ��.
// ����: ��.
// �汾: V1.0, 2012-10-22
//========================================================================
void	TxString(u8 *p)
{
	for( ; *p > 0; p++)	TxByte(*p);
}


/**********************************************/
void Send_595(unsigned char dat)	//����һ���ֽ�
{		
	unsigned char	i;
	for(i=0; i<8; i++)
	{
		dat <<= 1;
		P_HC595_SER = CY;
		P_HC595_SRCLK = 1;
		P_HC595_SRCLK = 0;
	}
}

/**********************************************/
#define		LED_TYPE	0xff		//����LED����, 0x00--����, 0xff--����

void DisplayScan(void)	//��ʾɨ�躯��
{	
	Send_595((~LED_TYPE) ^ T_COM[display_index]);			//���λ��
	Send_595(  LED_TYPE  ^ t_display[LED8[display_index]]);	//�������

	P_HC595_RCLK = 1;
	P_HC595_RCLK = 0;							//�����������
	if(++display_index >= 8)	display_index = 0;	//8λ������0
}

//========================================================================
// ����: void tm0(void) interrupt 1
// ����: ��ʱ��0�жϳ���, for UART �Բ�����3�����ٶȲ����ж� ��ʼλ.
// ����: ��.
// ����: ��.
// �汾: V1.0, 2012-10-22
//========================================================================

void tm0(void) interrupt 1
{
	//====================== ģ�⴮�ڽ��ճ��� ========================================
	if (RxStartFlag)			//�ѽ��յ���ʼλ
	{
		if (--RxSample == 0)			//���������Զ�ʱ����1/3������
		{
			RxSample = 3;               //���ý��ռ�����  ���������Զ�ʱ����1/3������	reset send baudrate counter
			if (--RxBitCnt == 0)		//������һ֡����
			{
				RxStartFlag = 0;        //ֹͣ����			stop receive
				if(P_RXB && !B_Rx_OK)	//ȷ��ֹͣλ��ȷ,���һ����ѿ�
				{
					RxBuf[Rx_Cnt] = RxShiftReg;     //�洢���ݵ�������	save the data to RBUF
					if(++Rx_Cnt >= RxLength)	Rx_Cnt = 0;	//����ж�
					RxTimeOut = 105;				//��ʱ������ֵ, 35��λ��ʱ��(��Ӧ5���ֽ�), �ο�MODBUSЭ��
				}
			}
			else
			{
				RxShiftReg >>= 1;			  //�ѽ��յĵ�b���� �ݴ浽 RxShiftReg(���ջ���)
				if (P_RXB) RxShiftReg |= 0x80;  //shift RX data to RX buffer
			}
		}
	}

	else if (!P_RXB)		//�ж��ǲ��ǿ�ʼλ P_RXB=0;
	{
		RxStartFlag = 1;       //����������ÿ�ʼ���ձ�־λ 	set start receive flag
		RxSample = 4;       //��ʼ�����ղ����ʼ�����       	initial receive baudrate counter
		RxBitCnt = RxBitLenth;       //��ʼ�����յ�����λ��(8������λ+1��ֹͣλ)    initial receive bit number (8 data bits + 1 stop bit)
	}
	if(RxTimeOut > 0)	//���ճ�ʱ����
	{
		if(--RxTimeOut == 0)	B_Rx_OK = 1;	//��־���յ�һ֡����
	}

	//====================== ģ�⴮�ڷ��ͳ��� ========================================
	if (TxBusyFlag)			//���Ϳ�ʼ��־λ   judge whether sending
	{
		if (--TxSample == 0)			//���������Զ�ʱ����1/3������
		{
			TxSample = 3;				//���÷��ͼ�����   reset send baudrate counter
			if (TxBitCnt == 0)			//���ͼ�����Ϊ0 �������ֽڷ��ͻ�û��ʼ
			{
				P_TXB = 0;			//���Ϳ�ʼλ     					send start bit
				TxShiftReg = TxSBUF;		//�ѻ�������ݷŵ����͵�buff		load data from TxSBUF to TxShiftReg
				TxBitCnt = TxBitLenth;	//��������λ�� (8����λ+1ֹͣλ)	initial send bit number (8 data bits + 1 stop bit)
			}
			else					//���ͼ�����Ϊ��0 ���ڷ�������
			{
				if (--TxBitCnt == 0)	//���ͼ�������Ϊ0 �������ֽڷ��ͽ���
				{
					P_TXB = 1;			//��ֹͣλ����
					TxBusyFlag = 0;	//����ֹͣλ    			stop send
				}
				else
				{
					TxShiftReg >>= 1;		//�����λ�͵� CY(�洦��־λ) shift data to CY
					P_TXB = CY;		//���͵�b����				write CY to TX port
				}
			}
		}
	}

	if(--cnt_2ms == 0)
	{
		cnt_2ms = ((u32)BaudRate * 6 + 500)/1000;
		DisplayScan();
	}
}

