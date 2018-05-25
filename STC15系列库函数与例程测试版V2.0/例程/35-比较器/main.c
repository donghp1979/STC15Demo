
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

/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/
extern	bit	B_Timer0;

/************************ �Ƚ������� ****************************/
void	CMP_config(void)
{
	CMP_InitDefine CMP_InitStructure;					//�ṹ����
	CMP_InitStructure.CMP_EN = ENABLE;					//����Ƚ���		ENABLE,DISABLE
	CMP_InitStructure.CMP_RiseInterruptEn = DISABLE;		//�����������ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_FallInterruptEn = DISABLE;		//�����½����ж�	ENABLE,DISABLE
	CMP_InitStructure.CMP_P_Select     = CMP_P_P55;		//�Ƚ�������������ѡ��, CMP_P_P55: ѡ���ڲ�P5.5��������, CMP_P_ADCIS: ��ADCIS[2:0]��ѡ���ADC�������������.
	CMP_InitStructure.CMP_N_Select     = CMP_N_BGv;		//�Ƚ������븺����ѡ��, CMP_N_BGv: ѡ���ڲ�BandGap��ѹBGv��������, CMP_N_P54: ѡ���ⲿP5.4������.
	CMP_InitStructure.CMP_OutptP12_En  = ENABLE;		//����ȽϽ�������P1.2,   ENABLE,DISABLE
	CMP_InitStructure.CMP_InvCMPO      = ENABLE;		//�Ƚ������ȡ��, 	ENABLE,DISABLE
	CMP_InitStructure.CMP_100nsFilter  = DISABLE;		//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE
	CMP_InitStructure.CMP_OutDelayDuty = 0;			//�ȽϽ���仯��ʱ������, 0~63
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
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);			//��ʼ��
}

/************************ ��ʱ������ ****************************/
/*
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - ((1000 * Main_Fosc_KHZ) / 1000);	//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}
*/

/******************** task A **************************/
void main(void)
{
P20 = 1;
	GPIO_config();
//	Timer_config();
	CMP_config();

	EA = 1;

	while (1)
	{
//MCU_IDLE();
NOP(4);
	/*	if(B_Timer0)
		{
			B_Timer0 = 0;
			
			
		}*/
	}
}



