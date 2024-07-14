//#include "main.h"
#include "lcd_hight.h"

#define x_Kar 56
#define y_Kar 0

#define x_Err 40
#define y_Err 125

unsigned char pred_kar=0, pred_err=0;
unsigned char viv_kar=55, viv_err=55;

initLCD(void){

	ili9341_init(); //initial driver setup to drive ili9341

	_delay_ms(1000);
	
	
	ili9341_clear(RED); //fill screen with black colour
	
	PrintAkk(2, 284);
	PrintVolt(30, 283, 0);

	PrintMasl(132, 287);
	PrintOil(162, 283, 333);
	
	

	PrintTemp(20, 220);
	//PrintRas(10, 137);
	Printl100km(130, 124);
	
	//PrintKar(kar_TORM);
	//DelKar();

	//PrintError(kar_TORM);
	//DelError();

}

void Error(void)
{

	for(int i = pred_err; i < 2; i++)
	{
		switch(i)
		{
			case 0:
			{
				if(lowLevelAntifreezePicture)
				{
					if(viv_err!=0)
					PrintError(kar_TEMP);
					viv_err = 0;
					pred_err=1;
					return;
				}
				break;
			}
			case 1:
			{
				if(lowBrakeFluidPicture)
				{
					if(viv_err!=1)
					PrintError(kar_TORM);
					viv_err = 1;
					pred_err=0;
					return;
				}
				break;
			}			
		}
	}
	if(!lowLevelAntifreezePicture && !lowBrakeFluidPicture && viv_err !=55)
	{
	DelError();
	PrintTemp(20, 220);
	//PrintRas(10, 137);
	Printl100km(130, 124);
	viv_err=55;
	}
pred_err = 0;
}


void Kartinki(void)
{

	for(int i = pred_kar; i < 5; i++)
	{
		switch(i)
		{
			case 0:
			{
				if(f_NUOZ)
				{
					if(viv_kar!=0)
					//PrintKar(kar_NYOZ);
					viv_kar = 0;
					pred_kar=1;
					return;
				}
				break;
			}
			case 1:
			{
				if(f_LF)
				{
					if(viv_kar!=1)
					//PrintKar(kar_FAR);
					viv_kar = 1;
					pred_kar=2;
					return;
				}
				break;
			}
			case 2:
			{
				if(TERM)
				{
					if(viv_kar!=2)
					PrintKar(kar_TERM);
					viv_kar = 2;
					pred_kar=3;
					return;
				}
				break;
			}
			case 3:
			{
				if(lowVoltagePicture)
				{
					if(viv_kar!=3)
					PrintKar(kar_AKK);
					viv_kar = 3;
					pred_kar=4;
					return;
				}
				break;
			}
			case 4:
			{
				if(lowGasolinePicture)
				{
					if(viv_kar!=4)
					PrintKar(kar_BENZ);
					viv_kar = 4;
					pred_kar=0;
					return;
				}
				break;
			}
		}
	}
	if(!f_NUOZ && !f_LF && !f_LS && !lowVoltagePicture && !lowGasolinePicture && viv_kar != 55)
	{
	DelKar();
	viv_kar = 55;
	}
pred_kar = 0;
}

void DelError(void)
{
	unsigned int i,j;
	ili9341_setaddress(0,110,239,260);
	
	for(i=0;i<21120;i++)
	{
		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=RED>>8;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);

		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=RED;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);		
	}

	

	ili9341_pink_double_line();

	for(i=0;i<13200;i++)
	{
		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=RED>>8;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);

		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=RED;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);		
	}	
} 

void PrintError(unsigned char *cas) 
{
	ili9341_fillrect(0, y_Err - 10, 240, 10, RED);

	unsigned char line;
	ili9341_setaddress(0, y_Err, 239, y_Err+119);
	for (int8_t i=0; i<24; i++ ) 
	{
		for( int jjj=0; jjj<5; jjj++)
		for( int jj=0; jj<6; jj++)
		{
			if(jj>0 && jj <5 )
			{
				line = pgm_read_byte(cas+(i*4)+jj-1);
			}
			else
			{
				line = 0x00;
			}
			for (int j = 0; j<8; j++) 
			{
				if (line & 0b10000000) 
				{
					ili9341_pushcolour(PINK);
					ili9341_pushcolour(PINK);
					ili9341_pushcolour(PINK);	
					ili9341_pushcolour(PINK);	
					ili9341_pushcolour(PINK);			 
				} 
				else
				{
					ili9341_pushcolour(RED);
					ili9341_pushcolour(RED);
					ili9341_pushcolour(RED);
					ili9341_pushcolour(RED);
					ili9341_pushcolour(RED);			
				}
				line <<= 1;
			}
		}		
	}

	ili9341_fillrect(0, y_Err + 120, 240, 10, RED);
}

