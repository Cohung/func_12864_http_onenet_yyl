#include "STC89C5xRC_RDP.h"
#include "MAIN.H"
#include "sms.h"


uchar code ATE0[]="ATE0\r\n";	//ȡ��������� 

//extern unsigned char num[11];//����Ա�ֻ�����
extern unsigned char mach_num[9];
//uchar code CMGF[]="AT+CMGF=0\r\n";	//����PDU��ʽ
//uchar code rec_CMGF[]="AT+CMGF=1\r\n";	//�����ı���ʽ
extern unsigned char TIME[6];
extern unsigned char xdata com2_cmd_buf[100];
extern unsigned char com2_cmd_num;

uchar code SEND[]={0x1A,'\0'};		//���Ͷ���
 /*	flag:��7λ����1����ʾ���Է�������������
 	��6λ����ʾ�����������ѷ���
	��5λ����ʾ��ȡ��������ʱ���յ��ڶ����س����з�
	��4λ����ʾ����������������ʱ���յ�����
	��3��2λ������	
	��1λ����ʾ����������ʱ�յ���һ���س�����
	��0λ����ʾ��ȡ��ַʱ���յ���һ���ո�	*/
//uchar flag2=0;		//��־λ,һ���շ��������ʱΪF3
//extern uchar Position[5];	  //�յ��Ķ��Ŵ洢��λ�ã�Ĭ�϶�Ϣ��ַ�Ǹ�λ����ֻ��һλ��
//extern uchar YN;			  //�յ��Ķ������ݣ�y��n����Сд����
//extern uchar i;		   //�洢��ַʱ�����ڱ�ʾPosition�е�λ�á�
//extern uchar recv_temp;	 //�洢��һ���յ���ֵ
uchar code CCID[]="AT+CCID\r\n";	//��ѯsim���Ƿ����
uchar code CREG[]="AT+CREG?\r\n"; 	//��ѯ�ź�ǿ��
uchar code CGATT[]="AT+CGATT=1\r\n";	// 
uchar code CGACT[]="AT+CGACT=1,1\r\n"; //

uchar code CIPSTART[]="AT+CIPSTART=\"TCP\",\"api.heclouds.com\",80\r\n";
uchar code CIPSEND[]="AT+CIPSEND\r\n";
uchar code CIPCLOSE[]="AT+CIPCLOSE\r\n";
//�½�������
uchar code POST[]="POST /devices HTTP/1.1\r\nHost:api.heclouds.com\r\nAccept:*/*\r\napi-key:X6gNMI=4KPqDDLuHNLR40rS=9nY=\r\nContent-Type:application/json\r\ncharset=utf-8\r\nContent-Length:";
unsigned int POST_num = 30;
uchar code POST2[]="\r\n\r\n{\"title\":\"mydevice\",\"desc\":\"";
//string POST_name = conghun2;
uchar code POST3[]="\"}\r\n";
//��ȡԶ�̿��Ʋ���
uchar code Remote[]="GET /devices/23879956/datapoints HTTP/1.1\r\nHost:api.heclouds.com\r\nAccept:*/*\r\napi-key:vuYc9rA4K9cepRHWzy22FO7SncI=\r\nContent-Length:0\r\nConnection:close\r\n\r\n\r\n";

extern uchar wait_send;

void delay(uint ms)// ��ʱ�ӳ��� ,delay(1)��Լ0.09s
{
    uint i;
    while(ms--)
    {
        for(i=0;i<30000;i++);
    }
}


void Print_Char(uchar ch)//���͵����ַ�
{
    SBUF=ch; //���뻺����
    while(TI!=1); //�ȴ��������
    TI=0; //�������
}

