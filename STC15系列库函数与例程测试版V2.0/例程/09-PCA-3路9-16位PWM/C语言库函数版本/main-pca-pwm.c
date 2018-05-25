
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
#include	"PCA.h"
#include	"delay.h"

/*************	����˵��	**************

���3·9~16λ�仯��PWM�źš�����"������"������.

PWMƵ�� = MAIN_Fosc / PWM_DUTY, ���� MAIN_Fosc = 24MHZ, PWM_DUTY = 6000, �����PWMƵ��Ϊ4000HZ.

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

u16	pwm0,pwm1,pwm2;


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/


void	PCA_config(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_1T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P24_P25_P26_P27;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityHigh;			//���ȼ�����	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;			//���������ʱ, Ϊƥ��Ƚ�ֵ
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;				//���������ʱ, Ϊƥ��Ƚ�ֵ
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 65535;				//���������ʱ, Ϊƥ��Ƚ�ֵ
	PCA_Init(PCA2,&PCA_InitStructure);
}


/******************** task A **************************/
void main(void)
{
	P2M1 &= ~(0xe0);	//P2.7 P2.6 P2.5 ����Ϊ�������
	P2M0 |=  (0xe0);
	
	PCA_config();
	pwm0 = (PWM_DUTY / 4 * 1);	//��PWMһ����ֵ
	pwm1 = (PWM_DUTY / 4 * 2);
	pwm2 = (PWM_DUTY / 4 * 3);

	PWMn_Update(PCA0,pwm0);
	PWMn_Update(PCA1,pwm1);
	PWMn_Update(PCA2,pwm2);

	EA = 1;
	
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



