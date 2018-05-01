#ifndef _DS1302_H
#define _DS1302_H
unsigned char DS1302ReadByte();
unsigned char DS1302Read(unsigned char cmd);
void DS1302Write(unsigned char cmd,unsigned char dat);
void DS1302WriteByte(unsigned char dat);
void RE_DS1302();
void Init_DS1302();
//void Split_Display(unsigned char address);
void Display_Menu(unsigned char *t);
void Initialize_device(unsigned char *t);
void LCD_Display_Clock(unsigned char *dig,unsigned char *img);
#endif