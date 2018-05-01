#include "STC89C5xRC_RDP.h"
#include "STC15EROM.h"
/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：禁止IAP操作，清IAP相关寄存器
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
 void Q0()
{
      IAP_CONTR = 0;    
      IAP_CMD   = 0;          
      IAP_TRIG  = 0;                   
      IAP_ADDRH = 0x80;        
      IAP_ADDRL = 0;              
}

/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：读一字节		   
入口：第n扇区，第x字节 
出口：dat
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
uchar R_EROM(uchar n,uint x)
{
      uchar dat; 
	  uint  addr;
	  addr = (n)*0x0200 + x;     //换芯片型号时需要改动！！！                   
      IAP_CONTR = EN_IAP;  
      IAP_CMD   = 1;       	      //允许读取EEPROM
      IAP_ADDRL = addr;                
      IAP_ADDRH = addr >> 8;     
      IAP_TRIG  = 0x5a;                
      IAP_TRIG  = 0xa5;           //换芯片型号时需要改动！！！       
      _nop_();                                   
      dat = IAP_DATA;            
      Q0();	                          
      return dat;                        
}
/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：写一字节
入口：第n扇区，第x字节，待写入数据dat
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
void W_EROM(uchar n,uint x,uchar dat)
{
	  uint  addr;
	  addr = (n)*0x0200 + x;  //换芯片型号时需要改动！！！
      IAP_CONTR = EN_IAP;        
      IAP_CMD   = 2;    	      //允许编写EEPROM
      IAP_ADDRL = addr;                
      IAP_ADDRH = addr >> 8;           
      IAP_DATA  = dat;                  
      IAP_TRIG  = 0x5a;                
      IAP_TRIG  = 0xa5;              //换芯片型号时需要改动！！！    
      _nop_();                                    
      Q0();    
}
/*┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
函数：擦除某一扇区	
入口：第n扇区		
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈*/
void C_EROM(uchar n)
{
      uint  addr;
	  addr = (n)*0x0200; 	  //换芯片型号时需要改动！！！
	  IAP_CONTR = EN_IAP; 
      IAP_CMD   = 3;    	      //允许擦除EEPROM     
      IAP_ADDRL = addr;                
      IAP_ADDRH = addr >> 8;        
      IAP_TRIG  = 0x5a;                
      IAP_TRIG  = 0xa5;             //换芯片型号时需要改动！！！     
      _nop_();                                   
      Q0();
}