
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

ͨ�����ڶ�STC�ڲ��Դ���EEPROM(FLASH)���ж�д���ԡ�

��FLASH������������д�롢�����Ĳ���������ָ����ַ��

�̶�������:  9600,8,N,1. 
Ĭ����ʱ��:  22118400HZ.

������������: (��ĸ�����ִ�Сд)
	E 0x0012 	        --> ��EEPROM������������������ָ��ʮ�����Ƶ�ַ.
	W 0x0012 1234567890	--> ��EEPROM��������д�������ָ��ʮ�����Ƶ�ַ��. ����Ϊд������.
	R 0x0012 16	        --> ��EEPROM������������������ָ��ʮ�����Ƶ�ַ��. 

ע�⣺Ϊ��ͨ�ã�����ʶ���ַ�Ƿ���Ч���û��Լ����ݾ�����ͺ���������

*/

#include	"config.h"
#include	"EEPROM.h"


/*************	���س�������	**************/


/*************	���ر�������	**************/

#define Timer0_Reload		(65536 - MAIN_Fosc / BaudRate / 3)
#define RxLength		32		//���ջ��峤��
#define	Tmp_Length		32

sbit P_RXB = P3^0;			//����ģ�⴮�ڽ���IO
sbit P_TXB = P3^1;			//����ģ�⴮�ڷ���IO

u8	idata	tmp[Tmp_Length];

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

/*************	���غ�������	**************/
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
// ����: u8	CheckData(u8 dat)
// ����: ���ַ�"0~9,A~F��a~f"ת��ʮ������.
// ����: dat: Ҫ�����ַ�.
// ����: 0x00~0x0FΪ��ȷ. 0xFFΪ����.
// �汾: V1.0, 2012-10-22
//========================================================================
u8	CheckData(u8 dat)
{
	if((dat >= '0') && (dat <= '9'))		return (dat-'0');
	if((dat >= 'A') && (dat <= 'F'))		return (dat-'A'+10);
	return 0xff;
}

//========================================================================
// ����: u16	GetAddress(void)
// ����: ����������뷽ʽ�ĵ�ַ.
// ����: ��.
// ����: 16λEEPROM��ַ.
// �汾: V1.0, 2013-6-6
//========================================================================
u16	GetAddress(void)
{
	u16	address;
	u8	i,j;
	
	address = 0;
	if((RxBuf[2] == '0') && (RxBuf[3] == 'X'))
	{
		for(i=4; i<8; i++)
		{
			j = CheckData(RxBuf[i]);
			if(j >= 0x10)	return 65535;	//error
			address = (address << 4) + j;
		}
		return (address);
	}
	return	65535;	//error
}

/**************** ��ȡҪ�������ݵ��ֽ��� ****************************/
u8	GetDataLength(void)
{
	u8	i;
	u8	length;
	
	length = 0;
	for(i=9; i<Rx_Cnt; i++)
	{
		if(CheckData(RxBuf[i]) >= 10)	break;
		length = length * 10 + CheckData(RxBuf[i]);
	}
	return (length);
}

//========================================================================
// ����: void main(void)
// ����: ������.
// ����: ��.
// ����: ��.
// �汾: V1.0, 2012-10-22
//========================================================================
void	TxByte(u8 dat)
{
	TxSBUF = dat;		//Ҫ���͵�����д�뷢�ͻ���
	TxBusyFlag = 1;			//��������
	while(TxBusyFlag);		//�ȴ��������
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
	u8	i,j;
	u16 addr;

	Timer0_1T();
	Timer0_AsTimer();
	Timer0_16bitAutoReload();
	Timer0_Load(Timer0_Reload);
	Timer0_InterruptEnable();
//	PT0 = 1;	//��ʱ��0�����ȼ��ж�
	Timer0_Run();
	EA = 1;						//�����ж�					open global interrupt switch

	UART_INIT();				//UARTģ��ĳ�ʼ����
	TxString("STC15F/Lϵ�е�Ƭ��EEPROM���Գ���!\r\n");	//����һ����ʾ�ַ�
	
	while (1)
	{
		if (B_Rx_OK)	//������ı�־λ, �յ����ݿ�ϵͳ����1, �û��������ݺ������0
		{
			F0 = 0;
			if(Rx_Cnt > 0)	//ȷ��������
			{
		//		for(i=0; i<Rx_Cnt; i++)	TxByte(RxBuf[i]);	//���յ�������ԭ������,���ڲ���

				if((Rx_Cnt >= 8) && (RxBuf[1] == ' '))	//�������Ϊ8���ֽ�
				{
					for(i=0; i<8; i++)
					{
						if((RxBuf[i] >= 'a') && (RxBuf[i] <= 'z'))	RxBuf[i] = RxBuf[i] - 'a' + 'A';	//Сдת��д
					}
					addr = GetAddress();
					if(addr < 63488)	//������0~123����
					{
						if(RxBuf[0] == 'E')	//PC�������һ������
						{
							EEPROM_SectorErase(addr);
							TxString("�����������\r\n");
							F0 = 1;
						}

						else if((RxBuf[0] == 'W') && (Rx_Cnt >= 10) && (RxBuf[8] == ' '))	//д��N���ֽ�
						{
							j = Rx_Cnt - 9;
							if(j > Tmp_Length)	j = Tmp_Length;	//Խ����
							for(i=0; i<j; i++)	tmp[i] = 0xff;
							i = EEPROM_read_Compare(addr,tmp,j);	//���Ҫд��Ŀռ��Ƿ�Ϊ��
							if(i > 0)	TxString("Ҫд��ĵ�ַΪ�ǿ�,����д��,���Ȳ���!\r\n");
							else
							{
								EEPROM_write_n(addr,&RxBuf[9],j);		//дN���ֽ�
								i = EEPROM_read_Compare(addr,&RxBuf[9],j);	//�Ƚ�д�������
								if(i == 0)
								{
									TxString("��д��");
									if(j >= 100)	{TxByte(j/100+'0');	j = j % 100;}
									if(j >= 10)		{TxByte(j/10+'0');	j = j % 10;}
									TxByte(j%10+'0');
									TxString("�ֽ�����!\r\n");
								}
								else	TxString("д�����!\r\n");
							}
							F0 = 1;
						}

						else if((RxBuf[0] == 'R') && (Rx_Cnt >= 10) && (RxBuf[8] == ' '))	//PC���󷵻�N�ֽ�EEPROM����
						{
							j = GetDataLength();
							if(j > Tmp_Length)	j = Tmp_Length;	//Խ����
							if(j > 0)
							{
								TxString("����");
								TxByte(j/10+'0');
								TxByte(j%10+'0');
								TxString("���ֽ��������£�\r\n");
								EEPROM_read_n(addr,tmp,j);
								for(i=0; i<j; i++)	TxByte(tmp[i]);
								TxByte(0x0d);
								TxByte(0x0a);
								F0 = 1;
							}
						}
					}
				}
			}
			if(!F0)	TxString("�������!\r\n");
			Rx_Cnt  = 0;	//����ֽ���
			B_Rx_OK = 0;	//���������ɱ�־
		}
	}
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
}

