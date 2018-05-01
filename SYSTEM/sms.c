#include "STC89C5xRC_RDP.h"
#include "MAIN.H"
#include "sms.h"


uchar code ATE0[]="ATE0\r\n";	//取消命令回显 

//extern unsigned char num[11];//管理员手机号码
extern unsigned char mach_num[9];
//uchar code CMGF[]="AT+CMGF=0\r\n";	//设置PDU格式
//uchar code rec_CMGF[]="AT+CMGF=1\r\n";	//设置文本格式
extern unsigned char TIME[6];
extern unsigned char xdata com2_cmd_buf[100];
extern unsigned char com2_cmd_num;

uchar code SEND[]={0x1A,'\0'};		//发送短信
 /*	flag:第7位：置1，表示可以发读短信命令了
 	第6位：表示读短信命令已发送
	第5位：表示读取短信内容时，收到第二个回车换行符
	第4位：表示解析短信提醒命令时，收到逗号
	第3、2位：保留	
	第1位：表示读短信内容时收到第一个回车换行
	第0位：表示获取地址时，收到第一个空格	*/
//uchar flag2=0;		//标志位,一轮收发短信完毕时为F3
//extern uchar Position[5];	  //收到的短信存储的位置，默认短息地址是个位数，只有一位。
//extern uchar YN;			  //收到的短信内容，y或n，大小写均可
//extern uchar i;		   //存储地址时，用于表示Position中的位置。
//extern uchar recv_temp;	 //存储上一个收到的值
uchar code CCID[]="AT+CCID\r\n";	//查询sim卡是否插入
uchar code CREG[]="AT+CREG?\r\n"; 	//查询信号强度
uchar code CGATT[]="AT+CGATT=1\r\n";	// 
uchar code CGACT[]="AT+CGACT=1,1\r\n"; //

uchar code CIPSTART[]="AT+CIPSTART=\"TCP\",\"api.heclouds.com\",80\r\n";
uchar code CIPSEND[]="AT+CIPSEND\r\n";
uchar code CIPCLOSE[]="AT+CIPCLOSE\r\n";
//新建传感器
uchar code POST[]="POST /devices HTTP/1.1\r\nHost:api.heclouds.com\r\nAccept:*/*\r\napi-key:X6gNMI=4KPqDDLuHNLR40rS=9nY=\r\nContent-Type:application/json\r\ncharset=utf-8\r\nContent-Length:";
unsigned int POST_num = 30;
uchar code POST2[]="\r\n\r\n{\"title\":\"mydevice\",\"desc\":\"";
//string POST_name = conghun2;
uchar code POST3[]="\"}\r\n";
//获取远程控制参数
uchar code Remote[]="GET /devices/23879956/datapoints HTTP/1.1\r\nHost:api.heclouds.com\r\nAccept:*/*\r\napi-key:vuYc9rA4K9cepRHWzy22FO7SncI=\r\nContent-Length:0\r\nConnection:close\r\n\r\n\r\n";

extern uchar wait_send;

void delay(uint ms)// 延时子程序 ,delay(1)大约0.09s
{
    uint i;
    while(ms--)
    {
        for(i=0;i<30000;i++);
    }
}


void Print_Char(uchar ch)//发送单个字符
{
    SBUF=ch; //送入缓冲区
    while(TI!=1); //等待发送完毕
    TI=0; //软件清零
}