void DelKar(void) 
{
	ili9341_setaddress(x_Kar, y_Kar, x_Kar+127, y_Kar+95);
	for (int8_t i=0; i<24; i++ ) 
	{
		for( int jjj=0; jjj<4; jjj++)
		for( int jj=0; jj<4; jj++)
		{
			for (int j = 0; j<8; j++) 
			{
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);			
				
			}
		}		
	}
}

void PrintKar(unsigned char *cas) 
{
	unsigned char line;
	ili9341_setaddress(x_Kar, y_Kar, x_Kar+127, y_Kar+95);
	for (int8_t i=0; i<24; i++ ) 
	{
		for( int jjj=0; jjj<4; jjj++)
		for( int jj=0; jj<4; jj++)
		{
			line = pgm_read_byte(cas+(i*4)+jj);
			for (int j = 0; j<8; j++) 
			{
				if (line & 0b10000000) 
				{
					ili9341_pushcolour(YELLOW);
					ili9341_pushcolour(YELLOW);
					ili9341_pushcolour(YELLOW);	
					ili9341_pushcolour(YELLOW);				 
				} 
				else
				{
					ili9341_pushcolour(RED);
					ili9341_pushcolour(RED);
					ili9341_pushcolour(RED);
					ili9341_pushcolour(RED);			
				}
				line <<= 1;
			}
		}		
	}
}

Printl100km(int x, int y)
{
	ili9341_fillrect(x, y, 110, 50, RED);
	if(modeLcd == 0) {
		drawchar_21(x+39, y, 10, PINK);
		drawchar_21(x+54, y, 11, PINK);	
		drawchar_21(x+20, y+22, 1, PINK);
		drawchar_21(x+11, y+22, 13, PINK);
		drawchar_21(x, y+22, 12, PINK);	
		drawchar_21(x+37, y+22, 0, PINK);
		drawchar_21(x+54, y+22, 0, PINK);
		drawchar_21(x+71, y+22, 14, PINK);
		drawchar_21(x+88, y+22, 15, PINK);
	}
	x+=20;
	if(modeLcd == 1) {
		drawchar_21(x, y, 18, PINK);
		drawchar_21(x+17, y, 19, PINK);	
		drawchar_21(x+34, y, 20, PINK);
		drawchar_21(x+37, y+22, 15, PINK);	
		drawchar_21(x+54, y+22, 21, PINK);
	}
	if(modeLcd == 2) {
		drawchar_21(x+20, y+22, 22, PINK);
		drawchar_21(x+37, y+22, 33, PINK);
		drawchar_21(x+54, y+22, 23, PINK);
	}
	if(modeLcd == 3) {
		drawchar_21(x+20, y+22, 24, PINK);
		drawchar_21(x+37, y+22, 33, PINK);
		drawchar_21(x+54, y+22, 25, PINK);
	}
	if(modeLcd == 4) {
		drawchar_21(x+20, y+22, 25, PINK);
		drawchar_21(x+37, y+22, 26, PINK);
		drawchar_21(x+54, y+22, 26, PINK);
	}
	if(modeLcd == 5) {
		drawchar_21(x+20, y+22, 25, PINK);
		drawchar_21(x+37, y+22, 27, PINK);
		drawchar_21(x+54, y+22, 15, PINK);
	}
	if(modeLcd == 6) {
		drawchar_21(x, y, 28, PINK);
		drawchar_21(x+17, y, 29, PINK);	
		drawchar_21(x+34, y, 25, PINK);
		drawchar_21(x+37, y+22, 27, PINK);	
		drawchar_21(x+54, y+22, 33, PINK);
	}
	if(modeLcd == 7) {
		drawchar_21(x+20, y+22, 28, PINK);
		drawchar_21(x+37, y+22, 13, PINK);
		drawchar_21(x+54, y+22, 31, PINK);
	}
	if(modeLcd == 8) {
		drawchar_21(x+20, y+22, 28, PINK);

		ili9341_fillrect_3(x+54, y+22, 2, 2, PINK);
	
		ili9341_fillrect_3(x+61, y+25, 1, 5, PINK);
		ili9341_fillrect_3(x+64, y+22, 3, 1, PINK);
		ili9341_fillrect_3(x+64, y+40, 3, 1, PINK);
		ili9341_drawpixel_3(x+73,y+25,PINK);
		ili9341_drawpixel_3(x+73,y+37,PINK);
	}
	if(modeLcd == 9) {
		drawchar_21(x+20, y+22, 32, PINK);
		drawchar_21(x+37, y+22, 31, PINK);
		drawchar_21(x+54, y+22, 21, PINK);
	}
}

