
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
#include	"adc.h"
#include	"delay.h"
#include	"soft_uart.h"

#define P1n_pure_input(bitn)		P1M1 |=  (bitn),	P1M0 &= ~(bitn)

/*************	����˵��	**************

��������ʾ��·ADC��ѯ������ͨ��ģ�⴮�ڷ��͸���λ����������9600,8,n,1��
�û������޸�Ϊ1~8·��ADCת����

˵��:
    ADC�ĵ�9ͨ�������������ڲ�BandGap�ο���ѹ��,�����ڲ�BandGap�ο���
ѹ���ȶ�,������оƬ�Ĺ�����ѹ�ĸı���仯,���Կ���ͨ�������ڲ�BandGap
�ο���ѹ,Ȼ��ͨ��ADC��ֵ��ɷ��Ƴ�VCC�ĵ�ѹ,�Ӷ��û�����ʵ���Լ��ĵ�
ѹ��⹦��.
    ADC�ĵ�9ͨ���Ĳ�������:���Ƚ�P1ASF��ʼ��Ϊ0,���ر�����P1�ڵ�ģ�⹦��
Ȼ��ͨ��������ADCת���ķ�����ȡ��0ͨ����ֵ,����ͨ��ADC�ĵ�9ͨ����ȡ��ǰ
�ڲ�BandGap�ο���ѹֵ.
    �û�ʵ���Լ��ĵ�ѹ��⹦�ܵ�ʵ�ַ���:�����û���Ҫ��VCC�ܾ�׼�������
(����5.0V),�������ڲ�BandGap�ο���ѹ��ADCת��ֵ(����ΪBGV5),�����ֵ����
��EEPROM��,Ȼ���ڵ�ѹ���Ĵ�����,��ʵ��VCC�仯��,�����������ڲ�BandGap
�ο���ѹ��ADCת��ֵ(����ΪBGVx),ͨ�����㹫ʽ: ʵ��VCC = 5.0V * BGV5 / BGVx,
���ɼ����ʵ�ʵ�VCC��ѹֵ,��Ҫע�����,��һ����BGV5�Ļ�׼����һ��Ҫ��ȷ.

�궨�ڲ���׼Vref, �ṩһ���ȶ��Ĺ�����ѹVcc, ���ڲ���׼Nref, �����ڲ���׼ Vref = Nref * Vcc / 1024.

����ĳ����ѹ, ��ADCֵNx, ���ѹ Ux = Vref * Nx / Nref. һ��Vref = 1220mV.


******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/
u16	Nref;


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/


void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P10 | ADC_P11 | ADC_P12;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE

	P1n_pure_input(0x07);	//��ADC������Ϊ��������
}



/**********************************************/
void main(void)
{
	u8	i;
	u16	j;
//	u8	k;

	ADC_config();

	while (1)
	{
		for(i=0; i<4; i++)
		{
			delay_ms(250);		//Ϊ���÷��͵��ٶ���һ�㣬��ʱ250ms

			if(i <3)	//ADC0~ADC2
			{
				j = Get_ADC10bitResult(i);	//����0~7,��ѯ��ʽ��һ��ADC, ����ֵ���ǽ��, == 1024 Ϊ����
				TxSend('A');
				TxSend('D');
				TxSend(i+'0');
			}
			else		//�ڻ�׼
			{
				P1ASF = 0;
				j = Get_ADC10bitResult(0);	//���ڲ���׼��ѹ, P1ASF=0, ��0ͨ��
				P1ASF = ADC_P10 | ADC_P11 | ADC_P12;
				Nref = j;
				TxSend('A');
				TxSend('D');
				TxSend('r');
				TxSend('e');
				TxSend('f');
			}
			TxSend('=');
			TxSend(j/1000 + '0');
			TxSend(j%1000/100 + '0');
			TxSend(j%100/10 + '0');
			TxSend(j%10 + '0');
			TxSend(' ');
			TxSend(' ');

			TxSend('V');
			TxSend('=');
			j = (u32)1250 * (u32)j / Nref;			//Ux = Vref * Nx / Nref. Vref = 1250mV
			TxSend(j/1000 + '0');
			TxSend('.');
			TxSend(j%1000/100 + '0');
			TxSend(j%100/10 + '0');
			TxSend(j%10 + '0');
			TxSend(' ');
			TxSend(' ');
		}
		PrintString("\r\n");
		//=====================================================================
	}
}



