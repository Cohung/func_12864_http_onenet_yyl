#include "STC89C5xRC_RDP.h"
#include "STC15EROM.h"
/*������������������������������������������������������������
��������ֹIAP��������IAP��ؼĴ���
������������������������������������������������������������*/
 void Q0()
{
      IAP_CONTR = 0;    
      IAP_CMD   = 0;          
      IAP_TRIG  = 0;                   
      IAP_ADDRH = 0x80;        
      IAP_ADDRL = 0;              
}

/*������������������������������������������������������������
��������һ�ֽ�		   
��ڣ���n��������x�ֽ� 
���ڣ�dat
������������������������������������������������������������*/
uchar R_EROM(uchar n,uint x)
{
      uchar dat; 
	  uint  addr;
	  addr = (n)*0x0200 + x;     //��оƬ�ͺ�ʱ��Ҫ�Ķ�������                   
      IAP_CONTR = EN_IAP;  
      IAP_CMD   = 1;       	      //������ȡEEPROM
      IAP_ADDRL = addr;                
      IAP_ADDRH = addr >> 8;     
      IAP_TRIG  = 0x5a;                
      IAP_TRIG  = 0xa5;           //��оƬ�ͺ�ʱ��Ҫ�Ķ�������       
      _nop_();                                   
      dat = IAP_DATA;            
      Q0();	                          
      return dat;                        
}
/*������������������������������������������������������������
������дһ�ֽ�
��ڣ���n��������x�ֽڣ���д������dat
������������������������������������������������������������*/
void W_EROM(uchar n,uint x,uchar dat)
{
	  uint  addr;
	  addr = (n)*0x0200 + x;  //��оƬ�ͺ�ʱ��Ҫ�Ķ�������
      IAP_CONTR = EN_IAP;        
      IAP_CMD   = 2;    	      //������дEEPROM
      IAP_ADDRL = addr;                
      IAP_ADDRH = addr >> 8;           
      IAP_DATA  = dat;                  
      IAP_TRIG  = 0x5a;                
      IAP_TRIG  = 0xa5;              //��оƬ�ͺ�ʱ��Ҫ�Ķ�������    
      _nop_();                                    
      Q0();    
}
/*������������������������������������������������������������
����������ĳһ����	
��ڣ���n����		
������������������������������������������������������������*/
void C_EROM(uchar n)
{
      uint  addr;
	  addr = (n)*0x0200; 	  //��оƬ�ͺ�ʱ��Ҫ�Ķ�������
	  IAP_CONTR = EN_IAP; 
      IAP_CMD   = 3;    	      //��������EEPROM     
      IAP_ADDRL = addr;                
      IAP_ADDRH = addr >> 8;        
      IAP_TRIG  = 0x5a;                
      IAP_TRIG  = 0xa5;             //��оƬ�ͺ�ʱ��Ҫ�Ķ�������     
      _nop_();                                   
      Q0();
}