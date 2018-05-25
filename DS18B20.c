#include "DS18B20.H"

char DS18B20_Init() {
	unsigned char i;
	if(!DS18B20) return 0;
	DS18B20=0;			 //将总线拉低480us~960us
	for(i=0;i<5;i++) Delay100us();
	DS18B20=1;			//然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低
	i=0;
	while(DS18B20)	//等待DS18B20拉低总线
	{
		i++;
		if(i>5000)//等待>5MS
			return 0;//初始化失败	
	}
	return 1;//初始化成功
}

void DS18B20_WriteByte(unsigned char dat) {
	unsigned char j;
	for(j=0;j<8;j++)
	{
		EA=0;
		_nop_();
		_nop_();
		DSPORT=0;			//每写入一位数据之前先把总线拉低1us
		Delay1us();
		DSPORT=dat&0x01; //然后写入一个数据，从最低位开始
		EA=1;
		Delay10us();
		Delay10us();
		Delay10us();
		DSPORT=1;	//然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		Delay1us();
		dat>>=1;
	}
}

unsigned char DS18B20_ReadByte() {
	unsigned char byte,bi;
	unsigned char i,j;
	byte=0;
	for(j=8;j>0;j--)
	{
		EA=0;
		DSPORT=0;//先将总线拉低1us
		Delay1us();
		DSPORT=1;//然后释放总线
		Delay6us();
		bi=DSPORT;	 //读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
		byte=(byte>>1)|(bi<<7);
		EA=1;
		i=4;
		while(i--) Delay10us();		//读取完之后等待48us再接着读取下一个数
		DSPORT=1;
		Delay1us();
	}				
	return byte;
}

void DS18B20_ChangeTemperatureCOM() {
	DS18B20_Init();
	Delay1ms(1);
	DS18B20_WriteByte(0xcc); // 跳过ROM操作
	DS18B20_WriteByte(0x44); // 进行温度变换
	Delay1ms(600);
}

void DS18B20_ReadTemperatureCOM() {
	DS18B20_Init();
	Delay1ms(1);
	DS18B20_WriteByte(0xcc); // 跳过ROM操作
	DS18B20_WriteByte(0xbe); // 读温度寄存器
}

int DS18B20_ReadTemperature() {
	int temp=0;
	unsigned char tmh,tml;
	DS18B20_ChangeTemperatureCOM();			 	//先写入转换命令
	DS18B20_ReadTemperatureCOM();			//然后等待转换完后发送读取温度命令
	tml=DS18B20_ReadByte();		//读取温度值共16位，先读低字节
	tmh=DS18B20_ReadByte();		//再读高字节
	temp=tmh;
	temp<<=8;
	temp|=tml;
	return temp;
}

