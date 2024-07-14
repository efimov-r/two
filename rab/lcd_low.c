#include "lcd_low.h"


//draw pixel
void ili9341_drawpixel(uint16_t x3,uint16_t y3,uint16_t colour1) //pixels will always be counted from right side.x is representing LCD width which will always be less tha 240.Y is representing LCD height which will always be less than 320
{

ili9341_setaddress(x3,y3,x3+1,y3+1);

ili9341_pushcolour(colour1);
}

void ili9341_drawpixel_3(uint16_t x3,uint16_t y3,uint16_t colour1) //pixels will always be counted from right side.x is representing LCD width which will always be less tha 240.Y is representing LCD height which will always be less than 320
{

ili9341_setaddress(x3,y3,x3+2,y3+2);

ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
ili9341_pushcolour(colour1);
}

//draw colour filled rectangle
void ili9341_fillrect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,uint16_t colour)
{
	ili9341_setaddress(x, y, x+w-1, y+h-1);	

	for(y=w*h; y>0; y--) 
	{
		ili9341_pushcolour(colour);		
	}
}

void ili9341_fillrect_3(unsigned int x,unsigned int y,unsigned int w,unsigned int h,uint16_t colour)
{
	ili9341_setaddress(x, y, x+(w*3)-1, y+(h*3)-1);	

	for(y=w*h*9; y>0; y--) 
	{
		ili9341_pushcolour(colour);		
	}
}

void ili9341_hard_init(void)//init hardware
{
rstddr|=(1<<rst);//output for reset
rstport |=(1<<rst);//pull high for normal operation
dcddr|=(1<<dc);//D/C as output
}


void ili9341_spi_init(void)//set spi speed and settings 
{
DDRB |=(1<<4)|(1<<5)|(1<<7);//CS,SS,MOSI,SCK as output(although SS will be unused throughout the program)
SPCR=(1<<SPE)|(1<<MSTR);//mode 0,fosc/4
SPCR&=~((1<<SPR1)|(1<<SPR0));//mode 0,fosc/4
SPSR |=(1<<SPI2X);//doubling spi speed.i.e final spi speed-fosc/2
csport |=(1<<cs);//cs off during startup
}


void ili9341_spi_send(unsigned char spi_data)//send spi data to display
{
SPDR=spi_data;//move data into spdr
while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
}


void ili9341_writecommand8(uint8_t com)//command write
{
csport &=~(1<<cs);//dc and cs both low to send command
dcport &=~(1<<dc);//dc and cs both low to send command
//_delay_us(5);//little delay
SPDR=com;//move data into spdr
while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
csport |=(1<<cs);//pull high cs
}


void ili9341_writedata8(uint8_t data)//data write
{
dcport |=(1<<dc);//st dc high for data
//_delay_us(1);//delay
csport &=~(1<<cs);//set cs low for operation
SPDR=data;//move data into spdr
while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
csport |=(1<<cs);
}




void ili9341_setaddress(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)//set coordinate for print or other function
{
ili9341_writecommand8(0x2A);
ili9341_writedata8(x1>>8);
ili9341_writedata8(x1);
ili9341_writedata8(x2>>8);
ili9341_writedata8(x2);

ili9341_writecommand8(0x2B);
ili9341_writedata8(y1>>8);
ili9341_writedata8(y1);
ili9341_writedata8(y2>>8);
ili9341_writedata8(y2);

ili9341_writecommand8(0x2C);//meory write
}

void ili9341_hard_reset(void)//hard reset display
{
rstport |=(1<<rst);//pull high if low previously
_delay_ms(200);
rstport &=~(1<<rst);//low for reset
_delay_ms(200);
rstport |=(1<<rst);//again pull high for normal operation
_delay_ms(200);
}


