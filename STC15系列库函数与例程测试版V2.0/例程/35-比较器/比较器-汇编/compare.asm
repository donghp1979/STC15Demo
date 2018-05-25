
;/*------------------------------------------------------------------*/
;/* --- STC MCU International Limited -------------------------------*/
;/* --- STC 1T Series MCU RC Demo -----------------------------------*/
;/* --- Mobile: (86)13922805190 -------------------------------------*/
;/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
;/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
;/* --- Web: www.GXWMCU.com -----------------------------------------*/
;/* --- QQ:  800003751 ----------------------------------------------*/
;/* If you want to use the program or the program referenced in the  */
;/* article, please specify in which data and procedures from STC    */
;/*------------------------------------------------------------------*/



P1M1	DATA	091H	; P1M1.n,P1M0.n 	=00--->Standard,	01--->push-pull		ʵ����1T�Ķ�һ��
P1M0	DATA	092H	; 					=10--->pure input,	11--->open drain

CMPCR1	DATA	0E6H

CMPEN	EQU	080H	; 1: ����Ƚ���, 0: ��ֹ,�رձȽ�����Դ
CMPIF	EQU	040H	; �Ƚ����жϱ�־, ���������ػ��½����ж�, �����0
PIE		EQU	020H	; 1: �ȽϽ����0��1, �����������ж�
NIE		EQU	010H	; 1: �ȽϽ����1��0, �����½����ж�
PIS		EQU	008H	; ����������ѡ��, 0: ѡ���ڲ�P5.5��������,           1: ��ADCIS[2:0]��ѡ���ADC�������������.
NIS		EQU	004H	; ���븺����ѡ��, 0: ѡ���ڲ�BandGap��ѹBGv��������, 1: ѡ���ⲿP5.4������.
CMPOE	EQU	002H	; 1: ����ȽϽ�������P1.2, 0: ��ֹ.
CMPRES	EQU	001H	; �ȽϽ��, 1: CMP+��ƽ����CMP-,  0: CMP+��ƽ����CMP-,  ֻ��

CMPCR2	DATA	0E7H
INVCMPO	EQU	080H	; 1: �Ƚ������ȡ��,  0: ��ȡ��
DISFLT	EQU	040H	; 1: �ر�0.1uF�˲�,   0: ����
LCDTY	EQU	000H	;	0~63, �ȽϽ���仯��ʱ������


	ORG	000H
	LJMP	MAIN
	
	ORG	00ABH
	LJMP	CMP_ISR
	
	
	ORG	100H
MAIN:
	MOV		P1M1,#0
	MOV		P1M0,#0x04	;P1.2 push-pull output
	
	MOV		CMPCR1,#0	;
	MOV		CMPCR2,#0	; �ȽϽ���仯��ʱ������, 0~63
;	ORL		CMPCR1,#PIE	; 1: �ȽϽ����0��1, �����������ж�
;	ORL		CMPCR1,#NIE	; 1: �ȽϽ����1��0, �����½����ж�
;	ORL		CMPCR1,#PIS	; ����������ѡ��, 0: ѡ���ⲿP5.5��������, 	1: ��ADCIS[2:0]��ѡ���ADC�������������.
	ORL		CMPCR1,#NIS	; ���븺����ѡ��, 0: ѡ���ڲ�BandGap��ѹBGv��������,1: ѡ���ⲿP5.4������.
	ORL		CMPCR1,#CMPOE	; 1: ����ȽϽ�������P1.2, 0: ��ֹ
	ORL		CMPCR2,#INVCMPO	; 1: �Ƚ������ȡ��,  0: ��ȡ��
;	ORL		CMPCR2,#DISFLT	; 1: �ر�0.1uF�˲�,   0: ����
	ORL		CMPCR1,#CMPEN	; 1: ����Ƚ���, 0: ��ֹ,�رձȽ�����Դ

	SETB	EA
L_Loop:
CPL   P1.3	
	SJMP	L_Loop
	SJMP	$
	


;================== �Ƚ����жϺ��� =======================
CMP_ISR:
	PUSH	PSW
	PUSH	ACC
	
	ANL		CMPCR1,#NOT CMPIF	;�����־

	CPL		P1.3		;���ж�,ȡ��P1.3,ʾ�����۲�

	MOV		A,CMPCR1
	JNB		ACC.0,L_CmpOutLow
	CPL		P1.4		;�������ж�, ȡ��P1.4
	SJMP	L_QuitCmpISR

L_CmpOutLow:
	CPL		P1.5		;�½����ж�, ȡ��P1.5

L_QuitCmpISR:
	POP		ACC
	POP		PSW
	RETI
;==================	�Ƚ����жϺ��� end ======================

	END
	
	
	
	