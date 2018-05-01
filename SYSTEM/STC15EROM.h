/****************************************************************/ 
#ifndef __STC15EROM_H__
#define __STC15EROM_H__ 

#include <intrins.h>
#define  uchar unsigned char    
#define  uint  unsigned int  
										  //以下是ISP/IAP操作时间宏定义选项
//#define  EN_IAP        0x80             //系统时钟 ≤ 30MHz
#define  EN_IAP        0x81             //系统时钟 ≤ 24MHz
//#define  EN_IAP        0x82             //系统时钟 ≤ 20MHz
//  #define  EN_IAP        0x83             //系统时钟 ≤ 12MHz
//#define  EN_IAP        0x84             //系统时钟 ≤ 6MHz
//#define  EN_IAP        0x85             //系统时钟 ≤ 3MHz
//#define  EN_IAP        0x86             //系统时钟 ≤ 2MHz
//#define  EN_IAP        0x87             //系统时钟 ≤ 1MHz



//使用说明：
//每次写入之前都要擦除，否则数据不对。




/****************【供用户调用的3个函数】****************/
 void Q0();
 uchar R_EROM(uchar n,uint x);			  //读第n扇区，第x字节的数据
 void  W_EROM(uchar n,uint x,uchar dat);  //向第n扇区，第x字节写入数据dat
 void  C_EROM(uchar n);					  //擦除第n扇区



#endif
