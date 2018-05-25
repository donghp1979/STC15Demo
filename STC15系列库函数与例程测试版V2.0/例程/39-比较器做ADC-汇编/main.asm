

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

/****************************
��ʾ����Keil������������ѡ��Intel��8052оƬ�ͺŽ��б���

������MCU�Ĺ���Ƶ��Ϊ22.1184MHz.

ʹ��MCU�Դ��ıȽ�������ADCת��, ��ͨ������������. �ö�ʱ��0����10us�жϲ�ѯ�Ƚ�����״̬.

ʹ�ñȽ�����ADC, ԭ��ͼ����.
��ADC��ԭ���ǻ��ڵ��ƽ��ļ���ʽADC.
��ѹ��Vin����, ͨ��100K+104�˲�, ����Ƚ�����P5.5�������, �����Ƚ����ıȽ�, ����������P1.2��ͨ��100K+104�˲���
�ͱȽ���P5.4�������,�������ѹƽ��.
������������: ��������(����)adc_duty �� �ȽϽ���ߵ�ƽ�ļ���ֵ adc, adc�ϸ�����������ѹ.
ADC�Ļ�׼����P1.2�ĸߵ�ƽ. ����ߵ�ƽ׼ȷ,�Ƚ����ķŴ����㹻��,��ADC������׼ȷ.
���ȽϽ��Ϊ�ߵ�ƽ,��P1.2���1, ����adc+1.
���ȽϽ��Ϊ�͵�ƽ,��P1.2���0, adcֵ����.
ÿһ�αȽ϶��жϼ��������Ƿ����,�����adc���ֵ����ADC���.
���ƽ�����ʽADC�����������������ñ��õ�˫����ADC, ����������Ϊ20ms�ı���ʱ,���к�ǿ�Ŀ���Ƶ��������,�ܺõ����Ժ;���.
ԭ����Բο�ADD3501(3 1/2λ�������ñ�)��ADD3701(3 3/4λ�������ñ�), Ҳ���Բο�AD7740 VFC��·.

��: �Ƚ�һ�ε�ʱ����Ϊ10us, ����Ϊ10000, ����1��ADC��ʱ��Ϊ100ms. �Ƚ�������Ӧʱ��Խ��, �����ADC��Խ��.

����Ҫ��ÿ�αȽ�ʱ������Ҫ���,������C�������ڶ�ʱ���ж������, ��ʱ������Ϊ�Զ���װ, �����ȼ��ж�, �����жϾ������ȼ�.
�û��Ļ�, ��֤�Ƚ������ƽ�����ʱ��Ҫ���.

                                         100K
                            /| P5.5       ___
                    P1.2   /+|---------o-|___|- ------- Vin
                     .----<  | P5.4    |
                     |     \-|---.     |
                     |      \|   |     |
                     |           |     |
                     |    ___    |     |
                     '---|___|---o     |
                        100K     |     |
                                ---   ---
                                ---   ---
                            104  |     |  104
                                 |     |
                                ===   ===
                                GND   GND

******************************/


;*************	���س�������	**************/
AUXR        EQU     08EH        //�����Ĵ���
T2H         DATA    0D6H        //��ʱ��2��8λ
T2L         DATA    0D7H        //��ʱ��2��8λ

P_SW1       EQU     0A2H        //���蹦���л��Ĵ���1
S1_S0       EQU     40H         //P_SW1.6
S1_S1       EQU     80H         //P_SW1.7

P1M1		EQU 091H	;	//P1M1.n,P1M0.n 	=00--->Standard,	01--->push-pull		ʵ����1T�Ķ�һ��
P1M0		EQU 092H	;	//					=10--->pure input,	11--->open drain
P5M1		EQU 0C9H	;	//	P5M1.n,P5M0.n 	=00--->Standard,	01--->push-pull
P5M0		EQU 0CAH	;	//					=10--->pure input,	11--->open drain

CMPCR1 		EQU 0E6H
CMPCR2 		EQU 0E7H


