#ifndef ILI9341_H
#define ILI9341_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <limits.h>
#include <inttypes.h>

#define dcport PORTB
#define dcddr DDRB
#define dcpin PINB
#define csport PORTB
#define csddr DDRB
#define cspin PINB
#define rstport PORTB
#define rstddr DDRB
#define rstpin PINB
#define ledport PORTB
#define ledddr DDRB
#define dc 0
#define cs 2
#define rst 1
#define led 3
#define ILI9341_TFTHEIGHT 240 
#define ILI9341_TFTWIDTH 320

//высота линий разделения
#define line_a 100
#define line_b 200
#define line_c 270




    
//#define RED         0x4000   
//#define RED         0x3020  //фон 
//#define RED         0x2000 
#define RED 0x0000 
#define YELLOW      0xFE60     
//#define PINK        0xEA62  //текст
//#define PINK ((((232>>3)<<11) | ((76>>2)<<5) | (16>>3)))
//#define PINK ((((218>>3)<<11) | ((31>>2)<<5) | (31>>3)))
//#define PINK ((((243>>3)<<11) | ((5>>2)<<5) | (5>>3)))
//#define PINK ((((255>>3)<<11) | ((0>>2)<<5) | (0>>3)))
#define PINK ((((185>>3)<<11) | ((0>>2)<<5) | (0>>3)))
//11101 = 232
//010011 = 76
//00010 = 16




void ili9341_hard_init(void);
void ili9341_spi_init(void);
void ili9341_spi_send(unsigned char spi_data);
void ili9341_writecommand8(uint8_t com);
void ili9341_writedata8(uint8_t data);
void ili9341_setaddress(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void ili9341_hard_reset(void);
void ili9341_init(void);
void ili9341_pushcolour(uint16_t colour);
void ili9341_pink_double_line(void);
void ili9341_clear(uint16_t colour);
void ili9341_drawpixel(uint16_t x3,uint16_t y3,uint16_t colour1);
void ili9341_drawpixel_3(uint16_t x3,uint16_t y3,uint16_t colour1);
void ili9341_fillrect(unsigned int x,unsigned int y, unsigned int w,unsigned int h,uint16_t colour);
void ili9341_fillrect_3(unsigned int x,unsigned int y,unsigned int w,unsigned int h,uint16_t colour);



#endif

