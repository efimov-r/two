#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#include "lcd_hight.h"
#include "prototype_task.h"
#include "tempure_sensor.h"

#define offMode          0 //выкл
#define onMode          1 //вкл
#define connectNew       2 //подана команда на коннект
#define connect1True     3 //1 верный байт
#define connect2True     4
#define connectTrue      5 //все байты верны, есть коннект
#define input1True       6
#define input2True       7
#define input3True       8
#define inputTrue        9
#define inputBute        10 //пошли даные, номер бита + это

void TaskLongPress(void)
{
		/*r_a = 120/100;
		r_b = (120%100)/10;
		r_c = 120%10;
		PrintRas(10, 137);*/

}


void TestQuickPress(void)
{
	if(mode!=0) modeLcd++;
	if(modeLcd >=10) modeLcd = 0;
	if(!lowLevelAntifreezePicture && !lowBrakeFluidPicture)
	{
	Printl100km(130, 124);
	}

}

int dataVr;

void TaskPrintSred(void)
{
	//fuelConsumption = mode;//////////////////////////
	if(mode>= connectTrue && ignitionOn())
	{
		if(coolendTemperature<0) TERM = 1;
		else if(coolendTemperature<40 && rpm > 2400)
		{ 
			TERM = 1;
		}
		else if(coolendTemperature<60 && rpm > 3400)
		{ 
			TERM = 1;
		}
		else if(coolendTemperature<80 && rpm >4200)
		{ 
			TERM = 1;
		}
		else TERM = 0;

	}
	else
	{
		TERM = 0;
	}



	if(!lowLevelAntifreezePicture && !lowBrakeFluidPicture)
	{
		if(modeLcd == 0)
		{
			if(fuelConsumption > 99) {
			PrintJan((fuelConsumption%1000)/100,(fuelConsumption%100)/10,14,fuelConsumption%10);
			} else {
			PrintJan(13,(fuelConsumption%100)/10,14,fuelConsumption%10);
			}
		}
		if(modeLcd == 1)
		{
			if(injectionPulseTime > 99) {
			PrintJan((injectionPulseTime%1000)/100,(injectionPulseTime%100)/10, 14, injectionPulseTime%10);
			} else {
			PrintJan(13,(injectionPulseTime%100)/10, 14, injectionPulseTime%10);
			}

		}
		if(modeLcd == 2)
		{
			char b;
			if(ignitionTiming == 0) PrintJan(13, 13, 13, 0);
			dataVr = ignitionTiming;
			if(ignitionTiming<0)
			{
				dataVr *= -1;
				if(dataVr>=1 && dataVr <= 9) PrintJan(13, 13, 12, dataVr);
				if(dataVr>=10 && dataVr <= 99) PrintJan(13, 12, (dataVr%100)/10, dataVr%10);
			}
			else
			{
				if(dataVr>=1 && dataVr <= 9) PrintJan(13, 13, 13, dataVr);
				if(dataVr>=10 && dataVr <= 99) PrintJan(13, 13, (dataVr%100)/10, dataVr%10);
			}
		}
		if(modeLcd == 3)
		{
			PrintJan((injectionTimeCorrection%1000)/100, 14,(injectionTimeCorrection%100)/10, injectionTimeCorrection%10);
		}
		if(modeLcd == 4)
		{
			if(positionRhh > 99) {
			PrintJan(13, (positionRhh%1000)/100,(positionRhh%100)/10, positionRhh%10);
			}
			else if(positionRhh >9) {
			PrintJan(13, 13,(positionRhh%100)/10, positionRhh%10); 
			}
			else {
			PrintJan(13, 13,13, positionRhh%10); 
			}
		}
		if(modeLcd == 5)
		{
			if(rpm > 999) {
			PrintJan((rpm%10000)/1000, (rpm%1000)/100,(rpm%100)/10, rpm%10);
			} else if(rpm > 99) {
			PrintJan(13, (rpm%1000)/100,(rpm%100)/10, rpm%10);
			} else {
			PrintJan(13, 13,(rpm%100)/10, rpm%10);
			}

		}
		if(modeLcd == 6)
		{
			if(throttlePosition >99) {
			PrintJan(13, (throttlePosition%1000)/100,(throttlePosition%100)/10, throttlePosition%10);
			} else if(throttlePosition > 9) {
			PrintJan(13, 13,(throttlePosition%100)/10, throttlePosition%10);
			} else {
			PrintJan(13, 13,13, throttlePosition%10);
			}
		}
		if(modeLcd == 7)
		{
			if(fuelAir > 99) {
			PrintJan((fuelAir%1000)/100,(fuelAir%100)/10,14,fuelAir%10);
			} else {
			PrintJan(13,(fuelAir%100)/10,14,fuelAir%10);
			}
		}
		if(modeLcd == 8)
		{
			char a;
			if(coolendTemperature == 0) PrintJan(13, 13, 13, 0);
			dataVr = coolendTemperature;
			if(dataVr<0)
			{
				dataVr *= -1;
				if(dataVr>=1 && dataVr <= 9) PrintJan(13, 13, 12, dataVr);
				if(dataVr>=10 && dataVr <= 99) PrintJan(13, 12, (dataVr%100)/10, dataVr%10);
				if(dataVr>=100) PrintJan(12, (dataVr%1000)/100,(dataVr%100)/10, dataVr%10);
			}
			else
			{
				if(dataVr>=1 && dataVr <= 9) PrintJan(13, 13, 13, dataVr);
				if(dataVr>=10 && dataVr <= 99) PrintJan(13, 13, (dataVr%100)/10, dataVr%10);
				if(dataVr>=100) PrintJan(13, (dataVr%1000)/100,(dataVr%100)/10, dataVr%10);
			}
		}
		if(modeLcd == 9)
		{
			if(gazoline > 99) {
			PrintJan(13, (gazoline%1000)/100,(gazoline%100)/10, gazoline%10);
			} else {
			PrintJan(13, 13,(gazoline%100)/10, gazoline%10); }
		}
	}
	AddTask (TaskPrintSred, 50);
}