;CMPCR1
CMPEN	EQU	080H	//1: ����Ƚ���, 0: ��ֹ,�رձȽ�����Դ
CMPIF	EQU	040H;	//�Ƚ����жϱ�־, ���������ػ��½����ж�, �����0
PIE		EQU	020H;	//1: �ȽϽ����0��1, �����������ж�
NIE		EQU	010H;	//1: �ȽϽ����1��0, �����½����ж�
PIS		EQU	008H;	//����������ѡ��, 0: ѡ���ڲ�P5.5��������,           1: ��ADCIS[2:0]��ѡ���ADC�������������.
NIS		EQU	004H;	//���븺����ѡ��, 0: ѡ���ڲ�BandGap��ѹBGv��������, 1: ѡ���ⲿP5.4������.
CMPOE	EQU	002H;	//1: ����ȽϽ�������P1.2, 0: ��ֹ.
CMPRES	EQU	001H;	//�ȽϽ��, 1: CMP+��ƽ����CMP-,  0: CMP+��ƽ����CMP-,  ֻ��

;CMPCR2
INVCMPO	EQU	080H;	//1: �Ƚ������ȡ��,  0: ��ȡ��
DISFLT	EQU	040H;	//1: �ر�0.1uF�˲�,   0: ����
LCDTY	EQU	000H;	//0~63, �ȽϽ���仯��ʱ������

TIM_16BitAutoReload			EQU	0
TIM_16Bit					EQU	1
TIM_8BitAutoReload			EQU	2
TIM_16BitAutoReloadNoMask	EQU	3


;*************	���ر�������	**************/



ADC_SCALE	EQU	50000		;ADC������, ������Ҫ����

P_ADC		BIT	P1.2;			//P1.2 �Ƚ���תIO�����
adc_h		DATA	30H;		//ADC�м�ֵ, �û��㲻�ɼ�
adc_l		DATA	31H;
adc_duty_h	DATA	32H;		//ADC��������, �û��㲻�ɼ�
adc_duty_l	DATA	33H;
adc_value_h	DATA	34H;		//ADCֵ, �û���ʹ��
adc_value_l	DATA	35H

FALG		DATA	20H
adc_ok		BIT		FALG.0;		//ADC������־, Ϊ1��adc_value��ֵ����. �˱�־���û����ѯ,������0



		ORG		00H				;reset
		LJMP	F_MAIN_FUNC

		ORG		03H				;INT0 interrupt
	;	LJMP	F_INT0_interrupt      
		RETI

		ORG		0BH				;Timer0 interrupt
		LJMP	F_Timer0_interrupt
		RETI

		ORG		13H				;INT1 interrupt
	;	LJMP	F_INT1_interrupt      

		ORG		1BH				;Timer1 interrupt
	;	LJMP	F_Timer1_interrupt
		RETI

		ORG		00023H
	;	LJMP	F_UART0_RCV
		RETI


;******************** ������ **************************/
F_MAIN_FUNC:

	MOV		SP,#80H

;������ʼ��
	MOV  	adc_duty_l,#LOW  ADC_SCALE	;���ڼ�������ֵ
	MOV  	adc_duty_h,#HIGH ADC_SCALE
	MOV  	adc_l,#0					;���ADCֵ
	MOV  	adc_h,#0
	MOV		adc_value_h,#0				;ADCֵ, �û���ʹ��
	MOV		adc_value_l,#0
	MOV		FALG,#0



;IO�ڳ�ʼ��
	ANL		P1M1,#NOT 00000100B	;P1.2 push-pull
	ORL		P1M0,# 00000100B
	ORL		P5M1,# 00110000B		;P5.4 P5.5 pure input
	ANL		P5M0,#NOT 00110000B

;�Ƚ�����ʼ��
	MOV		CMPCR1,#0
	MOV		CMPCR2,#20;			//�ȽϽ���仯��ʱ������, 0~63
	ORL		CMPCR1,#CMPEN;		//����Ƚ���		ENABLE,DISABLE
;	ORL		CMPCR1,#PIE;		//�����������ж�	ENABLE,DISABLE
;	ORL		CMPCR1,#NIE;		//�����½����ж�	ENABLE,DISABLE
;	ORL		CMPCR1,#PIS;		//����������ѡ��, 0: ѡ���ڲ�P5.5��������,           1: ��ADCIS[2:0]��ѡ���ADC�������������.
	ORL		CMPCR1,#NIS;		//���븺����ѡ��, 0: ѡ���ڲ�BandGap��ѹBGv��������, 1: ѡ���ⲿP5.4������
;	ORL		CMPCR1,#CMPOE;		//����ȽϽ�������P1.2,   ENABLE,DISABLE
;	ORL		CMPCR2,#INVCMPO;	//�Ƚ������ȡ��, 	ENABLE,DISABLE
	ORL		CMPCR2,#DISFLT;		//�ڲ�0.1uF�˲�,  	ENABLE,DISABLE

;��ʱ��0 ��ʼ��
	MOV		A,TMOD
	ANL		A,#NOT 00FH;	//����ģʽ,0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ, 3: 16λ�Զ���װ, ���������ж�
	ORL		A,TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	MOV		TMOD,A
	ORL		AUXR,#080H;	//1T
	SETB	ET0;	//�����ж�
	SETB	PT0;	//�����ȼ��ж�

	MOV		TH0,#HIGH (-221);		//��װֵ	100KHZ, 10us,	65536 - (MAIN_Fosc)/100000
	MOV		TL0,#LOW  (-221);
	SETB	TR0;	//��ʼ����

;����1��ʼ��
    MOV   A,P_SW1
    ANL   A,#03FH               //S1_S0=0 S1_S1=0
    MOV   P_SW1,A               //(P3.0/RxD, P3.1/TxD)
    
//  MOV   A,P_SW1
//  ANL   A,#03FH               //S1_S0=1 S1_S1=0
//  ORL   A,#S1_S0              //(P3.6/RxD_2, P3.7/TxD_2)
//  MOV   P_SW1,A
//  
//  MOV   A,P_SW1
//  ANL   A,#03FH               //S1_S0=0 S1_S1=1
//  ORL   A,#S1_S1              //(P1.6/RxD_3, P1.7/TxD_3)
//  MOV   P_SW1,A

    MOV   SCON,#50H             //8λ�ɱ䲨����

    MOV		T2L,#LOW  (-576)		//���ò�������װֵ(65536-22118400/4/9600)
    MOV		T2H,#HIGH (-576)
    MOV		A,AUXR
	ANL		A,#NOT 01DH
	ORL		AUXR,#14H				//T2Ϊ1Tģʽ, ��������ʱ��2
    ORL		AUXR,#01H				//ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
//  SETB	ES					//ʹ�ܴ����ж�

	SETB	EA;		//�ж�ȫ������

	MOV		DPTR,#TestString1	//���Ͳ����ַ���
	LCALL	F_SendSring

L_MainLoop:
	JNB		adc_ok,$			;�ȴ�ת������
	CLR		adc_ok;				//���ADC�ѽ�����־
	MOV		DPTR,#StringADC		//���Ͳ����ַ���
	LCALL	F_SendSring

	MOV		R3,adc_value_h
	MOV		R4,adc_value_l
	LCALL	F_HEX2_DEC		;������תʮ����, (R3---R4) HEX Change to DEC ---> (R5---R7), use (R2~R7),	344T
	
	MOV		A,R5			;BCD��չ����ת��ASCII��
	ADD		A,#'0'
	MOV		R3,A
	MOV		A,R6
	SWAP	A
	ANL		A,#0FH
	ADD		A,#'0'
	MOV		R4,A
	MOV		A,R6
	ANL		A,#0FH
	ADD		A,#'0'
	MOV		R5,A
	MOV		A,R7
	SWAP	A
	ANL		A,#0FH
	ADD		A,#'0'
	MOV		R6,A
	MOV		A,R7
	ANL		A,#0FH
	ADD		A,#'0'
	MOV		R7,A

	MOV		A,R3	;����Ч0
	CJNE	A,#'0',L_Cut_0_end
	MOV		R3,#' '
	MOV		A,R4
	CJNE	A,#'0',L_Cut_0_end
	MOV		R4,#' '
	MOV		A,R5
	CJNE	A,#'0',L_Cut_0_end
	MOV		R5,#' '
	MOV		A,R6
	CJNE	A,#'0',L_Cut_0_end
	MOV		R6,#' '
