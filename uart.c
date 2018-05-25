#include "uart.h"
#include <intrins.h>
#include "stc_config.h"

bit Enable_DEBUG=1;

void UART_Init(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xD0;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	TI = 1;       // ��ʼ��Ϊ1����ʾ�ѷ��Ϳ���
	RI = 0;       // ��ʼ��Ϊ0����ʾ���������
	ES = 1;       //ʹ�ܴ����ж�
}
