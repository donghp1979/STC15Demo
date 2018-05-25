
;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-755-82905966 -------------------------------------------*/
;/* --- Tel: 86-755-82948412 -------------------------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
;/*---------------------------------------------------------------------*/

;*************	����˵��	**************

;ʹ��STC15ϵ�е�Timer2����ģ�⴮��. P3.0����, P3.1����, ��˫��.

;�ٶ�����оƬ�Ĺ���Ƶ��Ϊ22118400Hz. ʱ��Ϊ5.5296MHZ ~ 35MHZ. 

;�����ʸߣ���ʱ��ҲҪѡ��, ����ʹ�� 22.1184MHZ, 11.0592MHZ.

;���Է���: ��λ����������, MCU�յ����ݺ�ԭ������.

;���ڹ̶�����: 1λ��ʼλ, 8λ����λ, 1λֹͣλ,  �������ڷ�Χ����.

;******************************************

;UART3_BitTime	EQU		9216	; 1200bps @ 11.0592MHz	UART3_BitTime = (MAIN_Fosc / Baudrate)
;UART3_BitTime	EQU		4608	; 2400bps @ 11.0592MHz
;UART3_BitTime	EQU		2304	; 4800bps @ 11.0592MHz
;UART3_BitTime	EQU		1152	; 9600bps @ 11.0592MHz
;UART3_BitTime	EQU		 576	;19200bps @ 11.0592MHz
;UART3_BitTime	EQU		 288	;38400bps @ 11.0592MHz
;UART3_BitTime	EQU		 192	;57600bps @ 11.0592MHz

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
;UART3_BitTime	EQU		384		;57600bps @ 22.1184MHz
UART3_BitTime	EQU		192		;115200bps @ 22.1184MHz

;UART3_BitTime	EQU		27648	; 1200bps @ 33.1776MHz
;UART3_BitTime	EQU		13824	; 2400bps @ 33.1776MHz
;UART3_BitTime	EQU		6912	; 4800bps @ 33.1776MHz
;UART3_BitTime	EQU		3456	; 9600bps @ 33.1776MHz
;UART3_BitTime	EQU		1728	;19200bps @ 33.1776MHz
;UART3_BitTime	EQU		864		;38400bps @ 33.1776MHz
;UART3_BitTime	EQU		576		;57600bps @ 33.1776MHz
;UART3_BitTime	EQU		288		;115200bps @ 33.1776MHz

IE2			DATA 	0AFH
AUXR		DATA 	08EH
INT_CLKO	DATA 	08FH
T2H			DATA 	0D6H
T2L			DATA 	0D7H


;===================== ģ�⴮����� ===========================
P_RX3 		BIT 	P3.0	; ����ģ�⴮�ڽ���IO
P_TX3 		BIT 	P3.1	; ����ģ�⴮�ڷ���IO

Rx3_Ring	BIT		20H.0	; ���ڽ��ձ�־, �Ͳ����ʹ��, �û����򲻿ɼ�
Tx3_Ting	BIT		20H.1	; ���ڷ��ͱ�־, �û���1������, �ײ㷢�������0
RX3_End		BIT		20H.2	; ���յ�һ���ֽ�, �û���ѯ ����0
B_RxOk		BIT		20H.3	; ���ս�����־

Tx3_DAT		DATA	30H		; ������λ����, �û����ɼ�
Rx3_DAT		DATA	31H		; ������λ����, �û����ɼ�
Tx3_BitCnt	DATA	32H		; �������ݵ�λ������, �û����ɼ�
Rx3_BitCnt	DATA	33H		; �������ݵ�λ������, �û����ɼ�
Rx3_BUF		DATA	34H		; ���յ����ֽ�, �û���ȡ
Tx3_BUF		DATA	35H		; Ҫ���͵��ֽ�, �û�д��

;=================================================================
RxTimeOutH	DATA	36H		;
RxTimeOutL	DATA	37H		;

