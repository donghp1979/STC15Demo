/*---------------------------------------------------------------------*/
/* --- STC MCU International Limited ----------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ���˺꾧�Ƽ������ϼ�����   */
/*---------------------------------------------------------------------*/

#include 	<intrins.h>
#include	"config.h"
#include	"timer.h"
#include	"LCD_IO16.h"


/*************	����˵��	**************

��STC115ϵ�в���IOֱ����������LCD(6��8��LCD, 1/4 Dutys, 1/3 bias)��

�ϵ����ʾһ��ʱ��(ʱ����).

P3.2�Եؽ�һ������,��������˯�߻���.


******************************************/

/*************	���س�������	**************/





/*************	���ر�������	**************/
u8	cnt_500ms;
u8	second,minute,hour;
bit	B_Second;	//���ź�

/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/
extern	bit	B_2ms;



/************************ ��ʱ������ ****************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / 500);	//��ֵ, ����Ϊ500HZ
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}

/******************** ��ʾʱ�� **************************/
void	LoadRTC(void)
{
	LCD_load(1,hour/10);
	LCD_load(2,hour%10);
	LCD_load(3,minute/10);
	LCD_load(4,minute%10);
	LCD_load(5,second/10);
	LCD_load(6,second%10);
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
void  delay_ms(u8 ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

/******************** ������**************************/
void main(void)
{

	Init_LCD_Buffer();
	Timer_config();
	EA = 1;

	LCD_SET_2M;	//��ʾʱ�ּ��:
	LCD_SET_4M;	//��ʾ������:
	LoadRTC();	//��ʾʱ��
				
	while (1)
	{
		PCON |= 0x01;	//Ϊ��ʡ��, �������ģʽ(������Լ2.5~3.0mA @5V)����Timer0 2ms�����˳�
		_nop_();
		_nop_();
		_nop_();

		if(B_2ms)	//2ms���ĵ�
		{
			B_2ms = 0;

			if(++cnt_500ms >= 250)	//500ms��
			{
				cnt_500ms = 0;
			//	LCD_FLASH_2M;	//��˸ʱ�ּ��:
			//	LCD_FLASH_4M;	//��˸������:
			
				B_Second = ~B_Second;
				if(B_Second)
				{
					if(++second >= 60)	//1���ӵ�
					{
						second = 0;
						if(++minute >= 60)	//1Сʱ��
						{
							minute = 0;
							if(++hour >= 24)	hour = 0;	//24Сʱ��
						}
					}
					LoadRTC();	//��ʾʱ��
				}
			}
		
			if(!P32)	//�����£�׼��˯��
			{
				LCD_CLR_2M;	//��ʾʱ�ּ��:
				LCD_CLR_4M;	//��ʾ������:
				LCD_load(1,DIS_BLACK);
				LCD_load(2,DIS_BLACK);
				LCD_load(3,0);
				LCD_load(4,0x0F);
				LCD_load(5,0x0F);
				LCD_load(6,DIS_BLACK);
				
				while(!P32)	delay_ms(10);	//�ȴ��ͷŰ���
				delay_ms(50);
				while(!P32)	delay_ms(10);	//�ٴεȴ��ͷŰ���
				
				TR0 = 0;	//�رն�ʱ��
				IE0 = 0;	//���ж�0��־λ
				EX0 = 1;	//INT0 Enable
				IT0 = 1;	//INT0 �½����ж�		

				P1n_push_pull(0xff);	//com��segȫ�����0
				P2n_push_pull(0xff);	//com��segȫ�����0
				P1 = 0;
				P2 = 0;

				PCON |= 0x02;	//Sleep
				_nop_();
				_nop_();
				_nop_();

				LCD_SET_2M;	//��ʾʱ�ּ��:
				LCD_SET_4M;	//��ʾ������:
				LoadRTC();	//��ʾʱ��
				TR0 = 1;	//�򿪶�ʱ��
				while(!P32)	delay_ms(10);	//�ȴ��ͷŰ���
				delay_ms(50);
				while(!P32)	delay_ms(10);	//�ٴεȴ��ͷŰ���
			}
		}
	}
}


/********************* INT0�жϺ��� *************************/
void INT0_int (void) interrupt INT0_VECTOR
{
	EX0 = 0;			//INT0 Disable
	IE0  = 0;			//���ж�0��־λ
}




