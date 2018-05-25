
;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-755-82905966 -------------------------------------------*/
;/* --- Tel: 86-755-82948412 -------------------------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
;/*---------------------------------------------------------------------*/

;/*************	����˵��	**************

;����2ȫ˫���жϷ�ʽ�շ�ͨѶ���򡣱�����ʹ��22.1184MHZʱ�ӣ���Ҫ�ı䣬���޸������"������ʱ��"��ֵ�����±��롣

;��������Ϊ��115200,8,n,1.

;ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

;******************************************/

;/****************************** �û������ ***********************************/

;UART2_Baudrate	EQU		(-4608)	;   600bps @ 11.0592MHz
;UART2_Baudrate	EQU		(-2304)	;  1200bps @ 11.0592MHz	UART2_Baudrate = (MAIN_Fosc / Baudrate)
;UART2_Baudrate	EQU		(-1152)	;  2400bps @ 11.0592MHz
;UART2_Baudrate	EQU		(-576)	;  4800bps @ 11.0592MHz
;UART2_Baudrate	EQU		(-288)	;  9600bps @ 11.0592MHz
;UART2_Baudrate	EQU		(-144)	; 19200bps @ 11.0592MHz
;UART2_Baudrate	EQU		(-72)	; 38400bps @ 11.0592MHz
;UART2_Baudrate	EQU		(-48)	; 57600bps @ 11.0592MHz
;UART2_Baudrate	EQU		(-24)	;115200bps @ 11.0592MHz

;UART2_Baudrate	EQU		(-7680)	;   600bps @ 18.432MHz
;UART2_Baudrate	EQU		(-3840)	;  1200bps @ 18.432MHz
;UART2_Baudrate	EQU		(-1920)	;  2400bps @ 18.432MHz
;UART2_Baudrate	EQU		(-960)	;  4800bps @ 18.432MHz
;UART2_Baudrate	EQU		(-480)	;  9600bps @ 18.432MHz
;UART2_Baudrate	EQU		(-240)	; 19200bps @ 18.432MHz
;UART2_Baudrate	EQU		(-120)	; 38400bps @ 18.432MHz
;UART2_Baudrate	EQU		(-80)	; 57600bps @ 18.432MHz
;UART2_Baudrate	EQU		(-40)	;115200bps @ 18.432MHz

;UART2_Baudrate	EQU		(-9216)	;   600bps @ 22.1184MHz
;UART2_Baudrate	EQU		(-4608)	;  1200bps @ 22.1184MHz
;UART2_Baudrate	EQU		(-2304)	;  2400bps @ 22.1184MHz
;UART2_Baudrate	EQU		(-1152)	;  4800bps @ 22.1184MHz
;UART2_Baudrate	EQU		(-576)	;  9600bps @ 22.1184MHz
;UART2_Baudrate	EQU		(-288)	; 19200bps @ 22.1184MHz
;UART2_Baudrate	EQU		(-144)	; 38400bps @ 22.1184MHz
;UART2_Baudrate	EQU		(-96)	; 57600bps @ 22.1184MHz
UART2_Baudrate	EQU		(-48)	;115200bps @ 22.1184MHz

;UART2_Baudrate	EQU		(-6912)	; 1200bps @ 33.1776MHz
;UART2_Baudrate	EQU		(-3456)	; 2400bps @ 33.1776MHz
;UART2_Baudrate	EQU		(-1728)	; 4800bps @ 33.1776MHz
;UART2_Baudrate	EQU		(-864)	; 9600bps @ 33.1776MHz
;UART2_Baudrate	EQU		(-432)	;19200bps @ 33.1776MHz
;UART2_Baudrate	EQU		(-216)	;38400bps @ 33.1776MHz
;UART2_Baudrate	EQU		(-144)	;57600bps @ 33.1776MHz
;UART2_Baudrate	EQU		(-72)	;115200bps @ 33.1776MHz