Tx3_read	DATA	38H		; ���Ͷ�ָ��
Rx3_write	DATA	39H		; ����дָ��

RX_Lenth	EQU		32		; ���ճ���
buf3		EQU		40H		; 40H ~ 5FH	���ջ���

STACK_POIRTER	EQU		0D0H	;��ջ��ʼ��ַ

;*******************************************************************
;*******************************************************************
		ORG		00H				;reset
		LJMP	F_Main

		ORG		63H				;12  Timer2 interrupt
		LJMP	F_Timer2_Interrupt

		ORG		83H				;16  INT4 interrupt
		LJMP	F_INT4_Interrupt

;******************** ������ **************************/
F_Main:
	
	MOV		SP, #STACK_POIRTER
	MOV		PSW, #0
	USING	0		;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
	LCALL	F_UART_Init		;UART��ʼ��
	SETB	EA
	
		
;=================== ��ѭ�� ==================================
L_MainLoop:
	JNB		RX3_End, L_QuitRx3	; ����Ƿ��յ�һ���ֽ�
	CLR		RX3_End				; �����־
	MOV		A, #buf3
	ADD		A, Rx3_write
	MOV		R0, A
	MOV		@R0, Rx3_BUF		; д�뻺��
	MOV		RxTimeOutH, #HIGH 1000	; װ�س�ʱʱ��
	MOV		RxTimeOutL, #LOW  1000	;
	INC		Rx3_write			; ָ����һ��λ��
	MOV		A, Rx3_write
	CLR		C
	SUBB	A, #RX_Lenth		; ������
	JC		L_QuitRx3
	MOV		Rx3_write, #0
L_QuitRx3:

	MOV		A, RxTimeOutL
	ORL		A, RxTimeOutH
	JZ		L_QuitTimeOut	; ��ʱʱ���Ƿ��0?
	MOV		A, RxTimeOutL
	DEC		RxTimeOutL		; (��ʱʱ��  - 1) == 0?
	JNZ		$+4
	DEC		RxTimeOutH
	DEC		A
	ORL		A, RxTimeOutH
	JNZ		L_QuitTimeOut
	
	SETB	B_RxOk						; ��ʱ, ��־�������
	ANL		AUXR, #NOT (1 SHL 4)		; Timer2 ֹͣ����
	ANL		INT_CLKO, #NOT (1 SHL 6)	; ��ֹINT4�ж�
	MOV		T2H, #HIGH (65536 - UART3_BitTime)	;����λ
	MOV		T2L, #LOW (65536 - UART3_BitTime)	;
	ORL		AUXR, #(1 SHL 4)			; Timer2 ��ʼ����
L_QuitTimeOut:	

	JNB		B_RxOk, L_QuitTx3	; ����Ƿ����OK?
	JB		Tx3_Ting, L_QuitTx3	; ����Ƿ��Ϳ���
	MOV		A, Tx3_read
	XRL		A, Rx3_write
	JZ		L_TxFinish			; ����Ƿ������

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
	SJMP	L_QuitTx3

L_TxFinish:
	CLR		B_RxOk
	ANL		AUXR, #NOT (1 SHL 4)	; Timer2 ֹͣ����
	ORL		INT_CLKO, #(1 SHL 6)	; ����INT4�ж�

L_QuitTx3:

	LJMP	L_MainLoop

;=================== ��������� ==================================

;========================================================================
; ����: F_UART_Init
; ����: UART��ʼ������.
; ����: none
; ����: none.
; �汾: V1.0, 2013-11-22
;========================================================================
F_UART_Init:
	MOV		Tx3_read,  #0
	MOV		Rx3_write, #0
	CLR		Tx3_Ting
	CLR		RX3_End
	CLR		Rx3_Ring
	MOV		Tx3_BitCnt, #0

	MOV		RxTimeOutH, #0
	MOV		RxTimeOutL, #0
	CLR		B_RxOk

	ANL		AUXR, #NOT(1 SHL 4)		; Timer2 ֹͣ����
	MOV		T2H, #HIGH (65536 - UART3_BitTime)	; ����λ
	MOV		T2L, #LOW (65536 - UART3_BitTime)	; ����λ
	ORL		INT_CLKO, #(1 SHL 6)	; ����INT4�ж�
	ORL		IE2, #(1 SHL 2)			; ����Timer2�ж�
	ORL		AUXR, #(1 SHL 2)		; 1Tģʽ
	RET

