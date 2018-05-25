#include "stc_config.h"
#include "GPIO.h"
#include "timer.h"

void GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitDef;
	
	GPIO_InitDef.Mode = GPIO_PullUp;
	GPIO_InitDef.Pin = GPIO_Pin_All;
	
	GPIO_Inilize(GPIO_P0, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P1, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P2, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P3, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P4, &GPIO_InitDef);
	GPIO_Inilize(GPIO_P5, &GPIO_InitDef);
}

void Timer0_Init() {
	TIM_InitTypeDef TIM_Def;
	
	TIM_Def.TIM_Mode = TIM_16BitAutoReload;
	TIM_Def.TIM_Polity = PolityLow;
	TIM_Def.TIM_Interrupt = ENABLE;
	TIM_Def.TIM_ClkSource = TIM_CLOCK_1T;
	TIM_Def.TIM_ClkOut = DISABLE;
	TIM_Def.TIM_Value = 0xA99A; // 2∫¡√Î@11.0592MHz 1T
	TIM_Def.TIM_Run = ENABLE;
	
	Timer_Inilize(Timer0, &TIM_Def);
}

