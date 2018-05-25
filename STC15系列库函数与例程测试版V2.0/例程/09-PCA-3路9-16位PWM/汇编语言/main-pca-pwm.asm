
/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-755-82905966 -------------------------------------------*/
/* --- Tel: 86-755-82948412 -------------------------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
/*---------------------------------------------------------------------*/



;*************	����˵��	**************

; ���3·9~16λPWM�źš�

; PWMƵ�� = MAIN_Fosc / PWM_DUTY, ���� MAIN_Fosc = 24MHZ, PWM_DUTY = 6000, �����PWMƵ��Ϊ4000HZ.

; ******************************************

;***************************�û��궨��*******************************************************
Fosc_KHZ		EQU		24000	//������ʱ��, KHZ

PWM_DUTY		EQU		6000		//����PWM�����ڣ���ֵΪPCA��ѡ���ʱ�����������
PWM_HIGH_MIN	EQU		80			//����PWM�������Сռ�ձ�, �����ж�����װ����ʱ�䲻����
PWM_HIGH_MAX	EQU		(PWM_DUTY - PWM_HIGH_MIN)		//����PWM��������ռ�ձȡ�

;********************************************************************************************

PCA0					EQU		0
PCA1					EQU		1
PCA2					EQU		2
PCA_Counter				EQU		3
PCA_P12_P11_P10_P37		EQU		(0 SHL 4)
PCA_P34_P35_P36_P37		EQU		(1 SHL 4)
PCA_P24_P25_P26_P27		EQU		(2 SHL 4)
PCA_Mode_Capture		EQU		0
PCA_Mode_SoftTimer		EQU		048H
PCA_Mode_HighPulseOutput	EQU	04CH
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

P2M1 DATA 095H		; P2M1.n,P2M0.n 	=00--->Standard,	01--->push-pull
P2M0 DATA 096H		; 					=10--->pure input,	11--->open drain

;================================================================

P25 BIT P2.5
P26 BIT P2.6
P27 BIT P2.7

PWM0_high_H		DATA	030H
PWM0_high_L		DATA	031H
PWM0_low_H		DATA	032H
PWM0_low_L		DATA	033H

PWM1_high_H		DATA	034H
PWM1_high_L		DATA	035H
PWM1_low_H		DATA	036H
PWM1_low_L		DATA	037H

PWM2_high_H		DATA	038H
PWM2_high_L		DATA	039H
PWM2_low_H		DATA	03AH
PWM2_low_L		DATA	03BH

pwm0_H			DATA	03CH
pwm0_L			DATA	03DH
pwm1_H			DATA	03EH
pwm1_L			DATA	03FH
pwm2_H			DATA	040H
pwm2_L			DATA	041H

STACK_POIRTER	EQU		0D0H	;��ջ��ʼ��ַ

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
	ANL		P2M1, #NOT 0E0H		; P2.7 P2.6 P2.5 ����Ϊ�������
	ORL		P2M0, #0E0H

;=================== ��ѭ�� ==================================
L_MainLoop:
	MOV		R7, #2
	LCALL	F_delay_ms

	MOV		A, pwm0_L		; if(++pwm0 >= PWM_HIGH_MAX)	pwm0 = PWM_HIGH_MIN
	ADD		A, #1
	MOV		pwm0_L, A
	MOV		A, pwm0_H
	ADDC	A, #0
	MOV		pwm0_H, A
	MOV		A, pwm0_L
	CLR		C
	SUBB	A, #LOW PWM_HIGH_MAX
	MOV		A, pwm0_H
	SUBB	A, #HIGH PWM_HIGH_MAX
	JC		L_PWM0_NotOverFollow
	MOV		pwm0_H, #HIGH PWM_HIGH_MIN
	MOV		pwm0_L, #LOW PWM_HIGH_MIN
L_PWM0_NotOverFollow:
	MOV		R5, #PCA0
	MOV		R6, pwm0_H
	MOV		R7, pwm0_L
	LCALL	F_PWMn_Update

	MOV		A, pwm1_L		; if(++pwm1 >= PWM_HIGH_MAX)	pwm1 = PWM_HIGH_MIN
	ADD		A, #1
	MOV		pwm1_L, A
	MOV		A, pwm1_H
	ADDC	A, #0
	MOV		pwm1_H, A
	MOV		A, pwm1_L
	CLR		C
	SUBB	A, #LOW PWM_HIGH_MAX
	MOV		A, pwm1_H
	SUBB	A, #HIGH PWM_HIGH_MAX
	JC		L_PWM1_NotOverFollow
	MOV		pwm1_H, #HIGH PWM_HIGH_MIN
	MOV		pwm1_L, #LOW PWM_HIGH_MIN
