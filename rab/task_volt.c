#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#include "lcd_hight.h"
#include "prototype_task.h"
#include "tempure_sensor.h"

extern unsigned char voltage;

unsigned char countError = 0; //счетчик низкого напряжения

char error = 0;

void offPictureVoltage(void)
{
	lowVoltagePictureOff();
}

//отображение напряжения
void Volt(void)
{
	unsigned int res1, res2, res3;
	unsigned char res;
	res1 =  ADC_b(6);
	_delay_us(80);
	res2 =  ADC_b(6);
	_delay_us(80);
	res3 =  ADC_b(6);; 

	res1 = (res1 + res2 + res3)/3;
	res1 *= 0.758;
	res = res1;

	PrintVolt(30, 283, res);
	voltage = res1;

	if(ignitionOn()) //если зажигание вкл
	{
		if(voltage < 124) //сильная просадка при работающем двигателе
		{
			lowVoltagePictureOn();			
		}
		if(voltage < 128) //длительная просадка
		{
			countError++;
			if(countError >= 3)
			{
				countError=0;
				lowVoltagePictureOn();				
			}
		}
		else
		{
			countError=0;
		}
		if(voltage > 131)
		{
			lowVoltagePictureOff();
		}
	}
	else
	{
		if(voltage < 119) //длительная просадка
		{
			countError++;
			if(countError >= 6)
			{
				countError=0;
				lowVoltagePictureOn();
			}
		}
		else
		{
			countError=0;
		}
		if(voltage > 121)
		{
			lowVoltagePictureOff();		
		}
	}
	 
	AddTask (Volt, 50);
}