L_Cut_0_end:
	MOV		A,R3			//������
	LCALL	F_TxSendByte
	MOV		A,R4
	LCALL	F_TxSendByte
	MOV		A,R5
	LCALL	F_TxSendByte
	MOV		A,R6
	LCALL	F_TxSendByte
	MOV		A,R7
	LCALL	F_TxSendByte
	MOV		A,#' '
	LCALL	F_TxSendByte
	MOV		A,#' '
	LCALL	F_TxSendByte
	
	MOV		A,#0x0d			;����
	LCALL	F_TxSendByte
	MOV		A,#0x0a
	LCALL	F_TxSendByte

	LJMP	L_MainLoop		;ѭ��




TestString1:
    DB  0dh,0ah,"ʹ�ñȽ�����ADC����",0dh,0ah,0
StringADC:
    DB  "ADC = ",0

;********************* ���ڷ���һ���ֽ� ************************/
; ����: ���ڷ���һ���ֽڡ�9600��N��8��1
; ����: ACC: Ҫ���͵������ֽ�.
; ����: none.
F_TxSendByte:	;ACC--Ҫ���͵��ֽ�,
	CLR		TI
	MOV		SBUF,A
	JNB		TI,$
	CLR		TI
	RET

;********************* �����ַ��� ****************************
F_SendSring:
	CLR		A
	MOVC	A,@A+DPTR			//��ȡ�ַ�
	JZ		L_QuitSendString	//����ַ���������־
	INC		DPTR				//�ַ�����ַ+1
	LCALL	F_TxSendByte		//���͵�ǰ�ַ�
	SJMP	F_SendSring
L_QuitSendString:
    RET

;***************************************************************************
F_HEX2_DEC:        	;(R3---R4) HEX Change to DEC ---> (R5---R7), use (R2~R7),	344T
       
		MOV  	R2,#16
		MOV  	R5,#0
		MOV  	R6,#0
		MOV  	R7,#0

L_HEX2_DEC:
		CLR  	C
		MOV  	A,R4
		RLC  	A
		MOV  	R4,A
		MOV  	A,R3
		RLC  	A
		MOV  	R3,A

		MOV  	A,R7
		ADDC 	A,R7
		DA   	A
		MOV  	R7,A

		MOV  	A,R6
		ADDC 	A,R6
		DA   	A
		MOV  	R6,A

		MOV  	A,R5
		ADDC 	A,R5
		DA   	A
		MOV  	R5,A

		DJNZ 	R2,L_HEX2_DEC
		RET


;********************* Timer0�жϺ���************************/
F_Timer0_interrupt:
	PUSH	PSW
	PUSH	ACC
	
	MOV		A,CMPCR1
	JNB		ACC.0,L_CMP_Out0

	SETB	P_ADC		;�Ƚ�������ߵ�ƽ,	��P_ADC����ߵ�ƽ, ���������������. 
	INC		adc_l
	MOV		A,adc_l		;ADC����+1
	JNZ		L_ADC_CheckDuty
	INC		adc_h
	SJMP	L_ADC_CheckDuty

L_CMP_Out0:				;�Ƚ�������͵�ƽ
	CLR		P_ADC		;P_ADC����͵�ƽ, ���������������. 

L_ADC_CheckDuty:
	MOV  	A,adc_duty_l	;ADC����-1, ��0��ADC����
	DEC  	adc_duty_l
	JNZ  	L_ADC_CheckDuty1
	DEC  	adc_duty_h
L_ADC_CheckDuty1:
	DEC  	A
	ORL  	A,adc_duty_h
	JNZ  	L_ADC_CheckDuty2

	MOV  	adc_duty_l,#LOW  ADC_SCALE	;���ڼ�������ֵ
	MOV  	adc_duty_h,#HIGH ADC_SCALE
	MOV  	adc_value_l,adc_l	;����ADCֵ
	MOV  	adc_value_h,adc_h
	MOV  	adc_l,#0			;���ADCֵ
	MOV  	adc_h,#0
	SETB 	adc_ok				;��־ADC�ѽ���

L_ADC_CheckDuty2:
	POP  	ACC
	POP		PSW
	RETI 	

	END
	

	
