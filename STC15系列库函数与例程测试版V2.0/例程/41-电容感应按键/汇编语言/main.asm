
;/*---------------------------------------------------------------------*/
;/* --- STC MCU Limited ------------------------------------------------*/
;/* --- Mobile: (86)13922805190 ----------------------------------------*/
;/* --- Fax: 86-755-82905966 -------------------------------------------*/
;/* --- Tel: 86-755-82948412 -------------------------------------------*/
;/* --- Web: www.STCMCU.com --------------------------------------------*/
;/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ���˺꾧�Ƽ������ϼ�����   */
;/*---------------------------------------------------------------------*/

;/*************	����˵��	**************

;����ʹ��STC15W408AS��ADC���ĵ��ݸ�Ӧ������.

;�ٶ�����оƬ�Ĺ���Ƶ��Ϊ24MHz

;******************************************/

;/****************************** �û������ ***********************************/

Fosc_KHZ		EQU	24000	;������ʱ�� KHZ

STACK_POIRTER	EQU		0D0H	;��ջ��ʼ��ַ

Timer0_Reload	EQU		(65536 - Fosc_KHZ/600)	;Timer 0 ��װֵ�� ��Ӧ300KHZ


;*******************************************************************
;*******************************************************************
        
P1ASF     DATA 0x9D;	//ֻд��ģ������ѡ��
ADC_CONTR DATA 0xBC;	//��ADϵ��
ADC_RES   DATA 0xBD;	//��ADϵ��
ADC_RESL  DATA 0xBE;	//��ADϵ��
AUXR      DATA 0x8E;
AUXR2     DATA 0x8F;



/*************	���س�������	**************/

TOUCH_CHANNEL	EQU	8	;ADCͨ����

ADC_90T		EQU (3 SHL 5)	;ADCʱ�� 90T
ADC_180T	EQU (2 SHL 5)	;ADCʱ�� 180T
ADC_360T	EQU (1 SHL 5)	;ADCʱ�� 360T
ADC_540T	EQU 0			;ADCʱ�� 540T
ADC_FLAG	EQU (1 SHL 4)	;�����0
ADC_START	EQU (1 SHL 3)	;�Զ���0

/*************	���ر�������	**************/
P_LED7	BIT P2.7;
P_LED6	BIT P2.6;
P_LED5	BIT P2.5;
P_LED4	BIT P2.4;
P_LED3	BIT P2.3;
P_LED2	BIT P2.2;
P_LED1	BIT P2.1;
P_LED0	BIT P2.0;

adc				EQU	30H	;	��ǰADCֵ   	30H~3FH, ���ֽ�һ��ֵ
adc_prev		EQU	40H	;	��һ��ADCֵ 	40H~4FH, ���ֽ�һ��ֵ
TouchZero		EQU	50H	;	0��ADCֵ    	50H~5FH, ���ֽ�һ��ֵ
TouchZeroCnt	EQU	60H	;	0���Զ����ټ���	60H~67H

cnt_250ms		DATA 68H	;



;*******************************************************************
;*******************************************************************
		ORG		00H				;reset
		LJMP	F_Main

		ORG		03H				;0 INT0 interrupt
		RETI
		LJMP	F_INT0_Interrupt      

		ORG		0BH				;1  Timer0 interrupt
		LJMP	F_Timer0_Interrupt

		ORG		13H				;2  INT1 interrupt
		LJMP	F_INT1_Interrupt      

		ORG		1BH				;3  Timer1 interrupt
		LJMP	F_Timer1_Interrupt

		ORG		23H				;4  UART1 interrupt
		LJMP	F_UART1_Interrupt

		ORG		2BH				;5  ADC and SPI interrupt
		LJMP	F_ADC_Interrupt

		ORG		33H				;6  Low Voltage Detect interrupt
		LJMP	F_LVD_Interrupt

		ORG		3BH				;7  PCA interrupt
		LJMP	F_PCA_Interrupt

		ORG		43H				;8  UART2 interrupt
		LJMP	F_UART2_Interrupt

		ORG		4BH				;9  SPI interrupt
		LJMP	F_SPI_Interrupt

		ORG		53H				;10  INT2 interrupt
		LJMP	F_INT2_Interrupt

		ORG		5BH				;11  INT3 interrupt
		LJMP	F_INT3_Interrupt

		ORG		63H				;12  Timer2 interrupt
		LJMP	F_Timer2_Interrupt

		ORG		83H				;16  INT4 interrupt
		LJMP	F_INT4_Interrupt




;******************** ������ **************************/
F_Main:
	
	MOV		R0, #1
