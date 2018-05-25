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
#include	"compare.h"
#include	"timer.h"
#include	"GPIO.h"

/*************	����˵��	**************

ͨ�ó���


�����ļ�:
inilize.c
interrupt.c
main.c

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

u8	cnt0,cnt2;

sbit	P_TXD = P3^1;	//����ģ�⴮�ڷ��Ͷ�,����������IO

void	TxSend(u8 dat);
void 	PrintString(unsigned char code *puts);

/*************	���غ�������	**************/



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


/******************** task A **************************/
void main(void)
{
	u8	i;
	u8	tmp[5];

P20 = 1;
	GPIO_config();
	Timer_config();
	CMP_config();

	EA = 1;

	PrintString("\r\nʹ�ñȽ�����ADC����\r\n");

	while (1)
	{
		if(adc_ok)
		{
			adc_ok = 0;				//���ADC�ѽ�����־
			adc_value += 5;
			adc_value /= 10;
			PrintString("ADC = ");	//תʮ����
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
			for(i=0; i<5; i++)	TxSend(tmp[i]);	//������
			PrintString("\r\n");
		}
//TxSend(0);
	/*	if(B_Timer0)
		{
			B_Timer0 = 0;
			
			
		}*/
	}
}


//========================================================================
// ����: void	BitTime(void)
// ����: λʱ�亯����
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void	BitTime(void)
{
	u16 i;
	i = ((MAIN_Fosc / 100) * 80) / 130000L - 1;		//������ʱ��������λʱ��	104
	while(--i);
}

//========================================================================
// ����: void	TxSend(uchar dat)
// ����: ģ�⴮�ڷ���һ���ֽڡ�9600��N��8��1
// ����: dat: Ҫ���͵������ֽ�.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void	TxSend(u8 dat)
{
	u8	i;
//	EA = 0;
	P_TXD = 0;
	BitTime();
	for(i=0; i<8; i++)
	{
		if(dat & 1)		P_TXD = 1;
		else			P_TXD = 0;
		dat >>= 1;
		BitTime();
	}
	P_TXD = 1;
//	EA = 1;
	BitTime();
	BitTime();
}

//========================================================================
// ����: void PrintString(unsigned char code *puts)
// ����: ģ�⴮�ڷ���һ���ַ�����9600��N��8��1
// ����: *puts: Ҫ���͵��ַ�ָ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void PrintString(unsigned char code *puts)
{
    for (; *puts != 0;	puts++)  TxSend(*puts);
}