void ili9341_init(void)//set up display using predefined command sequence
{
ili9341_hard_init();
ili9341_spi_init();
ili9341_hard_reset();
ili9341_writecommand8(0x01);//soft reset
_delay_ms(1000);
//power control A
ili9341_writecommand8(0xCB);
ili9341_writedata8(0x39);
ili9341_writedata8(0x2C);
ili9341_writedata8(0x00);
ili9341_writedata8(0x34);
ili9341_writedata8(0x02);

//power control B
ili9341_writecommand8(0xCF);
ili9341_writedata8(0x00);
ili9341_writedata8(0xC1);
ili9341_writedata8(0x30);

//driver timing control A
ili9341_writecommand8(0xE8);
ili9341_writedata8(0x85);
ili9341_writedata8(0x00);
ili9341_writedata8(0x78);

//driver timing control B
ili9341_writecommand8(0xEA);
ili9341_writedata8(0x00);
ili9341_writedata8(0x00);

//power on sequence control
ili9341_writecommand8(0xED);
ili9341_writedata8(0x64);
ili9341_writedata8(0x03);
ili9341_writedata8(0x12);
ili9341_writedata8(0x81);

//pump ratio control
ili9341_writecommand8(0xF7);
ili9341_writedata8(0x20);

//power control,VRH[5:0]
ili9341_writecommand8(0xC0);
ili9341_writedata8(0x23);

//Power control,SAP[2:0];BT[3:0]
ili9341_writecommand8(0xC1);
ili9341_writedata8(0x10);

//vcm control
ili9341_writecommand8(0xC5);
ili9341_writedata8(0x3E);
ili9341_writedata8(0x28);

//vcm control 2
ili9341_writecommand8(0xC7);
ili9341_writedata8(0x86);

//memory access control
ili9341_writecommand8(0x36);
ili9341_writedata8(0x48);

//pixel format
ili9341_writecommand8(0x3A);
ili9341_writedata8(0x55);

//frameration control,normal mode full colours
ili9341_writecommand8(0xB1);
ili9341_writedata8(0x00);
ili9341_writedata8(0x18);

//display function control
ili9341_writecommand8(0xB6);
ili9341_writedata8(0x08);
ili9341_writedata8(0x82);
ili9341_writedata8(0x27);

//3gamma function disable
ili9341_writecommand8(0xF2);
ili9341_writedata8(0x00);

//gamma curve selected
ili9341_writecommand8(0x26);
ili9341_writedata8(0x01);

//set positive gamma correction
ili9341_writecommand8(0xE0);
ili9341_writedata8(0x0F);
ili9341_writedata8(0x31);
ili9341_writedata8(0x2B);
ili9341_writedata8(0x0C);
ili9341_writedata8(0x0E);
ili9341_writedata8(0x08);
ili9341_writedata8(0x4E);
ili9341_writedata8(0xF1);
ili9341_writedata8(0x37);
ili9341_writedata8(0x07);
ili9341_writedata8(0x10);
ili9341_writedata8(0x03);
ili9341_writedata8(0x0E);
ili9341_writedata8(0x09);
ili9341_writedata8(0x00);

//set negative gamma correction
ili9341_writecommand8(0xE1);
ili9341_writedata8(0x00);
ili9341_writedata8(0x0E);
ili9341_writedata8(0x14);
ili9341_writedata8(0x03);
ili9341_writedata8(0x11);
ili9341_writedata8(0x07);
ili9341_writedata8(0x31);
ili9341_writedata8(0xC1);
ili9341_writedata8(0x48);
ili9341_writedata8(0x08);
ili9341_writedata8(0x0F);
ili9341_writedata8(0x0C);
ili9341_writedata8(0x31);
ili9341_writedata8(0x36);
ili9341_writedata8(0x0F);

//exit sleep
ili9341_writecommand8(0x11);
_delay_ms(120);
//display on
ili9341_writecommand8(0x29);

}

//set colour for drawing
void ili9341_pushcolour(uint16_t colour)
{
dcport |=(1<<dc);//st dc high for data
//_delay_us(1);//delay
csport &=~(1<<cs);//set cs low for operation
SPDR=colour>>8;//move data into spdr
while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
csport |=(1<<cs);

dcport |=(1<<dc);//st dc high for data
//_delay_us(1);//delay
csport &=~(1<<cs);//set cs low for operation
SPDR=colour;//move data into spdr
while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
csport |=(1<<cs);
}


void ili9341_pink_double_line(void)
{
uint16_t j;
for(j=0;j<960;j++)
	{
		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=PINK>>8;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);

		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=PINK;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);
	}
}


//clear lcd and fill with colour
void ili9341_clear(uint16_t colour)
{
	uint16_t i,j;
	unsigned int c;
	ili9341_setaddress(0,0,239,319);

	
	c = (line_a-2)*240;
	for(i=0;i<c;i++)
	{
		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour>>8;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);

		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);		
	}

	ili9341_pink_double_line();

	c=((line_b-2)-(line_a+2))*240;

	for(i=0;i<c;i++)
	{
		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour>>8;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);

		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);		
	}

	ili9341_pink_double_line();

	c = ((line_c-2)-(line_b+2))*240;

	for(i=0;i<c;i++)
	{
		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour>>8;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);

		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);		
	}

	ili9341_pink_double_line();

	c=(320 - (line_c+2))*240;

	for(i=0;i<c;i++)
	{
		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour>>8;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);

		dcport |=(1<<dc);//st dc high for data
		//_delay_us(1);//delay
		csport &=~(1<<cs);//set cs low for operation
		SPDR=colour;//move data into spdr
		while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
		csport |=(1<<cs);		
	}
	ledport |=(1<<led);//pull high for normal operation
	ledddr|=(1<<led);//D/C as output
}