L_ClearRamLoop:			;���RAM
	MOV		@R0, #0
	INC		R0
	MOV		A, R0
	CJNE	A, #0FFH, L_ClearRamLoop
	
	MOV		SP, #STACK_POIRTER
	MOV		PSW, #0
	USING	0		;ѡ���0��R0~R7

;================= �û���ʼ������ ====================================
	MOV		R7, #50
	LCALL	F_delay_ms

	CLR		ET0				; ��ʼ��Timer0���һ��300KHZʱ��
	CLR		TR0	;
	ORL		AUXR,  #080H	; Timer0 set as 1T mode
	ORL		AUXR2, #01H		; �������ʱ��
	MOV		TMOD,  #0		; Timer0 set as Timer, 16 bits Auto Reload.
	MOV		TH0, #HIGH Timer0_Reload
	MOV		TL0, #LOW  Timer0_Reload;
	SETB	TR0

	LCALL	F_ADC_init
	MOV		R7, #50
	LCALL	F_delay_ms

	MOV		R0, #adc_prev	; ��ʼ����һ��ADCֵ
L_Init_Loop1:
	MOV		@R0, #03H
	INC		R0
	MOV		@R0, #0FFH
	INC		R0
	MOV		A, R0
	CJNE	A, #(adc_prev + TOUCH_CHANNEL * 2), L_Init_Loop1

	MOV		R0, #TouchZero	; ��ʼ��0��ADCֵ
L_Init_Loop2:
	MOV		@R0, #03H
	INC		R0
	MOV		@R0, #0FFH
	INC		R0
	MOV		A, R0
	CJNE	A, #(TouchZero + TOUCH_CHANNEL * 2), L_Init_Loop2

	MOV		R0, #TouchZeroCnt	; ��ʼ���Զ����ټ���ֵ
L_Init_Loop3:
	MOV		@R0, #0
	INC		R0
	MOV		A, R0
	CJNE	A, #(TouchZeroCnt + TOUCH_CHANNEL), L_Init_Loop3

	MOV		cnt_250ms, #5

;=================== ��ѭ�� ==================================
L_MainLoop:
	MOV		R7, #50			;��ʱ50ms
	LCALL	F_delay_ms
	LCALL	F_ShowLED		; ����һ�δ�����ֵ
	DJNZ	cnt_250ms, L_MainLoop
	
	MOV		cnt_250ms, #5	;250ms����һ��0���Զ�����
	LCALL	F_AutoZero		;�Զ��������

	SJMP	L_MainLoop

;=================== ��������� ==================================



; /*************  ADC��ʼ������ *****************/
F_ADC_init:
	MOV  	P1ASF,#0FFH		;8·ADC
	MOV  	ADC_CONTR,#080H	;����ADC
	RET  	
; END OF ADC_init


; //========================================================================
; // ����: F_Get_ADC10bitResult
; // ����: ��ѯ����һ��ADC���.
; // ����: R7: ѡ��Ҫת����ADC.
; // ����: R6 R7 == 10λADC���.
; // �汾: V1.0, 2014-3-25
; //========================================================================

F_Get_ADC10bitResult:
	USING	0		;ѡ���0��R0~R7

	MOV  	ADC_RES, #0
	MOV  	ADC_RESL,#0
	MOV  	A, R7
	ORL  	A, #0E8H	;(0x80 OR ADC_90T OR ADC_START)	;����ADC
	MOV  	ADC_CONTR, A
	NOP  	
	NOP  	
	NOP  	
	NOP  	

L_10bitADC_Loop1:
	MOV  	A, ADC_CONTR
	JNB  	ACC.4, L_10bitADC_Loop1		;�ȴ�ADCת������

	MOV  	ADC_CONTR,#080H		//�����־
	MOV  	A,ADC_RES
	MOV  	B,#04H
	MUL  	AB
	MOV  	R7,A
	MOV  	R6,B
	MOV  	A, ADC_RESL
	ANL  	A, #03H
	ORL  	A,R7
	MOV  	R7,A
	RET
; END OF _Get_ADC10bitResult



; /********************* �Զ�0����ٺ��� *************************/
F_AutoZero:			;250ms����һ�� ����ʹ������2�������Ĳ�ľ���ֵ֮������⡣
	USING	0		;ѡ���0��R0~R7

	CLR  	A
	MOV  	R5,A