L_PWM1_NotOverFollow:
	MOV		R5, #PCA1
	MOV		R6, pwm1_H
	MOV		R7, pwm1_L
	LCALL	F_PWMn_Update

	MOV		A, pwm2_L		; if(++pwm2 >= PWM_HIGH_MAX)	pwm2 = PWM_HIGH_MIN
	ADD		A, #1
	MOV		pwm2_L, A
	MOV		A, pwm2_H
	ADDC	A, #0
	MOV		pwm2_H, A
	MOV		A, pwm2_L
	CLR		C
	SUBB	A, #LOW PWM_HIGH_MAX
	MOV		A, pwm2_H
	SUBB	A, #HIGH PWM_HIGH_MAX
	JC		L_PWM2_NotOverFollow
	MOV		pwm2_H, #HIGH PWM_HIGH_MIN
	MOV		pwm2_L, #LOW PWM_HIGH_MIN
L_PWM2_NotOverFollow:
	MOV		R5, #PCA2
	MOV		R6, pwm2_H
	MOV		R7, pwm2_L
	LCALL	F_PWMn_Update

	LJMP	L_MainLoop


;//========================================================================
;// ����: F_delay_ms
;// ����: ��ʱ�ӳ���
;// ����: R7: ��ʱms��.
;// ����: none.
;// �汾: VER1.0
;// ����: 2013-4-1
;// ��ע: ����ACCC��PSW��, ���õ���ͨ�üĴ�������ջ
;//========================================================================
F_delay_ms:
	PUSH	AR3		;��ջR3
	PUSH	AR4		;��ջR4

L_delay_ms_1:
	MOV		R3, #HIGH (Fosc_KHZ / 13)
	MOV		R4, #LOW (Fosc_KHZ / 13)
	
L_delay_ms_2:
	MOV		A, R4			;1T		Total 13T/loop
	DEC		R4				;2T
	JNZ		L_delay_ms_3	;4T
	DEC		R3
L_delay_ms_3:
	DEC		A				;1T
	ORL		A, R3			;1T
	JNZ		L_delay_ms_2	;4T
	
	DJNZ	R7, L_delay_ms_1

	POP		AR4		;��ջR2
	POP		AR3		;��ջR3
	RET


;========================================================================
; ����: F_PWMn_Update
; ����: ����ռ�ձ����ݡ�
; ����: R5: PCAͨ������
;		R6,R7: PWMֵ.
; ����: ��
; �汾: VER1.0
; ����: 2014-2-15
; ��ע: 
;========================================================================
F_PWMn_Update:
	PUSH	AR3
	PUSH	AR4

	CLR		C
	MOV		A, R7
	SUBB	A, #LOW PWM_HIGH_MAX
	MOV		A, R6
	SUBB	A, #HIGH PWM_HIGH_MAX
	JC		L_QuitCheckPwm_1
	MOV		R6, #HIGH PWM_HIGH_MAX	; ���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
	MOV		R7, #LOW  PWM_HIGH_MAX
L_QuitCheckPwm_1:

	CLR		C
	MOV		A, R7
	SUBB	A, #LOW PWM_HIGH_MIN
	MOV		A, R6
	SUBB	A, #HIGH PWM_HIGH_MIN
	JNC		L_QuitCheckPwm_2
	MOV		R6, #HIGH PWM_HIGH_MIN	; ���д��С����Сռ�ձ����ݣ�ǿ��Ϊ��Сռ�ձȡ�
	MOV		R7, #LOW  PWM_HIGH_MIN
L_QuitCheckPwm_2:

	CLR		C
	MOV		A, #LOW PWM_DUTY	;���㲢����PWM����͵�ƽ��PCAʱ���������
	SUBB	A, R7
	MOV		R4, A
	MOV		A, #HIGH PWM_DUTY
	SUBB	A, R6
	MOV		R3, A

	CJNE	R5, #PCA0, L_NotLoadPCA0
	CLR		CR		; ֹͣPCAһ�ᣬ һ�㲻��Ӱ��PWM��
	MOV		PWM0_high_H, R6		; ����װ��ռ�ձȱ�����
	MOV		PWM0_high_L, R7
	MOV		PWM0_low_H, R3
	MOV		PWM0_low_L, R4
	SETB	CR				; ����PCA��
L_NotLoadPCA0:

	CJNE	R5, #PCA1, L_NotLoadPCA1
	CLR		CR		; ֹͣPCAһ�ᣬ һ�㲻��Ӱ��PWM��
	MOV		PWM1_high_H, R6		; ����װ��ռ�ձȱ�����
	MOV		PWM1_high_L, R7
	MOV		PWM1_low_H, R3
	MOV		PWM1_low_L, R4
	SETB	CR				; ����PCA��
L_NotLoadPCA1:

	CJNE	R5, #PCA2, L_NotLoadPCA2
	CLR		CR		; ֹͣPCAһ�ᣬ һ�㲻��Ӱ��PWM��
	MOV		PWM2_high_H, R6		; ����װ��ռ�ձȱ�����
	MOV		PWM2_high_L, R7
	MOV		PWM2_low_H, R3
	MOV		PWM2_low_L, R4
	SETB	CR				; ����PCA��
