#include "DS18B20.H"

char DS18B20_Init() {
	unsigned char i;
	if(!DS18B20) return 0;
	DS18B20=0;			 //����������480us~960us
	for(i=0;i<5;i++) Delay100us();
	DS18B20=1;			//Ȼ���������ߣ����DS18B20������Ӧ�Ὣ��15us~60us����������
	i=0;
	while(DS18B20)	//�ȴ�DS18B20��������
	{
		i++;
		if(i>5000)//�ȴ�>5MS
			return 0;//��ʼ��ʧ��	
	}
	return 1;//��ʼ���ɹ�
}

void DS18B20_WriteByte(unsigned char dat) {
	unsigned char j;
	for(j=0;j<8;j++)
	{
		EA=0;
		_nop_();
		_nop_();
		DSPORT=0;			//ÿд��һλ����֮ǰ�Ȱ���������1us
		Delay1us();
		DSPORT=dat&0x01; //Ȼ��д��һ�����ݣ������λ��ʼ
		EA=1;
		Delay10us();
		Delay10us();
		Delay10us();
		DSPORT=1;	//Ȼ���ͷ����ߣ�����1us�����߻ָ�ʱ����ܽ���д��ڶ�����ֵ
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
		DSPORT=0;//�Ƚ���������1us
		Delay1us();
		DSPORT=1;//Ȼ���ͷ�����
		Delay6us();
		bi=DSPORT;	 //��ȡ���ݣ������λ��ʼ��ȡ
		/*��byte����һλ��Ȼ����������7λ���bi��ע���ƶ�֮���Ƶ���λ��0��*/
		byte=(byte>>1)|(bi<<7);
		EA=1;
		i=4;
		while(i--) Delay10us();		//��ȡ��֮��ȴ�48us�ٽ��Ŷ�ȡ��һ����
		DSPORT=1;
		Delay1us();
	}				
	return byte;
}

void DS18B20_ChangeTemperatureCOM() {
	DS18B20_Init();
	Delay1ms(1);
	DS18B20_WriteByte(0xcc); // ����ROM����
	DS18B20_WriteByte(0x44); // �����¶ȱ任
	Delay1ms(600);
}

void DS18B20_ReadTemperatureCOM() {
	DS18B20_Init();
	Delay1ms(1);
	DS18B20_WriteByte(0xcc); // ����ROM����
	DS18B20_WriteByte(0xbe); // ���¶ȼĴ���
}

int DS18B20_ReadTemperature() {
	int temp=0;
	unsigned char tmh,tml;
	DS18B20_ChangeTemperatureCOM();			 	//��д��ת������
	DS18B20_ReadTemperatureCOM();			//Ȼ��ȴ�ת������Ͷ�ȡ�¶�����
	tml=DS18B20_ReadByte();		//��ȡ�¶�ֵ��16λ���ȶ����ֽ�
	tmh=DS18B20_ReadByte();		//�ٶ����ֽ�
	temp=tmh;
	temp<<=8;
	temp|=tml;
	return temp;
}

