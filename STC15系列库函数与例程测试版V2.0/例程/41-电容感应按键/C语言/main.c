
/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
/*---------------------------------------------------------------------*/

/*************	����˵��	**************

����ʹ��STC15W408AS��ADC���ĵ��ݸ�Ӧ������.

�ٶ�����оƬ�Ĺ���Ƶ��Ϊ24MHz

******************************************/



#include	<reg52.h>
#include	<intrins.h>

#define MAIN_Fosc		24000000UL	//������ʱ��

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#define	Timer0_Reload	(65536UL -(MAIN_Fosc / 600000))		//Timer 0 ��װֵ�� ��Ӧ300KHZ

sfr P1ASF     = 0x9D;	//ֻд��ģ������ѡ��
sfr ADC_CONTR = 0xBC;	//��ADϵ��
sfr ADC_RES   = 0xBD;	//��ADϵ��
sfr ADC_RESL  = 0xBE;	//��ADϵ��
sfr	AUXR      = 0x8E;
sfr	AUXR2     = 0x8F;



/*************	���س�������	**************/

#define	TOUCH_CHANNEL	8	//ADCͨ����

#define ADC_90T		(3<<5)	//ADCʱ�� 90T
#define ADC_180T	(2<<5)	//ADCʱ�� 180T
#define ADC_360T	(1<<5)	//ADCʱ�� 360T
#define ADC_540T	0		//ADCʱ�� 540T
#define ADC_FLAG	(1<<4)	//�����0
#define ADC_START	(1<<3)	//�Զ���0

/*************	���ر�������	**************/
sbit	P_LED7 = P2^7;
sbit	P_LED6 = P2^6;
sbit	P_LED5 = P2^5;
sbit	P_LED4 = P2^4;
sbit	P_LED3 = P2^3;
sbit	P_LED2 = P2^2;
sbit	P_LED1 = P2^1;
sbit	P_LED0 = P2^0;

u16	idata adc[TOUCH_CHANNEL];			//��ǰADCֵ
u16	idata adc_prev[TOUCH_CHANNEL];		//��һ��ADCֵ
u16	idata TouchZero[TOUCH_CHANNEL];		//0��ADCֵ
u8	idata TouchZeroCnt[TOUCH_CHANNEL];	//0���Զ����ټ���

u8	cnt_250ms;

/*************	���غ�������	**************/
void	delay_ms(u8 ms);
void	ADC_init(void);
u16		Get_ADC10bitResult(u8 channel);
void	AutoZero(void);
u8		check_adc(u8 index);
void	ShowLED(void);



/******************** ������ **************************/
void main(void)
{
	u8	i;

	delay_ms(50);

	ET0 = 0;		//��ʼ��Timer0���һ��300KHZʱ��
	TR0 = 0;
	AUXR |=  0x80;	//Timer0 set as 1T mode
	AUXR2 |= 0x01;	//�������ʱ��
	TMOD = 0;		//Timer0 set as Timer, 16 bits Auto Reload.
	TH0 = (u8)(Timer0_Reload >> 8);
	TL0 = (u8)Timer0_Reload;
	TR0 = 1;

	ADC_init();		//ADC��ʼ��
	delay_ms(50);	//��ʱ50ms

	for(i=0; i<TOUCH_CHANNEL; i++)		//��ʼ��0�����һ��ֵ��0���Զ����ټ���
	{
		adc_prev[i]  = 1023;
		TouchZero[i] = 1023;
		TouchZeroCnt[i] = 0;
	}
	cnt_250ms = 0;

	while (1)
	{
		delay_ms(50);		//ÿ��50ms����һ�ΰ���
		ShowLED();
		if(++cnt_250ms >= 5)
		{
			cnt_250ms = 0;
			AutoZero();		//ÿ��250ms����һ��0���Զ�����
		}
		
	}
}
/**********************************************/


//========================================================================
// ����: void  delay_ms(unsigned char ms)
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

/*************  ADC��ʼ������ *****************/
void	ADC_init(void)
{
	P1ASF = 0xff;		//8·ADC
	ADC_CONTR = 0x80;	//����ADC
}

