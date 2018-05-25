
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




;�ϵ��, ����ID�ţ�ͨ��ģ�⴮�ڷ��͸���λ����������9600,8,n,1��

;������Ϊ����256�ֽ�RAM, 8K ROM��MCU, ����STC15W408AS.
;�������͵�MCU,����ʵ������޸�"���س�������"�еĵ�ַ.


;*************	���س�������	**************
IDATA_ID_START_ADDR	EQU	0f1H		;ָ��idata��ID�ŵĿ�ʼ��ַ
CODE_ID_START_ADDR	EQU	(8192-7)	;ָ��code ��ID�ŵĿ�ʼ��ַ


;*************	���ر�������	**************/
ID_idata	EQU	030H	;idata ����ID��
ID_code		EQU	037H	;code  ����ID��

P_TXD	BIT	P3.1


	ORG     0000H
	LJMP    L_RESET

L_RESET:
	MOV     SP,#50H

	MOV		R0,#IDATA_ID_START_ADDR
	MOV		R1,#ID_idata
	MOV		R2,#7
L_ReadIdataID_Loop1:
	MOV		A,@R0
	MOV		@R1,A
	INC		R0
	INC		R1
	DJNZ	R2,L_ReadIdataID_Loop1

	MOV		R1,#ID_code
	MOV		DPTR,#CODE_ID_START_ADDR
	MOV		R2,#7
L_ReadCodeID_Loop1:
	CLR		A
	MOVC	A,@A+DPTR
	MOV		@R1,A
	INC		R1
	INC		DPTR
	DJNZ	R2,L_ReadCodeID_Loop1

	LCALL	F_Tx_0D0A
	MOV		DPTR,#T_Txt1
	MOV		B,#21
	LCALL	PrintString		;B -- ����,  DPTR -- �׵�ַ, ռ��R5,R6,R7

	MOV		R1,#ID_idata
	MOV		R2,#7
L_TxIdataID_Loop1:
	MOV		A,@R1
	SWAP	A
	LCALL	F_Hex2Ascii
	LCALL	F_TxSend	;ACC -- Ҫ���͵��ֽ�, ռ��R5,R6,R7
	MOV		A,@R1
	LCALL	F_Hex2Ascii
	LCALL	F_TxSend	;ACC -- Ҫ���͵��ֽ�, ռ��R5,R6,R7
	INC		R1
	DJNZ	R2,L_TxIdataID_Loop1

	LCALL	F_Tx_0D0A
	MOV		DPTR,#T_Txt2
	MOV		B,#21
	LCALL	PrintString		;B -- ����,  DPTR -- �׵�ַ, ռ��R5,R6,R7

	MOV		R1,#ID_code
	MOV		R2,#7
L_TxCodeID_Loop1:
	MOV		A,@R1
	SWAP	A
	LCALL	F_Hex2Ascii
	LCALL	F_TxSend	;ACC -- Ҫ���͵��ֽ�, ռ��R5,R6,R7
	MOV		A,@R1
	LCALL	F_Hex2Ascii
	LCALL	F_TxSend	;ACC -- Ҫ���͵��ֽ�, ռ��R5,R6,R7
	INC		R1
	DJNZ	R2,L_TxCodeID_Loop1
	LCALL	F_Tx_0D0A

	SJMP	$

T_Txt1:
	DB	"Read ID from idata = "
T_Txt2:
	DB	"Read ID from CODE  = "

;**********************************************/
F_Hex2Ascii:
	ANL		A,#0FH
	CLR		C
	SUBB	A,#10
	JC		L_Hex0_9
	ADD		A,#'A'
	RET
L_Hex0_9:
	ADD		A,#(10+'0')
	RET


;========================================================================
F_BitTime:		//2304T = 104.16us
	MOV		R6,#3
	MOV		R7,#(64-2)	;(3-1)*256+64=576, 576*4=2304, 2*4Ϊ���÷���
L_BitTimeLoop:
	DJNZ	R7,$
	DJNZ	R6,L_BitTimeLoop
	RET

;========================================================================
F_TxSend:	;ACC -- Ҫ���͵��ֽ�, ռ��R5,R6,R7
	MOV		R5,#8
	CLR		P_TXD
	LCALL	F_BitTime

L_TxSendLoop:
	RRC		A
	MOV		P_TXD,C
	LCALL	F_BitTime
	DJNZ	R5,L_TxSendLoop

	SETB	P_TXD
	LCALL	F_BitTime
	LCALL	F_BitTime
	RET

;========================================================================
PrintString:	;B -- ����,  DPTR -- �׵�ַ, ռ��R5,R6,R7
	CLR		A
	MOVC	A,@A+DPTR
	LCALL	F_TxSend
	INC		DPTR
	DJNZ	B,PrintString
	RET

F_Tx_0D0A:
	MOV		A,#0DH
	LCALL	F_TxSend
	MOV		A,#0AH
	LCALL	F_TxSend
	RET


END


