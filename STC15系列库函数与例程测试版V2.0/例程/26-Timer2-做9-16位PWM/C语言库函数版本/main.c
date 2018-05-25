
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
#include	"timer.h"
#include	"delay.h"


/*************	����˵��	**************

��������ʾʹ�ö�ʱ�������PWM��

��ʱ��2��16λ�Զ���װ���жϣ���T2CLKO�������PWM��

��������ʹ��STC15F/Lϵ��MCU�Ķ�ʱ��T2��ģ��PWM�����̡�

PWM��������������̡��������������װ��Ҫһ��ʱ�䣬����PWMռ�ձ���СΪ32T/���ڣ����Ϊ(����-32T)/����, TΪʱ�����ڡ�

PWMƵ��Ϊ���ڵĵ�������������Ϊ4096, ʹ��24.576MHZ����Ƶ����PWMƵ��Ϊ6000HZ��

******************************************/

/*************	���س�������	**************/

#define		PWM_DUTY		4096			//����PWM�����ڣ���ֵΪʱ��������������ʹ��24.576MHZ����Ƶ����PWMƵ��Ϊ6000HZ��
#define		PWM_ON			1				//����PWM���ռ�ձȵļ���

#define		PWM_HIGH_MAX	(PWM_DUTY-32)	//����PWM��������ռ�ձȡ��û������޸ġ�
#define		PWM_HIGH_MIN	32				//����PWM�������Сռ�ձȡ��û������޸ġ�


/*************	���ر�������	**************/

u16		PWM_high,PWM_low;	//�м�������û������޸ġ�

u16		pwm;				//����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����



/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/




/************************ ��ʱ������ ****************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - PWM_HIGH_MIN;	//��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer2,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
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



/******************** ������**************************/
void main(void)
{

	Timer_config();
	EA = 1;

	pwm = PWM_DUTY / 10;	//��PWMһ����ֵ������Ϊ10%ռ�ձ�
	LoadPWM(pwm);			//����PWM��װֵ

	while (1)
	{
		while(pwm < PWM_HIGH_MAX)
		{
			pwm += 8;		//PWM�𽥼ӵ����
			LoadPWM(pwm);
			delay_ms(8);
		}
		while(pwm > PWM_HIGH_MIN)
		{
			pwm -= 8;	//PWM�𽥼�����С
			LoadPWM(pwm);
			delay_ms(8);
		}
	}
}



