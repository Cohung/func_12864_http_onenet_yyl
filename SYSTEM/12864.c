#include "STC89C5xRC_RDP.h"
#include "string.h"
#include "main.h"
#include "12864.h"

unsigned char code which_channel[]={"请选货道"};
unsigned char code error[]={"对不起，超出领用限制"};
unsigned char code no_inv[]={"对不起，此货道缺货请选择其他货道！"};
unsigned char code get_inv[]={"请取货！"};
	  
///////延时函数//////////////////////////////////
//void delay1(unsigned int z)
//{
//	unsigned int x,y;
//	for(x=z;x>0;x--)
//	for(y=110;y>0;y--);	
//}
/*-----------------------------------
 写入一字节数据/命令
 -----------------------------------*/
 void WriteByte_S(unsigned char byte)
 {
 	unsigned char i;
	 Delay_50us(5);
	for (i=0; i<8; i++)
	{
		E = 0;
		
		if(byte&0x80)	  //设定串行数据线上的数值
		RW = 1;
		else
		RW = 0;
	  
		byte <<= 1;
		
		
		E = 1;
	   
	} 
		Delay_50us(5);
 }
///////////////////////////////////////////////
//////写命令////////////////////////////////////
void WriteCommand(unsigned char Cmd)
{



unsigned char cmd;
cmd = 0xf8;
RS = 1;



WriteByte_S(cmd);
WriteByte_S(Cmd&0xf0);
WriteByte_S(Cmd<<4);  //Delay_50us(10);
}
//////////////////////////////////////////////////


//////写数据//////////////////////////////////////
void WriteData(unsigned char Data)  
{

//串行
     
unsigned char cmd;
cmd = 0xfa;
RS = 1;



WriteByte_S(cmd);
WriteByte_S(Data&0xf0);
WriteByte_S(Data<<4);
//Delay_50us(10);
}
//////////////////////////////////////////////////



///////初始化////////////////////////////////////
void initial_LCD()      
{   


	 WriteCommand(0x01);  //清屏
	 WriteCommand(0x30);  //功能设置。8字节界面，基本指令集。（0011 x0xx）
	 WriteCommand(0x0c);  //显示设置。整体显示，光标不显示，光标不反向。（0000 1110）
	 WriteCommand(0x06);   //输入方式设置。光标右移，地址位加1；整体右移。（0000 0110)
	 DelayMs(10);
	// WriteCommand(0x14);   //光标右移。
}
/////////////////////////////////////////////////
/*------------------------------------------------
                   显示字符串
x:横坐标值，范围0~7
y:纵坐标值，范围1~4
------------------------------------------------*/
void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s)
{ 
	WriteCommand(0x30);
 switch(y)
     {
	  case 1: WriteCommand(0x80+x);break;
	  case 2: WriteCommand(0x90+x);break;
	  case 3: WriteCommand(0x88+x);break;
	  case 4: WriteCommand(0x98+x);break;
      default:break;
	 }
 while(*s>0)
   { 
      WriteData(*s);
      s++;
     
   }
 //Delay_50us(20);
  
}
/*------------------------------------------------
                      清屏
------------------------------------------------*/
void ClrScreen()
{ 
   WriteCommand(0x01);
   DelayMs(20);
}
void Clean_12864_GDRAM(void)
{
    unsigned char x,y;

    WriteCommand(0x34);
    WriteCommand(0x30);     //基本指令操作(扩充指令操作为:0x34)
	WriteCommand(0x0C);     //整体显示ON，游标OFF，游标位置OFF
	WriteCommand(0x06);  	 //光标右移,DDRAM位址计数器(AC)加1,不整屏移动
	ClrScreen();           //清屏 (清DDRAM)

    WriteCommand(0x36);	//需要两次，本次设置扩展指令集

    for (y=0;y<32;y++)
    {
        WriteCommand(0x80+y);  //设置y=1000+00xx，y+1则往下一行
        WriteCommand(0x80);	//设置x=1000 0000
        for (x=0;x<16;x++)
        {
            WriteData(0x00);   //高字节数据
            WriteData(0x00);	//低字节数据
        }
    }
}