L_AutoZero_Loop:
			;[R6 R7] = adc[i],  (j = adc[i])
	MOV  	A,R5
	ADD  	A,ACC
	ADD  	A,#LOW (adc)
	MOV  	R0,A
	MOV  	A,@R0
	MOV  	R6,A
	INC  	R0
	MOV  	A,@R0
	MOV  	R7,A

			; �����ֵ [R2 R3] = adc[i] - adc_prev[i],   (k = j - adc_prev[i];)	//��ǰһ������
	MOV  	A,R5
	ADD  	A,ACC
	ADD  	A,#LOW (adc_prev+01H)
	MOV  	R0,A
	CLR  	C
	MOV  	A,R7
	SUBB 	A,@R0
	MOV  	R3,A
	MOV  	A,R6
	DEC  	R0
	SUBB 	A,@R0
	MOV  	R2,A

			; ���ֵ�ľ���ֵ [R2 R3],  if(k & 0x8000)	F0 = 1,	k = 0 - k;	//�ͷ�	������β����Ĳ�ֵ
	CLR  	F0		;����
	JNB  	ACC.7, L_AutoZero_1
	SETB 	F0
	CLR		C
	CLR		A
	SUBB	A, R3
	MOV		R3, A
	MOV  	A,R3
	CLR		A
	SUBB	A, R2
	MOV		R2, A

L_AutoZero_1:
	CLR  	C		;���� [R2 R3] - #20,  if(k >= 20)	//�仯�Ƚϴ�
	MOV  	A,R3
	SUBB 	A,#20
	MOV  	A,R2
	SUBB 	A,#00H
	JC   	L_AutoZero_2	;[R2 R3] , 20, ת

	MOV  	A,#LOW (TouchZeroCnt)	;����仯�Ƚϴ�����0������	TouchZeroCnt[i] = 0;
	ADD  	A,R5
	MOV  	R0,A
	MOV  	@R0, #0

			; 	if(F0)	TouchZero[i] = j;		//������ͷţ����ұ仯�Ƚϴ���ֱ�����
	JNB  	F0,L_AutoZero_3
	MOV  	A,R5
	ADD  	A,ACC
	ADD  	A,#LOW (TouchZero)
	MOV  	R0,A
	MOV  	@R0,AR6
	INC  	R0
	MOV  	@R0,AR7
	SJMP 	L_AutoZero_3

L_AutoZero_2:		;�仯�Ƚ�С�����䶯���Զ�0�����
					; 	if(++TouchZeroCnt[i] >= 20)		//������⵽С�仯20��/4 = 5��.
	MOV  	A,#LOW (TouchZeroCnt)
	ADD  	A,R5
	MOV  	R0,A
	INC  	@R0
	MOV  	A,@R0
	CLR  	C
	SUBB 	A,#20
	JC   	L_AutoZero_3	;if(TouchZeroCnt[i] < 20), ת

	MOV  	@R0, #0		;TouchZeroCnt[i] = 0;

	MOV  	A,R5		;TouchZero[i] = adc_prev[i];	//�仯������ֵ��Ϊ0��
	ADD  	A,ACC
	ADD  	A,#LOW (adc_prev)
	MOV  	R0,A
	MOV  	A,@R0
	MOV  	R2,A
	INC  	R0
	MOV  	A,@R0
	MOV  	R3,A
	MOV  	A,R5
	ADD  	A,ACC
	ADD  	A,#LOW (TouchZero)
	MOV  	R0,A
	MOV  	@R0,AR2
	INC  	R0
	MOV  	@R0,AR3

L_AutoZero_3:
			; 	�������ֵ	adc_prev[i] = j;
	MOV  	A,R5
	ADD  	A,ACC
	ADD  	A,#LOW (adc_prev)
	MOV  	R0,A
	MOV  	@R0,AR6
	INC  	R0
	MOV  	@R0,AR7

	INC  	R5
	MOV  	A,R5
	XRL  	A,#08H
	JZ   	$ + 5H
	LJMP 	L_AutoZero_Loop
	RET  	
; END OF AutoZero



; /********************* ��ȡ������Ϣ���� 50ms����1�� *************************/