void Print_Str(uchar *str)//串
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
	//取消命令回显，多发几次
	

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

	//查看手机号
	
	for(i=0;i<9;i++)
		Print_Char(CCID[i]);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	//查看信号强度
	
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
	AUXR1 = 0X40;//串口在3.6,3.7
	//TR1 = 1;
   com2_cmd_num = 0;
	
	//随便发几个命令唤醒沉睡的A6
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


	
	//先计算姓名的长度确定占多少字节
	for(i=14;;i++)
	{
		if((a[i]==0x20) && (a[i+1]==0x00))
		{
		//要注意每次循环时POST_num的值，曾经有个bug就是每次开机只能上传一次数据，因为写成了POST_num=POST_num+...
			POST_num = 30 + (i-14)*3 + 108 + 2*6 + 6 + 89 + 9*6 + 6 + 6;	//第一加加的是姓名，第二加加的是固定长度的身份证编码18*6，第三加加的是2位数的年龄,第四加加的是性别，第五加加的是19位的时间戳,第六加加的是9位的机器编号9*6，第七加加的是货道编号1*6，最后一加加是6个逗号
			name_num=(i-14)/2;
			break;
		}
	}
	//连接OneNet
	
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
	//发送数据指令
	//wait_send = 0x01;
	for(i=0;i<12;i++)
		Print_Char(CIPSEND[i]);
	//while(wait_send);
	//delay(60);//等待服务器返回>
	while(com2_cmd_buf[0]!='>');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	Print_Str(POST);//第一串字符常量
	Print_Char(POST_num/100+0x30);//Content-length
	Print_Char(POST_num/10%10+0x30);
	Print_Char(POST_num%10+0x30);
	Print_Str(POST2);  //第二串字符常量
	for(i=0;i<18;i++)	//先输出18位身份证号
	//33 00 36 00 32 00 32 00 30 00 33 00 31 00 39 00 39 00 30 00 31 00 31 00 32 00 32 00 30 00 34 00 31 00 38 00//身份证号码36字节
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
	for(i=0;i<name_num;i++)	//根据姓名长度输出相应unicode的字符串形式
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
	
	//输出2位数的年龄相应unicode的字符串形式
	
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
	//输出性别
		Print_Char('\\');
		Print_Char('u');
		if(a[44]==0x31)
			//性别男	
			{		
				Print_Char(0x37);
				Print_Char(0x35);
				Print_Char(0x33);
				Print_Char(0x37);			
			}
		else
			//性别女
			{
				
				Print_Char(0x35);
				Print_Char(0x39);
				Print_Char(0x37);
				Print_Char(0x33);
				
			}
		Print_Char(',');
	//输出19位格式的时间戳：2017/09/01 17:40:43
	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x32);  //2017之2

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30);  //2017之0

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[0]/16);  //2017之1

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[0]%16);  //2017之7
	Print_Char('-');

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[1]/16);  //09之0

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[1]%16);  //09之9
	Print_Char('-');

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[2]/16);  //01之0

	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+TIME[2]%16);  //01之1

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
	Print_Char(0x30+TIME[3]%16);  //17之7
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
	Print_Char(0x30+TIME[4]%16);  //40之0
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
	Print_Char(0x30+TIME[5]%16);  //43之3

	Print_Char(',');
	//发送9位机器编号
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
	//发送1位货道编号
	Print_Char('\\');
	Print_Char('u');
	Print_Char(0x30);
	Print_Char(0x30);
	Print_Char(0x33);
	Print_Char(0x30+chan); 

	Print_Str(POST3);	//末尾字符常量
	Print_Str(SEND);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	delay(10);
	for(i=0;i<13;i++)
		Print_Char(CIPCLOSE[i]);	
	

	AUXR1 = 0X00;//串口在3.0,3.1 。注意此时切回读卡器串口，可能导致读卡器串口缓存数据。好在读卡器的所有函数在发送完命令后都会主动将recv_num置0，因此不受影响。

}
void message(uchar *tele,uchar chan)
{	
	
	
	uchar i;

	AUXR1 = 0X40;//串口在3.6,3.7
	com2_cmd_num = 0;



	//随便发几个命令唤醒沉睡的A6
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
	
	//要注意POST_num的值，曾经有个bug就是每次开机只能上传一次数据，因为写成了POST_num=POST_num+...
	POST_num = 30 + 11 + 9 + 1 + 2;	//第一加加的是手机号，第二加加的是机器编号，第三加加的是货道编号,最后加的是两个逗号吧	

	//连接Yeelink
	
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	for(i=0;i<40;i++)
		Print_Char(CIPSTART[i]);
	while(com2_cmd_buf[0]!='O');

	while(com2_cmd_buf[0]!='C');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	//发送数据指令
	//wait_send = 0x01;
	for(i=0;i<12;i++)
		Print_Char(CIPSEND[i]);
	//while(wait_send);
	//delay(50);//等待服务器返回>
	while(com2_cmd_buf[0]!='>');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	Print_Str(POST);//第一串字符常量
	Print_Char(0x35);//Content-length
	Print_Char(0x33);
	Print_Str(POST2);  //第二串字符常量
	for(i=0;i<11;i++)	//发送11位手机号
	{
	Print_Char(0x30+tele[i]);
	}
	
	Print_Char(',');
	//发送9位机器编号
	for(i=0;i<9;i++)
	{
	Print_Char(0x30+mach_num[i]);
	}

	Print_Char(',');
	//发送1位货道编号
	Print_Char(0x30+chan); 

	Print_Str(POST3);	//末尾字符常量
	Print_Str(SEND);
	while(com2_cmd_buf[0]!='O');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';

	delay(10);
	for(i=0;i<13;i++)
		Print_Char(CIPCLOSE[i]);	
	
	AUXR1 = 0X00;//串口在3.0,3.1
}
void Remote_Set()
{
	uchar i;

	AUXR1 = 0X40;//串口在3.6,3.7
	com2_cmd_num = 0;



	//随便发几个命令唤醒沉睡的A6
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
	//发送数据指令
	//wait_send = 0x01;
	for(i=0;i<12;i++)
		Print_Char(CIPSEND[i]);
	//while(wait_send);
	//delay(100);//等待服务器返回>
	while(com2_cmd_buf[0]!='>');
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	Print_Str(Remote);//第一串字符常量
	Print_Str(SEND);
//	while(com2_cmd_buf[0]!='O');
//	com2_cmd_buf[0]=' ';
//	com2_cmd_buf[1]=' ';

	while(com2_cmd_buf[0]!='{'); //等待有{}信息的datapoints数据回传
	com2_cmd_buf[0]=' ';
	com2_cmd_buf[1]=' ';
	delay(10);
//	for(i=0;i<13;i++)	 //前面语句中有connect:close，就不用再close了
//		Print_Char(CIPCLOSE[i]);	
	
	AUXR1 = 0X00;//串口在3.0,3.1
}
void Init_UART_SMS(void)//串口初始化、定时器初始化
{
	TR1 = 0;
	AUXR1 = 0X40;//串口在3.6,3.7
	SCON = 0x50 ;  //SCON: serail mode 1, 8-bit UART, enable ucvr
    //UART为模式1，8位数据，允许接收
    TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
    //定时器1为模式2,8位自动重装
    PCON |= 0x80 ; //SMOD=1;
    TH1 = 0xFF ;   //Baud:115200 fosc=22.1184MHz
    TL1=0xFF;
    //IE |= 0x90 ;     //Enable Serial Interrupt
	ES=1;
    TR1 = 1 ;       // timer 1 run
 
}




