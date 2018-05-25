
;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-755-82905966 -------------------------------------------*/
;/* --- Tel: 86-755-82948412 -------------------------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
;/*---------------------------------------------------------------------*/

;*************	����˵��	**************

;ʹ��STC15ϵ�е�PCA0��PCA1����ģ�⴮��. PCA0����(P2.5), PCA1����(P2.6).

;�ٶ�����оƬ�Ĺ���Ƶ��Ϊ22118400Hz. ʱ��Ϊ5.5296MHZ ~ 35MHZ. 

;�����ʸߣ���ʱ��ҲҪѡ��, ����ʹ�� 22.1184MHZ, 11.0592MHZ.

;���Է���: ��λ����������,MCU�յ����ݺ�ԭ������.

;���ڹ̶�����: 1λ��ʼλ, 8λ����λ, 1λֹͣλ.

;******************************************

STACK_POIRTER	EQU		0D0H	;��ջ��ʼ��ַ

;UART3_BitTime	EQU		9216	; 1200bps @ 11.0592MHz	UART3_BitTime = (MAIN_Fosc / Baudrate)
;UART3_BitTime	EQU		4608	; 2400bps @ 11.0592MHz
;UART3_BitTime	EQU		2304	; 4800bps @ 11.0592MHz
;UART3_BitTime	EQU		1152	; 9600bps @ 11.0592MHz
;UART3_BitTime	EQU		 576	;19200bps @ 11.0592MHz
;UART3_BitTime	EQU		 288	;38400bps @ 11.0592MHz

;UART3_BitTime	EQU		15360	; 1200bps @ 18.432MHz
;UART3_BitTime	EQU		7680	; 2400bps @ 18.432MHz
;UART3_BitTime	EQU		3840	; 4800bps @ 18.432MHz
;UART3_BitTime	EQU		1920	; 9600bps @ 18.432MHz
;UART3_BitTime	EQU		960		;19200bps @ 18.432MHz
;UART3_BitTime	EQU		480		;38400bps @ 18.432MHz
;UART3_BitTime	EQU		320		;57600bps @ 18.432MHz

;UART3_BitTime	EQU		18432	; 1200bps @ 22.1184MHz
;UART3_BitTime	EQU		9216	; 2400bps @ 22.1184MHz
;UART3_BitTime	EQU		4608	; 4800bps @ 22.1184MHz
;UART3_BitTime	EQU		2304	; 9600bps @ 22.1184MHz
;UART3_BitTime	EQU		1152	;19200bps @ 22.1184MHz
;UART3_BitTime	EQU		576		;38400bps @ 22.1184MHz
UART3_BitTime	EQU		384		;57600bps @ 22.1184MHz

;UART3_BitTime	EQU		27648	; 1200bps @ 33.1776MHz
;UART3_BitTime	EQU		13824	; 2400bps @ 33.1776MHz
;UART3_BitTime	EQU		6912	; 4800bps @ 33.1776MHz
;UART3_BitTime	EQU		3456	; 9600bps @ 33.1776MHz
;UART3_BitTime	EQU		1728	;19200bps @ 33.1776MHz
;UART3_BitTime	EQU		864		;38400bps @ 33.1776MHz
;UART3_BitTime	EQU		576		;57600bps @ 33.1776MHz
;UART3_BitTime	EQU		288		;115200bps @ 33.1776MHz

PCA_P12_P11_P10_P37		EQU		(0 SHL 4)
PCA_P34_P35_P36_P37		EQU		(1 SHL 4)
PCA_P24_P25_P26_P27		EQU		(2 SHL 4)
PCA_Mode_Capture		EQU		0
PCA_Mode_SoftTimer		EQU		048H
PCA_Clock_1T			EQU		(4 SHL 1)
PCA_Clock_2T			EQU		(1 SHL 1)
PCA_Clock_4T			EQU		(5 SHL 1)
PCA_Clock_6T			EQU		(6 SHL 1)
PCA_Clock_8T			EQU		(7 SHL 1)
PCA_Clock_12T			EQU		(0 SHL 1)
PCA_Clock_ECI			EQU		(3 SHL 1)
PCA_Rise_Active			EQU		(1 SHL 5)
PCA_Fall_Active			EQU		(1 SHL 4)

ENABLE					EQU		1

AUXR1  DATA 0xA2
CCON   DATA 0xD8
CMOD   DATA 0xD9
CCAPM0 DATA 0xDA	; PCAģ��0�Ĺ���ģʽ�Ĵ�����
CCAPM1 DATA 0xDB	; PCAģ��1�Ĺ���ģʽ�Ĵ�����
CCAPM2 DATA 0xDC	; PCAģ��2�Ĺ���ģʽ�Ĵ�����

CL     DATA 0xE9
CCAP0L DATA 0xEA	; PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
CCAP1L DATA 0xEB	; PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
CCAP2L DATA 0xEC	; PCAģ��2�Ĳ�׽/�ȽϼĴ�����8λ��

