#define F_CPU 16000000UL
#include "prototype_task.h"
#include "global_variable.h"
#include "adc.h"
#include "tempure_sensor.h"
#include <avr/io.h>
#include <avr/interrupt.h>


#define F_CPU 8000000UL

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

/// Main //////////////
int main(void)
{  
	initLCD();
	ADC_Init();
	OWI_Init(OWI_PIN_1);
	InitScheduler();
    
	AddTask (TaskError, 20);
	AddTask (Volt, 20);
	AddTask (Temp_out_a, 20);
	AddTask (Temp_oila, 20);
	AddTask (TaskGazoline, 30);
	AddTask (TaskGazolineWrite, 1000);
	AddTask (printKartinki, 100);   

	UBRRH=0x00;
	UBRRL=0x5F;
	UCSRA=0b00000000;
	UCSRB=0b10011000;
	UCSRC=0b10000110;

	if(PINA&0b00001000) //ненажата
	{
		mode = onMode;		
	}
	else
	{
		fuelConsumption = 333;
	}
	
	AddTask (TaskJanuaryConnect, 100);
	AddTask (TaskJanuaryData, 200);

	AddTask (TaskIgnition, 100);

	AddTask (TaskPrintSred, 10);

   sei();

   
        
   while (1) 
   {
      DispatchTask();
   }            
}

unsigned char data3; //для двухбайтовых данных
signed char data4=0;
float dataFloat; //вычисления

ISR (USART_RXC_vect)
{
	unsigned char status,data, data2,numByte;
	status=UCSRA;
	data=UDR;
	if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	{
		if(mode == connectNew && data == 0xF1) mode = connect1True;
		else if(mode == connect1True && data == 0x10) mode = connect2True;
		else if(mode == connect2True && data == 0xC1) mode = connectTrue;

		else if(mode == connectTrue && data == 0xF1) {mode = input1True;} 
		else if(mode == input1True && data == 0x10) {mode = input2True;}
		else if(mode == input2True && data == 0x26) {mode = input3True;}
		else if(mode == input3True && data == 0x61) {mode = inputBute+1;}
		else if(mode>inputBute)
		{
			data2 = data;
			mode++;
		}
		
        if(mode > 10) numByte = mode - 10; else numByte = 0;
		if(numByte > 35) {mode = connectTrue; }
		else if(numByte > 10)
		{
		

			if(numByte == 11)
			{
				coolendTemperature = data2-40;
			}
			if(numByte == 12)
			{
				dataFloat = data2 + 128;
				dataFloat *= 14.7;
				dataFloat /= 256; //истина 14.7
				fuelAir = dataFloat*10;
			}
			if(numByte == 13)
			{
				throttlePosition = data2;
			}
			if(numByte == 14)
			{
				rpm = data2*40;
			}
			if(numByte == 17)
			{
				positionRhh = data2;
			}
			if(numByte == 18)
			{
				dataFloat = data2;
				dataFloat += 128;
				dataFloat /= 256; //истина 
				injectionTimeCorrection = dataFloat*100;
			}
			if(numByte == 19)
			{
				data4 = 0;
				if(0 != (data2 & 0b10000000)) data4 |= 0b10000000;
				if(0 != (data2 & 0b01000000)) data4 |= 0b01000000;
				if(0 != (data2 & 0b00100000)) data4 |= 0b00100000;
				if(0 != (data2 & 0b00010000)) data4 |= 0b00010000;
				if(0 != (data2 & 0b00001000)) data4 |= 0b00001000;
				if(0 != (data2 & 0b00000100)) data4 |= 0b00000100;
				if(0 != (data2 & 0b00000010)) data4 |= 0b00000010;
				if(0 != (data2 & 0b00000001)) data4 |= 0b00000001;

				data4 /=2;  //истина

				ignitionTiming = data4;
			}
			if(numByte == 25)
			{
				data3 = data2;
			}
			if(numByte == 26)
			{
				dataFloat = (data2<<8) + data3;
				dataFloat /= 125; //истина
				injectionPulseTime = dataFloat * 10;
			}
			if(numByte == 33)
			{
				data3 = data2;
			}
			if(numByte == 34)
			{
				dataFloat = (data2<<8) + data3;
				dataFloat /= 128; //истина
				fuelConsumption = dataFloat * 10;
			}
			
		}
	}
}



void SendCommand (unsigned char *command, unsigned char length) {
  while (length--) {
    while(!(UCSRA & (1<<UDRE)));  
    UDR = *command++;
  }
}




void InitScheduler (void)
{
   unsigned int i, q;
   
   TCCR0 |= (1<<CS02)|(1<<CS00);
   TIFR = 1<<TOV0;
   TIMSK |= 1<<TOIE0;
   TCNT0 = StartFrom;

   for (i=0; i<MAXnTASKS; i++)
   {
      TaskArray[i].pfunc = 0;      
   }
}


unsigned char AddTask (void (*taskfunc)(void), unsigned int time)
{
   unsigned char n=0;

   for (n=0; n<MAXnTASKS; n++)
	   {
	      if(TaskArray[n].pfunc == 0)
		  {
			 TaskArray[n].pfunc = taskfunc;
			 TaskArray[n].fl = 0;
			 TaskArray[n].delay = time;
			 return 1;
	      }
	   }	   
   return 0;
}

unsigned int buttonTime=0;

ISR(TIMER0_OVF_vect)
{

   unsigned char m, n;

   time++;

   TCNT0 = StartFrom;
   
   for (m=0; m<MAXnTASKS; m++)
   {
      if (TaskArray[m].pfunc)
	      {   
	         if (TaskArray[m].fl == 0) 
	         {
	             if(TaskArray[m].delay > 0)
				 {
				     TaskArray[m].delay--;
					 if(TaskArray[m].delay==0)
					 {
                         TaskArray[m].fl=1;
					 }
				 }				 
	         }
	      }      
   }
   
	if(PINA&0b00001000) //ненажата
	{
		if(buttonTime>10)
		{
			if(buttonTime>200)
			{
				AddTask (TaskLongPress, 1);
			}
			else
			{
				AddTask (TestQuickPress, 1);
			}
			buttonTime=0;			
		}		
	}
	else //нажата
	{
		buttonTime++;		
	}

}



void DispatchTask (void)
{
	unsigned char m, n;	

	for (m=0; m<MAXnTASKS; m++)
	{
  
	      if (TaskArray[m].pfunc)
	      {   
	         if (TaskArray[m].fl == 1) 
	         {
	            if (TaskArray[m].delay == 0)
				{
	                 (*TaskArray[m].pfunc)();
					 TaskArray[m].fl=0;
					 TaskArray[m].pfunc=0;
					 return;
				}							 
	         }
	      }  
	}
}







