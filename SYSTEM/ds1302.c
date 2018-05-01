#include "STC89C5xRC_RDP.h"
#include "MAIN.H"
#include "12864.H"
#include "DS1302.H"
//驱动程序	 管脚SDA、SCLK、RST在MAIN.H中定义

//年 月 日 时 分 秒 星期
unsigned char Chinese_1[17]={0xc4,0xea,  0xd4,0xc2,  0xc8,0xd5,  0xca,0xb1,  0xb7,0xd6,  0xc3,0xeb,  0xd0,0xc7,  0xc6,0xda};
//星期一二三四五六日
//unsigned char Chinese_2[15]={0xd2,0xbb,  0xb6,0xfe,  0xc8,0xfd,  0xcb,0xc4,  0xce,0xe5,  0xc1,0xf9,  0xcc,0xec,' '};
//定义光标移动的位置
unsigned char Cursor[6]={0x81,0x83,0x85,0x90,0x92,0x94};

unsigned char DS1302_YEAR_READ=0X8d;
unsigned char DS1302_MONTH_READ=0X89;
unsigned char DS1302_DAY_READ=0X87;
unsigned char DS1302_HOUR_READ=0X85;
unsigned char DS1302_MINUTE_READ=0X83;
unsigned char DS1302_SECOND_READ=0X81;
unsigned char DS1302_WEEK_READ=0X8b;

unsigned char DS1302_YEAR_WRITE=0X8c;
unsigned char DS1302_MONTH_WRITE=0X88;
unsigned char DS1302_DAY_WRITE=0X86;
unsigned char DS1302_HOUR_WRITE=0X84;
unsigned char DS1302_MINUTE_WRITE=0X82;
unsigned char DS1302_SECOND_WRITE=0X80;
unsigned char DS1302_WEEK_WRITE=0X8a;
extern unsigned int led_start,led_end;
extern unsigned int power_start,power_end;	 //开关机时间
extern unsigned char set_num;
extern unsigned char TIME[6];
extern unsigned char code dig_9[12];
extern unsigned char xdata dd_1[256];
extern bit led_on;
//extern unsigned char set_led_time[4];
/*************************************************************/
/*************************************************************/
//按位写
void DS1302WriteByte(unsigned char dat)
{
  unsigned char i;
  SCLK=0;
  Delay_50us(1);
  for(i=0;i<8;i++)
  {
    SDA=dat&0x01; //先低位后高位
	Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
//	Delay_50us(1);
	SCLK=1;		  //一个上升沿写一位
	Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
	SCLK=0;
	Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
	dat>>=1;	 //右移，高位往低位移
  }
}


/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/


//按位读
unsigned char DS1302ReadByte()
{
  	unsigned char i,dat;
	//SCLK=1;
	//Delay_50us(1);
	for(i=0;i<8;i++)
	{
	   dat>>=1;		 //读也是先低位后高位
	   if(SDA==1)
	   dat|=0x80;	 //
	   Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
	   SCLK=1;
	   Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
	   SCLK=0;
	   Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
	   //Delay_50us(1);
	}
	return dat;
}


/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/

//读数据
unsigned char DS1302Read(unsigned char cmd)
{
  unsigned char dat;
  RST=0;
   //Delay_50us(1);	  
  SCLK=0;	  //关时钟总线
   	  
  RST=1;	 //开传输控制位，准备传送命令或数据	 
  //Delay_50us(1);
  DS1302WriteByte(cmd);	 //cmd有控制位有地址位
  Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
  dat=DS1302ReadByte();
  SCLK=1;
  Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
  RST=0;
  Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
  SDA = 0;
  return dat;
}
/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/

