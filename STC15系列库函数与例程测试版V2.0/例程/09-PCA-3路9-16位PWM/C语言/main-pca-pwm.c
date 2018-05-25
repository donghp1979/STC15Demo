
/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
/*---------------------------------------------------------------------*/


#include	<reg52.h>


/*************	����˵��	**************

���3·9~16λPWM�źš�

PWMƵ�� = MAIN_Fosc / PWM_DUTY, ���� MAIN_Fosc = 24MHZ, PWM_DUTY = 6000, �����PWMƵ��Ϊ4000HZ.

******************************************/

/***************************�û��궨��*******************************************************/
#define 	MAIN_Fosc		24000000UL	//������ʱ��

#define		PWM_DUTY		6000		//����PWM�����ڣ���ֵΪPCA��ѡ���ʱ�����������
#define		PWM_HIGH_MIN	80			//����PWM�������Сռ�ձ�, �����ж�����װ����ʱ�䲻����
#define		PWM_HIGH_MAX	(PWM_DUTY - PWM_HIGH_MIN)		//����PWM��������ռ�ձȡ�

/********************************************************************************************/

#define	PCA0			0
#define	PCA1			1
#define	PCA2			2
#define	PCA_Counter		3
#define	PCA_P12_P11_P10_P37	(0<<4)
#define	PCA_P34_P35_P36_P37	(1<<4)
#define	PCA_P24_P25_P26_P27	(2<<4)
#define	PCA_Mode_PWM				0x42
#define	PCA_Mode_Capture			0
#define	PCA_Mode_SoftTimer			0x48
#define	PCA_Mode_HighPulseOutput	0x4c
#define	PCA_Clock_1T	(4<<1)
#define	PCA_Clock_2T	(1<<1)
#define	PCA_Clock_4T	(5<<1)
#define	PCA_Clock_6T	(6<<1)
#define	PCA_Clock_8T	(7<<1)
#define	PCA_Clock_12T	(0<<1)
#define	PCA_Clock_Timer0_OF	(2<<1)
#define	PCA_Clock_ECI	(3<<1)
#define	PCA_Rise_Active	(1<<5)
#define	PCA_Fall_Active	(1<<4)
#define	PCA_PWM_8bit	(0<<6)
#define	PCA_PWM_7bit	(1<<6)
#define	PCA_PWM_6bit	(2<<6)

#define		ENABLE		1
#define		DISABLE		0

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

sfr AUXR1 = 0xA2;
sfr CCON = 0xD8;
sfr CMOD = 0xD9;
sfr CCAPM0 = 0xDA;		//PCAģ��0�Ĺ���ģʽ�Ĵ�����
sfr CCAPM1 = 0xDB;		//PCAģ��1�Ĺ���ģʽ�Ĵ�����
sfr CCAPM2 = 0xDC;		//PCAģ��2�Ĺ���ģʽ�Ĵ�����

sfr CL     = 0xE9;
sfr CCAP0L = 0xEA;		//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1L = 0xEB;		//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP2L = 0xEC;		//PCAģ��2�Ĳ�׽/�ȽϼĴ�����8λ��

sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1H = 0xFB;		//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP2H = 0xFC;		//PCAģ��2�Ĳ�׽/�ȽϼĴ�����8λ��

sbit CCF0  = CCON^0;	//PCA ģ��0�жϱ�־����Ӳ����λ�������������0��
sbit CCF1  = CCON^1;	//PCA ģ��1�жϱ�־����Ӳ����λ�������������0��
sbit CCF2  = CCON^2;	//PCA ģ��2�жϱ�־����Ӳ����λ�������������0��
sbit CR    = CCON^6;	//1: ����PCA������������0: ��ֹ������
sbit CF    = CCON^7;	//PCA�����������CH��CL��FFFFH��Ϊ0000H����־��
                        //PCA�������������Ӳ����λ�������������0��
sbit PPCA  = IP^7;		//PCA �ж� ���ȼ��趨λ

sfr P2M1 = 0x95;	//P2M1.n,P2M0.n 	=00--->Standard,	01--->push-pull
sfr P2M0 = 0x96;	//					=10--->pure input,	11--->open drain

//================================================================

sbit	P25 = P2^5;
sbit	P26 = P2^6;
sbit	P27 = P2^7;

u16		CCAP0_tmp,PWM0_high,PWM0_low;
u16		CCAP1_tmp,PWM1_high,PWM1_low;
u16		CCAP2_tmp,PWM2_high,PWM2_low;

u16	pwm0,pwm1,pwm2;

void	PWMn_Update(u8 PCA_id, u16 pwm);
void	PCA_Init(void);
void	delay_ms(u8 ms);

