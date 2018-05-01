#include "STC89C5xRC_RDP.h"
#include "MAIN.H"
#include "STC15EROM.h"
#include "12864.H"
#include "DS1302.H"
#include "KEY.H"

extern bit b;
unsigned char set_num=0,tab_num=0; 
extern unsigned char TIME[6];
unsigned char t_tmp[6];
extern unsigned char start_long_set,long_set,tmp_show;
char flag[5]={0,0,0,0,0};

extern unsigned char Time_Range;
unsigned char store_timerange;
unsigned char store_ledtime[4];
unsigned char num[11];
unsigned char mach_num[9];
extern unsigned char idata set_led_time[4];
unsigned char set_led_num=0;
extern unsigned int led_start,led_end;

extern unsigned char  DS1302_SECOND_WRITE;
extern unsigned char  DS1302_YEAR_WRITE;
extern unsigned char  DS1302_MONTH_WRITE;
extern unsigned char  DS1302_DAY_WRITE;
extern unsigned char  DS1302_HOUR_WRITE;
extern unsigned char  DS1302_MINUTE_WRITE;

extern  unsigned char DS1302_SECOND_READ;
extern unsigned char  DS1302_YEAR_READ;
extern unsigned char  DS1302_MONTH_READ;
extern unsigned char  DS1302_DAY_READ;
extern unsigned char  DS1302_HOUR_READ;
extern unsigned char  DS1302_MINUTE_READ;

extern unsigned char code dig_9[13];
extern unsigned char xdata dd_1[256];
extern unsigned char code Chinese_SHU[32];
extern unsigned char code ALL_ZERO[32];
extern unsigned char code  Cl[256];
extern unsigned char aa,set_long_time,set_long_time_flag;
/*-------------------------------------------------
                   更新时间函数
--------------------------------------------------*/


void update_ds1302_time(unsigned char add,unsigned char time)
{
//DS1302Write(DS1302_SECOND_WRITE,0x80);//停时钟
  DS1302Write(0x8e,0x00);  //开写保护
  DS1302Write(add,time);
  DS1302Write(0x8e,0x80);	//关写保护	
//DS1302Write(DS1302_SECOND_WRITE,0x00);//开时钟
}


void Display_TIME(unsigned char add,unsigned char num,unsigned char *time )
{
    
  	WriteCommand(add);
	DelayMs(10);
	WriteData('0'+time[num]/16);
	DelayMs(10);
	WriteData('0'+time[num]%16);

}



/*-------------------------------------------------
                   时间加函数
--------------------------------------------------*/
 unsigned char  inc_function(unsigned char num)
 { 
   unsigned char num1;
   num1=(num<<4);
   num1=((num1>>4)-0x09);
   if(num1!=0)   
	num++;
   else
	{
	 num1=(num>>4)+1;
	 num=(num1<<4);
    }
   return num;
 }
/*-------------------------------------------------
         	   时间减函数
---------------------------------------------------*/
unsigned char sub_function(unsigned char num)
{
     unsigned char num1;
	 num1=(num<<4);
	 num1=((num1>>4)-0x00);
	 if(num1!=0)
	    num--;
	 else
	 {	
	    num--;
	    num1=(num>>4);
		num=((num1<<4)+0x09);
	 }
	 return num;
}

/*------------------------------------------------*/
void Correct_Day(unsigned char *ti)
{
   unsigned char temp;
   if((*(ti+1)==0x02)||(*(ti+1)==0x04)||(*(ti+1)==0x06)||(*(ti+1)==0x09)||(*(ti+1)==0x11))
   {
      if((*(ti+1)==0x04)||(*(ti+1)==0x06)||(*(ti+1)==0x09)||(*(ti+1)==0x11))
	  {
	    if(*(ti+2)==0x31)
		   *(ti+2)=0x01;
		if(*(ti+2)==0x00)
		   *(ti+2)=0x30;
	  }

	  if(*(ti+1)==0x02)
	  {
	     temp=(*(ti)/16)*10+(*(ti)%16);
	    if(temp%4==0)
		   {
		     if(*(ti+2)==0x30)
			    *(ti+2)=0x01;
			 if(*(ti+2)==0x00)
			    *(ti+2)=0x29;
		   }
		if(temp%4!=0)
		   {
		     if(*(ti+2)==0x29)
			    *(ti+2)=0x01;
			 if(*(ti+2)==0x00)
			    *(ti+2)=0x28;
		   }
	  }
   } 
   else
   {
      if(*(ti+2)==0x32)
	    *(ti+2)=0x01;
	  if(*(ti+2)==0x00)
	    *(ti+2)=0x31;
   }
}



