#ifndef __INCLUDE_DS18B20_H_
#define __INCLUDE_DS18B20_H_

extern char DS18B20_Init();
extern void DS18B20_WriteByte(unsigned char dat);
extern unsigned char DS18B20_ReadByte();
extern int DS18B20_ReadTemperature();

#endif