;======================================================================

;========================================================================
; ����: void	F_Timer2_Interrupt
; ����: Timer2�жϴ������.
; ����: None
; ����: none.
; �汾: V1.0, 2012-11-22
;========================================================================
F_Timer2_Interrupt:
	PUSH	PSW
	PUSH	ACC

	JNB		Rx3_Ring, L_QuitRx	; ���յ���ʼλ
	DJNZ	Rx3_BitCnt, L_RxBit	; ������һ֡����
	
	CLR		Rx3_Ring			; ֹͣ����
	MOV		Rx3_BUF, Rx3_DAT	; �洢���ݵ�������
	SETB	RX3_End				;
	ANL		AUXR, #NOT (1 SHL 4); Timer2 ֹͣ����
	ORL		INT_CLKO, #(1 SHL 6); ����INT4�ж�
	SJMP	L_QuitRx
	
L_RxBit:
	MOV		A, Rx3_DAT			; �ѽ��յĵ�b���� �ݴ浽 RxShiftReg(���ջ���)
	MOV		C, P_RX3
	RRC		A
	MOV		Rx3_DAT, A
L_QuitRx:

	JNB		Tx3_Ting, L_QuitTx	; ������, �˳�
	MOV		A, Tx3_BitCnt
	JNZ		L_TxData			; ���ͼ�����Ϊ0 �������ֽڷ��ͻ�û��ʼ
	CLR		P_TX3				; ���Ϳ�ʼλ
	MOV		Tx3_DAT, Tx3_BUF	; �ѻ�������ݷŵ����͵�buff
	MOV		Tx3_BitCnt, #9		; ��������λ�� (8����λ+1ֹͣλ)
	SJMP	L_QuitTx
L_TxData:						; ���ͼ�����Ϊ��0 ���ڷ�������
	DJNZ	Tx3_BitCnt, L_TxBit	; ���ͼ�������Ϊ0 �������ֽڷ��ͽ���
	SETB	P_TX3				; ��ֹͣλ����
	CLR		Tx3_Ting			; ����ֹͣ
	SJMP	L_QuitTx
L_TxBit:
	MOV		A, Tx3_DAT			; �����λ�͵� CY(�洦��־λ)
	RRC		A
	MOV		P_TX3, C			; ����һ��bit����
	MOV		Tx3_DAT, A
L_QuitTx:

	POP		ACC
	POP		PSW

	RETI
	

;===================== INT4�жϺ��� ==================================
F_INT4_Interrupt:
	PUSH	PSW
	PUSH	ACC

	ANL		AUXR, #NOT(1 SHL 4)	;Timer2 ֹͣ����
	MOV		T2H, #HIGH (65536 - (UART3_BitTime / 2 + UART3_BitTime))	; ��ʼλ + �������λ
	MOV		T2L, #LOW (65536 - (UART3_BitTime / 2 + UART3_BitTime))		; ��ʼλ + �������λ
	ORL		AUXR, #(1 SHL 4)	;Timer2 ��ʼ����
	SETB	Rx3_Ring			; ��־���յ���ʼλ
	MOV		Rx3_BitCnt, #9		; ��ʼ�����յ�����λ��(8������λ+1��ֹͣλ)
	
	ANL		INT_CLKO,  #NOT(1 SHL 6);	//��ֹINT4�ж�
	MOV		T2H, #HIGH (65536 - UART3_BitTime)	; ����λ
	MOV		T2L, #LOW (65536 - UART3_BitTime)	; ����λ

	POP		ACC
	POP		PSW
	RETI

	END