void Print_Str(uchar *str)//��
{
    while(*str!='\0')
    {
        Print_Char(*str);
       //for(i=0;i<200;i++); //delayl(2);
        str++;
    }

}  
/*void get_sms(unsigned char the_term)
{
	uchar i;
	for(i=0;i<8;i++)
		Print_Char(CMGR[i]);
	Print_Char(the_term);
	Print_Char(13);
	Print_Char(10);
}*/
/*void dele_sms(unsigned char the_term)
{
	uchar i;
	for(i=0;i<8;i++)
		Print_Char(CMGD[i]);
	Print_Char(the_term);
	Print_Char(13);
	Print_Char(10);
}*/
void init_gprs()
{
	uchar i;
	//ȡ��������ԣ��෢����
	

	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	for(i=0;i<6;i++)
		Print_Char(ATE0[i]);	
	delay(10);
	while(com2_cmd_buf[0]!='O');

	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	for(i=0;i<6;i++)
		Print_Char(ATE0[i]);	
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	
	for(i=0;i<6;i++)
		Print_Char(ATE0[i]);	
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	//�鿴�ֻ���
	
	for(i=0;i<9;i++)
		Print_Char(CCID[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	//�鿴�ź�ǿ��
	
	for(i=0;i<10;i++)
		Print_Char(CREG[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	//
	
	for(i=0;i<12;i++)
		Print_Char(CGATT[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	//
	
	for(i=0;i<14;i++)
		Print_Char(CGACT[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	
}
uchar ChangeChar(uchar in)
{
	uchar out;
	if((0<=in) && (in<=9))
		out = in + 0x30;
	else
		out = in + 0x37;
	
	return out;
}

void send_net(uchar *a,uchar age,uchar chan)
{	
	
	uchar i;
	uchar name_num=0;
	uchar tmp1,tmp2;

		//TR1 = 0;
	AUXR1 = 0X40;//������3.6,3.7
	//TR1 = 1;
   com2_cmd_num = 0;
	
	//��㷢��������ѳ�˯��A6
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	for(i=0;i<6;i++)
		Print_Char(ATE0[i]);	
	delay(10);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

		//
	
	for(i=0;i<12;i++)
		Print_Char(CGATT[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	//
	
	for(i=0;i<14;i++)
		Print_Char(CGACT[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';


	
	//�ȼ��������ĳ���ȷ��ռ�����ֽ�
	for(i=14;;i++)
	{
		if((a[i]==0x20) && (a[i+1]==0x00))
		{
		//Ҫע��ÿ��ѭ��ʱPOST_num��ֵ�������и�bug����ÿ�ο���ֻ���ϴ�һ�����ݣ���Ϊд����POST_num=POST_num+...
			POST_num = 30 + (i-14)*3 + 108 + 2*6 + 6 + 89 + 9*6 + 6 + 6;	//��һ�Ӽӵ����������ڶ��Ӽӵ��ǹ̶����ȵ����֤����18*6�������Ӽӵ���2λ��������,���ļӼӵ����Ա𣬵���Ӽӵ���19λ��ʱ���,�����Ӽӵ���9λ�Ļ������9*6�����߼Ӽӵ��ǻ������1*6�����һ�Ӽ���6������
			name_num=(i-14)/2;
			break;
		}
	}
	//����OneNet
	
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	for(i=0;i<40;i++)
		Print_Char(CIPSTART[i]);
	while(com2_cmd_buf[0]!='O');
//	com2_cmd_buf[0]=' ';
//	com2_cmd_buf[1]=' ';
	while(com2_cmd_buf[0]!='C');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	//��������ָ��
	//wait_send = 0x01;
	for(i=0;i<12;i++)
		Print_Char(CIPSEND[i]);
	//while(wait_send);
	//delay(60);//�ȴ�����������>
	while(com2_cmd_buf[0]!='>');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	Print_Str(POST);//��һ���ַ�����
	Print_Char(POST_num/100+0x30);//Content-length
	Print_Char(POST_num/10%10+0x30);
	Print_Char(POST_num%10+0x30);
	Print_Str(POST2);  //�ڶ����ַ�����
	for(i=0;i<18;i++)	//�����18λ���֤��
	//33 00 36 00 32 00 32 00 30 00 33 00 31 00 39 00 39 00 30 00 31 00 31 00 32 00 32 00 30 00 34 00 31 00 38 00//���֤����36�ֽ�
	{
		Print_Char('\\');
		Print_Char('u');
		tmp1 = ChangeChar(a[136+i*2+1]/16);
		tmp2 = ChangeChar(a[136+i*2+1]%16);
		Print_Char(tmp1);
		Print_Char(tmp2);
		tmp1 = ChangeChar(a[136+i*2]/16);
		tmp2 = ChangeChar(a[136+i*2]%16);
		Print_Char(tmp1);
		Print_Char(tmp2);
	}
	Print_Char(',');
	for(i=0;i<name_num;i++)	//�����������������Ӧunicode���ַ�����ʽ
	{
		Print_Char('\\');
		Print_Char('u');
		tmp1 = ChangeChar(a[14+i*2+1]/16);
		tmp2 = ChangeChar(a[14+i*2+1]%16);
		Print_Char(tmp1);
		Print_Char(tmp2);
		tmp1 = ChangeChar(a[14+i*2]/16);
		tmp2 = ChangeChar(a[14+i*2]%16);
		Print_Char(tmp1);
		Print_Char(tmp2);
	}
	Print_Char(',');
	
	//���2λ����������Ӧunicode���ַ�����ʽ
	
		Print_Char('\\');
		Print_Char('u');
		Print_Char(0x30);
		Print_Char(0x30);
		Print_Char(0x33);
		Print_Char(0x30+age/10);
		Print_Char('\\');
		Print_Char('u');
		Print_Char(0x30);
		Print_Char(0x30);
		Print_Char(0x33);
		Print_Char(0x30+age%10);

		Print_Char(',');
	//����Ա�
		Print_Char('\\');
		Print_Char('u');
		if(a[44]==0x31)
			//�Ա���	
			{		
				Print_Char(0x37);
				Print_Char(0x35);
				Print_Char(0x33);
				Print_Char(0x37);			
			}
		else
			//�Ա�Ů
			{
				
				Print_Char(0x35);
				Print_Char(0x39);
				Print_Char(0x37);
				Print_Char(0x33);
				
			}
		Print_Char(',');
	//���19λ��ʽ��ʱ�����2017/09/01 17:40:43
	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x32);  //2017֮2

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30);  //2017֮0

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[0]/16);  //2017֮1

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[0]%16);  //2017֮7
	Print_Char('-');

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[1]/16);  //09֮0

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[1]%16);  //09֮9
	Print_Char('-');

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[2]/16);  //01֮0

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[2]%16);  //01֮1

	Print_Char(' ');

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[3]/16);

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[3]%16);  //17֮7
	Print_Char(':');

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[4]/16);

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[4]%16);  //40֮0
	Print_Char(':');

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[5]/16);

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[5]%16);  //43֮3

	Print_Char(',');
	//����9λ�������
	for(i=0;i<9;i++)
	{
	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+mach_num[i]);
	}

	Print_Char(',');
	//����1λ�������
	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+chan); 

	Print_Str(POST3);	//ĩβ�ַ�����
	Print_Str(SEND);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	delay(10);
	for(i=0;i<13;i++)
		Print_Char(CIPCLOSE[i]);	
	

	AUXR1 = 0X00;//������3.0,3.1 ��ע���ʱ�лض��������ڣ����ܵ��¶��������ڻ������ݡ����ڶ����������к����ڷ���������󶼻�������recv_num��0����˲���Ӱ�졣

}
void message(uchar *tele,uchar chan)
{	
	
	
	uchar i;

	AUXR1 = 0X40;//������3.6,3.7
	com2_cmd_num = 0;



	//��㷢��������ѳ�˯��A6
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	for(i=0;i<6;i++)
		Print_Char(ATE0[i]);	
	delay(10);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

		//
	
	for(i=0;i<12;i++)
		Print_Char(CGATT[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	//
	
	for(i=0;i<14;i++)
		Print_Char(CGACT[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	
	//Ҫע��POST_num��ֵ�������и�bug����ÿ�ο���ֻ���ϴ�һ�����ݣ���Ϊд����POST_num=POST_num+...
	POST_num = 30 + 11 + 9 + 1 + 2;	//��һ�Ӽӵ����ֻ��ţ��ڶ��Ӽӵ��ǻ�����ţ������Ӽӵ��ǻ������,���ӵ����������Ű�	

	//����Yeelink
	
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	for(i=0;i<40;i++)
		Print_Char(CIPSTART[i]);
	while(com2_cmd_buf[0]!='O');

	while(com2_cmd_buf[0]!='C');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	//��������ָ��
	//wait_send = 0x01;
	for(i=0;i<12;i++)
		Print_Char(CIPSEND[i]);
	//while(wait_send);
	//delay(50);//�ȴ�����������>
	while(com2_cmd_buf[0]!='>');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	Print_Str(POST);//��һ���ַ�����
	Print_Char(0x35);//Content-length
	Print_Char(0x33);
	Print_Str(POST2);  //�ڶ����ַ�����
	for(i=0;i<11;i++)	//����11λ�ֻ���
	{
	Print_Char(0x30+tele[i]);
	}
	
	Print_Char(',');
	//����9λ�������
	for(i=0;i<9;i++)
	{
	Print_Char(0x30+mach_num[i]);
	}

	Print_Char(',');
	//����1λ�������
	Print_Char(0x30+chan); 

	Print_Str(POST3);	//ĩβ�ַ�����
	Print_Str(SEND);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	delay(10);
	for(i=0;i<13;i++)
		Print_Char(CIPCLOSE[i]);	
	
	AUXR1 = 0X00;//������3.0,3.1
}
void Remote_Set()
{
	uchar i;

	AUXR1 = 0X40;//������3.6,3.7
	com2_cmd_num = 0;



	//��㷢��������ѳ�˯��A6
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	for(i=0;i<6;i++)
		Print_Char(ATE0[i]);	
	delay(10);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

		//
	
//	for(i=0;i<12;i++)
//		Print_Char(CGATT[i]);
//	while(com2_cmd_buf[0]!='O');
//	com2_cmd_buf[0]=' ';
//	com2_cmd_buf[1]=' ';
//
//	//
//	
//	for(i=0;i<14;i++)
//		Print_Char(CGACT[i]);
//	while(com2_cmd_buf[0]!='O');
//	com2_cmd_buf[0]=' ';
//	com2_cmd_buf[1]=' ';
	
	for(i=0;i<40;i++)
		Print_Char(CIPSTART[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	while(com2_cmd_buf[0]!='C');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	//��������ָ��
	//wait_send = 0x01;
	for(i=0;i<12;i++)
		Print_Char(CIPSEND[i]);
	//while(wait_send);
	//delay(100);//�ȴ�����������>
	while(com2_cmd_buf[0]!='>');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	Print_Str(Remote);//��һ���ַ�����
	Print_Str(SEND);
//	while(com2_cmd_buf[0]!='O');
//	com2_cmd_buf[0]=' ';
//	com2_cmd_buf[1]=' ';

	while(com2_cmd_buf[0]!='{'); //�ȴ���{}��Ϣ��datapoints���ݻش�
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	delay(10);
//	for(i=0;i<13;i++)	 //ǰ���������connect:close���Ͳ�����close��
//		Print_Char(CIPCLOSE[i]);	
	
	AUXR1 = 0X00;//������3.0,3.1
}
void Init_UART_SMS(void)//���ڳ�ʼ������ʱ����ʼ��
{
	TR1 = 0;
	AUXR1 = 0X40;//������3.6,3.7
	SCON = 0x50 ;  //SCON: serail mode 1, 8-bit UART, enable ucvr
    //UARTΪģʽ1��8λ���ݣ��������
    TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
    //��ʱ��1Ϊģʽ2,8λ�Զ���װ
    PCON |= 0x80 ; //SMOD=1;
    TH1 = 0xFF ;   //Baud:115200 fosc=22.1184MHz
    TL1=0xFF;
    //IE |= 0x90 ;     //Enable Serial Interrupt
	ES=1;
    TR1 = 1 ;       // timer 1 run
 
}