unsigned char ignitionOld=3;

void TaskIgnition(void)
{
	if(ignitionOld==3)
	{
		if(ignitionOn())
		{
			ignitionOld = 1;
			if(mode != offMode)
			{
				mode = connectNew;
			}
		}
		else ignitionOld = 0;
	}
	if(ignitionOld==0 && ignitionOn())
	{
		ignitionOld=1;
		if(mode != offMode)
		{
			mode = connectNew;
		}
	}
	if(ignitionOld==1 && !ignitionOn())
	{
		ignitionOld=0;
		if(mode != offMode)
		{
			mode = onMode;
		}
	}
		
	AddTask (TaskIgnition, 50);
}

void TaskJanuaryConnect(void)
{

	unsigned char startCommunication[] = {0x81,0x10,0xf1,0x81,0x03};

	if(mode == connectNew)
	{
		SendCommand(startCommunication,5);		
	}	
	AddTask (TaskJanuaryConnect, 100);
}

void TaskJanuaryData(void)
{
	unsigned char readDataByLocalIdentifier_RLI_ASS[]  = {0x82,0x10,0xF1,0x21,0x01,0xA5};
	if(mode >= connectTrue)
	{
		SendCommand(readDataByLocalIdentifier_RLI_ASS,6);
		mode = connectTrue;
		
	}
	AddTask (TaskJanuaryData, 60);
}


void printKartinki(void)
{
	Kartinki();
	Error();
	AddTask (printKartinki, 100);	
}



void TaskError(void)
{
	if(!(PIND&0b00000100)) if(PIND&0b00010000) lowBrakeFluidPicture=1;
	if(!(PINB&0b01000000)) 
	{
		if(PIND&0b00010000) lowLevelAntifreezePicture=1; 
	}
	else 
	{
		lowLevelAntifreezePicture = 0;
	}
	if(!(PIND&0b00010000))
	{
		lowLevelAntifreezePicture = 0;
		lowBrakeFluidPicture=0;
	}	
	
	AddTask (TaskError, 100);
}
