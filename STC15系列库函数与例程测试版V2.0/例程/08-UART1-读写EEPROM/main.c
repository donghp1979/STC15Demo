
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


/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      �����򾭹�������ȫ����, ���ṩ�绰����֧��, �粻�����, �����в�����ػ���.  */


/*************	��������˵��	**************

��EEPROM������������д��64�ֽڡ�����64�ֽڵĲ�����

EEPROMͨ�ò��Գ��򡣿����������ļ�"config.h"��ѡ��MCU���͡�

������:  9600,8,N,1. �����������ļ�"config.h"���޸�.

��������:
	E 0, E 12, E 120	--> ��EEPROM����������������������Ϊ�����ţ�ʮ����.
	W 0, W 12, W 120	--> ��EEPROM��������д�����������Ϊ�����ţ�ʮ����. �������Ŀ�ʼ��ַд�롣
	R 0, R 12, R 120	--> ��EEPROM����������������������Ϊ�����ţ�ʮ����. �������Ŀ�ʼ��ַ������
	
	E 0x1234, E 0x1af5, E 0x10AB	--> ��EEPROM����������������������Ϊ��ʼ��ַ��ʮ������.
	W 0x1234, W 0x1af5, W 0x10AB	--> ��EEPROM��������д�����������Ϊ��ʼ��ַ��ʮ������. 
	R 0x1234, R 0x1af5, R 0x10AB	--> ��EEPROM����������������������Ϊ��ʼ��ַ��ʮ������. 

ע�⣺Ϊ��ͨ�ã�����ʶ�������Ƿ���Ч���û��Լ����ݾ�����ͺ���������

�����ļ�:
STC_EEPROM.c
test_EEPROM.c

����: 2013-5-10

******************************************/

#include	"config.h"
#include	"USART.h"
#include	"delay.h"
#include 	"eeprom.h"



/*************	���س�������	**************/
u8	code	T_Strings[]={"ȥ����մ����У������һ���ӳ�졣���治֪�δ�ȥ���һ�����Ц���硣"};


/*************	���ر�������	**************/
u8	xdata	tmp[70];


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

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BaudRate  = 57600ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART2_SW_P10_P11;	//�л��˿�,   UART2_SW_P10_P11,UART2_SW_P46_P47
	USART_Configuration(USART2, &COMx_InitStructure);		//��ʼ������2 USART1,USART2

	PrintString1("STC15F2K60S2ϵ�е�Ƭ��EEPROM���Գ���!\r\n");	//SUART1����һ���ַ���
	PrintString2("STC15F2K60S2ϵ�е�Ƭ��EEPROM���Գ���!\r\n");	//SUART2����һ���ַ���
}


/**********************************************/

u8	CheckData(u8 dat)
{
	if((dat >= '0') && (dat <= '9'))		return (dat-'0');
	if((dat >= 'A') && (dat <= 'F'))		return (dat-'A'+10);
	if((dat >= 'a') && (dat <= 'f'))		return (dat-'a'+10);
	return 0xff;
}

u16	GetAddress(void)
{
	u16	address;
	u8	i;
	
	address = 0;
	if(COM1.RX_Cnt <  3)	return 65535;	//error
	if(COM1.RX_Cnt <= 5)	//5���ֽ�����������������ʮ����, ֧������: 	E 0, E 12, E 120
						//												W 0, W 12, W 120
						//												R 0, R 12, R 120
	{
		for(i=2; i<COM1.RX_Cnt; i++)
		{
			if(CheckData(RX1_Buffer[i]) > 9)	return 65535;	//error
			address = address * 10 + CheckData(RX1_Buffer[i]);
		}
		if(address < 124)	//������0~123����
		{
			address <<= 9;
			return (address);
		}
	}
	else if(COM1.RX_Cnt == 8)	//8���ֽ�ֱ�ӵ�ַ������ʮ������, ֧������: E 0x1234, W 0x12b3, R 0x0A00
	{
		if((RX1_Buffer[2] == '0') && ((RX1_Buffer[3] == 'x') || (RX1_Buffer[3] == 'X')))
		{
			for(i=4; i<8; i++)
			{
				if(CheckData(RX1_Buffer[i]) > 0x0F)		return 65535;	//error
				address = (address << 4) + CheckData(RX1_Buffer[i]);
			}
			if(address < 63488)	return (address);	//������0~123����
		}
	}
	
	 return	65535;	//error
}

/********************* ������ *************************/
void main(void)
{
	u8	i;
	u16 addr;

	UART_config();
	EA = 1;

	while(1)
	{
		delay_ms(1);
		if(COM1.RX_TimeOut > 0)		//��ʱ����
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(RX1_Buffer[1] == ' ')
				{
					addr = GetAddress();
					if(addr < 63488)	//������0~123����
					{
						if(RX1_Buffer[0] == 'E')	//PC�������һ������
						{
							EEPROM_SectorErase(addr);
							PrintString1("�����������");
						}

						else if(RX1_Buffer[0] == 'W')	//PC����д��EEPROM 64�ֽ�����
						{
							EEPROM_write_n(addr,T_Strings,64);
							PrintString1("д��������");
						}

						else if(RX1_Buffer[0] == 'R')	//PC���󷵻�64�ֽ�EEPROM����
						{
							PrintString1("�������������£�\r\n");
							EEPROM_read_n(addr,tmp,64);
							for(i=0; i<64; i++)	TX1_write2buff(tmp[i]);	//�����ݷ��ظ�����
							TX1_write2buff(0x0d);
							TX1_write2buff(0x0a);
						}
						else	PrintString1("�������!\r\n");
					}
					else	PrintString1("�������!\r\n");
				}

				COM1.RX_Cnt = 0;
			}
		}
	}
} 
/**********************************************/


