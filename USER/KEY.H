# ifndef _KEY_H_
# define _KEY_H_
//void display_ds1302_time(unsigned char add1,unsigned char add2);
unsigned char  inc_function(unsigned char num); 
void set_table();
//void RE_DS1302();
void Display_TIME(unsigned char add,unsigned char num,unsigned char *time );
void Month_Correct_Day(unsigned char *ti);
void Correct_Day(unsigned char *ti);
void Year_Correct_Day(unsigned char *ti);
# endif																					