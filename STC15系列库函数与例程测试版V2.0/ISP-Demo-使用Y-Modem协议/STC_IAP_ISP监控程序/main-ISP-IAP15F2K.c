

#define MAIN_Fosc		22118400L	//������ʱ��
#define Baudrate0		115200UL		//����0������600~115200

#include	"STC15Fxxxx.H"
#include	"ymodem.h"


#define	WDT_Enable	0			//1: �����ڲ����Ź�,	0: ��ֹ

#define D_BRT0_1T_16bit		(65536UL - MAIN_Fosc / 4 / Baudrate0)		//���㲨����	1T  16bit



#define	ISP_ADDRESS		0xE400				//ISP��ʼ��ַ, �ߵ�ַ������ż��, ע��ҪԤ��ISP�ռ�,��������3K
#define	UserflashLenth 	(ISP_ADDRESS-3)		//�û�FLASH����, �������ֽڴ���û���ַ�������ת��ַ



/*************	���ر�������	**************/

typedef		void (*pFunction)(void);
pFunction	Jump_To_Application;
u16			JumpAddress;

u16		FileLength;
u16		Y_TimeOut,WaitTime;
u8		packets_received, session_begin;	//session -- �Ự
u8		file_name[FILE_NAME_LENGTH];
u8		xdata file_size[FILE_SIZE_LENGTH];
u8		StartCode[3];
u16		FlashDestination;
u16		RxCnt;
u8		HandCnt;
u8	xdata	RxBuff[1032];

u32		DownCheckSum,FlashCheckSum;


/*************	���غ�������	**************/
u8		Hex2Ascii(u8 dat);
u8		UART_Download(void);
u16		Str2Int(u8 *inputstr);
void	ISP_WriteByte(u16 addr, u8 dat);
void	ISP_EraseSector(u16 addr);
u8	ISP_ReadByte(u16 addr);




//========================================================================
// ����: void	TX1_write2buff(u8 dat)
// ����: ���ڷ���һ���ֽ�.
// ����: dat: Ҫ���͵��ֽ�.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	TX1_write2buff(u8 dat)
{
	TI = 0;
	SBUF = dat;
	while(!TI);
}

//========================================================================
// ����: void	PrintString1(u8 *p)
// ����: ���ڷ���һ���ַ���.
// ����: *p: Ҫ���͵��ַ���ָ��.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	PrintString1(u8 *p)
{
	for(; *p > 0; p++)	TX1_write2buff(*p);
}

//========================================================================
// ����: void	Tx_DEC_U16(u16 j)
// ����: ��һ��16λ������ת��ʮ�����ʹ��ڷ���.
// ����: j: Ҫ�����16λ������.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	Tx_DEC_U16(u16 j)
{
	u8	i;
	u8	tmp[10];
	for(i=4; i<5; i--)	tmp[i] = j % 10 + '0',	j = j / 10;
	for(i=0; i<4; i++)
	{
		if(tmp[i] != '0')	break;
		tmp[i] = ' ';
	}
	for(i=0; i<5; i++)	TX1_write2buff(tmp[i]);
}

//========================================================================
// ����: void	Tx_HEX_U32(u32 j)
// ����: ��һ��32λ��������ת��ʮ�����ʹ��ڷ���.
// ����: j: Ҫ�����32λ������.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	Tx_HEX_U32(u32 j)
{
	u8	i,k;
	u8	tmp[10];
	for(i=8; i>0; i--)
	{
		k = ((u8)j) & 0x0f;
		if(k <= 9)	tmp[i] = k+'0';
		else		tmp[i] = k-10+'A';
		j >>= 4;
	}
	for(i=1; i<9; i++)	TX1_write2buff(tmp[i]);
}


