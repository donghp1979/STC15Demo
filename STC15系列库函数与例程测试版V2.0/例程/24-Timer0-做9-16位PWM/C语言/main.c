
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

#include	<reg52.h>


/*************	����˵��	**************

��������ʾʹ�ö�ʱ�������PWM��

��ʱ��0��16λ�Զ���װ���жϣ���T0CLKO�������PWM��

��������ʹ��STC15F/Lϵ��MCU�Ķ�ʱ��T0��ģ��PWM�����̡�

PWM��������������̡��������������װ��Ҫһ��ʱ�䣬����PWMռ�ձ���СΪ32T/���ڣ����Ϊ(����-32T)/����, TΪʱ�����ڡ�

PWMƵ��Ϊ���ڵĵ�������������Ϊ6000, ʹ��24MHZ����Ƶ����PWMƵ��Ϊ4000HZ��

******************************************/

#define 	MAIN_Fosc		24000000UL		//������ʱ��

#define		PWM_DUTY		6000			//����PWM�����ڣ���ֵΪʱ��������������ʹ��24.576MHZ����Ƶ����PWMƵ��Ϊ6000HZ��

#define		PWM_HIGH_MIN	32				//����PWM�������Сռ�ձȡ��û������޸ġ�
#define		PWM_HIGH_MAX	(PWM_DUTY-PWM_HIGH_MIN)	//����PWM��������ռ�ձȡ��û������޸ġ�

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
sfr	AUXR  = 0x8E;
sfr INT_CLKO = 0x8F;

sbit	P_PWM = P3^5;		//����PWM������š�
//sbit	P_PWM = P1^4;		//����PWM������š�STC15W204S

u16		pwm;				//����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����

u16		PWM_high,PWM_low;	//�м�������û������޸ġ�

void  	delay_ms(unsigned char ms);
void	LoadPWM(u16 i);


/******************** ������**************************/
void main(void)
{
	P_PWM = 0;
	P3M1 &= ~(1 << 5);	//P3.5 ����Ϊ�������
	P3M0 |=  (1 << 5);

//	P1M1 &= ~(1 << 4);	//P1.4 ����Ϊ�������	STC15W204S
//	P1M0 |=  (1 << 4);

	TR0 = 0;		//ֹͣ����
	ET0 = 1;		//�����ж�
	PT0 = 1;		//�����ȼ��ж�
	TMOD &= ~0x03;	//����ģʽ,0: 16λ�Զ���װ
	AUXR |=  0x80;	//1T
	TMOD &= ~0x04;	//��ʱ
	INT_CLKO |=  0x01;	//���ʱ��

	TH0 = 0;
	TL0 = 0;
	TR0 = 1;	//��ʼ����

	EA = 1;

	pwm = PWM_DUTY / 10;	//��PWMһ����ֵ������Ϊ10%ռ�ձ�
	LoadPWM(pwm);			//����PWM��װֵ


	while (1)
	{
		while(pwm < (PWM_HIGH_MAX-8))
		{
			pwm += 8;		//PWM�𽥼ӵ����
			LoadPWM(pwm);
			delay_ms(8);
		}
		while(pwm > (PWM_HIGH_MIN+8))
		{
			pwm -= 8;	//PWM�𽥼�����С
			LoadPWM(pwm);
			delay_ms(8);
		}
	}
}


//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;
     }while(--ms);
}

/**************** ����PWM��װֵ���� *******************/
void	LoadPWM(u16 i)
{
	u16	j;

	if(i > PWM_HIGH_MAX)		i = PWM_HIGH_MAX;	//���д��������ռ�ձ����ݣ���ǿ��Ϊ���ռ�ձȡ�
	if(i < PWM_HIGH_MIN)		i = PWM_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ���ǿ��Ϊ��Сռ�ձȡ�
	j = 65536UL - PWM_DUTY + i;	//����PWM�͵�ƽʱ��
	i = 65536UL - i;			//����PWM�ߵ�ƽʱ��
	EA = 0;
	PWM_high = i;	//װ��PWM�ߵ�ƽʱ��
	PWM_low  = j;	//װ��PWM�͵�ƽʱ��
	EA = 1;
}

/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt 1
{
	if(P_PWM)
	{
		TH0 = (u8)(PWM_low >> 8);	//���������ߵ�ƽ����װ�ص͵�ƽʱ�䡣
		TL0 = (u8)PWM_low;
	}
	else
	{
		TH0 = (u8)(PWM_high >> 8);	//���������͵�ƽ����װ�ظߵ�ƽʱ�䡣
		TL0 = (u8)PWM_high;
	}
}