/*PrintRas(int x, int y)
{
	if(r_a==0)
	{
		drawchar_27(x, y, 13, PINK);
		drawchar_27(x+21, y, r_b, PINK);
		ili9341_fillrect_3(x+42, y+21, 2, 2, PINK);
		drawchar_27(x+51, y, r_c, PINK);
		return;
	}
	if(r_a==1 || r_a==2 || r_a==3 || r_a==4)
	{
		drawchar_27(x, y, r_a, PINK);
		drawchar_27(x+21, y, r_b, PINK);
		ili9341_fillrect_3(x+42, y+21, 2, 2, PINK);
		drawchar_27(x+51, y, r_c, PINK);
		return;
	}
	if(r_a==5)
	{
		drawchar_27(x, y, 12, PINK);
		drawchar_27(x+21, y, 12, PINK);
		ili9341_fillrect_3(x+42, y+21, 2, 2, PINK);
		drawchar_27(x+51, y, 12, PINK);
		return;
	}
	

}*/

PrintJan(char a, char b, char c, char d)
{	
	    int x =10;
	    int y = 137;

		drawchar_27(x, y, a, PINK);
		drawchar_27(x+21, y, b, PINK);
		drawchar_27(x+42, y, c, PINK);
		drawchar_27(x+63, y, d, PINK);

}

PrintTemp(int x, int y)
{
	if(t_r==0)
	{
		sneg_set(x, y, PINK);
		ili9341_fillrect_3(x+50, y, 6, 4, RED);
		ili9341_fillrect(x+50, y+12, 17, 3, PINK);
		ili9341_fillrect_3(x+50, y+15, 6, 4, RED);
		
	}
	if(t_r==1)
	{
		sneg_set(x, y, PINK);
		ili9341_fillrect(x+57, y+5, 3, 17, PINK);
		ili9341_fillrect(x+50, y+12, 17, 3, PINK);
	}
	if(t_r==2)
	{
		sneg_get(x, y);
		ili9341_fillrect(x+57, y+5, 3, 17, PINK);
		ili9341_fillrect(x+50, y+12, 17, 3, PINK);
	}
	
	if(t_r==3)
	{
		sneg_get(x, y);
		ili9341_fillrect_3(x+50, y, 6, 4, RED);
		ili9341_fillrect(x+50, y+12, 17, 3, RED);
		ili9341_fillrect_3(x+50, y+15, 6, 4, RED);

		drawchar_27(x+70, y, 12, PINK);
		drawchar_27(x+91, y, 12, PINK);
		ili9341_fillrect_3(x+115, y+21, 2, 2, PINK);
		drawchar_27(x+127, y, 12, PINK);
		drawchar_27(x+148, y, 10, PINK);
		drawchar_27(x+169, y, 11, PINK);
		return;
	}	
	
	if(t_a == 0)
	{
		drawchar_27(x+70, y, 13, PINK);
	}
	else
	{
		drawchar_27(x+70, y, t_a, PINK);
	}
	drawchar_27(x+91, y, t_b, PINK);
	ili9341_fillrect_3(x+115, y+21, 2, 2, PINK);
	drawchar_27(x+127, y, t_c, PINK);
	drawchar_27(x+148, y, 10, PINK);
	drawchar_27(x+169, y, 11, PINK);
	
		
}


