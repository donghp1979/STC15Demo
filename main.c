#include "STC15/USART.h"
#include "STC15/delay.h"

#include "SysInit.h"
#include "DS18B20.h"
#include <stdio.h>

/*******************************************************************************
* 实验名			   : 动态显示数码管实验
* 使用的IO	     : 数码管使用P0,P2.2,P2.3,P2.4
* 实验效果       : 数码管显示76543210。
*	注意					 ：
*******************************************************************************/

#define GPIO_DIG P1

sbit LSA=P3^2;
sbit LSB=P3^3;
sbit LSC=P3^4;

unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
unsigned char xdata DisplayData[8];
//用来存放要显示的8位数的值
unsigned char xdata StrTemp[6]; // 存放转换后的温度字符串，XX.XX

void DigDisplay(); //动态显示函数
/*******************************************************************************
* 函数名         : main
* 函数功能		   : 主函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void main(void)
{
	unsigned char i;
	int temp;
	GPIO_Init();
	Timer0_Init(2);
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
			} else temp=0;
		}
		else {
			temp = 1;
		}

		DisplayData[0]=DIG_CODE[temp/1000];
		DisplayData[1]=DIG_CODE[temp%1000/100]|0x80;
		DisplayData[2]=DIG_CODE[temp%1000%100/10];
		DisplayData[3]=DIG_CODE[temp%1000%100%10];
		DisplayData[4]=DIG_CODE[0];
		DisplayData[5]=DIG_CODE[0];
		DisplayData[6]=DIG_CODE[0];
		DisplayData[7]=DIG_CODE[0];
		
//		StrTemp[0] = temp/1000+'0';
//		StrTemp[1] = temp%1000/100+'0';
//		StrTemp[2] = '.';
//		StrTemp[3] = temp%100/10+'0';
//		StrTemp[4] = temp%10+'0';
//		StrTemp[5] = 0;
//		
//		PrintString1("Temp:");
//		PrintString1(StrTemp);
//		PrintString1("\r\n");
		
		printf("Temp : %2.2f\n", (float)temp/100);
		delay_ms(250);
		delay_ms(250);
	}
}

/*******************************************************************************
* 函数名         : DigDisplay
* 函数功能		   : 使用数码管显示
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void DigDisplay()
{
	static unsigned char Num = 0;
	//unsigned char i;
	//unsigned int j;
	//for(i=0;i<8;i++)
	{
		GPIO_DIG=0x00;//消隐
		switch(Num)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(6):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; break;//显示第7位	
		}
		GPIO_DIG=DisplayData[Num];//发送段码
		//GPIO_DIG=AUXR;
		Num++;
		if(Num>7) Num=0;
	}
}