//========================================================================
// ����: void	ReturnNameAndLength(void)
// ����: ���س�����ļ����ͳ���, ���ۼ�У���.
// ����: none.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	ReturnNameAndLength(void)
{
	u16	i;
	PrintString1("================================\r\n File name:     ");
	for(i=0; i<FILE_NAME_LENGTH; i++)	
	{
		if(file_name[i] == 0)	break;
		TX1_write2buff(file_name[i]);
	}
	PrintString1("\r\n File length:   ");
	Tx_DEC_U16(FileLength);
	PrintString1(" Bytes\r\n DownChexkSum:  ");
	Tx_HEX_U32(DownCheckSum);
	PrintString1("\r\n ISP Versiom:   2013-4-29 by Coody");
	PrintString1("\r\n================================\r\n\r\n");
}

//========================================================================
// ����: void	UART1_RxPackage(void)
// ����: ���ڽ���һ�����ݿ�.
// ����: none.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	UART1_RxPackage(void)
{
	u16	j;	//5ms��ʱ
//	RI = 0;
	RxCnt = 0;
	for(j = 0;	j < 5300; j++)	//����յ�һ���ֽ�5ms��,��ʱ�˳�	5300
	{
		if(RI)
		{
			RI = 0;
			RxBuff[RxCnt] = SBUF;
			if(++RxCnt >= 1032)	RxCnt = 0;
			j = 0;	//���¶�ʱ5ms
		}
	}
	#if (WDT_Enable > 0)
		WDT_reset(D_WDT_SCALE_256);
	#endif
}

//========================================================================
// ����: void	main(void)
// ����: ������.
// ����: none.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	main(void)
{
	u8	i;

	EA = 0;
	Timer0_Stop();
	Timer1_Stop();
	#if (WDT_Enable > 0)
		WDT_reset(D_WDT_SCALE_256);
	#endif

	S1_USE_P30P31();
	S1_8bit();
	S1_RX_Enable();
	S1_BRT_UseTimer1();
	S1_TXD_RXD_OPEN();
	Timer1_AsTimer();
	Timer1_1T();
	Timer1_16bitAutoReload();
	Timer1_InterruptDisable();
	TH1 = D_BRT0_1T_16bit / 256;
	TL1 = D_BRT0_1T_16bit % 256;
	Timer1_Run();

//	PrintString1("\r\n STC ISP Demo\r\n");
	
	while(1)
	{
	//	TX1_write2buff('A');
		HandCnt = 0;
		for(WaitTime = 0; WaitTime < 300; WaitTime++)		//1.5�볬ʱ, �ȴ�����u	
		{
			UART1_RxPackage();
			if((RxCnt == 1) && (RxBuff[0] == 'd'))
			{
				if(++HandCnt >= 10)
				{
					i = UART_Download();
					WaitTime = 250;
					if(i == 1)			PrintString1("\r\n User abort!\r\n");
					else if(i == 2)		PrintString1("\r\n PC Cancel!\r\n");
					else if(i == 4)		PrintString1("\r\n Programming Error!\r\n");
					else if(i == 0)
					{
						PrintString1("\r\n\r\n Programming Completed Successfully!\r\n");
						ReturnNameAndLength();
					}
				}
			}
		//	else	HandCnt = 0;
		}

		if(ISP_ReadByte(ISP_ADDRESS-3) == 0x02)
		{
            SCON = 0;
			AUXR = 0;
            TMOD = 0;
            TL0 = 0;
            TH0 = 0;
            TH1 = 0;
            TL1 = 0;
            TCON = 0;
			IAP_CMD = 0;

			JumpAddress = ISP_ReadByte(ISP_ADDRESS-2);
			JumpAddress = (JumpAddress << 8) | ISP_ReadByte(ISP_ADDRESS-1);	//Jump to user application
			Jump_To_Application = (pFunction) JumpAddress;
			Jump_To_Application();
		}
		PrintString1(" No AP\r\n");
	}
}


