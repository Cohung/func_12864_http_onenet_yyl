/*������������������˶�������֤�Ķ����ı�׼�����壬��RC531оƬ*/
#ifndef __MAIN_H
#define __MAIN_H
#define uchar unsigned char
#define uint unsigned int

/////////////////////////////////////////////////////////////////////
//�˿ڶ���
/////////////////////////////////////////////////////////////////////

//����оƬ����
sbit 	 sc_data 		=	 P1^2;
sbit 	 sc_rst 		=	 P1^3;
//DS18B20����
sbit     buzz       =    P4^7;
//sbit     flesh      =    P1^1;
sbit 	 RS       =    P1^1;
sbit 	 RW       =    P5^4;                      
sbit 	 E        =    P1^7; 
sbit     lcdak      =    P1^6;

sbit     RST		=	 P0^2;
sbit     SCLK		=	 P0^4;
sbit 	 SDA		=	 P0^3;
//�û����ð���
sbit 	 SET		=	 P5^5;
sbit 	 TAB		=	 P2^0;
sbit 	 INC		=	 P4^4;
sbit 	 SUB		=	 P1^0;
//ͨ��ѡ�񰴼�
sbit     SCH1       =    P2^1;

//4·������(���ý�)
sbit     m1         =    P1^5;
sbit     m2         =    P0^5;
sbit     L298N_EA         =    P4^1;
sbit     L298N_EB         =    P1^4;
sbit     HC139_EN         =    P4^2;
//�г̿��ش��㵽���ź�yyl
sbit     pos2       =    P3^5;

//EEPROM����YYL
sbit     AT_SDA		=	 P2^3;
sbit 	 AT_SCL		=	 P2^2;


//ȱ��ͨ����ȱ���ź����yyl
sbit     out1       =    P3^4;
sbit  	 out2 		= 	 P3^3;
sbit     out3		=  	 P3^2;
sbit	 out4		=	 P4^0;

sbit     out5       =    P0^1;
sbit  	 out6 		= 	 P0^0;
sbit     out7		=  	 P2^6;
sbit	 out8		=	 P2^5;
//LED�ƹ���������ź�
sbit     mled       =    P4^6;
//�������ư�yyl
sbit     busy       =    P0^7;
sbit  	 io1 		= 	 P2^4;
sbit     io3		=  	 P2^7;
sbit     io4		=  	 P0^6;
sbit     io2		=  	 P4^3;
sbit	 io5		=	 P4^5;
/////////////////////////////////////////////////////////////////////
//��ʱ�����������ʶ���
/////////////////////////////////////////////////////////////////////


#define OSC_FREQ                22118400L	 
#define BOUND115200             256 - (OSC_FREQ/192L)/115200L   // 255
#define BOUND57600              256 - (OSC_FREQ/192L)/57600L    // 254
#define BOUND38400              256 - (OSC_FREQ/192L)/38400L    // 253
#define BOUND28800              256 - (OSC_FREQ/192L)/28800L    // 252
#define BOUND19200              256 - (OSC_FREQ/192L)/19200L    // 250
#define BOUND14400              256 - (OSC_FREQ/192L)/14400L    // 248
#define BOUND9600               256 - (OSC_FREQ/192L)/9600L     // 244
#define BOUND4800               256 - 24
#define RCAP2_50us              65536L - OSC_FREQ/240417L
#define RCAP2_1ms               65536L - OSC_FREQ/12021L
#define RCAP2_10ms              65536L - OSC_FREQ/1200L
#define RCAP2_1s                65536L - OSC_FREQ/12L

#define CH1   0X01
#define CH2   0X02
#define CH3   0X03
#define CH4   0X04

#define               USER_STATE               0X0A
#define               USER_STATE_ERR           0X0B
#define               POS_ON				   0x0c
#define               GET_INV_STATE            0X0D
#define 			  NO_INV_STATE	           0X0E
#define			      THANKS	               0x0f
#define				  TO_THANKS				   0X10


/////////////////////////////////////////////////////////////////////
//����ԭ��
/////////////////////////////////////////////////////////////////////
void InitializeSystem();                                   
void Rc632Ready();                                         
void AnswerCommandOk();
void AnswerCommandOk1();
void AnswerOk(unsigned char *answerdata,unsigned int answernum);          
void AnswerErr(char faultcode);                            
void DelayMs(unsigned int j);                                      
void Delay_50us(unsigned char j);                                  


//void BEEP(unsigned int i);
void Write_E(uchar n,uint x,uchar dat);
//uchar runnian(uchar y);
void Write_EE_rec(uchar y,uchar m,uchar d);
uchar panduan(uchar y,uchar m, uchar d);
void request();
void flesh_clr();
void flesh_set();
void select_ch(uchar channel);
void user_ch();
void deselect_ch();
void Ini_UART(void);
void IDR_Request();
void IDR_Select();
void IDR_Read();
void SendNet();

#endif


	