void sneg_get(int x, int y)
{
	ili9341_fillrect(x, y, 27, 27, RED);
}

void sneg_set(int x, int y,uint16_t colour) 
{
	unsigned char line;

	
	
	ili9341_setaddress(x, y, x+12, y+12);
	for (int8_t i=0; i<13; i++ ) 
	{
		line = pgm_read_byte(sneg+(i*2)+1);
		for (int j = 0; j<5; j++) 
		{
			if (line & 0b00001000) 
			{
				ili9341_pushcolour(colour);								 
			} 
			else
			{
				ili9341_pushcolour(RED);			
			}
			line >>= 1;
		}
		line = pgm_read_byte(sneg+(i*2));
		for (int j = 0; j<8; j++) 
		{
			if (line & 0b10000001) 
			{
				ili9341_pushcolour(colour);							 
			} 
			else
			{
				ili9341_pushcolour(RED);							
			}
			line >>= 1;
		}				
	}

	ili9341_setaddress(x+14, y, x+26, y+12);
	for (int8_t i=0; i<13; i++ ) 
	{
		line = pgm_read_byte(sneg+(i*2));
		for (int j = 0; j<8; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);							 
			} 
			else
			{
				ili9341_pushcolour(RED);							
			}
			line <<= 1;
		}
		line = pgm_read_byte(sneg+(i*2)+1);
		for (int j = 0; j<5; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);								 
			} 
			else
			{
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}		
	}

	ili9341_setaddress(x, y + 14, x+12, y+26);
	for (int8_t i=12; i>=0; i-- ) 
	{
		line = pgm_read_byte(sneg+(i*2)+1);
		for (int j = 0; j<5; j++) 
		{
			if (line & 0b00001000) 
			{
				ili9341_pushcolour(colour);								 
			} 
			else
			{
				ili9341_pushcolour(RED);			
			}
			line >>= 1;
		}
		line = pgm_read_byte(sneg+(i*2));
		for (int j = 0; j<8; j++) 
		{
			if (line & 0b10000001) 
			{
				ili9341_pushcolour(colour);							 
			} 
			else
			{
				ili9341_pushcolour(RED);							
			}
			line >>= 1;
		}				
	}

	ili9341_setaddress(x+14, y+14, x+26, y+26);
	for (int8_t i=12; i>=0; i-- ) 
	{
		line = pgm_read_byte(sneg+(i*2));
		for (int j = 0; j<8; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);							 
			} 
			else
			{
				ili9341_pushcolour(RED);							
			}
			line <<= 1;
		}
		line = pgm_read_byte(sneg+(i*2)+1);
		for (int j = 0; j<5; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);								 
			} 
			else
			{
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}		
	}

	ili9341_drawpixel(x+5,y+1,RED);
	ili9341_drawpixel(x+5,y+2,RED);
	ili9341_drawpixel(x+5,y+3,RED);
	ili9341_drawpixel(x+5,y+25,RED);
	ili9341_drawpixel(x+5,y+24,RED);
	ili9341_drawpixel(x+5,y+23,RED);

	ili9341_fillrect(x+13, y, 1, 27, PINK);
	ili9341_fillrect(x+10, y+13, 7, 1, PINK);
}


void drawchar_27(int x, int y, unsigned char c,uint16_t colour) 
{
	unsigned char line;
	ili9341_setaddress(x, y, x+17, y+26);
	for (int8_t i=0; i<9; i++ ) 
	{
		line = pgm_read_byte(c_27+(c*9)+i);
		for (int j = 0; j<6; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);				 
			} 
			else
			{
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}
		line = pgm_read_byte(c_27+(c*9)+i);
		for (int j = 0; j<6; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);				 
			} 
			else
			{
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}
		line = pgm_read_byte(c_27+(c*9)+i);
		for (int j = 0; j<6; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);				 
			} 
			else
			{
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}
	}
}

