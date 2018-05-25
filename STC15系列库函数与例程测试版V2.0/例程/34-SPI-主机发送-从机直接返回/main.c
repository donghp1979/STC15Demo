
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


#include	"config.h"
#include	"delay.h"
#include	"USART1.h"
#include	"spi.h"

/*************	����˵��	**************

������MCU��P1.2 P1.3 P1.4 P1.5�ֱ�ͬ��������, ����MCU�ֱ���������PC����A��B���Ϳ����ô������ֲ�����.

���Դ���A����MCU-A, MCU-A���յ�������ͨ��SPI����MCU-B, MCU-B��SPI���յ����ݺ�,�Ӵ��ڷ������Դ���B.

PC-A --RS232--> MCU-A ---SPI---> MCU_B ---RS232---> PC_B

PC-A <--RS232-- MCU-A <---SPI--- MCU_B <---RS232--- PC_B

Ĭ�ϲ�����:  115200,8,N,1. ������"���ڳ�ʼ������"���޸�.
Ĭ����ʱ��:  22118400HZ, �����������ļ�"config.h"���޸�.


�����ļ�:
inilize.c
interrupt.c
main.c

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2

	PrintString1("STC15F2K60S2 SPI Test Prgramme!\r\n");	//SUART1����һ���ַ���
}

void	SPI_config(void)
{
	SPI_InitTypeDef		SPI_InitStructure;
	SPI_InitStructure.SPI_Module    = ENABLE;              //SPI����    ENABLE, DISABLE
	SPI_InitStructure.SPI_SSIG      = DISABLE;			  //Ƭѡλ     ENABLE, DISABLE
	SPI_InitStructure.SPI_FirstBit  = SPI_MSB;			  //��λ����   SPI_MSB, SPI_LSB
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Slave;	  //����ѡ��   SPI_Mode_Master, SPI_Mode_Slave
	SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;      //ʱ����λ   SPI_CPOL_High,   SPI_CPOL_Low
	SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;	  //���ݱ���   SPI_CPHA_1Edge,  SPI_CPHA_2Edge
	SPI_InitStructure.SPI_Interrupt = ENABLE;			  //�ж�����   ENABLE,DISABLE
	SPI_InitStructure.SPI_Speed     = SPI_Speed_64;		  //SPI�ٶ�    SPI_Speed_4, SPI_Speed_16, SPI_Speed_64, SPI_Speed_128
	SPI_InitStructure.SPI_IoUse     = SPI_P12_P13_P14_P15; //IO���л�   SPI_P12_P13_P14_P15, SPI_P24_P23_P22_P21, SPI_P54_P40_P41_P43
	SPI_Init(&SPI_InitStructure);
	
	SPI_TxRxMode = SPI_Mode_Slave;
}


/******************** task A **************************/
void main(void)
{
	u8	i;

	UART_config();
	SPI_config();
	EA = 1;

	while (1)
	{
		delay_ms(1);
				
		if(COM1.RX_TimeOut > 0)
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(COM1.RX_Cnt > 0)
				{
					SPI_TxRead = 0;
					SPI_TxWrite = 0;
					SPI_RxCnt = 0;
					COM1.RX_Cnt++;	//�෢һ����Ч���ֽ�
					for(i=0; i<COM1.RX_Cnt; i++)	SPI_WriteToTxBuf(RX1_Buffer[i]);	//�����յ�������ת��SPI
					SPI_TrigTx();
				}
				COM1.RX_Cnt = 0;	COM1.B_RX_OK = 0;
			}
		}
		if(SPI_RxTimerOut > 0)
		{
			if(--SPI_RxTimerOut == 0)
			{
				if(SPI_RxCnt > 1)
				{
					i = 0;
					if(B_RxSlave0_Master1)	i++;
					else					SPI_RxCnt--;
					for(; i<SPI_RxCnt; i++)	TX1_write2buff(SPI_RxBuffer[i]);
				}
				SPI_RxCnt = 0;	//B_SPI_RxOk = 0;
			}
		}
	}
}



