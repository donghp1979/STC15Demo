
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
#include	"USART.h"
#include	"PCA.h"

/*************	����˵��	**************

PCA0  Ϊ8λPWM. ����仯��PWM�źš�����"������"������.
PCA1  Ϊ��׽. �������ӵ�P2.7�������Բ�׽,��׽��ʱ��Ӵ���1���. Ҳ���Դ��ⲿ����һ���ź�����׽.
PCA2  Ϊ16λ�����ʱ, ��ʱʱ��Ϊ30000��PCAʱ��, ���Ҵ�P2.7��������ź�,�������Ϊ60000��PCAʱ��.
      P2.7�������ӵ�P2.6�������Բ�׽,��׽��ʱ��Ӵ���1���.

����1�ʹ���2���յ�������ԭ������.

��ʱ��0����1ms�Ļ�׼,��Ϊ�������еĽ���.

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

u8	cnt0;
u16	Cap_time;	//��һ�β�׽ʱ��
u8	pwm0;		//pwm
bit	B_PWM0_Dir;	//����, 0Ϊ+, 1Ϊ-.

/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



extern	bit	B_Timer0_1ms;

void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - ((1000 * Main_Fosc_KHZ) / 12000);	//��ֵ, 1000us
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}

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

	PrintString1("STC15F2K60S2 PCA Test Prgramme!\r\n");	//SUART1����һ���ַ���
	PrintString2("STC15F2K60S2 PCA Test Prgramme!\r\n");	//SUART2����һ���ַ���
}


void	PCA_config(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_12T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_IoUse    = PCA_P24_P25_P26_P27;	//PCA_P12_P11_P10_P37, PCA_P34_P35_P36_P37, PCA_P24_P25_P26_P27
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//ENABLE, DISABLE
	PCA_InitStructure.PCA_Polity   = PolityHigh;		//���ȼ�����	PolityHigh,PolityLow
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_PWM;		//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = PCA_PWM_8bit;		//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = DISABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 128 << 8;			//����PWM,��8λΪPWMռ�ձ�
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_Capture;	//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = PCA_Fall_Active | ENABLE;	//(PCA_Rise_Active, PCA_Fall_Active) or (ENABLE, DISABLE)
	PCA_InitStructure.PCA_Value    = 0;					//���ڲ�׽, ���ֵû����
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_Mode     = PCA_Mode_HighPulseOutput;	//PCA_Mode_PWM, PCA_Mode_Capture, PCA_Mode_SoftTimer, PCA_Mode_HighPulseOutput
	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit
	PCA_InitStructure.PCA_Interrupt_Mode = ENABLE;		//PCA_Rise_Active, PCA_Fall_Active, ENABLE, DISABLE
	PCA_InitStructure.PCA_Value    = 30000;				//���������ʱ, Ϊƥ��Ƚ�ֵ
	PCA_Init(PCA2,&PCA_InitStructure);

	CR = 1;
}


/******************** task A **************************/
void main(void)
{
	u8	i;
	u16	j;
	
	Timer_config();
	UART_config();
	PCA_config();
	EA = 1;
	
	Cap_time = 0;
	pwm0 = 128;
	B_PWM0_Dir = 0;

	while (1)
	{
		if(B_Timer0_1ms)
		{
			B_Timer0_1ms = 0;

			if(B_Capture1)
			{
				B_Capture1 = 0;
				j = CCAP1_tmp - Cap_time;	//����ʱ���
				Cap_time = CCAP1_tmp;
				TX1_write2buff(j/10000 + '0');
				TX1_write2buff(j%10000/1000 + '0');
				TX1_write2buff(j%1000/100 + '0');
				TX1_write2buff(j%100/10 + '0');
				TX1_write2buff(j%10 + '0');
				PrintString1("\r\n");
			}

			cnt0++;
			if((cnt0 & 15) == 15)	//16ms
			{
				if(B_PWM0_Dir)
				{
						if(--pwm0 <= 8)		B_PWM0_Dir = 0;
				}
				else	if(++pwm0 >= 248)	B_PWM0_Dir = 1;
				UpdatePwm(PCA0,pwm0);
			}

			if(COM1.RX_TimeOut > 0)		//��ʱ����
			{
				if(--COM1.RX_TimeOut == 0)
				{
					if(COM1.RX_Cnt > 0)
					{
						for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);	//�յ�������ԭ������
					}
					COM1.RX_Cnt = 0;
				}
			}
			if(COM2.RX_TimeOut > 0)		//��ʱ����
			{
				if(--COM2.RX_TimeOut == 0)
				{
					if(COM2.RX_Cnt > 0)
					{
						for(i=0; i<COM2.RX_Cnt; i++)	TX2_write2buff(RX2_Buffer[i]);	//�յ�������ԭ������
					}
					COM2.RX_Cnt = 0;
				}
			}
		}
	}
}



