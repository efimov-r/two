#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#include "lcd_hight.h"
#include "prototype_task.h"
#include "tempure_sensor.h"

unsigned char gazolineMass[50];
unsigned char pointer=0; //0 - начальное, 1-51 диапазон
unsigned char consider=0; //учитывать показания

void TaskGazoline(void)
{
	unsigned char benz; 
	if(PIND&0b00010000) //включено зажигание
	{
		benz = ADC_b(5) * 0.758;

		if(pointer == 0) //вначале весь массив как первое показание
		{
			for(int i =0; i<50; i++)
			{
				gazolineMass[i] = benz;
				pointer++;
			}
		}
		if(pointer > 48)  //на первом проходе начанаем учитывать
		{
			consider=1;
		}		
		gazolineMass[pointer-1]=benz;
		pointer++;
		if(pointer == 51)
		{
			pointer = 1;
		}
	}
	
	AddTask (TaskGazoline, 100);
}

void TaskGazolineWrite(void)
{
	unsigned long res = 0;
	for(int i =0; i<50; i++)
		{
			res += gazolineMass[i]; 
			
		}
	res/=50;
	gazoline = res;
	if(gazoline >=99)
	{
		lowGasolinePicture = 1;
	}
	else
	{
		lowGasolinePicture = 0;
	}

	AddTask (TaskGazolineWrite, 1000);
}
