#include "STC89C5xRC_RDP.h"
#include "MAIN.H"
#include "SC8020B.H"

void delay_us(uint n)
{
	uint i;
	for(i=0;i<n;i++);
}
//50us<脉冲持续时间<200us
void sc_speech(uchar n)
{
	uchar i;
	sc_data=0;
	sc_rst=1;
	delay_us(100); //
	sc_rst=0;
	delay_us(100);
	for(i=0;i < n;i++)
	{
	sc_data=1; //数据脉冲高
	delay_us(100); //持续100US
	sc_data=0; //数据脉冲低
	delay_us(100); //持续100US
	}
}