//void LCD_1616Graphic(unsigned char x,unsigned char y, unsigned char code *img)
//{ 
//
//
//    int i;
// 
//    WriteCommand(0x36);
////DelayMs(2); 
//	   for(i=0;i<16;i++)            
//	    { 	 
//		   switch(y)
//		   {
//	         case 1:
//			         WriteCommand(0x80 + i); //先设垂直地址
//	                 WriteCommand(0x80 + x);     //再设水平地址,显示在第1行
//					 break;
//			 case 2:
//			         WriteCommand(0x90 + i); //先设垂直地址
//	                 WriteCommand(0x80 + x);     //再设水平地址,显示在第2行
//					 break;
//		     case 3:
//			         WriteCommand(0x80 + i); //先设垂直地址
//	                 WriteCommand(0x88 + x);     //再设水平地址,显示在第3行
//					 break;
//		     case 4:
//			         WriteCommand(0x90 + i); //先设垂直地址
//	                 WriteCommand(0x88 + x);     //再设水平地址,显示在第4行
//					 break;	
//			 default:break;		 		 			 		 	
//		   }     	    
//	         WriteData(*img);
//			 img++;
//			 WriteData(*img);
//	         img++;   
//	    }
//
////	WriteCommand(0x36);
//	WriteCommand(0x30);
//
//	//DelayMs(5);
//           
//}

/*------------------------------------------------
                   显示自定义16*128图片
------------------------------------------------*/

void LCD_16128Graphic(unsigned char x,unsigned char y, unsigned char  *img)
{ 


 int i;

WriteCommand(0x36);

   for(i=0;i<16;i++)            
    { 
	 switch(y)
	 { case 1:
	          WriteCommand(0x80 + i); //先设垂直地址
              WriteCommand(0x80 + x);     //再设水平地址
			  break;
	   case 2:
	         WriteCommand(0x90 + i); //先设垂直地址
              WriteCommand(0x80 + x);     //再设水平地址
			  break;
	   case 3:
	         WriteCommand(0x80 + i); //先设垂直地址
              WriteCommand(0x88 + x);     //再设水平地址
			  break;
	   case 4:
              WriteCommand(0x90 + i); //先设垂直地址
              WriteCommand(0x98 + x);     //再设水平地址
			  break;
	   default:break;
     }

		  Delay_50us(50);
          WriteData(img[x*2+i*16]);
		  Delay_50us(50);
		  WriteData(img[x*2+1+i*16]);								
   Delay_50us(50);
    }
	//Delay_50us(1);
	WriteCommand(0x36);
	
   	WriteCommand(0x30);

	Delay_50us(60);
           
}



 void  Draw_Graphic(unsigned char hang_shu,unsigned char *img)
 {
   unsigned char i;
   for(i=0;i<8;i++)
     LCD_16128Graphic(i,hang_shu,img);
 }

void Draw_LOGO(unsigned char *img_1,unsigned char *img_2,unsigned char *img_3)
{
  Draw_Graphic(1,img_1);
	DelayMs(60);
  Draw_Graphic(2,img_2);
	DelayMs(60);
  Draw_Graphic(3,img_3);
  DelayMs(60);
}

/*------------------------------------------------
                  定义画点函数
--------------------------------------------------*/
 

 void set(unsigned char x,unsigned char y, unsigned char xdata *img)
{
  unsigned char byte_index, bit_index,temp,temp_bit;
  byte_index=x/8;
  bit_index=x%8;
  temp=*(img+y*16+byte_index);
  temp_bit=(temp>>(7-bit_index))&0x01;//测试要设置的点原来是否为1，是1就不更新
  if(temp_bit==0)//测试的点是0，则将其更新为1
    {
      temp=0x01;
      temp=temp<<(7-bit_index);
      *(img+y*16+byte_index)=(*(img+y*16+byte_index)|temp);
	}
 }