void  Month_Correct_Day(unsigned char *ti)			//根据用户刚设置的月份来更正不正确的日期
{
	unsigned char temp;
    if((*(ti+1)==0x02)||(*(ti+1)==0x04)||(*(ti+1)==0x06)||(*(ti+1)==0x09)||(*(ti+1)==0x11))
	{
	  if((*(ti+1)==0x04)||(*(ti+1)==0x06)||(*(ti+1)==0x09)||(*(ti+1)==0x11))
	  {
	  	 if(*(ti+2)==0x31)
		    *(ti+2)=0x30;
	  }
       if(*(ti+1)==0x02)
	   {
	     temp=(*(ti)/16)*10+(*(ti)%16);
	     if((temp%4!=0)&&(*(ti+2)>=0x29))
		     *(ti+2)=0x28; 
	   }

	}
//    else
//	{}
}



void Year_Correct_Day(unsigned char *ti)
{
     unsigned char temp;
	 temp=(*(ti)/16)*10+(*(ti)%16);
   	 if(temp%4!=0)
	   {
	     if(*(ti+2)==0x29)
		    *(ti+2)=0x28;
	   }
}



/****************************************************************/

void set_table()
 {
 unsigned int i;

 if(SET==0)
   {
     unsigned char i=0;
	if(set_num == 0)
		{start_long_set = 1;long_set = 0;TR0 = 1;}
//	else
//	{}
		    
			     if(set_num == 0) 
			      while((long_set<6)&&(SET==0));
				 else
				  while(!SET); 
//				  WriteCommand(0x85);
//			WriteData('0'+long_set%10);
				  if((set_num>0) || (long_set >= 5))
				  {
					//	  set_long_time_flag=0;
						  //while(!SET);
						  TR0 = 0;
					    //  aa=0;
					      set_num++;
						  tmp_show=0;
						  if(set_num == 1)
						  	Draw_LOGO(Cl,Cl,Cl);
						  switch(set_num)
						  {
						    case 1:  
								BEEP(15);
								for(i=1;i<4;i++) 	  
							         LCD_PutString(0,i,"                ");
						             LCD_PutString(0,1,"修改时间： " );
									 for(i=1;i<=6;i++)
									 	t_tmp[i-1] = TIME[i-1];
						             Display_Menu(t_tmp);
									 Draw_LOGO(Cl,Cl,Cl);
								//	 aa=0;
									 while(!SET);
									 long_set = 0;
									 start_long_set = 0;
									 break;
									  
							case 2:  
							BEEP(15);
									 for(i=1;i<4;i++) 	  
							         LCD_PutString(0,i,"                ");//TR0 = 0; 
							         LCD_PutString(0,1,"修改间隔时间:");
				//					 WriteCommand(0x85);
				//					 WriteData(0x30+5);
				//					 WriteData(0x30+2);
									// LCD_1616Graphic(5,1,Chinese_SHU);
							         LCD_PutString(0,2,"原间隔时间:");
									 LCD_PutString(7,2,"天");
									// LCD_1616Graphic(4,2,Chinese_SHU);
							         LCD_PutString(0,3,"现间隔时间:");
									 LCD_PutString(7,3,"天");
									// LCD_1616Graphic(5,3,Chinese_SHU);
				  
									 WriteCommand(0x96);
									 WriteData('0'+Time_Range/10);
									 WriteData('0'+Time_Range%10);
									 WriteCommand(0x8e);
									 WriteData('0'+Time_Range/10);
									 WriteData('0'+Time_Range%10);
									 store_timerange = Time_Range;	 
									 break;
							case 3:  
							BEEP(15);
							Write_E(57,0,Time_Range);
									 for(i=1;i<4;i++){ 	  
							         	LCD_PutString(0,i,"                ");
										store_ledtime[i-1] = set_led_time[i-1];
										}
									store_ledtime[3] = set_led_time[3];
//									 LCD_1616Graphic(5,1,ALL_ZERO);
//									 LCD_1616Graphic(4,2,ALL_ZERO);
//									 LCD_1616Graphic(5,3,ALL_ZERO);

									 LCD_PutString(0,1," LED  启停时间");
									 LCD_PutString(0,2,"启动:   时  分");
									 
									 WriteCommand(0x93); 
									 WriteData('0'+store_ledtime[0]/10);
									 WriteData('0'+store_ledtime[0]%10);
									 WriteCommand(0x95); 
									 WriteData('0'+store_ledtime[1]/10);
									 WriteData('0'+store_ledtime[1]%10);
							//		 LCD_PutString(2,2,"18时30分");
									 LCD_PutString(0,3,"停止：  时  分");
									 WriteCommand(0x8b); 
									 WriteData('0'+store_ledtime[2]/10);
									 WriteData('0'+store_ledtime[2]%10);
									 WriteCommand(0x8d);
									 WriteData('0'+store_ledtime[3]/10);
									 WriteData('0'+store_ledtime[3]%10);
								//	 LCD_PutString(2,4,"06时30分");
								    
									 break;
							case 4:  
							BEEP(15);
							led_start = set_led_time[0]*60+set_led_time[1];
							led_end = set_led_time[2]*60+set_led_time[3];
							Write_E(57,1,set_led_time[0]);
							Write_E(57,2,set_led_time[1]);
							Write_E(57,3,set_led_time[2]);
							Write_E(57,4,set_led_time[3]);
							LCD_PutString(0,1,"                ");
							LCD_PutString(0,2,"  管理员手机号  ");
							LCD_PutString(0,3,"                ");
							WriteCommand(0x89);
							for(i=0;i<11;i++){
								num[i] = R_EROM(57,5+i);
								if(num[i]>0x9){
									WriteData(0x30);
									num[i] = 0;
									}
								else
									WriteData(0x30+num[i]);
								}
							tab_num = 0;
							break;

							case 5:
							BEEP(15);
							for(i=0;i<11;i++){
								Write_E(57,5+i,num[i]);
								
								}
							LCD_PutString(0,1,"                ");
							LCD_PutString(0,2,"    机器编号：  ");
							LCD_PutString(0,3,"                ");
								WriteCommand(0x8a);
							for(i=0;i<9;i++){
								mach_num[i] = R_EROM(57,16+i);
								if(mach_num[i]>0x9){
									WriteData(0x30);
									mach_num[i] = 0;
									}
								else
									WriteData(0x30+mach_num[i]);
								}
							tab_num = 0;
							break;


							case 6:  set_num=0;//TR0 = 0;
									 for(i=0;i<9;i++){
								Write_E(57,16+i,mach_num[i]);
								
								} 
							         	  
							        LCD_PutString(0,1,"                ");
									LCD_PutString(0,3,"                ");
									LCD_PutString(0,2,"    设置成功！  ");
									BEEP(200);
							        LCD_PutString(0,2,"                ");
						
									 break;
						  }
						TR0 = 1;
						}
					else{}
	start_long_set = 0;
	long_set = 0;
//	LED = 1;		 
	// else
	  // TR0=0;
   }						 
  

    if((set_num==1)&&(TAB==0))
	  {	
	  
	  
	     DelayMs(10);
	     if((set_num==1)&&(TAB==0))
           {  
		      while(!TAB);
			  TR0 = 0; 
			  tmp_show = 0;
		      tab_num+=1;
			  if(tab_num == 5)
			  	tab_num = 0;
			
			  switch(tab_num)
			     {
			           
						  
				   case 1: 
				          WriteCommand(0x91);
						  WriteData(0x30+t_tmp[0]/16);
						  WriteData(0x30+t_tmp[0]%16);
						  break;
				   case 2:
						  WriteCommand(0x93);
						  WriteData(0x30+t_tmp[1]/16);
						  WriteData(0x30+t_tmp[1]%16);
						  break;
                   case 3: 
						  WriteCommand(0x95);
						  WriteData(0x30+t_tmp[2]/16);
						  WriteData(0x30+t_tmp[2]%16);
						  break;
				   case 4: 
						  WriteCommand(0x88);
						  WriteData(0x30+t_tmp[3]/16);
						  WriteData(0x30+t_tmp[3]%16);
						  break;
				   case 0:
				          WriteCommand(0x8a);
						  WriteData(0x30+t_tmp[4]/16);
						  WriteData(0x30+t_tmp[4]%16);
				   default: break;
				 }
		   }
		   TR0 = 1;

	  }
	  

	  if((set_num==1)&&(TAB!=0))
	  {	
	    if(INC==0)
		{ 
		  DelayMs(10);	 	
		  if(INC==0)
		  {
		    while(!INC);TR0 = 0;tmp_show=0;
			switch(tab_num)
			{
			  case 0: flag[0]=flag[0]+1;	  			     
			          t_tmp[0]=inc_function(t_tmp[0]); 				
					  if(t_tmp[0]==0x99)
					        t_tmp[0]=0x00;
					   Year_Correct_Day(t_tmp);
					   Display_TIME(0x91,0,t_tmp);
			           break;

			  case 1: flag[1]=flag[1]+1;
			          t_tmp[1]=inc_function(t_tmp[1]);
					  if(t_tmp[1]==0x13)
					       t_tmp[1]=0x01;
					  Month_Correct_Day(t_tmp);
					  Display_TIME(0x93,1,t_tmp);
					  break;

			  case 2: flag[2]=flag[2]+1;
			          t_tmp[2]=inc_function(t_tmp[2]);
//					  if(TIME[2]==0x32)
//					      TIME[2]=0x01;
                      Correct_Day(t_tmp);
					  Display_TIME(0x95,2,t_tmp);
					  break;

			  case 3: flag[3]=flag[3]+1;
			          t_tmp[3]=inc_function(t_tmp[3]); 
					  if(t_tmp[3]==0x24)
					      t_tmp[3]=0x00;
					  Display_TIME(0x88,3,t_tmp);
					  break;

			  case 4: flag[4]=flag[4]+1;
			          t_tmp[4]=inc_function(t_tmp[4]);
					  if(t_tmp[4]==0x60)
					      t_tmp[4]=0x00;
					  Display_TIME(0x8a,4,t_tmp);
					  break;
			 case 5:  tab_num=0;
			          break;
			 default: break;

			}
		  }
		  TR0 = 1;
		}

		 

         if(SUB==0)
		{ 
		  DelayMs(10);
		  if(SUB==0)
		  {
		    while(!SUB);
			TR0 = 0; 
			tmp_show=0; 	
		
			switch(tab_num)
			{
			  case 0: flag[0]=flag[0]-1;			     
			          t_tmp[0]=sub_function(t_tmp[0]);				
					  if(t_tmp[0]==0x00)
					        t_tmp[0]=0x99;
					  Year_Correct_Day(t_tmp);
					  Display_TIME(0x91,0,t_tmp);		
					  break;

			  case 1: flag[1]=flag[1]-1;
			          t_tmp[1]=sub_function(t_tmp[1]);
					  if(t_tmp[1]==0x00)
					       t_tmp[1]=0x12;
					  Month_Correct_Day(t_tmp);
					  Display_TIME(0x93,1,t_tmp);
					  break;

			  case 2: flag[2]=flag[2]-1;
			          t_tmp[2]=sub_function(t_tmp[2]);
//					  if(TIME[2]==0x00)
//					      TIME[2]=0x31;
                      Correct_Day(t_tmp);
					  Display_TIME(0x95,2,t_tmp);
					  break;

			  case 3: flag[3]=flag[3]-1;
			          t_tmp[3]=sub_function(t_tmp[3]);
					  if(t_tmp[3]==0xf9)
					      t_tmp[3]=0x23;
					  Display_TIME(0x88,3,t_tmp);
					  break;

			  case 4: flag[4]=flag[4]-1;
			          t_tmp[4]=sub_function(t_tmp[4]);
					  if(t_tmp[4]==0xf9)
					     t_tmp[4]=0x59;
					  Display_TIME(0x8a,4,t_tmp);
					  break;
			 case 5:  tab_num=0;
			          break;
			 default: break;

			}
		  }
		  TR0 = 1;
		}
	  }
		 
 

	 if(set_num==2)	   //设置完时间后，对有变化的时间进行更新
	 {	
	 for(i=0;i<=5;i++)
		 	TIME[i] = t_tmp[i];	 
		
		 TR0 = 0;
		 
	 	 if(flag[0]!=0)
	      {
		    update_ds1302_time(DS1302_YEAR_WRITE,TIME[0]);
		    Draw_Dig(3,TIME[0]/16,dig_9,dd_1); 
			Draw_Dig(4,TIME[0]%16,dig_9,dd_1);
		
		  }

		  if(flag[1]!=0)
	      {
		    update_ds1302_time(DS1302_MONTH_WRITE,TIME[1]);
		    Draw_Dig(6,TIME[1]/16,dig_9,dd_1);
			Draw_Dig(7,TIME[1]%16,dig_9,dd_1);
////			LCD_16128Graphic(2,4,dd_1);
////			flag[1]=0;
		  }

		  if(flag[2]!=0)
	      {	  
	        update_ds1302_time(DS1302_DAY_WRITE,TIME[2]);
		    Draw_Dig(9,TIME[2]/16,dig_9,dd_1);
			Draw_Dig(10,TIME[2]%16,dig_9,dd_1);
		
		  }

		  if(flag[3]!=0)
	      {
		    update_ds1302_time(DS1302_HOUR_WRITE,TIME[3]);
		    Draw_Dig(15,TIME[3]/16,dig_9,dd_1);
			Draw_Dig(16,TIME[3]%16,dig_9,dd_1);
		
		  }

		  if(flag[4]!=0)
	      {	
		    update_ds1302_time(DS1302_MINUTE_WRITE,TIME[4]);
		    Draw_Dig(18,TIME[4]/16,dig_9,dd_1);
			Draw_Dig(19,TIME[4]%16,dig_9,dd_1);
		
		
		  }


		  if(flag[0]!=0)
	      {
		  
			LCD_16128Graphic(1,4,dd_1);
			LCD_16128Graphic(2,4,dd_1);
			flag[0]=0;
		  }

		  if(flag[1]!=0)
	      {
		  
			LCD_16128Graphic(2,4,dd_1);
			flag[1]=0;
		  }

		  if(flag[2]!=0)
	      {	  
	      
			LCD_16128Graphic(3,4,dd_1); 
			LCD_16128Graphic(4,4,dd_1);
			flag[2]=0;
		  }

		  if(flag[3]!=0)
	      {
		   
			LCD_16128Graphic(5,4,dd_1); 
		    LCD_16128Graphic(6,4,dd_1);
			flag[3]=0;
		  }

		  if(flag[4]!=0)
	      {	
		   
			LCD_16128Graphic(6,4,dd_1); 
	        LCD_16128Graphic(7,4,dd_1); 
			flag[4]=0;
		
		  }
		
		  TR0 = 1;	  
	 }



	 if(set_num==2)
	 {
	 	
	   	tab_num=0;
		if((set_num==2)&&(INC==0))
		{
		    DelayMs(10);
			if(INC==0)
			{  
			  //flag2=1;
			  while(!INC);tmp_show=0;TR0 = 0;
			  Time_Range++;
			  if(Time_Range==31)
			      Time_Range=0;
               WriteCommand(0x8e);
			   WriteData('0'+Time_Range/10);
			   WriteData('0'+Time_Range%10);
			}
		}

		if((set_num==2)&&(SUB==0))
		{	    
		    DelayMs(1);
			if(SUB==0)
			{
			  //flag2=1;
			  while(!SUB);tmp_show=0;
			  Time_Range--;
			  if(Time_Range>=30)
			      Time_Range=30;
			  WriteCommand(0x8e);
			  WriteData('0'+Time_Range/10);
			   WriteData('0'+Time_Range%10);
			}
		}
		TR0 = 1;
	}
	




	if(set_num==3)
	{
	   

		if(TAB==0)
		{	TR0=0;
			DelayMs(10);
			if(TAB == 0){
	       while(!TAB);tmp_show=0;
	   	   tab_num+=1;
		
			
			if(tab_num==4) tab_num=0;
			switch(tab_num)
			{
			           case 1: 
					          WriteCommand(0x93);
							  WriteData(0x30+set_led_time[0]/10);
							  WriteData(0x30+set_led_time[0]%10);
							  break;
					   case 2:
							  WriteCommand(0x95);
							  WriteData(0x30+set_led_time[1]/10);
							  WriteData(0x30+set_led_time[1]%10);
							  break;
	                   case 3: 
							  WriteCommand(0x8b);
							  WriteData(0x30+set_led_time[2]/10);
							  WriteData(0x30+set_led_time[2]%10);
							  break;
					   
					   case 0:
					          WriteCommand(0x8d);
							  WriteData(0x30+set_led_time[3]/10);
							  WriteData(0x30+set_led_time[3]%10);
					   default: break;
	
			} 
			}
		}
	    TR0=1;
	}



	
	if((set_num==3)&&(INC==0))
	{
	  DelayMs(10);
	  if(INC==0)
	  {
	    while(!INC);TR0=0; tmp_show=0;
		switch(tab_num)
		{
		    case 0: set_led_time[0]+=1;
			        if(set_led_time[0]==24)
					   set_led_time[0]=0;
					WriteCommand(0x93);
					WriteData('0'+set_led_time[0]/10);    
					WriteData('0'+set_led_time[0]%10);
					break;
			case 1: set_led_time[1]+=1;
			        if(set_led_time[1]==60)
					   set_led_time[1]=0;
					WriteCommand(0x95);
					WriteData('0'+set_led_time[1]/10);    
					WriteData('0'+set_led_time[1]%10);
					break;
			case 2: set_led_time[2]+=1;
			        if(set_led_time[2]==24)
					   set_led_time[2]=0;
					WriteCommand(0x8b);
					WriteData('0'+set_led_time[2]/10);    
					WriteData('0'+set_led_time[2]%10);
					break;
            case 3: set_led_time[3]+=1;
			        if(set_led_time[3]==60)
					   set_led_time[3]=0;
					WriteCommand(0x8d);
					WriteData('0'+set_led_time[3]/10);    
					WriteData('0'+set_led_time[3]%10);
					break;
		} 
	  }
	  TR0=1;
	}



	if((set_num==3)&&(SUB==0))
	{
	 DelayMs(10);
	  if(SUB==0)
	  {
	    while(!SUB);TR0=0; tmp_show=0;
		switch(tab_num)
		{
		    case 0: set_led_time[0]-=1;
			        if(set_led_time[0]==255)
					   set_led_time[0]=23;
					WriteCommand(0x93);
					WriteData('0'+set_led_time[0]/10);    
					WriteData('0'+set_led_time[0]%10);
					break;
			case 1: set_led_time[1]-=1;
			        if(set_led_time[1]==255)
					   set_led_time[1]=59;
					WriteCommand(0x95);
					WriteData('0'+set_led_time[1]/10);    
					WriteData('0'+set_led_time[1]%10);
					break;
			case 2: set_led_time[2]-=1;
			        if(set_led_time[2]==255)
					   set_led_time[2]=23;
					WriteCommand(0x8b);
					WriteData('0'+set_led_time[2]/10);    
					WriteData('0'+set_led_time[2]%10);
					break;
            case 3: set_led_time[3]-=1;
			        if(set_led_time[3]==255)
					   set_led_time[3]=59;
					WriteCommand(0x8d);
					WriteData('0'+set_led_time[3]/10);    
					WriteData('0'+set_led_time[3]%10);
					break;
		} 
	  }
	  TR0=1;
	}






if(set_num==4)
	{
	   

		if(TAB==0)
		{	TR0=0;
			DelayMs(10);
			if(TAB == 0){
	       while(!TAB);tmp_show=0;
	   	   tab_num+=1;
		
			
			if(tab_num==11) tab_num=0;
			switch(tab_num)
			{
			           case 0: 
					   			WriteCommand(0x8e);
								WriteData('0'+num[10]);
					          WriteCommand(0x89);
							  WriteData(0x30+num[0]);
							 
							  break;
					case 1:
					WriteCommand(0x89);
							  WriteData(0x30+num[0]);
							  WriteData(0x30+num[1]);
					break;
					   case 2:
					   	WriteCommand(0x89);
					WriteData('0'+num[0]);
					WriteData('0'+num[1]);
							  WriteCommand(0x8a);
							  WriteData(0x30+num[2]);
							  
							  break;
						case 3:
						WriteCommand(0x8a);
							  WriteData(0x30+num[2]);
							  WriteData(0x30+num[3]);
						break;
	                   case 4:
					   WriteCommand(0x8a);
					WriteData('0'+num[2]);
					WriteData('0'+num[3]); 
							  WriteCommand(0x8b);
							  WriteData(0x30+num[4]);
							  
							  break;
					   case 5:
								WriteCommand(0x8b);
							  WriteData(0x30+num[4]);
							  WriteData(0x30+num[5]);
					   break;

					   case 6:
					   	WriteCommand(0x8b);
					WriteData('0'+num[4]);
					WriteData('0'+num[5]);
					          WriteCommand(0x8c);
							  WriteData(0x30+num[6]);
							  
							  break;
						case 7:
						WriteCommand(0x8c);
							  WriteData(0x30+num[6]);
							  WriteData(0x30+num[7]);
						break;
					   case 8:
					   	WriteCommand(0x8c);
					WriteData('0'+num[6]);
					WriteData('0'+num[7]);
					          WriteCommand(0x8d);
							  WriteData(0x30+num[8]);
							  
							  break;
						case 9:
						WriteCommand(0x8d);
							  WriteData(0x30+num[8]);
							  WriteData(0x30+num[9]);
						break;
					   case 10:
					   	WriteCommand(0x8d);
					WriteData('0'+num[8]);
					WriteData('0'+num[9]);
					          WriteCommand(0x8e);
							  WriteData(0x30+num[10]);
							  //WriteData(0x20);
							  break;
					   default: break;
	
			} 
			}
		}
	    TR0=1;
	}



	
	if((set_num==4)&&(INC==0))
	{
	  DelayMs(10);
	  if(INC==0)
	  {
	    while(!INC);TR0=0; tmp_show=0;
		switch(tab_num)
		{
		    case 0: num[0]+=1;
			        if(num[0]==10)
					   num[0]=0;
					
					WriteCommand(0x89);
					WriteData('0'+num[0]);    				
					break;
			case 1: num[1]+=1;
			        if(num[1]==10)
					   num[1]=0;
					WriteCommand(0x89);
					WriteData('0'+num[0]);    
					WriteData('0'+num[1]);
					break;
		
            case 2: num[2]+=1;
			        if(num[2]==10)
					   num[2]=0;
				
					WriteCommand(0x8a);
					WriteData('0'+num[2]);    				
					break;
			case 3: num[3]+=1;
			        if(num[3]==10)
					   num[3]=0;
					WriteCommand(0x8a);
					WriteData('0'+num[2]);    
					WriteData('0'+num[3]);
					break;
			case 4: num[4]+=1;
			        if(num[4]==10)
					   num[4]=0;
					
					WriteCommand(0x8b);
					WriteData('0'+num[4]);    				
					break;
			case 5: num[5]+=1;
			        if(num[5]==10)
					   num[5]=0;
					WriteCommand(0x8b);
					WriteData('0'+num[4]);    
					WriteData('0'+num[5]);
					break;
			case 6: num[6]+=1;
			        if(num[6]==10)
					   num[6]=0;
				
					WriteCommand(0x8c);
					WriteData('0'+num[6]);    				
					break;
			case 7: num[7]+=1;
			        if(num[7]==10)
					   num[7]=0;
					WriteCommand(0x8c);
					WriteData('0'+num[6]);    
					WriteData('0'+num[7]);
					break;
			case 8: num[8]+=1;
			        if(num[8]==10)
					   num[8]=0;
				
					WriteCommand(0x8d);
					WriteData('0'+num[8]);    				
					break;
			case 9: num[9]+=1;
			        if(num[9]==10)
					   num[9]=0;
					WriteCommand(0x8d);
					WriteData('0'+num[8]);    
					WriteData('0'+num[9]);
					break;
			case 10: num[10]+=1;
			        if(num[10]==10)
					   num[10]=0;
				
					WriteCommand(0x8e);
					WriteData('0'+num[10]);    				
					break;
		} 
	  }
	  TR0=1;
	}



	if((set_num==4)&&(SUB==0))
	{
	 DelayMs(10);
	  if(SUB==0)
	  {
	    while(!SUB);TR0=0; tmp_show=0;
		switch(tab_num)
		{
		    case 0: num[0]-=1;
			        if(num[0]==255)
					   num[0]=9;
					WriteCommand(0x89);
					WriteData('0'+num[0]);    
				
					break;
			case 1: num[1]-=1;
			        if(num[1]==255)
					   num[1]=9;
					WriteCommand(0x89);
					WriteData('0'+num[0]);    
					WriteData('0'+num[1]);
					break;
			case 2: num[2]-=1;
			        if(num[2]==255)
					   num[2]=9;
					WriteCommand(0x8a);
					WriteData('0'+num[2]);    
				
					break;
			case 3: num[3]-=1;
			        if(num[3]==255)
					   num[3]=9;
					WriteCommand(0x8a);
					WriteData('0'+num[2]);    
					WriteData('0'+num[3]);
					break;
			case 4: num[4]-=1;
			        if(num[4]==255)
					   num[4]=9;
					WriteCommand(0x8b);
					WriteData('0'+num[4]);    
				
					break;
			case 5: num[5]-=1;
			        if(num[5]==255)
					   num[5]=9;
					WriteCommand(0x8b);
					WriteData('0'+num[4]);    
					WriteData('0'+num[5]);
					break;
			case 6: num[6]-=1;
			        if(num[6]==255)
					   num[6]=9;
					WriteCommand(0x8c);
					WriteData('0'+num[6]);    
				
					break;
			case 7: num[7]-=1;
			        if(num[7]==255)
					   num[7]=9;
					WriteCommand(0x8c);
					WriteData('0'+num[6]);    
					WriteData('0'+num[7]);
					break;
			case 8: num[8]-=1;
			        if(num[8]==255)
					   num[8]=9;
					WriteCommand(0x8d);
					WriteData('0'+num[8]);    
				
					break;
			case 9: num[9]-=1;
			        if(num[9]==255)
					   num[9]=9;
					WriteCommand(0x8d);
					WriteData('0'+num[8]);    
					WriteData('0'+num[9]);
					break;
			case 10: num[10]-=1;
			        if(num[10]==255)
					   num[10]=9;
					WriteCommand(0x8e);
					WriteData('0'+num[10]);    
				
					break;
		
		} 
	  }
	  TR0=1;
	}
	
	
	
	
	
	
	
	
	
	
	if(set_num==5)
	{
	   

		if(TAB==0)
		{	TR0=0;
			DelayMs(10);
			if(TAB == 0){
	       while(!TAB);tmp_show=0;
	   	   tab_num+=1;
		
			
			if(tab_num==9) tab_num=0;
			switch(tab_num)
			{
			           case 0: 
					   			WriteCommand(0x8e);
								WriteData('0'+mach_num[8]);
					          WriteCommand(0x8a);
							  WriteData(0x30+mach_num[0]);
							 
							  break;
					case 1:
					WriteCommand(0x8a);
							  WriteData(0x30+mach_num[0]);
							  WriteData(0x30+mach_num[1]);
					break;
					case 2:
					WriteCommand(0x8a);
							  WriteData(0x30+mach_num[0]);
							  WriteData(0x30+mach_num[1]);
					WriteCommand(0x8b);
							  WriteData(0x30+mach_num[2]);
					break;
					case 3:
					WriteCommand(0x8b);
							  WriteData(0x30+mach_num[2]);
							  WriteData(0x30+mach_num[3]);
					break;
					case 4:
					WriteCommand(0x8b);
							  WriteData(0x30+mach_num[2]);
							  WriteData(0x30+mach_num[3]);
					WriteCommand(0x8c);
							  WriteData(0x30+mach_num[4]);
					break;
					case 5:
					WriteCommand(0x8c);
							  WriteData(0x30+mach_num[4]);
							  WriteData(0x30+mach_num[5]);
					break;
					case 6:
					WriteCommand(0x8c);
							  WriteData(0x30+mach_num[4]);
							  WriteData(0x30+mach_num[5]);
					WriteCommand(0x8d);
							  WriteData(0x30+mach_num[6]);
					break;
					case 7:
					WriteCommand(0x8d);
							  WriteData(0x30+mach_num[6]);
							  WriteData(0x30+mach_num[7]);
					case 8:
					WriteCommand(0x8d);
							  WriteData(0x30+mach_num[6]);
							  WriteData(0x30+mach_num[7]);
					WriteCommand(0x8e);
							  WriteData(0x30+mach_num[8]);
					break;
					default: break;
	
			} 
			}
		}
	    TR0=1;
	}



	
	if((set_num==5)&&(INC==0))
	{
	  DelayMs(10);
	  if(INC==0)
	  {
	    while(!INC);TR0=0; tmp_show=0;
		switch(tab_num)
		{
		    case 0: mach_num[0]+=1;
			        if(mach_num[0]==10)
					   mach_num[0]=0;
					
					WriteCommand(0x8a);
					WriteData('0'+mach_num[0]);    				
					break;
			case 1: mach_num[1]+=1;
			        if(mach_num[1]==10)
					   mach_num[1]=0;
					WriteCommand(0x8a);
					WriteData('0'+mach_num[0]);    
					WriteData('0'+mach_num[1]);
					break;
			case 2: mach_num[2]+=1;
			        if(mach_num[2]==10)
					   mach_num[2]=0;
					WriteCommand(0x8b);
					WriteData('0'+mach_num[2]);    
				
					break;
			case 3: mach_num[3]+=1;
			        if(mach_num[3]==10)
					   mach_num[3]=0;
					WriteCommand(0x8b);
					WriteData('0'+mach_num[2]);    
					WriteData('0'+mach_num[3]);
					break;
			case 4: mach_num[4]+=1;
			        if(mach_num[4]==10)
					   mach_num[4]=0;
					WriteCommand(0x8c);
					WriteData('0'+mach_num[4]);    
				
					break;
			case 5: mach_num[5]+=1;
			        if(mach_num[5]==10)
					   mach_num[5]=0;
					WriteCommand(0x8c);
					WriteData('0'+mach_num[4]);    
					WriteData('0'+mach_num[5]);
					break;
			case 6: mach_num[6]+=1;
			        if(mach_num[6]==10)
					   mach_num[6]=0;
					WriteCommand(0x8d);
					WriteData('0'+mach_num[6]);    
				
					break;
			case 7: mach_num[7]+=1;
			        if(mach_num[7]==10)
					   mach_num[7]=0;
					WriteCommand(0x8d);
					WriteData('0'+mach_num[6]);    
					WriteData('0'+mach_num[7]);
					break;
			case 8: mach_num[8]+=1;
			        if(mach_num[8]==10)
					   mach_num[8]=0;
					WriteCommand(0x8e);
					WriteData('0'+mach_num[8]);    
				
					break;
			
		} 
	  }
	  TR0=1;
	}



	if((set_num==5)&&(SUB==0))
	{
	 DelayMs(10);
	  if(SUB==0)
	  {
	    while(!SUB);TR0=0; tmp_show=0;
		switch(tab_num)
		{
		    case 0: mach_num[0]-=1;
			        if(mach_num[0]==255)
					   mach_num[0]=9;
					WriteCommand(0x8a);
					WriteData('0'+mach_num[0]);    
				
					break;
			case 1: mach_num[1]-=1;
			        if(mach_num[1]==255)
					   mach_num[1]=9;
					WriteCommand(0x8a);
					WriteData('0'+mach_num[0]);    
					WriteData('0'+mach_num[1]);
					break;
			case 2: mach_num[2]-=1;
			        if(mach_num[2]==255)
					   mach_num[2]=9;
					WriteCommand(0x8b);
					WriteData('0'+mach_num[2]);    
				
					break;
			case 3: mach_num[3]-=1;
			        if(mach_num[3]==255)
					   mach_num[3]=9;
					WriteCommand(0x8b);
					WriteData('0'+mach_num[2]);    
					WriteData('0'+mach_num[3]);
					break;
			case 4: mach_num[4]-=1;
			        if(mach_num[4]==255)
					   mach_num[4]=9;
					WriteCommand(0x8c);
					WriteData('0'+mach_num[4]);    
				
					break;
			case 5: mach_num[5]-=1;
			        if(mach_num[5]==255)
					   mach_num[5]=9;
					WriteCommand(0x8c);
					WriteData('0'+mach_num[4]);    
					WriteData('0'+mach_num[5]);
					break;
			case 6: mach_num[6]-=1;
			        if(mach_num[6]==255)
					   mach_num[6]=9;
					WriteCommand(0x8d);
					WriteData('0'+mach_num[6]);    
				
					break;
			case 7: mach_num[7]-=1;
			        if(mach_num[7]==255)
					   mach_num[7]=9;
					WriteCommand(0x8d);
					WriteData('0'+mach_num[6]);    
					WriteData('0'+mach_num[7]);
					break;
			case 8: mach_num[8]-=1;
			        if(mach_num[8]==255)
					   mach_num[8]=9;
					WriteCommand(0x8e);
					WriteData('0'+mach_num[8]);    
				
					break;
		
		
		} 
	  }
	  TR0=1;
	}		
}