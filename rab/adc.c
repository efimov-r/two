#include <avr/io.h>
#include "adc.h"
#include <avr/interrupt.h>

void ADC_Init(void)
{
	 ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //делитель 128, вкл
  ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<ADLAR);  //опорное 2,56 встроенное

}

unsigned char ADC_b(char mux)
{
	unsigned char res;
	cli();
	switch(mux)
	{
		case 0: ADMUX = 0b11100000; break;
		case 1: ADMUX = 0b11100001; break;
		case 2: ADMUX = 0b11100010; break;
		case 3: ADMUX = 0b11100011; break;
		case 4: ADMUX = 0b11101110; break;
		case 5: ADMUX = 0b11100101; break;
		case 6: ADMUX = 0b11100110; break;
		case 7: ADMUX = 0b11100111; break;
	}
	
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC)));
	res = ADCH;
	sei();
	return res;

}