F_check_adc:		;�жϼ����»��ͷ�,�лز����
	USING	0		;ѡ���0��R0~R7

	MOV  	R4, AR7
			; 	adc[index] = 1023 - Get_ADC10bitResult(index);		//��ȡADCֵ, ת�ɰ��¼�, ADCֵ����
	LCALL	F_Get_ADC10bitResult	;���ص�ADCֵ�� [R6 R7]
	CLR  	C
	MOV  	A,#0FFH		;1023 - [R6 R7]
	SUBB 	A,R7
	MOV  	R7,A
	MOV  	A,#03H
	SUBB 	A,R6
	MOV  	R6,A
	
	MOV  	A,R4		;���� adc[index]
	ADD  	A,ACC
	ADD  	A,#LOW (adc)
	MOV  	R0,A
	MOV  	@R0,AR6
	INC  	R0
	MOV  	@R0,AR7

			; 	if(adc[index] < TouchZero[index])	return	0;	//��0�㻹С��ֵ������Ϊ�Ǽ��ͷ�
	MOV  	A,R4
	ADD  	A,ACC
	ADD  	A,#LOW (TouchZero+01H)
	MOV  	R1,A
	MOV  	A,R4
	ADD  	A,ACC
	ADD  	A,#LOW (adc)
	MOV  	R0,A
	MOV  	A,@R0
	MOV  	R6,A
	INC  	R0
	MOV  	A,@R0
	CLR  	C
	SUBB 	A,@R1		;���� adc[index] - TouchZero[index]
	MOV  	A,R6
	DEC  	R1
	SUBB 	A,@R1
	JNC  	L_check_adc_1	;if(adc[index] >= TouchZero[index]), ת
	MOV  	R7,#00H			;if(adc[index] < TouchZero[index]), ��0�㻹С��ֵ������Ϊ�Ǽ��ͷ�, ����0
	RET  	

L_check_adc_1:
			; �����ֵ 	[R6 R7] = delta = adc[index] - TouchZero[index];
	MOV  	A,R4
	ADD  	A,ACC
	ADD  	A,#LOW (TouchZero+01H)
	MOV  	R1,A
	MOV  	A,R4
	ADD  	A,ACC
	ADD  	A,#LOW (adc+01H)
	MOV  	R0,A
	CLR  	C
	MOV  	A,@R0
	SUBB 	A,@R1
	MOV  	R7,A
	DEC  	R0
	MOV  	A,@R0
	DEC  	R1
	SUBB 	A,@R1
	MOV  	R6,A

			;---- Variable 'delta' assigned to Register 'R6/R7' ----
	CLR  	C
	MOV  	A,R7
	SUBB 	A,#40
	MOV  	A,R6
	SUBB 	A,#00H
	JC   	L_check_adc_2	;if(delta < 40), ת
	MOV  	R7,#1			;if(delta >= 40)	return 1;	//������  ����1
	RET  	

L_check_adc_2:
	SETB 	C
	MOV  	A,R7
	SUBB 	A,#20
	MOV  	A,R6
	SUBB 	A,#00H
	JNC  	L_check_adc_3
	MOV  	R7,#0	;if(delta <= 20)	return 0;	//���ͷ�  ����0
	RET  	
L_check_adc_3:
	MOV  	R7,#2	;if((delta > 20) && (delta < 40))	����ԭ״̬  ����2
	RET  	
; END OF _check_adc


/********************* ������ 50ms����1�� *************************/

F_ShowLED:
	USING	0		;ѡ���0��R0~R7

	MOV  	R7, #0
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck0
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED0, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck0:

	MOV  	R7, #1
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck1
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED1, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck1:

	MOV  	R7, #2
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck2
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED2, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck2:

	MOV  	R7, #3
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck3
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED3, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck3:

	MOV  	R7, #4
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck4
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED4, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck4:

	MOV  	R7, #5
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck5
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED5, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck5:

	MOV  	R7, #6
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck6
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED6, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck6:

	MOV  	R7, #7
	LCALL	F_check_adc
	MOV  	A,R7
	ANL		A, #0FEH
	JNZ  	L_QuitCheck7
	MOV		A, R7
	MOV		C, ACC.0
	CPL		C
	MOV		P_LED7, C	;if(i == 0) ָʾ����, if(i == 1) ָʾ����
L_QuitCheck7:

	RET  	
; END OF ShowLED


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


;*******************************************************************
;**************** �жϺ��� ***************************************************

F_Timer0_Interrupt:
	RETI
	
F_Timer1_Interrupt:
	RETI

F_Timer2_Interrupt:
	RETI

F_INT0_Interrupt:
	RETI
	
F_INT1_Interrupt:
	RETI

F_INT2_Interrupt:
	RETI

F_INT3_Interrupt:
	RETI

F_INT4_Interrupt:
	RETI

F_UART1_Interrupt:
	RETI

F_UART2_Interrupt:
	RETI

F_ADC_Interrupt:
	RETI

F_LVD_Interrupt:
	RETI

F_PCA_Interrupt:
	RETI

F_SPI_Interrupt:
	RETI

	END
	