PrintOil(int x, int y, int tempOil)
{
	//переменные температуры масла, три цифры. Первая цифра 0 не выводится, 1 как 1, 2 знак -, 3 все прочерки, 4 знак <
	unsigned char tm_a, tm_b, tm_c, vrem;
	if(tempOil<40) 
	{
		tm_a = 4;
		tm_b = 4;
		tm_c = 0;
	}
	else
	if(tempOil>39 && tempOil < 171)
	{
		tm_a = tempOil/100;
		vrem = tempOil;
		tm_b = (vrem%100)/10;
		tm_c = vrem%10;
	}
	else tm_a = 3;

	if(tm_a==0)
	{
		drawchar_21(x, y, 17, PINK);
		drawchar_21(x+18, y, tm_b, PINK);
		drawchar_21(x+36, y, tm_c, PINK);
	}
	if(tm_a==1)
	{
		drawchar_21(x, y, tm_a, PINK);
		drawchar_21(x+18, y, tm_b, PINK);
		drawchar_21(x+36, y, tm_c, PINK);
	}
	if(tm_a==2)
	{
		drawchar_21(x, y, 16, PINK);
		drawchar_21(x+18, y, tm_b, PINK);
		drawchar_21(x+36, y, tm_c, PINK);
	}
	if(tm_a==3)
	{
		drawchar_21(x, y, 16, PINK);
		drawchar_21(x+18, y, 16, PINK);
		drawchar_21(x+36, y, 16, PINK);
	}
	if(tm_a==4)
	{
		ili9341_fillrect(x, y, 15, 21, RED);
		ili9341_drawpixel_3(x+12,y,PINK);
		ili9341_drawpixel_3(x+9,y+3,PINK);
		ili9341_drawpixel_3(x+6,y+6,PINK);
		ili9341_drawpixel_3(x+3,y+9,PINK);
		ili9341_drawpixel_3(x+6,y+12,PINK);
		ili9341_drawpixel_3(x+9,y+15,PINK);
		ili9341_drawpixel_3(x+12,y+18,PINK);
		

		drawchar_21(x+18, y, tm_b, PINK);
		drawchar_21(x+36, y, tm_c, PINK);
	}
	
	ili9341_fillrect_3(x+54, y, 2, 2, PINK);
	
	ili9341_fillrect_3(x+61, y+3, 1, 5, PINK);
	ili9341_fillrect_3(x+64, y, 3, 1, PINK);
	ili9341_fillrect_3(x+64, y+18, 3, 1, PINK);
	ili9341_drawpixel_3(x+73,y+3,PINK);
	ili9341_drawpixel_3(x+73,y+15,PINK);
		
}

//если <10, то выводятся прочерки 
PrintVolt(int x, int y, unsigned char voltage)
{
	//перменные напряжения, две цифры до запятой и одна после. Если цифры до запятой 0 и 0, то выводятся прочерки
	unsigned char v_a=0, v_b=0, v_c=3;
	v_a=voltage/100;
	v_b=(voltage%100)/10;
	v_c=voltage%10;
	if(v_a==0 && v_b == 0)
	{
		drawchar_21(x, y, 16, PINK);
		drawchar_21(x+18, y, 16, PINK);
		ili9341_fillrect_3(x+36, y+15, 2, 2, PINK);
		drawchar_21(x+45, y, 16, PINK);
	}
	else
	{
		if(v_a == 0)
		{
			drawchar_21(x, y, 17, PINK);
		}
		else
		{
			drawchar_21(x, y, v_a, PINK);
		}
		drawchar_21(x+18, y, v_b, PINK);
		ili9341_fillrect_3(x+36, y+15, 2, 2, PINK);
		drawchar_21(x+45, y, v_c, PINK);	
	}

	ili9341_fillrect_3(x+63, y, 1, 3, PINK);
	ili9341_fillrect_3(x+66, y+9, 1, 3, PINK);
	ili9341_drawpixel_3(x+69,y+18,PINK);
	ili9341_fillrect_3(x+72, y+9, 1, 3, PINK);
	ili9341_fillrect_3(x+75, y, 1, 3, PINK);
}

