#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#include "lcd_hight.h"
#include "prototype_task.h"
#include "tempure_sensor.h"


float adcToTempure (float adcTempure, float adcVoltage)
{
	float result;
	adcVoltage = adcVoltage / 159;
	adcTempure *= 0.957;
	if(adcTempure<122 && adcTempure > 75)
	{
		adcTempure=(844-(4*adcTempure))/9;
		result= adcTempure*adcVoltage;
	}
	else
	if(adcTempure<77 && adcTempure > 48)
	{
		adcTempure=(3140-20*adcTempure)/27;
		result= adcTempure*adcVoltage;
	}
	else
	if(adcTempure<50 && adcTempure > 23)
	{
		adcTempure=(792-8*adcTempure)/5;
		result= adcTempure*adcVoltage;
	}
	else
	if(adcTempure<25 && adcTempure > 11)
	{
		adcTempure=(1320-25*adcTempure)/6;
		result= adcTempure*adcVoltage;
	}
	else
	if(adcTempure<12)
	{
		result= 200;
	}
	else
	if(adcTempure > 121)
	{
		result= 39;
	}
	return result;
}


void Temp_oila(void)
{
	float adcTempure = ADC_b(7);
	float adcVoltage = ADC_b(6);
	float res1 = adcToTempure(adcTempure, adcVoltage);
	_delay_us(80);
	adcTempure = ADC_b(7);
	adcVoltage = ADC_b(6);
	float res2 = adcToTempure(adcTempure, adcVoltage);
	_delay_us(80);
	adcTempure = ADC_b(7);
	adcVoltage = ADC_b(6);
	float res3 = adcToTempure(adcTempure, adcVoltage);

    float res = (res1 + res2 + res3)/3;
    res=res-5;
	int resint = res;

	PrintOil(162, 283, resint);
	AddTask (Temp_oila, 1000);

}