//写数据
void DS1302Write(unsigned char cmd, unsigned char dat)
{
  RST=0;
  SCLK=0; //DelayMs(10);
   Delay_50us(1);	  Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
  RST=1;	Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
  DS1302WriteByte(cmd);
  DS1302WriteByte(dat);
  SCLK=1;	 Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);Delay_50us(1);
  RST=0;
  
}
/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/
// void Initialize_device(unsigned char *t)
// {
//    Init_ST7920();
//	DelayMs(10);
//	Init_DS1302();
//	DelayMs(10);
//	Display_Menu(t);
// }
/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/
//初始化
void RE_DS1302()
{		
        TIME[0]=DS1302Read(DS1302_YEAR_READ);
		if(TIME[0]/16>3 || TIME[0]%16>9)
			TIME[0] = 0X11;
		TIME[1]=DS1302Read(DS1302_MONTH_READ);
		if(TIME[1]/16>1 || TIME[1]%16>9)
			TIME[1] = 0X11;
		
		TIME[2]=DS1302Read(DS1302_DAY_READ);
		if(TIME[2]/16>3 || TIME[2]%16>9)
			TIME[2] = 0X11;
		TIME[3]=DS1302Read(DS1302_HOUR_READ);
		if(TIME[3]/16>2 || TIME[3]%16>9)
			TIME[3] = 0X11;
	    TIME[4]=DS1302Read(DS1302_MINUTE_READ);
		if(TIME[4]/16>5 || TIME[4]%16>9)
			TIME[4] = 0X11;
 
}
void Init_DS1302()
{
   DS1302Write(0x8e,0x00);	//写保护，即写允许
   DS1302Write(DS1302_SECOND_WRITE,0x30);	//初始化秒，0x80是秒值在DS1302的地址
//   DS1302Write(DS1302_MINUTE_WRITE,0x20);	//分
//   DS1302Write(DS1302_HOUR_WRITE,0x05);	//时
//   DS1302Write(DS1302_DAY_WRITE,0x25);  //天
//   DS1302Write(DS1302_MONTH_WRITE,0x12);	//月
//   DS1302Write(DS1302_WEEK_WRITE,0x02);  //星期
//   DS1302Write(DS1302_YEAR_WRITE,0x13);	//年
   DS1302Write(0x90,0x01);	//关充电（只有高位是1010时才可使能VCC2到VCC1的充电功能）
   DS1302Write(0xc0,0xf0);	//初始化标示
   DS1302Write(0x8e,0x80);	//关写保护
}
/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/


//void Split_Display(unsigned char address)
//{
//	unsigned char i;
//	i=DS1302Read(address);
//	i=i/16+'0';   
//	WriteData(i);
//	DelayMs(10);
//	i=DS1302Read(address);
//	i=i%16+'0';
//	WriteData(i);
//	DelayMs(10);
//}