L_NotLoadPCA2:
	
	POP		AR4
	POP		AR3
	RET

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

	MOV		CCAPM0, #(PCA_Mode_HighPulseOutput OR ENABLE)	; 16λ�����ʱ����������������ж�ģʽ
	MOV		CCAPM1, #(PCA_Mode_HighPulseOutput OR ENABLE)	; 16λ�����ʱ����������������ж�ģʽ
	MOV		CCAPM2, #(PCA_Mode_HighPulseOutput OR ENABLE)	; 16λ�����ʱ����������������ж�ģʽ

	MOV		pwm0_H, #HIGH (PWM_DUTY / 4 * 1)	;��PWMһ����ֵ
	MOV		pwm0_L, #LOW  (PWM_DUTY / 4 * 1)
	MOV		pwm1_H, #HIGH (PWM_DUTY / 4 * 2)
	MOV		pwm1_L, #LOW  (PWM_DUTY / 4 * 2)
	MOV		pwm2_H, #HIGH (PWM_DUTY / 4 * 3)
	MOV		pwm2_L, #LOW  (PWM_DUTY / 4 * 3)

	MOV		R5, #PCA0
	MOV		R6, pwm0_H
	MOV		R7, pwm0_L
	LCALL	F_PWMn_Update
	MOV		R5, #PCA1
	MOV		R6, pwm1_H
	MOV		R7, pwm1_L
	LCALL	F_PWMn_Update
	MOV		R5, #PCA2
	MOV		R6, pwm2_H
	MOV		R7, pwm2_L
	LCALL	F_PWMn_Update

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

	JNB		P25, L_PCA0_LoadLow
	MOV		A, CCAP0L			; ���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
	ADD		A, PWM0_high_L		; ���ϸߵ�ƽʱ�䣬
	MOV		CCAP0L, A			; ��дCCAP0L
	MOV		A, CCAP0H			;
	ADDC	A, PWM0_high_H	;
	MOV		CCAP0H, A			; ��дCCAP0H
	SJMP	L_QuitPCA0
L_PCA0_LoadLow:
	MOV		A, CCAP0L			; ���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
	ADD		A, PWM0_low_L		; ���ϵ͵�ƽʱ�䣬
	MOV		CCAP0L, A			; ��дCCAP0L
	MOV		A, CCAP0H			;
	ADDC	A, PWM0_low_H	;
	MOV		CCAP0H, A			; ��дCCAP0H
L_QuitPCA0:

	;=============== PCAģ��1�ж� ===================
	JNB		CCF1, L_QuitPCA1	; PCAģ��1�ж�
	CLR		CCF1				; ��PCAģ��1�жϱ�־

	JNB		P26, L_PCA1_LoadLow
	MOV		A, CCAP1L			; ���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
	ADD		A, PWM1_high_L		; ���ϸߵ�ƽʱ�䣬
	MOV		CCAP1L, A			; ��дCCAP1L
	MOV		A, CCAP1H			;
	ADDC	A, PWM1_high_H	;
	MOV		CCAP1H, A			; ��дCCAP1H
	SJMP	L_QuitPCA1
L_PCA1_LoadLow:
	MOV		A, CCAP1L			; ���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
	ADD		A, PWM1_low_L		; ���ϵ͵�ƽʱ�䣬
	MOV		CCAP1L, A			; ��дCCAP1L
	MOV		A, CCAP1H			;
	ADDC	A, PWM1_low_H	;
	MOV		CCAP1H, A			; ��дCCAP1H
L_QuitPCA1:

	;=============== PCAģ��2�ж� ===================
	JNB		CCF2, L_QuitPCA2	; PCAģ��2�ж�
	CLR		CCF2				; ��PCAģ��2�жϱ�־

	JNB		P27, L_PCA2_LoadLow
	MOV		A, CCAP2L			; ���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
	ADD		A, PWM2_high_L		; ���ϸߵ�ƽʱ�䣬
	MOV		CCAP2L, A			; ��дCCAP2L
	MOV		A, CCAP2H			;
	ADDC	A, PWM2_high_H	;
	MOV		CCAP2H, A			; ��дCCAP2H
	SJMP	L_QuitPCA2
L_PCA2_LoadLow:
	MOV		A, CCAP2L			; ���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
	ADD		A, PWM2_low_L		; ���ϵ͵�ƽʱ�䣬
	MOV		CCAP2L, A			; ��дCCAP2L
	MOV		A, CCAP2H			;
	ADDC	A, PWM2_low_H	;
	MOV		CCAP2H, A			; ��дCCAP2H
L_QuitPCA2:

	POP		ACC
	POP		PSW

	RETI
	

	END
