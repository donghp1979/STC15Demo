
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


/*************	����˵��	**************

��������ʾ��·ADC��ѯ������ͨ��ģ�⴮�ڷ��͸���λ����������9600,8,n,1��
�û������޸�Ϊ1~8·��ADCת����

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


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
}



/**********************************************/
void main(void)
{
	u8	i;
	u16	j;

	ADC_config();

	while (1)
	{

		for(i=0; i<3; i++)
		{
			delay_ms(250);

		//	Get_ADC10bitResult(i);		//����0~7,��ѯ��ʽ��һ��ADC, ����һ��
			j = Get_ADC10bitResult(i);	//����0~7,��ѯ��ʽ��һ��ADC, ����ֵ���ǽ��, == 1024 Ϊ����
			TxSend('A');
			TxSend('D');
			TxSend(i+'0');
			TxSend('=');
			TxSend(j/1000 + '0');
			TxSend(j%1000/100 + '0');
			TxSend(j%100/10 + '0');
			TxSend(j%10 + '0');
			TxSend(' ');
			TxSend(' ');
		}
		PrintString("\r\n");
	}
}



