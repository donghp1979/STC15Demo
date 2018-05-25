//#include "temp.h"
#include "DS18B20.h"
#include "uart.h"

/*******************************************************************************
* ʵ����			   : ��̬��ʾ�����ʵ��
* ʹ�õ�IO	     : �����ʹ��P0,P2.2,P2.3,P2.4
* ʵ��Ч��       : �������ʾ76543210��
*	ע��					 ��
*******************************************************************************/

#define GPIO_DIG P1

sbit LSA=P3^2;
sbit LSB=P3^3;
sbit LSC=P3^4;

unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0��1��2��3��4��5��6��7��8��9��A��b��C��d��E��F����ʾ��
unsigned char DisplayData[8];
//�������Ҫ��ʾ��8λ����ֵ
void DigDisplay(); //��̬��ʾ����
/*******************************************************************************
* ������         : main
* ��������		   : ������
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void main(void)
{
	unsigned char i;
	int temp;
	System_Init();
	UART_Init();
	EA = 1;
	for(i=0;i<8;i++)
	{
		DisplayData[i]=DIG_CODE[5];	
	}
	//GPIO_DIG=AUXR;
	while(1) {
		if(DS18B20_Init()==1) {
			temp = DS18B20_ReadTemperature();
			if(temp>0) {
				temp=temp*0.0625*100+0.5;
			} else temp=4444;
		}
		else {
			temp = 9999;
		}

		DisplayData[0]=DIG_CODE[temp/1000];
		DisplayData[1]=DIG_CODE[temp%1000/100]|0x80;
		DisplayData[2]=DIG_CODE[temp%1000%100/10];
		DisplayData[3]=DIG_CODE[temp%1000%100%10];
		DisplayData[4]=DIG_CODE[0];
		DisplayData[5]=DIG_CODE[0];
		DisplayData[6]=DIG_CODE[0];
		DisplayData[7]=DIG_CODE[0];
		//DEBUG(("Temp: %0.2f\n",(float)temp/100));
		Delay1ms(500);
	}
}

/*******************************************************************************
* ������         : DigDisplay
* ��������		   : ʹ���������ʾ
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void DigDisplay() interrupt 1 using 2
{
	static unsigned char Num = 0;
	//unsigned char i;
	//unsigned int j;
	//for(i=0;i<8;i++)
	{
		GPIO_DIG=0x00;//����
		switch(Num)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
			case(1):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
			case(2):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
			case(3):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ
			case(4):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(5):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
			case(6):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
			case(7):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ	
		}
		GPIO_DIG=DisplayData[Num];//���Ͷ���
		//GPIO_DIG=AUXR;
		Num++;
		if(Num>7) Num=0;
	}
}

/* ����1�жϺ��� */
void Uart() interrupt 4 using 1 {
	if(RI) {
		RI=0;
		P0=~P0;
	}
}

