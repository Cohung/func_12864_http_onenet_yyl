#ifndef _12864_H
#define _12864_H
#define SEL_CHAN 0X01
#define ERR_MES  0x02
#define NO_INV   0x03
#define GET_INV  0x04

void WriteByte_S(unsigned char byte);
void WriteCommand(unsigned char a);
void WriteData(unsigned char b);
void initial_LCD();

void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s);	//显示字符串string
void ClrScreen();											 //清屏（基本命令集）
void Clean_12864_GDRAM(void);
//void LCD_1616Graphic(unsigned char x,unsigned char y, unsigned char code *img);
void LCD_16128Graphic(unsigned char x, unsigned char y,unsigned char *img);//void LCD_16128Graphic(unsigned char x, unsigned char code *img[16][16]);
void set(unsigned char x,unsigned char y, unsigned char xdata *img);
void clr(unsigned char x,unsigned char y, unsigned char *img);
void Draw_Dig(unsigned char add,unsigned char num,unsigned char *dig,unsigned char *img);
void Draw_Graphic(unsigned char hang_shu,unsigned char *img);
void Draw_LOGO(unsigned char *img_1,unsigned char *img_2,unsigned char *img_3);

void DisplayLcd(uchar type);
//void DispalyLcd1();
//void disCardId(unsigned char *gID);





void LCD_Data(unsigned char LCDbyte );

#endif


