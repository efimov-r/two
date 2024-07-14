#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#include "lcd_hight.h"
#include "prototype_task.h"
#include "tempure_sensor.h"

#define BUS   OWI_PIN_1
#define DS18B20_SKIP_ROM                   0xcc
#define DS18B20_CONVERT_T                0x44
#define DS18B20_READ_SCRATCHPAD     0xbe

unsigned char scratchpad[9];
unsigned int tmp = 0;
unsigned char temperature;
unsigned char fl_d_t=0; //флаг присутствия датчика температуры

//запрос на измерение внешней температуры
void Temp_out_a(void)
{
	if(!OWI_DetectPresence(BUS))
	{
		fl_d_t=0;
	}
	else
	{
		fl_d_t=1;
	}
    OWI_SkipRom(BUS);
    OWI_SendByte(DS18B20_CONVERT_T ,BUS);

    AddTask (Temp_out_b, 100);

}

//обработка ответа внешней температуры
void Temp_out_b(void)
{
	OWI_DetectPresence(BUS);
    OWI_SendByte(DS18B20_SKIP_ROM, BUS);
    OWI_SendByte(DS18B20_READ_SCRATCHPAD, BUS);

	for(unsigned char i = 0; i<9; i++){
      scratchpad[i] = OWI_ReceiveByte(BUS);
    }

	if ((scratchpad[1]&128) == 0)
	{ 
		if(fl_d_t==1)
		{  
      		t_r=2;
	  	}
		else
		{
			t_r=3;
		}
    }
    else
	{
		if(fl_d_t==1)
		{
		      t_r=0;
			  tmp = ((unsigned int)scratchpad[1]<<8)|scratchpad[0];
		      tmp = ~tmp + 1;
		      scratchpad[0] = tmp;
		      scratchpad[1] = tmp>>8;
	  	}
		else
		{
			t_r=3;
		}
    }
t_c = scratchpad[0] & 0b00001111;
temperature = (scratchpad[0]>>4)|((scratchpad[1]&7)<<4);
t_a = temperature/10;
t_b= temperature%10;
if(t_b<2 && t_r==2 && t_a == 0) t_r = 1;
t_c /= 1.66666666;

if(lowLevelAntifreezePicture==0 && lowBrakeFluidPicture == 0) PrintTemp(20, 220);



	AddTask (Temp_out_a, 700);
}
