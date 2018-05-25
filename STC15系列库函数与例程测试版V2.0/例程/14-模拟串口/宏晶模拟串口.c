

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

	��������ʹ��STCϵ��MCU����ģ�⴮�ڡ��û������Լ���ʱ�ӺͲ������������ú�������ء�
	
	ʹ�ô���������MCU�������ݣ�MCU�յ���ԭ�����ظ�PC��
	
	������ʹ����Դ: Timer0�ж�.

*/

#include "config.h"


/***************************************************************************/

typedef bit BOOL;
typedef unsigned char 	uchar;
typedef unsigned int 	uint;

#define Timer0_Reload		(65536 - MAIN_Fosc / BaudRate / 3)
#define D_RxBitLenth	9		//9: 8 + 1 stop
#define D_TxBitLenth	9		//9: 1 stop bit

sbit RXB = P3^0;                //define UART TX/RX port
sbit TXB = P3^1;

uchar  data TBUF,RBUF;
uchar  data TDAT,RDAT;
uchar  data TCNT,RCNT;	//���ͺͽ��ռ�� ������(3�����ʼ��)
uchar  data TBIT,RBIT;	//���ͺͽ��յ����ݼ�����
uchar  data t, r;
uchar  data buf[16];

bit  TING,RING;	//���ڷ��ͻ����һ���ֽ�
bit  REND;	 	//������ı�־λ

#define	RxBitLenth	9	//8������λ+1��ֹͣλ
#define	TxBitLenth	9	//8������λ+1��ֹͣλ

//-----------------------------------------
//UARTģ��ĳ�ʼ����	initial UART module variable
void UART_INIT()
{
      TING = 0;
      RING = 0;
      REND = 0;
      TCNT = 0;
      RCNT = 0;
}

void main()
{
	InternalRAM_enable();
//	ExternalRAM_enable();

	Timer0_1T();
	Timer0_AsTimer();
	Timer0_16bitAutoReload();
	Timer0_Load(Timer0_Reload);
	Timer0_InterruptEnable();
	Timer0_Run();
	EA = 1;						//�����ж�					open global interrupt switch

	UART_INIT();				//UARTģ��ĳ�ʼ����

	while (1)
	{
		if (REND)				//���������,�ѽ��յ���ֵ�������buff
		{
			REND = 0;
			buf[r++ & 0x0f] = RBUF;
		}

		if(!TING)		//���Ϳ���
		{
			if (t != r)	//��Ҫ���͵�����
			{
				TBUF = buf[t++ & 0x0f];
				TING = 1;
			}
		}
	}
}


//-----------------------------------------
//��ʱ��0�жϳ���for UART �Բ�����3�����ٶȲ����ж� ��ʼλ		Timer interrupt routine for UART
void tm0(void) interrupt 1
{

	if (RING)
	{
		if (--RCNT == 0)				  //���������Զ�ʱ����1/3������
		{
			RCNT = 3;                   //���ý��ռ�����  ���������Զ�ʱ����1/3������	reset send baudrate counter
			if (--RBIT == 0)			  //������һ֡����
			{
				RBUF = RDAT;            //�洢���ݵ�������	save the data to RBUF
				RING = 0;               //ֹͣ����			stop receive
				REND = 1;               //������ɱ�־����	set receive completed flag
			}
			else
			{
				RDAT >>= 1;			  //�ѽ��յĵ�b���� �ݴ浽 RDAT(���ջ���)
				if (RXB) RDAT |= 0x80;  //shift RX data to RX buffer
			}
		}
	}

	else if (!RXB)		//�ж��ǲ��ǿ�ʼλ RXB=0;
	{
		RING = 1;       //����������ÿ�ʼ���ձ�־λ 	set start receive flag
		RCNT = 4;       //��ʼ�����ղ����ʼ�����       	initial receive baudrate counter
		RBIT = RxBitLenth;       //��ʼ�����յ�����λ��(8������λ+1��ֹͣλ)    initial receive bit number (8 data bits + 1 stop bit)
	}

	if (TING)			//���Ϳ�ʼ��־λ   judge whether sending
	{
		if (--TCNT == 0)			//���������Զ�ʱ����1/3������
		{
			TCNT = 3;				//���÷��ͼ�����   reset send baudrate counter
			if (TBIT == 0)			//���ͼ�����Ϊ0 �������ֽڷ��ͻ�û��ʼ
			{
				TXB = 0;			//���Ϳ�ʼλ     					send start bit
				TDAT = TBUF;		//�ѻ�������ݷŵ����͵�buff		load data from TBUF to TDAT
				TBIT = TxBitLenth;	//��������λ�� (8����λ+1ֹͣλ)	initial send bit number (8 data bits + 1 stop bit)
			}
			else					//���ͼ�����Ϊ��0 ���ڷ�������
			{
				if (--TBIT == 0)	//���ͼ�������Ϊ0 �������ֽڷ��ͽ���
				{
					TXB = 1;		//��ֹͣλ����
					TING = 0;		//����ֹͣλ    			stop send
				}
				else
				{
					TDAT >>= 1;		//�����λ�͵� CY(�洦��־λ) shift data to CY
					TXB = CY;		//���͵�b����				write CY to TX port
				}
			}
		}
	}
}