/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/
void Display_Menu(unsigned char *ti)
{
	WriteCommand(0x90);
	WriteData('2');
	WriteData('0');
	WriteData('0'+ti[0]/16);
	WriteData('0'+ti[0]%16);
	WriteData(Chinese_1[0]);
	WriteData(Chinese_1[1]);

    WriteData('0'+ti[1]/16);
	WriteData('0'+ti[1]%16);
	WriteData(Chinese_1[2]);
	WriteData(Chinese_1[3]);

    WriteData('0'+ti[2]/16);
	WriteData('0'+ti[2]%16);
	WriteData(Chinese_1[4]);
	WriteData(Chinese_1[5]);

	WriteCommand(0x88);
    WriteData('0'+ti[3]/16);
	WriteData('0'+ti[3]%16);
	WriteData(Chinese_1[6]);
	WriteData(Chinese_1[7]);

    WriteData('0'+ti[4]/16);
	WriteData('0'+ti[4]%16);
	WriteData(Chinese_1[8]);

	WriteData(Chinese_1[9]);
}
/*************************************************************/
/*************************************************************/
/*************************************************************/
/*************************************************************/
  void LCD_Display_Clock(unsigned char *dig,unsigned char *img)
  {
    unsigned char i;
	unsigned int led_now;
	
	i=DS1302Read(DS1302_SECOND_READ);
	
	if(i!=TIME[5])
	{ 
		  TIME[5]=i;
		  if(i%2==0){
		  Draw_Dig(17,11,dig_9,dd_1);
		  
		  LCD_16128Graphic(6,4,dd_1);
		 
		  }
		  else{
		  Draw_Dig(17,12,dig_9,dd_1);
		  
		  LCD_16128Graphic(6,4,dd_1);
		
		}
		led_now = (TIME[3]/16*10+TIME[3]%16)*60+TIME[4]/16*10+TIME[4]%16;
	   
	   
	   if(led_start>led_end){
	   		if(led_now>=led_start||led_now<=led_end)
				{led_on=1;mled=1;
				//lcdak = 0;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
			else
				{led_on=0;mled=0;
				//lcdak = 1;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
				}
		else if(led_start==led_end)
			{led_on=1;mled=1;
			//lcdak = 0;
			//cs_573 = 1;Delay_50us(1);cs_573=0;
			}
		else
		{
			  if(led_now>=led_start&&led_now<=led_end)
				{led_on=1;mled=1;
				//lcdak = 0;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
			else
				{led_on=0;mled=0;
				//lcdak = 1;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
				
		}
		/////////////////////以下为控制12864背光开闭设置
		if(power_start>power_end){
	   		if(led_now>=power_start||led_now<=power_end)
				{
				lcdak = 1;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
			else
				{
				lcdak = 0;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
				}
		else if(power_start==power_end)
			{
			lcdak = 0;
			//cs_573 = 1;Delay_50us(1);cs_573=0;
			}
		else
		{
			  if(led_now>=power_start&&led_now<=power_end)
				{
				lcdak = 1;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
			else
				{
				lcdak = 0;
				//cs_573 = 1;Delay_50us(1);cs_573=0;
				}
				
		}
		/////////////////////////
			i=DS1302Read(DS1302_MINUTE_READ);
	if(i!=TIME[4])
	{
	   TIME[4]=i;
	   Draw_Dig(18,TIME[4]/16,dig,img);
	   Draw_Dig(19,TIME[4]%16,dig,img);
	  
	   LCD_16128Graphic(6,4,img); 
	   LCD_16128Graphic(7,4,img);
	 

	   i=DS1302Read(DS1302_HOUR_READ);
	   if(i!=TIME[3])
		{
		   TIME[3]=i;
		   Draw_Dig(15,TIME[3]/16,dig,img);
		   Draw_Dig(16,TIME[3]%16,dig,img);
		   
		   LCD_16128Graphic(5,4,img); 
		   LCD_16128Graphic(6,4,img);
		   
		    
		   i=DS1302Read(DS1302_DAY_READ);
		   if(i!=TIME[2])
			{
			   TIME[2]=i;
			   Draw_Dig(9,TIME[2]/16,dig,img);
			   Draw_Dig(10,TIME[2]%16,dig,img);
			   
			   LCD_16128Graphic(3,4,img); 
			   LCD_16128Graphic(4,4,img); 
			    

			   i=DS1302Read(DS1302_MONTH_READ);
			   if(i!=TIME[1])
				{
				   TIME[1]=i;
				   Draw_Dig(6,TIME[1]/16,dig,img);
				   Draw_Dig(7,TIME[1]%16,dig,img);
				   LCD_16128Graphic(2,4,img); 

				   i=DS1302Read(DS1302_YEAR_READ);
				   if(i!=TIME[0])
					{
					   TIME[0]=i;
					   Draw_Dig(3,TIME[0]/16,dig,img);
					   Draw_Dig(4,TIME[0]%16,dig,img);
					   
					   LCD_16128Graphic(1,4,img);
					   LCD_16128Graphic(2,4,img); 
					   		   
				
					}
		    	}	
			}	
		}
	   		
	   	
				

	  }
	}

	
  }