
;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-755-82905966 -------------------------------------------*/
;/* --- Tel: 86-755-82948412 -------------------------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
;/*---------------------------------------------------------------------*/

;/*************	����˵��	**************

;����1ȫ˫���жϷ�ʽ�շ�ͨѶ���򡣱�����ʹ��22.1184MHZʱ�ӣ���Ҫ�ı䣬���޸������"������ʱ��"��ֵ�����±��롣

;��������Ϊ��115200,8,n,1.

;ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

;******************************************/

;/****************************** �û������ ***********************************/

;UART1_Baudrate	EQU		(-4608)	;   600bps @ 11.0592MHz
;UART1_Baudrate	EQU		(-2304)	;  1200bps @ 11.0592MHz	UART1_Baudrate = (MAIN_Fosc / Baudrate)
;UART1_Baudrate	EQU		(-1152)	;  2400bps @ 11.0592MHz
;UART1_Baudrate	EQU		(-576)	;  4800bps @ 11.0592MHz
;UART1_Baudrate	EQU		(-288)	;  9600bps @ 11.0592MHz
;UART1_Baudrate	EQU		(-144)	; 19200bps @ 11.0592MHz
;UART1_Baudrate	EQU		(-72)	; 38400bps @ 11.0592MHz
;UART1_Baudrate	EQU		(-48)	; 57600bps @ 11.0592MHz
;UART1_Baudrate	EQU		(-24)	;115200bps @ 11.0592MHz

;UART1_Baudrate	EQU		(-7680)	;   600bps @ 18.432MHz
;UART1_Baudrate	EQU		(-3840)	;  1200bps @ 18.432MHz
;UART1_Baudrate	EQU		(-1920)	;  2400bps @ 18.432MHz
;UART1_Baudrate	EQU		(-960)	;  4800bps @ 18.432MHz
;UART1_Baudrate	EQU		(-480)	;  9600bps @ 18.432MHz
;UART1_Baudrate	EQU		(-240)	; 19200bps @ 18.432MHz
;UART1_Baudrate	EQU		(-120)	; 38400bps @ 18.432MHz
;UART1_Baudrate	EQU		(-80)	; 57600bps @ 18.432MHz
;UART1_Baudrate	EQU		(-40)	;115200bps @ 18.432MHz

;UART1_Baudrate	EQU		(-9216)	;   600bps @ 22.1184MHz
;UART1_Baudrate	EQU		(-4608)	;  1200bps @ 22.1184MHz
;UART1_Baudrate	EQU		(-2304)	;  2400bps @ 22.1184MHz
;UART1_Baudrate	EQU		(-1152)	;  4800bps @ 22.1184MHz
;UART1_Baudrate	EQU		(-576)	;  9600bps @ 22.1184MHz
;UART1_Baudrate	EQU		(-288)	; 19200bps @ 22.1184MHz
;UART1_Baudrate	EQU		(-144)	; 38400bps @ 22.1184MHz
;UART1_Baudrate	EQU		(-96)	; 57600bps @ 22.1184MHz
UART1_Baudrate	EQU		(-48)	;115200bps @ 22.1184MHz

;UART1_Baudrate	EQU		(-6912)	; 1200bps @ 33.1776MHz
;UART1_Baudrate	EQU		(-3456)	; 2400bps @ 33.1776MHz
;UART1_Baudrate	EQU		(-1728)	; 4800bps @ 33.1776MHz
;UART1_Baudrate	EQU		(-864)	; 9600bps @ 33.1776MHz
;UART1_Baudrate	EQU		(-432)	;19200bps @ 33.1776MHz
;UART1_Baudrate	EQU		(-216)	;38400bps @ 33.1776MHz
;UART1_Baudrate	EQU		(-144)	;57600bps @ 33.1776MHz
;UART1_Baudrate	EQU		(-72)	;115200bps @ 33.1776MHz


;*******************************************************************
;*******************************************************************
P_SW1 		DATA	0A2H
AUXR		DATA 	08EH
T2H			DATA 	0D6H
T2L			DATA 	0D7H