CH     DATA 0xF9
CCAP0H DATA 0xFA	; PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
CCAP1H DATA 0xFB	; PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
CCAP2H DATA 0xFC	; PCAģ��2�Ĳ�׽/�ȽϼĴ�����8λ��

CCF0  BIT CCON.0	; PCA ģ��0�жϱ�־����Ӳ����λ�������������0��
CCF1  BIT CCON.1	; PCA ģ��1�жϱ�־����Ӳ����λ�������������0��
CCF2  BIT CCON.2	; PCA ģ��2�жϱ�־����Ӳ����λ�������������0��
CR    BIT CCON.6	; 1: ����PCA������������0: ��ֹ������
CF    BIT CCON.7	; PCA�����������CH��CL��FFFFH��Ϊ0000H����־��PCA�������������Ӳ����λ�������������0��
PPCA  BIT IP.7		; PCA �ж� ���ȼ��趨λ

;===================== ģ�⴮����� ===========================
P_RX3 		BIT 	P2.5	; ����ģ�⴮�ڽ���IO
P_TX3 		BIT 	P2.6	; ����ģ�⴮�ڷ���IO

Rx3_Ring	BIT		20H.0	; ���ڽ��ձ�־, �Ͳ����ʹ��, �û����򲻿ɼ�
Tx3_Ting	BIT		20H.1	; ���ڷ��ͱ�־, �û���1������, �ײ㷢�������0
RX3_End		BIT		20H.2	; ���յ�һ���ֽ�, �û���ѯ ����0

Tx3_DAT		DATA	30H		; ������λ����, �û����ɼ�
Rx3_DAT		DATA	31H		; ������λ����, �û����ɼ�
Tx3_BitCnt	DATA	32H		; �������ݵ�λ������, �û����ɼ�
Rx3_BitCnt	DATA	33H		; �������ݵ�λ������, �û����ɼ�
Rx3_BUF		DATA	34H		; ���յ����ֽ�, �û���ȡ
Tx3_BUF		DATA	35H		; Ҫ���͵��ֽ�, �û�д��

;=================================================================
Tx3_read	DATA	36H		; ���Ͷ�ָ��
Rx3_write	DATA	37H		; ����дָ��

RX_Lenth	EQU		16		; ���ճ���
buf3		EQU		40H		; 40H ~ 4FH	���ջ���

;*******************************************************************
;*******************************************************************
		ORG		00H				;reset
		LJMP	F_Main

		ORG		3BH				;7  PCA interrupt
		LJMP	F_PCA_Interrupt

;******************** ������ **************************/
F_Main:
	
	MOV		SP, #STACK_POIRTER
	MOV		PSW, #0
	USING	0		;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
	LCALL	F_PCA_Init	;PCA��ʼ��
	SETB	EA
	
	MOV		Tx3_read,  #0
	MOV		Rx3_write, #0
	CLR		Tx3_Ting
	CLR		RX3_End
	CLR		Rx3_Ring
	MOV		Tx3_BitCnt, #0
		
;=================== ��ѭ�� ==================================
L_MainLoop:
	JNB		RX3_End, L_QuitRx3	; ����Ƿ��յ�һ���ֽ�
	CLR		RX3_End				; �����־
	MOV		A, #buf3
	ADD		A, Rx3_write
	MOV		R0, A
	MOV		@R0, Rx3_BUF		; д�뻺��
	INC		Rx3_write			; ָ����һ��λ��
	MOV		A, Rx3_write
	CLR		C
	SUBB	A, #RX_Lenth		; ������
	JC		L_QuitRx3
	MOV		Rx3_write, #0
L_QuitRx3:

	JB		Tx3_Ting, L_QuitTx3	;����Ƿ��Ϳ���
	MOV		A, Tx3_read
	XRL		A, Rx3_write
	JZ		L_QuitTx3			;����Ƿ��յ����ַ�

	MOV		A, #buf3
	ADD		A, Tx3_read
	MOV		R0, A
	MOV		Tx3_BUF, @R0		; �ӻ����һ���ַ�����
	SETB	Tx3_Ting			; ���÷��ͱ�־
	INC		Tx3_read			; ָ����һ���ַ�λ��
	MOV		A, Tx3_read
	CLR		C
	SUBB	A, #RX_Lenth
	JC		L_QuitTx3			; ������
	MOV		Tx3_read, #0
L_QuitTx3:

	LJMP	L_MainLoop

;=================== ��������� ==================================