//========================================================================
// ����: u16 Str2Int(u8 *inputstr)
// ����: �ַ���ת����.
// ����: *inputstr: �ַ���ָ��.
// ����: 16λ������.
// �汾: V1.0, 2013-4-29
//========================================================================
u16 Str2Int(u8 *inputstr)
{
	u16	val;
	u8	i;
	
	val = 0;
	for (i = 0; i < 5; i++)
	{
		if((inputstr[i] < '0') || (inputstr[i] > '9'))	break;
		val = val * 10 + inputstr[i] - '0';
	}
	return val;
}

//========================================================================
// ����: void	ISP_WriteByte(u16 addr, u8 dat)
// ����: ��һ����ַдFLASHһ���ֽ�.
// ����: addr: 16λFLASH��ַ.
//       dat:  Ҫд���һ�ֽ�����.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	ISP_WriteByte(u16 addr, u8 dat)
{
	IAP_CONTR = ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY);	//ʹ��IAP����
	ISP_WRITE();			//�������
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	IAP_DATA  = dat;		//����ǰ������IAP���ݼĴ���
	ISP_TRIG();				//����ISP����
}

//========================================================================
// ����: u8	ISP_ReadByte(u16 addr)
// ����: ��һ����ַ��FLASHһ���ֽ�.
// ����: addr: 16λFLASH��ַ.
// ����: ������һ�ֽ�����.
// �汾: V1.0, 2013-4-29
//========================================================================
u8	ISP_ReadByte(u16 addr)
{
	IAP_CONTR = ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY);	//ʹ��IAP����
	ISP_READ();			//�������
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	ISP_TRIG();				//����ISP����
	return(IAP_DATA);
}

//========================================================================
// ����: void	ISP_EraseSector(u16 addr)
// ����: ��FLASH����һ������.
// ����: addr: 16λFLASH��ַ.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
void	ISP_EraseSector(u16 addr)
{
	IAP_CONTR = ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY);		//ʹ��IAP����
	ISP_ERASE();				//��������
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	ISP_TRIG();				//����ISP����
}

