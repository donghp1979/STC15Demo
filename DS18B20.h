#ifndef __INCLUDE_DS18B20_H_
#define __INCLUDE_DS18B20_H_

#include "stc_config.h"
#include <intrins.h>

sbit DS18B20 = P3^7;
sbit DSPORT = P3^7;

extern void Delay1ms(unsigned int y);
extern char DS18B20_Init();
extern void DS18B20_WriteByte(unsigned char dat);
extern unsigned char DS18B20_ReadByte();
extern int DS18B20_ReadTemperature();
extern void Timer0Init(void);

#endif