void drawchar_21(int x, int y, unsigned char c,uint16_t colour) 
{
	unsigned char line;
	ili9341_setaddress(x, y, x+14, y+20);
	for (int8_t i=0; i<7; i++ ) 
	{
		line = pgm_read_byte(c_21+(c*7)+i);
		for (int j = 0; j<5; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);				 
			} 
			else
			{
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}
		line = pgm_read_byte(c_21+(c*7)+i);
		for (int j = 0; j<5; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);				 
			} 
			else
			{
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}
		line = pgm_read_byte(c_21+(c*7)+i);
		for (int j = 0; j<5; j++) 
		{
			if (line & 0b10000000) 
			{
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);
				ili9341_pushcolour(colour);				 
			} 
			else
			{
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);
				ili9341_pushcolour(RED);			
			}
			line <<= 1;
		}
	}
}

PrintAkk(int x, int y)
{
	//корпус
	ili9341_fillrect(x, y + 2, 28, 2, PINK);
	ili9341_fillrect(x, y+17, 28, 2, PINK);
	ili9341_fillrect(x, y + 4, 2, 13, PINK);
	ili9341_fillrect(x + 26, y + 4, 2, 13, PINK);
	//клеммы
	ili9341_fillrect(x+4, y, 4, 2, PINK);
	ili9341_fillrect(x+20, y, 4, 2, PINK);
	//минус
	ili9341_fillrect(x+3, y+7, 6, 2, PINK);
	//плюс
	ili9341_fillrect(x+19, y+7, 6, 2, PINK);
	ili9341_fillrect(x+21, y+5, 2, 6, PINK);
}

PrintMasl(int x, int y)
{
	//ручка
	ili9341_fillrect(x + 1, y, 3, 2, PINK);
	ili9341_fillrect(x, y + 2, 2, 3, PINK);
	ili9341_fillrect(x + 2, y + 4, 2, 2, PINK);
	ili9341_drawpixel(x + 4,y + 1,PINK);
	ili9341_drawpixel(x + 4,y + 2,PINK);
	ili9341_drawpixel(x + 5,y + 2,PINK);
	//корпус
	ili9341_fillrect(x + 5, y+3, 9, 2, PINK);
	ili9341_fillrect(x + 4, y+4, 2, 8, PINK);
	ili9341_fillrect(x + 6, y+10, 11, 2, PINK);

	ili9341_drawpixel(x + 17,y + 10,PINK);
	ili9341_drawpixel(x + 17,y + 9,PINK);
	ili9341_drawpixel(x + 18,y + 9,PINK);
	ili9341_drawpixel(x + 18,y + 8,PINK);
	ili9341_drawpixel(x + 19,y + 8,PINK);
	ili9341_drawpixel(x + 19,y + 7,PINK);
	ili9341_drawpixel(x + 20,y + 7,PINK);
	ili9341_drawpixel(x + 20,y + 6,PINK);
	ili9341_drawpixel(x + 21,y + 6,PINK);
	ili9341_drawpixel(x + 21,y + 5,PINK);
	ili9341_drawpixel(x + 22,y + 5,PINK);
	ili9341_drawpixel(x + 22,y + 4,PINK);
	ili9341_drawpixel(x + 23,y + 4,PINK);
	ili9341_drawpixel(x + 23,y + 3,PINK);
	ili9341_drawpixel(x + 24,y + 3,PINK);
	ili9341_drawpixel(x + 24,y + 2,PINK);
	ili9341_drawpixel(x + 25,y + 2,PINK);
	ili9341_drawpixel(x + 25,y + 3,PINK);
	ili9341_drawpixel(x + 26,y + 3,PINK);
	ili9341_drawpixel(x + 26,y + 4,PINK);

	ili9341_fillrect(x + 14, y+4, 1, 2, PINK);
	ili9341_fillrect(x + 15, y+5, 3, 2, PINK);
	ili9341_fillrect(x + 18, y+4, 4, 2, PINK);
	//крышка
	ili9341_fillrect(x + 8, y, 5, 2, PINK);
	ili9341_drawpixel(x + 10,y + 2,PINK);
	//жижа
	ili9341_drawpixel(x + 26,y + 7,PINK);
	ili9341_drawpixel(x + 26,y + 8,PINK);
	ili9341_drawpixel(x + 25,y + 8,PINK);

	ili9341_drawpixel(x + 26,y + 12,PINK);
	ili9341_fillrect(x + 24, y+13, 5, 1, PINK);
}