;*******************************************************************
;*******************************************************************
P_SW2		DATA	0BAH
AUXR		DATA 	08EH
T2H			DATA 	0D6H
T2L			DATA 	0D7H
S2CON 		DATA	09AH
S2BUF 		DATA	09BH
IE2   		DATA	0AFH

RX2_Lenth	EQU		32		; ���ڽ��ջ��峤��

B_TX2_Busy	BIT		20H.0	; ����æ��־
TX2_Cnt		DATA	30H		; ���ͼ���
RX2_Cnt		DATA	31H		; ���ռ���
RX2_Buffer	DATA	40H		;40 ~ 5FH ���ջ���

STACK_POIRTER	EQU		0D0H	;��ջ��ʼ��ַ

;*******************************************************************
;*******************************************************************
		ORG		00H				;reset
		LJMP	F_Main

		ORG		43H				;8  UART2 interrupt
		LJMP	F_UART2_Interrupt


;******************** ������ **************************/
F_Main:
	
	MOV		SP, #STACK_POIRTER
	MOV		PSW, #0
	USING	0		;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
	CLR		B_TX2_Busy
	MOV		RX2_Cnt, #0;
	MOV		TX2_Cnt, #0;

	ANL		S2CON, #NOT (1 SHL 7)	;8λ����
	ANL		P_SW2, #NOT 01H			; UART2 ʹ��P1.0 P1.1��	Ĭ��
;	ORL		P_SW2, #1				; UART2 ʹ��P4.6 P4.7��

	ANL		AUXR, #NOT (1 SHL 4)	; Timer stop,  Ĭ��		������ʹ��Timer2����
	ANL		AUXR, #NOT (1 SHL 3)	; Timer2 set As Timer Ĭ��
	ORL		AUXR, #(1 SHL 2)		; Timer2 set as 1T mode
	MOV		T2H, #HIGH UART2_Baudrate
	MOV		T2L, #LOW  UART2_Baudrate
	ORL		S2CON, #(1 SHL 4)		; �������
	ORL		IE2, #1					; �����ж�
	ORL		AUXR, #(1 SHL 4)		; Timer run enable

	SETB	EA		; ����ȫ���ж�
	
;=================== ��ѭ�� ==================================
L_MainLoop:
	MOV		A, TX2_Cnt
	CJNE	A, RX2_Cnt, L_ReturnData
	SJMP	L_QuitCheckRx2
L_ReturnData:					; �յ�������
	JB		B_TX2_Busy, L_QuitCheckRx2		; ����æ�� �˳�
	SETB	B_TX2_Busy			; ��־����æ
	MOV		A, #RX2_Buffer
	ADD		A, TX2_Cnt
	MOV		R0, A
	MOV		S2BUF, @R0		; ��һ���ֽ�
	INC		TX2_Cnt
	MOV		A, TX2_Cnt
	CJNE	A, #RX2_Lenth, L_QuitCheckRx2
	MOV		TX2_Cnt, #0		; �����������
L_QuitCheckRx2:

	LJMP	L_MainLoop


;********************* UART2�жϺ���************************
F_UART2_Interrupt:
	PUSH	PSW
	PUSH	ACC
	
	MOV		A, S2CON
	JNB		ACC.0, L_QuitUartRx
	ANL		S2CON, #NOT 1;
	MOV		A, #RX2_Buffer
	ADD		A, RX2_Cnt
	MOV		R0, A
	MOV		@R0, S2BUF	;����һ���ֽ�
	INC		RX2_Cnt
	MOV		A, RX2_Cnt
	CJNE	A, #RX2_Lenth, L_QuitUartRx
	MOV		RX2_Cnt, #0		; �����������
L_QuitUartRx:

	MOV		A, S2CON
	JNB		ACC.1, L_QuitUartTx
	ANL		S2CON, #NOT 2;
	CLR		B_TX2_Busy		; �������æ��־
L_QuitUartTx:

	POP		ACC
	POP		PSW
	RETI

	END



