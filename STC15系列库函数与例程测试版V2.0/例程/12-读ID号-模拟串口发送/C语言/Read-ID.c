
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/



#include	"config.h"
#include	"soft_uart.h"


/*************	����˵��	**************

�ϵ��, ����ID�ţ�ͨ��ģ�⴮�ڷ��͸���λ����������9600,8,n,1��

������Ϊ����256�ֽ�RAM, 8K ROM��MCU, ����STC15W408AS.
�������͵�MCU,����ʵ������޸�"���س�������"�еĵ�ַ.

******************************************/

/*************	���س�������	**************/
#define		IDATA_ID_START_ADDR	0xf1		//ָ��idata��ID�ŵĿ�ʼ��ַ
#define		CODE_ID_START_ADDR	(8192-7)	//ָ��code ��ID�ŵĿ�ʼ��ַ


/*************	���ر�������	**************/
u8	idata ID_idata[7];	//idata ����ID��
u8	idata ID_code[7];	//code  ����ID��


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



/**********************************************/
u8	Hex2Ascii(u8 dat)
{
	dat &= 0x0f;
	if(dat <= 9)	return (dat+'0');
	return (dat-10+'A');
}

/**********************************************/
void main(void)
{
	u8	idata *ip;
	u8	code  *cp;
	u8	i;
	
	for(i=0, ip=IDATA_ID_START_ADDR; ip<(IDATA_ID_START_ADDR+7); ip++)	ID_idata[i++] = *ip;
	for(i=0, cp=CODE_ID_START_ADDR;  cp<(CODE_ID_START_ADDR+7);  cp++)	ID_code[i++]  = *cp;
	
	PrintString("\r\n Read ID from idata = ");
	for(i=0; i<7; i++)
	{
		TxSend(Hex2Ascii(ID_idata[i]>>4));
		TxSend(Hex2Ascii(ID_idata[i]));
	}
	PrintString("\r\n Read ID from CODE  = ");
	for(i=0; i<7; i++)
	{
		TxSend(Hex2Ascii(ID_code[i]>>4));
		TxSend(Hex2Ascii(ID_code[i]));
	}
	PrintString("\r\n");


	while (1)
	{
	
	}

}