void clr(unsigned char x,unsigned char y,unsigned char *img)
{
  unsigned char byte_index, bit_index,temp,temp_bit;
  byte_index=x/8;
  bit_index=x%8;
  temp=*(img+y*16+byte_index);
  temp_bit=(temp>>(7-bit_index))&0x01;//测试要设置的点原来是否为1 
  if(temp_bit==1)//测试的点是1，则将其更新为0
    {
    temp=0x01;
    temp=temp<<(7-bit_index);
    *(img+y*16+byte_index)=(*(img+y*16+byte_index)-temp);
	}
//   else			  //测试的点是0，则将其更新为1
//	{
//    temp=0x01;
//    temp=temp<<(7-bit_index);
//    *(img+y*16+byte_index)=(*(img+y*16+byte_index)+temp);
//	}


 }


 /*------------------------------------------------
                 画数字
------------------------------------------------*/
 void Draw_Dig(unsigned char add,unsigned char num,unsigned char *dig,unsigned char *img)
 {
   unsigned char temp,rol;
   unsigned char i,j;
   for(i=0;i<16;i++)
     for(j=0;j<6;j++)
	    clr(add*6+j,i,img);
   for(i=0;i<10;i++)
     {
	  rol=0x80; 
	  for(j=0;j<8;j++)
	    {
		  temp=*(dig+i+num*10);
		  temp=temp & rol;
		  if(temp!=0)
		  {
		    set(j+add*6,i+5,img);
		  }
		  rol=rol>>1;
		}
	 }	 
 }
   
void DisplayLcd(unsigned char type)
{
	 unsigned char i;

	 if(type == SEL_CHAN){
	 WriteCommand(0x92);  		
	 for (i=1;i<=8;i++)		
	 WriteData(which_channel[i-1])  ;} 
	 else if (type == ERR_MES){
	 WriteCommand(0x92);  		
	 for (i=1;i<=8;i++)		
	 WriteData(error[i-1])  ;
	 WriteCommand(0x89);
	 for (i=9;i<=20;i++)		
	 WriteData(error[i-1])  ; }
	 else if (type == NO_INV){
	 WriteCommand(0x82);  		
	 for (i=1;i<=8;i++)		
	 WriteData(no_inv[i-1])  ;
	 WriteCommand(0x91);
	 for (i=9;i<=18;i++)		
	 WriteData(no_inv[i-1])  ;
	 WriteCommand(0x88);
	 for (i=19;i<=34;i++)		
	 WriteData(no_inv[i-1])  ;
	 }
	 else if (type == GET_INV){
	 WriteCommand(0x92);  		
	 for (i=1;i<=8;i++)		
	 WriteData(get_inv[i-1])  ;
	 }
	
}

//void DispalyLcd1()
//{
//	 unsigned char i;
//	 WriteCommand(0x01);  //清屏
//	 WriteCommand(0x80);  		
//	 for (i=1;i<=15;i++)		
//	 WriteData(table_3[i-1])  ; 
//
//
//}

///////要显示的内容//////////////////////////////
   /*
void DispalyLcd()
{
	 unsigned char i;
	 WriteCommand(0x81);  		//页地址,从80开始,详情参见资料//
	 for (i=1;i<=14;i++)		//第一行显示的是7个汉字,1个汉字为2个字节,所以这里要发送14个字节//
	 WriteData(table_1[i-1])  ;   
 
	 WriteCommand(0x90);  		//页地址,从92开始,详情参见资料//
	 for (i=1;i<=5;i++)		
	 WriteData(table_2[i-1])  ; 


	 WriteCommand(0x88);  		//页地址,从88开始,详情参见资料//
	 for (i=1;i<=5;i++)		    //第三行
	 WriteData(table_3[i-1])  ; 


	 WriteCommand(0x98);  	
	 for (i=1;i<=16;i++)		
	 WriteData(table_9[i-1])  ; 




	 WriteCommand(0x98);  		//页地址,从88开始,详情参见资料//
	 for (i=1;i<=5;i++)		
	 WriteData(table_4[i-1])  ; 

}		*/
/////////////////////////////////////////////////////

//////////数值的转换格式  \ 显示数值 ////////////////

//void LCD_Data(unsigned char LCDbyte )
//{
//	 unsigned char count;
//	 count=(LCDbyte&0xf0)>>4;
//	 if (count>0x9) WriteData(count%9+0x40);
//	 else WriteData(count+0x30);
//	 count=LCDbyte&0x0f;
//	 if (count>9) WriteData(count%9+0x40);
//	 else WriteData(count+0x30);
//}
/////////////////////////////////////////////////////
//void disCardId(unsigned char *gID)
//{
//	unsigned char i;
//
//	ClrScreen();
//	DelayMs(50);
//	//DispalyLcd1();
//	for(i=0;i<8;i++)
//	{
//
//	    WriteCommand(0x88+i);
//		LCD_Data(gID[0+i]); //4个字节的序列号
//		
//	}
//	
//}