RX1_Lenth	EQU		32		; ���ڽ��ջ��峤��

B_TX1_Busy	BIT		20H.0	; ����æ��־
TX1_Cnt		DATA	30H		; ���ͼ���
RX1_Cnt		DATA	31H		; ���ռ���
RX1_Buffer	DATA	40H		;40 ~ 5FH ���ջ���

STACK_POIRTER	EQU		0D0H	;��ջ��ʼ��ַ

;*******************************************************************
;*******************************************************************
		ORG		00H				;reset
		LJMP	F_Main

		ORG		23H				;4  UART1 interrupt
		LJMP	F_UART1_Interrupt


;******************** ������ **************************/
F_Main:
	
	MOV		SP, #STACK_POIRTER
	MOV		PSW, #0
	USING	0		;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
	CLR		B_TX1_Busy
	MOV		RX1_Cnt, #0;
	MOV		TX1_Cnt, #0;

	
	MOV		A, SCON
	ANL		A, #03FH
	ORL		A, #040H	; 8λ����
	MOV		SCON, A

	ANL		P_SW1, #NOT 0C0H	; UART1 ʹ��P30 P31��	Ĭ��
;	ORL		P_SW1, #040H		; UART1 ʹ��P36 P37��
;	ORL		P_SW1, #080H		; UART1 ʹ��P16 P17��

;	CLR		TR1					; Timer Stop	������ʹ��Timer1����
;	ANL		AUXR, #NOT 01H		; S1 BRT Use Timer1;
;	ORL		AUXR, #(1 SHL 6)	; Timer1 set as 1T mode
;	MOV		TH1, #HIGH UART1_Baudrate
;	MOV		TL1, #LOW  UART1_Baudrate
;	SETB	TR1

	ANL		AUXR, #NOT (1 SHL 4)	; Timer stop	������ʹ��Timer2����
	ORL		AUXR, #001H				; S1 BRT Use Timer2;
	ORL		AUXR, #(1 SHL 2)		; Timer2 set as 1T mode
	MOV		T2H, #HIGH UART1_Baudrate
	MOV		T2L, #LOW  UART1_Baudrate
	ORL		AUXR, #(1 SHL 4)		; Timer run enable

	SETB	REN		; �������
	SETB	ES		; �����ж�

	SETB	EA		; ����ȫ���ж�
	
;=================== ��ѭ�� ==================================
L_MainLoop:
	MOV		A, TX1_Cnt
	CJNE	A, RX1_Cnt, L_ReturnData
	SJMP	L_QuitCheckRx1
L_ReturnData:					; �յ�������
	JB		B_TX1_Busy, L_QuitCheckRx1		; ����æ�� �˳�
	SETB	B_TX1_Busy			; ��־����æ
	MOV		A, #RX1_Buffer
	ADD		A, TX1_Cnt
	MOV		R0, A
	MOV		SBUF, @R0		; ��һ���ֽ�
	INC		TX1_Cnt
	MOV		A, TX1_Cnt
	CJNE	A, #RX1_Lenth, L_QuitCheckRx1
	MOV		TX1_Cnt, #0		; �����������
L_QuitCheckRx1:
	LJMP	L_MainLoop


;********************* UART1�жϺ���************************
F_UART1_Interrupt:
	PUSH	PSW
	PUSH	ACC
	
	JNB		RI, L_QuitUartRx
	CLR		RI
	MOV		A, #RX1_Buffer
	ADD		A, RX1_Cnt
	MOV		R0, A
	MOV		@R0, SBUF	;����һ���ֽ�
	INC		RX1_Cnt
	MOV		A, RX1_Cnt
	CJNE	A, #RX1_Lenth, L_QuitUartRx
	MOV		RX1_Cnt, #0		; �����������
L_QuitUartRx:

	JNB		TI, L_QuitUartTx
	CLR		TI
	CLR		B_TX1_Busy		; �������æ��־
L_QuitUartTx:

	POP		ACC
	POP		PSW
	RETI

	END