/******************** ������ **************************/
void main(void)
{
	
	PCA_Init();	//PCA��ʼ��
	EA = 1;
	P2M1 &= ~(0xe0);	//P2.7 P2.6 P2.5 ����Ϊ�������
	P2M0 |=  (0xe0);
	
	while (1)
	{
		delay_ms(2);

		if(++pwm0 >= PWM_HIGH_MAX)	pwm0 = PWM_HIGH_MIN;
		PWMn_Update(PCA0,pwm0);

		if(++pwm1 >= PWM_HIGH_MAX)	pwm1 = PWM_HIGH_MIN;
		PWMn_Update(PCA1,pwm1);

		if(++pwm2 >= PWM_HIGH_MAX)	pwm2 = PWM_HIGH_MIN;
		PWMn_Update(PCA2,pwm2);
	}
}


//========================================================================
// ����: void  delay_ms(u8 ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(u8 ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;
     }while(--ms);
}


//========================================================================
// ����: void PWMn_SetHighReg(unsigned int high)
// ����: ����ռ�ձ����ݡ�
// ����: high: 	ռ�ձ����ݣ���PWM����ߵ�ƽ��PCAʱ�����������
// ����: ��
// �汾: VER1.0
// ����: 2013-5-15
// ��ע: 
//========================================================================
void PWMn_Update(u8 PCA_id, u16 pwm)
{
	if(pwm > PWM_HIGH_MAX)	pwm = PWM_HIGH_MAX;	//���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
	if(pwm < PWM_HIGH_MIN)	pwm = PWM_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ�ǿ��Ϊ��Сռ�ձȡ�

	if(PCA_id == PCA0)
	{
		CR = 0;						//ֹͣPCAһ�ᣬ һ�㲻��Ӱ��PWM��
		PWM0_high = pwm;			//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM0_low = PWM_DUTY - pwm;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;						//����PCA��
	}
	else if(PCA_id == PCA1)
	{
		CR = 0;						//ֹͣPCA��
		PWM1_high = pwm;			//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM1_low = PWM_DUTY - pwm;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;						//����PCA��
	}
	else if(PCA_id == PCA2)
	{
		CR = 0;						//ֹͣPCA��
		PWM2_high = pwm;			//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM2_low = PWM_DUTY - pwm;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;						//����PCA��
	}
}

//========================================================================
// ����: void	PCA_Init(void)
// ����: PCA��ʼ������.
// ����: none
// ����: none.
// �汾: V1.0, 2013-11-22
//========================================================================
void	PCA_Init(void)
{
	CR = 0;
	AUXR1 = (AUXR1 & ~(3<<4)) | PCA_P24_P25_P26_P27;	//�л�IO��
	CCAPM0 = (PCA_Mode_HighPulseOutput | ENABLE);	//16λ�����ʱ����������������ж�ģʽ
	CCAPM1 = (PCA_Mode_HighPulseOutput | ENABLE);
	CCAPM2 = (PCA_Mode_HighPulseOutput | ENABLE);

	CH = 0;
	CL = 0;
	CMOD  = (CMOD  & ~(7<<1)) | PCA_Clock_1T;			//ѡ��ʱ��Դ
	PPCA  = 1;	// �����ȼ��ж�

	pwm0 = (PWM_DUTY / 4 * 1);	//��PWMһ����ֵ
	pwm1 = (PWM_DUTY / 4 * 2);
	pwm2 = (PWM_DUTY / 4 * 3);

	PWMn_Update(PCA0,pwm0);
	PWMn_Update(PCA1,pwm1);
	PWMn_Update(PCA2,pwm2);

	CR    = 1;	// ����PCA��ʱ��
}
//======================================================================

//========================================================================
// ����: void	PCA_Handler (void) interrupt 7
// ����: PCA�жϴ������.
// ����: None
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
void	PCA_Handler (void) interrupt 7
{
	if(CCF0)		//PCAģ��0�ж�
	{
		CCF0 = 0;		//��PCAģ��0�жϱ�־
		if(P25)	CCAP0_tmp += PWM0_high;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		else	CCAP0_tmp += PWM0_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
		CCAP0L = (u8)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP0H = (u8)(CCAP0_tmp >> 8);	//��дCCAP0H
	}

	if(CCF1)	//PCAģ��1�ж�
	{
		CCF1 = 0;		//��PCAģ��1�жϱ�־
		if(P26)	CCAP1_tmp += PWM1_high;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		else	CCAP1_tmp += PWM1_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
		CCAP1L = (u8)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP1H = (u8)(CCAP1_tmp >> 8);	//��дCCAP0H
	}

	if(CCF2)	//PCAģ��2�ж�
	{
		CCF2 = 0;		//��PCAģ��1�жϱ�־
		if(P27)	CCAP2_tmp += PWM2_high;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		else	CCAP2_tmp += PWM2_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
		CCAP2L = (u8)CCAP2_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP2H = (u8)(CCAP2_tmp >> 8);	//��дCCAP0H
	}
}



