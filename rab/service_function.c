#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "lcd_hight.h"
//#include "prototype_task.h"
//#include "tempure_sensor.h"


unsigned char ignitionOn(void) 
{
	if(PIND&0b00010000) {
		return 1;
	} else {
		return 0;
	}
}

void lowVoltagePictureOn(void)
{
	lowVoltagePicture = 1;
}

void lowVoltagePictureOff(void)
{
	lowVoltagePicture = 0;
}
