
;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-755-82905966 -------------------------------------------*/
;/* --- Tel: 86-755-82948412 -------------------------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
;/*---------------------------------------------------------------------*/

;/*************	����˵��	**************

;��������ʾʹ�ö�ʱ�������PWM��

;��ʱ��0��16λ�Զ���װ���жϣ���T0CLKO�������PWM��

;��������ʹ��STC15F/Lϵ��MCU�Ķ�ʱ��T0��ģ��PWM�����̡�

;PWM��������������̡��������������װ��Ҫһ��ʱ�䣬����PWMռ�ձ���СΪ32T/���ڣ����Ϊ(����-32T)/����, TΪʱ�����ڡ�

;PWMƵ��Ϊ���ڵĵ�������������Ϊ6000, ʹ��24MHZ����Ƶ����PWMƵ��Ϊ4000HZ��

;******************************************/

;***************************�û��궨��*******************************************************
Fosc_KHZ		EQU		24000	//������ʱ��, KHZ

PWM_DUTY		EQU		6000		//����PWM�����ڣ���ֵΪPCA��ѡ���ʱ�����������
PWM_HIGH_MIN	EQU		32			//����PWM�������Сռ�ձ�, �����ж�����װ����ʱ�䲻����
PWM_HIGH_MAX	EQU		(PWM_DUTY - PWM_HIGH_MIN)		//����PWM��������ռ�ձȡ�

;********************************************************************************************

P3M1		DATA 0B1H	; P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
P3M0		DATA 0B2H	; 					=10--->pure input,	11--->open drain
AUXR		DATA 08EH	;
INT_CLKO	DATA 08FH	;

P_PWM	BIT	P3.5	; ����PWM������š�
;P_PWM	BIT	P1.4	; ����PWM������š�STC15W204S

pwm_H		DATA	030H		; ����PWM����ߵ�ƽ��ʱ��ı������û�����PWM�ı�����
pwm_L		DATA	031H
PWM_high_H	DATA	032H	; �м�������û������޸�
PWM_high_L	DATA	033H
PWM_low_H	DATA	034H
PWM_low_L	DATA	035H

STACK_POIRTER	EQU		0D0H	;��ջ��ʼ��ַ

;*******************************************************************
;*******************************************************************

	ORG		00H				;reset
	LJMP	F_Main

	ORG		0BH				;1  Timer0 interrupt
	LJMP	F_Timer0_Interrupt


;******************** ������ **************************/
F_Main:
	MOV		SP, #STACK_POIRTER
	MOV		PSW, #0
	USING	0		;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
	CLR		P_PWM
	ANL		P3M1, #NOT (1 SHL 5)	; P3.5 ����Ϊ�������
	ORL		P3M0, #(1 SHL 5);

;	ANL		P1M1, #NOT (1 SHL 4)	; P1.4 ����Ϊ�������	STC15W204S
;	ORL		P1M0, #(1 SHL 4)		;

	CLR		TR0		; ֹͣ����
	SETB	ET0		; �����ж�
	SETB	PT0		; �����ȼ��ж�
	ANL		TMOD, #NOT 003H			; ����ģʽ,0: 16λ�Զ���װ
	ORL		AUXR, #080H			; 1T
	ANL		TMOD, #NOT 004H		; ��ʱ
	ORL		INT_CLKO, #001H		; ���ʱ��

	MOV		TH0, #0
	MOV		TL0, #0
	SETB	TR0		; ��ʼ����

	SETB	EA

	MOV		pwm_H, #HIGH (PWM_DUTY / 10)	;��PWMһ����ֵ������Ϊ10%ռ�ձ�
	MOV		pwm_L, #LOW  (PWM_DUTY / 10)
	MOV		R6, pwm_H
	MOV		R7, pwm_L
	LCALL	F_PWMn_Update		; ����PWM��װֵ

;=================== ��ѭ�� ==================================
L_MainLoop1:
	MOV		R7, #2
	LCALL	F_delay_ms

	MOV		A, pwm_L		; if(++pwm >= PWM_HIGH_MAX)
	ADD		A, #1
	MOV		pwm_L, A
	MOV		A, pwm_H
	ADDC	A, #0
	MOV		pwm_H, A
	MOV		A, pwm_L
	CLR		C
	SUBB	A, #LOW PWM_HIGH_MAX
	MOV		A, pwm_H
	SUBB	A, #HIGH PWM_HIGH_MAX
	JC		L_PWM_NotUpOverFollow		; PWM�𽥼ӵ����
	MOV		pwm_H, #HIGH PWM_HIGH_MAX
	MOV		pwm_L, #LOW PWM_HIGH_MAX
	SJMP	L_MainLoop2
L_PWM_NotUpOverFollow:
	MOV		R6, pwm_H
	MOV		R7, pwm_L
	LCALL	F_PWMn_Update
	SJMP	L_MainLoop1
	
L_MainLoop2:
	MOV		R7, #2
	LCALL	F_delay_ms

	MOV		A, pwm_L		; if(++pwm < PWM_HIGH_MIN)
	CLR		C
	SUBB	A, #1
	MOV		pwm_L, A
	MOV		A, pwm_H
	SUBB	A, #0
	MOV		pwm_H, A
	MOV		A, pwm_L
	CLR		C
	SUBB	A, #LOW PWM_HIGH_MIN
	MOV		A, pwm_H
	SUBB	A, #HIGH PWM_HIGH_MIN
	JNC		L_PWM_NotDnOverFollow		; PWM�𽥼�����С
	MOV		pwm_H, #HIGH PWM_HIGH_MIN
	MOV		pwm_L, #LOW PWM_HIGH_MIN
	SJMP	L_MainLoop1
L_PWM_NotDnOverFollow:
	MOV		R6, pwm_H
	MOV		R7, pwm_L
	LCALL	F_PWMn_Update
	SJMP	L_MainLoop2



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
; ����: R6,R7: PWMֵ��
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
	MOV		A, R7	;���㲢����PWM����͵�ƽ��T0ʱ���������
	SUBB	A, #LOW PWM_DUTY
	MOV		R4, A
	MOV		A, R6
	SUBB	A, #HIGH PWM_DUTY
	MOV		R3, A

	CLR		C
	MOV		A, #0	;���㲢����PWM����ߵ�ƽ��T0ʱ���������
	SUBB	A, R7
	MOV		R7, A
	MOV		A, #0
	SUBB	A, R6
	MOV		R6, A

	CLR		EA		; ��ֹһ���жϣ� һ�㲻��Ӱ��PWM��
	MOV		PWM_high_H, R6		; ����װ��ռ�ձȱ�����
	MOV		PWM_high_L, R7
	MOV		PWM_low_H, R3
	MOV		PWM_low_L, R4
	SETB	EA
	
	POP		AR4
	POP		AR3
	RET



;********************* Timer0�жϺ���************************/
F_Timer0_Interrupt:
	PUSH	PSW
	PUSH	ACC
	JNB		P_PWM, L_T0_LoadLow
	MOV		TH0, PWM_low_H		; ���������ߵ�ƽ����װ�ص͵�ƽʱ�䡣
	MOV		TL0, PWM_low_L
	SJMP	L_QuitTimer0
L_T0_LoadLow:
	MOV		TH0, PWM_high_H		; ���������͵�ƽ����װ�ظߵ�ƽʱ�䡣
	MOV		TL0, PWM_high_L

L_QuitTimer0:

	POP		ACC
	POP		PSW

	RETI
	

	END



