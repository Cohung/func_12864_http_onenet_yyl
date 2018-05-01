#include "STC89C5xRC_RDP.h"
#include "MAIN.H"
#include "ds18b20.h"
void ds_delay(uint ms)// Âª∂Êó∂Â≠êÁ®ãÂ∫è ,ds_delay(1)Â§ßÁ∫¶0.09s
{
    uint i;
    while(ms--)
    {
        for(i=0;i<12;i++);
    }
}
void init_tempr()
{
uchar n;
buzz=1;
ds_delay(16);
buzz=0;
ds_delay(160);   //500us
buzz=1;
ds_delay(28);
n=buzz;
ds_delay(40);
}
void ds_write_byte(uchar dat)
{
uchar i;
for(i=0;i<8;i++)
{
  buzz=0;
  buzz=dat&0x01;
  ds_delay(10);
  buzz=1;
  dat>>=1;
}
ds_delay(4);
}
uchar ds_read_byte(void)
{
uchar i,value;
for(i=0;i<8;i++)
{
  buzz=0;
  value>>=1;
  buzz=1;
  if(buzz)
  value|=0x80;
  ds_delay(8);
}
return value;
}
uchar readtempr(void)
{
  uint temp, a,b;
  float tt=0;

  init_tempr();
  ds_write_byte(0xcc);
  ds_write_byte(0x44);
  ds_delay(300);
  init_tempr();
  ds_write_byte(0xcc);
  ds_write_byte(0xbe);
  a=ds_read_byte();
  b=ds_read_byte();
  temp=b;  
    temp<<=8;  
    temp=temp|a;  
    tt=temp*0.0625;  
    temp= tt*10+0.5; //∑≈¥Û10±∂ ‰≥ˆ≤¢Àƒ…·ŒÂ»Î  
//  dian=(a&0x0f)*10*6/10; 

  return temp;
}