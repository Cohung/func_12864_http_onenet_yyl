#ifndef __SMS_H
#define __SMS_H
extern uchar code rec_CMGF[];	//�����ı���ʽ
extern uchar code rec_CMGS[];
//extern uchar code CMGR[];	// ������8
//extern uchar code CMGD[]; //ɾ������8
extern uchar code SEND[];
void delay(unsigned int ms);
void Print_Char(unsigned char ch);
void Print_Str(unsigned char *str);
void Init_UART_SMS(void);
uchar ChangeChar(uchar in);
void send_net(uchar *a,uchar age, uchar chan);
//void dele_sms(unsigned char the_term);
void init_gprs();
//void SMS_Del_All();
void message(uchar *tele,uchar channel);
void Remote_Set();
#endif
