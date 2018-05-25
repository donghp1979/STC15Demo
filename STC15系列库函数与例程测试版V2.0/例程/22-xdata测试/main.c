
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


#include	"config.h"
#include	"USART1.h"
#include	"delay.h"
#include	"app_test_xdata.h"


/*************	����˵��	**************

������ҵ�32K xdata����
���Է�ʽ: 
1: д��0x55, �������ж��Ƿ�ȫ����0x55.
2: д��0xaa, �������ж��Ƿ�ȫ����0xaa.
3: д��32768�ֽڵĺ����ֿ�(�������������), �������Ƚ�.

ͨ�����ڷ��͵����ַ�x��X, ��ʼ����, ��������صĲ��Խ��.

��������115200bps, 8, N, 1,  ��ʱ��Ϊ22.1184MHZ.

******************************************/

/*************	���س�������	**************/
#define	XDATA_LENTH		32768	//xdata����
#define	HZK_LENTH		4096	//32768	//�ֿⳤ��



/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/


/*************  ����1��ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2

	PrintString1("STC15F2K60S2 xdata test programme!\r\n");	//SUART1����һ���ַ���
}



/*************  ����xdata���� *****************/
u8	TestXRAM(void)
{
	u8	code	*ptc;
	u8	xdata	*ptx;
	u16	i,j;

	for(ptx=0; ptx<XDATA_LENTH; ptx++)	*ptx = 0x55;	//�����Ƿ���λ��·
	for(ptx=0; ptx<XDATA_LENTH; ptx++)	if(*ptx != 0x55)	return 1;	//����0x55����

	for(ptx=0; ptx<XDATA_LENTH; ptx++)	*ptx = 0xaa;	//�����Ƿ���λ��·
	for(ptx=0; ptx<XDATA_LENTH; ptx++)	if(*ptx != 0xaa)	return 2;	//����0xaa����

	i = 0;
	for(ptx=0; ptx<XDATA_LENTH; ptx++)
	{
		*ptx = (u8)(i >> 8);
		ptx++;
		*ptx = (u8)i;
		i++;
	}
	i = 0;
	for(ptx=0; ptx<XDATA_LENTH; ptx++)
	{
		j = *ptx;
		ptx++;
		j = (j << 8) + *ptx;
		if(i != j)	return 3;	//д�������ִ���
		i++;
	}

	ptx = 0;
	for(ptc=0; ptc<HZK_LENTH; ptc++)	{*ptx = *ptc;	ptx++;}
	ptx = 0;
	for(ptc=0; ptc<HZK_LENTH; ptc++)
	{
		if(*ptx != *ptc)	return 4;	//д�ֿ����
		ptx++;
	}

	return 0;

}


/*************  ���ʹ����ַ���� *****************/
void	TxErrorAddress(void)
{
	u16	i;
	i = 0x00fd;
	PrintString1("�����ַ = ");
	if(i >= 10000)	TX1_write2buff(i/10000+'0'),	i %= 10000;
	TX1_write2buff(i/1000+'0'),	i %= 1000;
	TX1_write2buff(i/100+'0'),	i %= 100;
	TX1_write2buff(i/10+'0');
	TX1_write2buff(i%10+'0');
	TX1_write2buff(0x0d);
	TX1_write2buff(0x0a);
}

/*************  xdata���Է�����Ϣ���� *****************/
void	Xdata_Test(void)
{
	u8	i;
	if((RX1_Buffer[0] == 'x') || (RX1_Buffer[0] == 'X'))
	{
		PrintString1("���ڲ��� xdata, ���Ժ�......\r\n");
		while(COM1.B_TX_busy > 0)	;
		
		i = TestXRAM();
			 if(i == 0)	PrintString1("���� xdata �����ȷ!\r\n");
		else if(i == 1)	PrintString1("���� xdata д��0x55����!  "),	TxErrorAddress();
		else if(i == 2)	PrintString1("���� xdata д��0xaa����!  "),	TxErrorAddress();
		else if(i == 3)	PrintString1("���� xdata ����д�����!  "),	TxErrorAddress();
		else if(i == 4)	PrintString1("���� xdata д���ֿ����!  "),	TxErrorAddress();

		while(COM1.B_TX_busy > 0)	;
	}
}

/***************** ������ *****************************/
void main(void)
{

	UART_config();	//���ڳ�ʼ��
	EA = 1;			//ȫ���ж�����

	BUS_SPEED_1T();	//1T  2T  4T  8T	3V@22MHZ��1T����ʴ���
	ExternalRAM_enable();	//�����ⲿXDATA
//	InternalRAM_enable();	//�����ڲ�XDATA

	while (1)
	{
		delay_ms(1);
		if(COM1.RX_TimeOut > 0)		//��ʱ����
		{
			if(--COM1.RX_TimeOut == 0)	//����ͨѶ����
			{
				if(COM1.RX_Cnt > 0)		//�յ������ֽ���
				{
					if(COM1.RX_Cnt == 1)	Xdata_Test();	//���ֽ�����
				}
				COM1.RX_Cnt = 0;
			}
		}
	}
}



