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



/****************************
��ʾ����Keil������������ѡ��Intel��8052оƬ�ͺŽ��б���

������MCU�Ĺ���Ƶ��Ϊ22.1184MHz.

ʹ��MCU�Դ��ıȽ�������ADCת��, ��ͨ������������. �ö�ʱ��0����10us�жϲ�ѯ�Ƚ�����״̬.

ʹ�ñȽ�����ADC, ԭ��ͼ����.
��ADC��ԭ���ǻ��ڵ��ƽ��ļ���ʽADC.
��ѹ��Vin����, ͨ��100K+104�˲�, ����Ƚ�����P5.5�������, �����Ƚ����ıȽ�, ����������P1.5��ͨ��100K+104�˲����ͱȽ���P5.4�������,�������ѹƽ��.
������������: ��������(����)adc_duty �� �ȽϽ���ߵ�ƽ�ļ���ֵ adc, adc�ϸ�����������ѹ.
ADC�Ļ�׼����P1.5�ĸߵ�ƽ. ����ߵ�ƽ׼ȷ,�Ƚ����ķŴ����㹻��,��ADC������׼ȷ.
���ȽϽ��Ϊ�ߵ�ƽ,��P1.5���1, ����adc+1.
���ȽϽ��Ϊ�͵�ƽ,��P1.5���0.
ÿһ�αȽ϶��жϼ��������Ƿ����,�����adc���ֵ����ADC���.
���ƽ�����ʽADC�����������������ñ��õ�˫����ADC, ����������Ϊ20ms�ı���ʱ,���к�ǿ�Ŀ���Ƶ��������,�ܺõ����Ժ;���.
ԭ����Բο�ADD3501(3 1/2λ�������ñ�)��ADD3701(3 3/4λ�������ñ�), Ҳ���Բο�AD7740 VFC��·.

��: �Ƚ�һ�ε�ʱ����Ϊ10us, ����Ϊ10000, ����1��ADC��ʱ��Ϊ100ms. �Ƚ�������Ӧʱ��Խ��, �����ADC��Խ��.

����Ҫ��ÿ�αȽ�ʱ������Ҫ���,������C�������ڶ�ʱ���ж������, ��ʱ������Ϊ�Զ���װ, �����ȼ��ж�, �����жϾ������ȼ�.
�û��Ļ�, ��֤�Ƚ������ƽ�����ʱ��Ҫ���.


                                         100K
                            /| P5.5       ___
                    P1.2   /+|---------o-|___|- ------- Vin
                     .----<  | P5.4    |
                     |     \-|---.     |
                     |      \|   |     |
                     |           |     |
                     |    ___    |     |
                     '---|___|---o     |
                        100K     |     |
                                ---   ---
                                ---   ---
                            104  |     |  104
                                 |     |
                                ===   ===
                                GND   GND



******************************/


#include	"config.h"
#include	"compare.h"
#include	"timer.h"
#include	"GPIO.h"
#include	"USART1.h"



/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/
void	TxString(u8 *puts);



/*************  �ⲿ�����ͱ������� *****************/
extern	bit	B_Timer0;
extern	u16		adc_value;		//ADCֵ, �û���ʹ��
extern	bit		adc_ok;			//ADC������־, Ϊ1��adc_value��ֵ����. �˱�־���û����ѯ,������0



/************************ �Ƚ������� ****************************/
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;					//�ṹ����
	CMP_InitStructure.CMP_EN = ENABLE;					//����Ƚ���		ENABLE,DISABLE
	CMP_InitStructure.CMP_RiseInterruptEn = DISABLE;	//�����������ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_FallInterruptEn = DISABLE;	//�����½����ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_P55;		//�Ƚ�������������ѡ��, CMP_P_P55: ѡ���ڲ�P5.5��������, CMP_P_ADCIS: ��ADCIS[2:0]��ѡ���ADC�������������.
	CMP_InitStructure.CMP_N_Select     = CMP_N_P54;		//�Ƚ������븺����ѡ��, CMP_N_BGv: ѡ���ڲ�BandGap��ѹBGv��������, CMP_N_P54: ѡ���ⲿP5.4������.
	CMP_InitStructure.CMP_OutptP12_En  = DISABLE;		//����ȽϽ�������P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = DISABLE;		//�Ƚ������ȡ��, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = ENABLE;		//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 20;			//�ȽϽ���仯��ʱ������, 0~63
//	CMP_InitStructure.CMP_Polity	   = PolityHigh;	//�ж����ȼ�,     PolityLow,PolityHigh
	CMP_Inilize(&CMP_InitStructure);				//��ʼ��Timer2	  Timer0,Timer1,Timer2
}


/************************ IO������ ****************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_HighZ;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);			//��ʼ��

	GPIO_InitStructure.Pin  = GPIO_Pin_2;				//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);			//��ʼ��
}

/************************ ��ʱ������ ****************************/

void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - MAIN_Fosc / 100000UL;	//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}

/*************  ����1��ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2
}

/******************** task A **************************/
void main(void)
{
	u8	i;
	u8	tmp[5];

	GPIO_config();
	Timer_config();
	CMP_config();
	UART_config();

	EA = 1;

	PrintString1("\r\nʹ�ñȽ�����ADC����\r\n");	//SUART1����һ���ַ���
	while(COM1.B_TX_busy > 0)	;	//�ȴ��������
	
	ES = 0;	//��ֹ�ж�, ����ʹ�ò�ѯ����

	while (1)
	{
		if(adc_ok)		//�ȴ�ADC����
		{
			adc_ok = 0;				//���ADC�ѽ�����־
			TxString("ADC = ");	//תʮ����
			tmp[0] = adc_value / 10000 + '0';
			tmp[1] = adc_value % 10000 / 1000 + '0';
			tmp[2] = adc_value % 1000 / 100 + '0';
			tmp[3] = adc_value % 100 / 10 + '0';
			tmp[4] = adc_value % 10 + '0';
			for(i=0; i<4; i++)		//����Ч0
			{
				if(tmp[i] != '0')	break;
				tmp[i] = ' ';
			}
			for(i=0; i<5; i++)		//������
			{
				TI = 0;
				SBUF = tmp[i];
				while(!TI);
				TI = 0;
			}
			TxString("\r\n");
		}
	}
}


void TxString(u8 *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)   	//����ֹͣ��0����
	{
		TI = 0;
		SBUF = *puts;
		while(!TI);
		TI = 0;
	}
}