//========================================================================
// ����: u8	UART_Download(void)
// ����: ��Ymodem�����ļ����ݲ�д���û�FLASH.
// ����: none.
// ����: none.
// �汾: V1.0, 2013-4-29
//========================================================================
u8	UART_Download(void)
{
	u16	i;
	u8	j;

	PrintString1("\r\n\r\n Waiting for the file to be sent ... (press 'a' to abort)\r\n");
	
	Y_TimeOut = 0;
	packets_received = 0;
	session_begin = 0;
	WaitTime = 40;
	DownCheckSum = 0;
	
	while(WaitTime > 0)
	{
		if(Y_TimeOut == 0)
		{
			TX1_write2buff(CRC16);
			Y_TimeOut = 300;
			if(WaitTime > 0) WaitTime--;
		}

		while(Y_TimeOut > 0)
		{
			UART1_RxPackage();
			if(RxCnt == 0)	Y_TimeOut--;
			else
			{
				if(RxCnt == 1)	//1�ֽ�
				{
					if(RxBuff[0] == EOT)	//�յ�EOT, ����
					{
						TX1_write2buff(ACK);
						Y_TimeOut = 40;
					}
					else if((RxBuff[0] == ABORT1) || (RxBuff[0] == ABORT2))	//�յ�A��a, ȡ��
					{
						return 1;    //ȡ��
					}
				}
				else if(RxCnt <= 5)	//С��5�ֽ�
				{
					if((RxBuff[0] == CANCEL) && (RxBuff[1] == CANCEL))	//�յ�����CANCEL
					{
						return 2;	//PC��ȡ��
					}
				}
				else if((RxCnt == 133) || (RxCnt == 1029))	//���ݰ�
				{
					if (RxBuff[PACKET_SEQNO_INDEX] != (RxBuff[PACKET_SEQNO_COMP_INDEX] ^ 0xff))	//�жϷ�������Ƿ���ȷ
					{
						TX1_write2buff(NAK);	//����, �����ط�
						Y_TimeOut = 300;
					}
					else
					{
						WaitTime = 5;
						if (packets_received == 0)	//�������Ϊ0, Ϊ�ļ������ݰ�
						{
							if (RxBuff[PACKET_HEADER] != 0)  //�ļ�����Ϊ��
							{
								for (i = 0; i < FILE_NAME_LENGTH; i++)	file_name[i] = 0;
								for (i = 0; i < FILE_SIZE_LENGTH; i++)	file_size[i] = 0;
								j = PACKET_HEADER;
								for (i = 0; (i < FILE_NAME_LENGTH) && (RxBuff[j] != 0); i++)
									file_name[i] = RxBuff[j++];		//�����ļ���

								for (i=0, j++; (RxBuff[j] != ' ') && (i < FILE_SIZE_LENGTH); i++)
									file_size[i] = RxBuff[j++];	//�����ļ�����
								FileLength = Str2Int(file_size);	//�ļ��������ַ���ת��ʮ����������

								if (FileLength >= UserflashLenth)	//���ȹ�������
								{
									TX1_write2buff(CANCEL);	//���󷵻�2�� CA
									TX1_write2buff(CANCEL);
									return 3;		//���ȹ���
								}
								
								StartCode[0] = ISP_ReadByte(0);	//����ISP��ת��ַ
								StartCode[1] = ISP_ReadByte(1);
								StartCode[2] = ISP_ReadByte(2);
								ISP_EraseSector(0);
								ISP_WriteByte(0,StartCode[0]);	//��дISP��ת��ַ
								ISP_WriteByte(1,StartCode[1]);
								ISP_WriteByte(2,StartCode[2]);
								for(i=0x200; i < UserflashLenth; i+=0x200)	//����Nҳ
									ISP_EraseSector(i);
							
								TX1_write2buff(ACK);	//�������, ����Ӧ��
								Y_TimeOut = 40;
								packets_received ++;
								session_begin = 1;
								FlashDestination = 0;
								DownCheckSum = 0;
							}
						}

						/* Filename packet is empty, end session ȫ0���ݰ�,�Ự����*/
						else if(session_begin == 1)	//�չ��ļ���
						{
							if(RxBuff[PACKET_SEQNO_INDEX] == 0)	//ȫ0����֡
							{
								ISP_WriteByte(ISP_ADDRESS-3,StartCode[0]);	//ȫ�����ؽ���,���д�û���ڵ�ַ
								ISP_WriteByte(ISP_ADDRESS-2,StartCode[1]);
								ISP_WriteByte(ISP_ADDRESS-1,StartCode[2]);
								TX1_write2buff(ACK);
								
								FlashCheckSum  = ISP_ReadByte(ISP_ADDRESS-3);
								FlashCheckSum += ISP_ReadByte(ISP_ADDRESS-2);
								FlashCheckSum += ISP_ReadByte(ISP_ADDRESS-1);
								for(i = 3;	i < FileLength; i++)	FlashCheckSum += ISP_ReadByte(i);	//����FLASH�ۼӺ�
								if(FlashCheckSum == DownCheckSum)	return 0;	//��ȷ
								else
								{
									ISP_EraseSector(ISP_ADDRESS-0x200);
									return 4;	//д�����
								}
							}
							else	//����֡
							{
								RxCnt -= 2;
								for(i = PACKET_HEADER; (i < RxCnt) && (FlashDestination < FileLength); i++)
								{
									if(FlashDestination == 0)
									{
										StartCode[0] = RxBuff[3];
										StartCode[1] = RxBuff[4];
										StartCode[2] = RxBuff[5];
										FlashDestination = 3;
										i += 3;
										DownCheckSum += RxBuff[3]; DownCheckSum += RxBuff[4]; DownCheckSum += RxBuff[5];
									}
									ISP_WriteByte(FlashDestination,RxBuff[i]);
									DownCheckSum += RxBuff[i];
									FlashDestination ++;
								}
								TX1_write2buff(ACK);	//�������, ����Ӧ��
								Y_TimeOut = 300;
								packets_received ++;
							}
						}
					}
				}
			}
		}
	}
	return 100;	//��������
}