;========================================================================
; ����: F_PCA_Init
; ����: PCA��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2013-11-22
;========================================================================
F_PCA_Init:
	CLR		CR
	MOV		CH, #0
	MOV		CL, #0
	MOV		A, AUXR1
	ANL		A, # NOT(3 SHL 4)
	ORL		A, #PCA_P24_P25_P26_P27		;�л�IO��
	MOV		AUXR1, A
	ANL		A, #NOT(7 SHL 1)
	ORL		A, #PCA_Clock_1T		;ѡ��ʱ��Դ
	MOV		CMOD, A

	MOV		CCAPM0, #(PCA_Mode_Capture OR PCA_Fall_Active OR ENABLE)	; 16λ�½��ز�׽�ж�ģʽ

	MOV		CCAPM1, #(PCA_Mode_SoftTimer OR ENABLE)	; 16λ�����ʱ��, �ж�ģʽ
	MOV		CCAP1L, #LOW UART3_BitTime				; ��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
	MOV		CCAP1H, #HIGH UART3_BitTime				; ��дCCAP0H

	SETB	PPCA		; �����ȼ��ж�
	SETB	CR			; ����PCA��ʱ��
	RET

;======================================================================

;========================================================================
; ����: F_PCA_Interrupt
; ����: PCA�жϴ������.
; ����: None
; ����: none.
; �汾: V1.0, 2012-11-22
;========================================================================
F_PCA_Interrupt:
	PUSH	PSW
	PUSH	ACC

	;=============== PCAģ��0�ж� ===================
	JNB		CCF0, L_QuitPCA0	; PCAģ��0�ж�
	CLR		CCF0				; ��PCAģ��0�жϱ�־

	JNB		Rx3_Ring, L_Rx3_Start	; ���յ���ʼλ
	DJNZ	Rx3_BitCnt, L_RxBit		; ������һ֡����
	
	CLR		Rx3_Ring			; ֹͣ����
	MOV		Rx3_BUF, Rx3_DAT	; �洢���ݵ�������
	SETB	RX3_End				;
	MOV		CCAPM0, #(PCA_Mode_Capture OR PCA_Fall_Active OR ENABLE)	; 16λ�½��ز�׽�ж�ģʽ
	SJMP	L_QuitPCA0
	
L_RxBit:
	MOV		A, Rx3_DAT			; �ѽ��յĵ�b���� �ݴ浽 RxShiftReg(���ջ���)
	MOV		C, P_RX3
	RRC		A
	MOV		Rx3_DAT, A
	MOV		A, CCAP0L			;
	ADD		A, #LOW UART3_BitTime	; ����λʱ��
	MOV		CCAP0L, A			; ��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
	MOV		A, CCAP0H			; ����λʱ��
	ADDC	A, #HIGH UART3_BitTime	;
	MOV		CCAP0H, A			; ��дCCAP0H
	SJMP	L_QuitPCA0

L_Rx3_Start:
	MOV		CCAPM0, #(PCA_Mode_SoftTimer OR ENABLE)	; 16λ�����ʱ�ж�ģʽ
	MOV		A, CCAP0L			; ����λʱ��
	ADD		A, #LOW (UART3_BitTime / 2 + UART3_BitTime)	;
	MOV		CCAP0L, A			; ��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
	MOV		A, CCAP0H			; ����λʱ��
	ADDC	A, #HIGH (UART3_BitTime / 2 + UART3_BitTime)	;
	MOV		CCAP0H, A			; ��дCCAP0H
	SETB	Rx3_Ring			; ��־���յ���ʼλ
	MOV		Rx3_BitCnt, #9		; ��ʼ�����յ�����λ��(8������λ+1��ֹͣλ)
L_QuitPCA0:

	;=============== PCAģ��1�ж� ===================
	JNB		CCF1, L_QuitPCA1	; PCAģ��1�ж�, 16λ�����ʱ�ж�ģʽ
	CLR		CCF1				; ��PCAģ��1�жϱ�־
	MOV		A, CCAP1L			;
	ADD		A, #LOW UART3_BitTime	; ����λʱ��
	MOV		CCAP1L, A			; ��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
	MOV		A, CCAP1H			; 
	ADDC	A, #HIGH UART3_BitTime	; ����λʱ��
	MOV		CCAP1H, A			; ��дCCAP0H

	JNB		Tx3_Ting, L_QuitPCA1	; ������, �˳�
	MOV		A, Tx3_BitCnt
	JNZ		L_TxData			; ���ͼ�����Ϊ0 �������ֽڷ��ͻ�û��ʼ
	CLR		P_TX3				; ���Ϳ�ʼλ
	MOV		Tx3_DAT, Tx3_BUF	; �ѻ�������ݷŵ����͵�buff
	MOV		Tx3_BitCnt, #9		; ��������λ�� (8����λ+1ֹͣλ)
	SJMP	L_QuitPCA1
L_TxData:						; ���ͼ�����Ϊ��0 ���ڷ�������
	DJNZ	Tx3_BitCnt, L_TxBit	; ���ͼ�������Ϊ0 �������ֽڷ��ͽ���
	SETB	P_TX3				; ��ֹͣλ����
	CLR		Tx3_Ting			; ����ֹͣ
	SJMP	L_QuitPCA1
L_TxBit:
	MOV		A, Tx3_DAT			; �����λ�͵� CY(�洦��־λ)
	RRC		A
	MOV		P_TX3, C			; ����һ��bit����
	MOV		Tx3_DAT, A
L_QuitPCA1:

	POP		ACC
	POP		PSW

	RETI
	

	END