//========================================================================
// ����: u16	Get_ADC10bitResult(u8 channel)
// ����: ��ѯ����һ��ADC���.
// ����: channel: ѡ��Ҫת����ADC.
// ����: 10λADC���.
// �汾: V1.0, 2012-10-22
//========================================================================
u16	Get_ADC10bitResult(u8 channel)	//channel = 0~7
{
	ADC_RES  = 0;
	ADC_RESL = 0;
	ADC_CONTR = 0x80 | ADC_90T | ADC_START | channel; //����ADC
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while((ADC_CONTR & ADC_FLAG) == 0)	;	//�ȴ�ADCת������
	ADC_CONTR = 0x80;		//�����־
	return(((u16)ADC_RES << 2) | ((u16)ADC_RESL & 3));	//����ADC���
}



/********************* �Զ�0����ٺ��� *************************/
void	AutoZero(void)		//250ms����һ�� ����ʹ������2�������Ĳ�ľ���ֵ֮������⡣
{
	u8	i;
	u16	j,k;

	for(i=0; i<TOUCH_CHANNEL; i++)		//����8��ͨ��
	{
		j = adc[i];
		k = j - adc_prev[i];			//��ǰһ������
		F0 = 0;								//����
		if(k & 0x8000)	F0 = 1,	k = 0 - k;	//�ͷ�	������β����Ĳ�ֵ
		if(k >= 20)	//�仯�Ƚϴ�
		{
			TouchZeroCnt[i] = 0;			//����仯�Ƚϴ�����0������
			if(F0)	TouchZero[i] = j;		//������ͷţ����ұ仯�Ƚϴ���ֱ�����
		}
		else		//�仯�Ƚ�С�����䶯���Զ�0�����
		{
			if(++TouchZeroCnt[i] >= 20)		//������⵽С�仯20��/4 = 5��.
			{
				TouchZeroCnt[i] = 0;
				TouchZero[i] = adc_prev[i];	//�仯������ֵ��Ϊ0��
			}
		}
		adc_prev[i] = j;	//������һ�εĲ���ֵ
	}
}



/********************* ��ȡ������Ϣ���� 50ms����1�� *************************/
u8	check_adc(u8 index)		//�жϼ����»��ͷ�,�лز����
{
	u16	delta;
	adc[index] = 1023 - Get_ADC10bitResult(index);	//��ȡADCֵ, ת�ɰ��¼�, ADCֵ����
	if(adc[index] < TouchZero[index])	return	0;	//��0�㻹С��ֵ������Ϊ�Ǽ��ͷ�
	delta = adc[index] - TouchZero[index];
	if(delta >= 40)	return 1;	//������
	if(delta <= 20)	return 0;	//���ͷ�
	return	2;		//����ԭ״̬
}


/********************* ������ 50ms����1�� *************************/
void	ShowLED(void)
{
	u8	i;
	
	i = check_adc(0);
	if(i == 0)		P_LED0 = 1;	//ָʾ����
	if(i == 1)		P_LED0 = 0;	//ָʾ����

	i = check_adc(1);
	if(i == 0)		P_LED1 = 1;	//ָʾ����
	if(i == 1)		P_LED1 = 0;	//ָʾ����

	i = check_adc(2);
	if(i == 0)		P_LED2 = 1;	//ָʾ����
	if(i == 1)		P_LED2 = 0;	//ָʾ����

	i = check_adc(3);
	if(i == 0)		P_LED3 = 1;	//ָʾ����
	if(i == 1)		P_LED3 = 0;	//ָʾ����

	i = check_adc(4);
	if(i == 0)		P_LED4 = 1;	//ָʾ����
	if(i == 1)		P_LED4 = 0;	//ָʾ����

	i = check_adc(5);
	if(i == 0)		P_LED5 = 1;	//ָʾ����
	if(i == 1)		P_LED5 = 0;	//ָʾ����

	i = check_adc(6);
	if(i == 0)		P_LED6 = 1;	//ָʾ����
	if(i == 1)		P_LED6 = 0;	//ָʾ����

	i = check_adc(7);
	if(i == 0)		P_LED7 = 1;	//ָʾ����
	if(i == 1)		P_LED7 = 0;	//ָʾ����

}


