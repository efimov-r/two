#ifndef MAIN_H
#define MAIN_H



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


#define F_CPU 16000000UL
#define StartFrom       0x63
#define MAXnTASKS       30

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<PE)
#define DATA_OVERRUN (1<<DOR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

//перменные напряжения, две цифры до запятой и одна после. Если цифры до запятой 0 и 0, то выводятся прочерки
//unsigned char v_a=0, v_b=0, v_c=3;
unsigned int voltage = 0;

//переменные температуры масла, три цифры. Первая цифра 0 не выводится, 1 как 1, 2 знак -, 3 все прочерки, 4 знак <
//unsigned char tm_a=3, tm_b=3, tm_c=3;
unsigned int tempureOil = 333;

//переменные температуры внешней. t_r режим, 0 знак - со снежинкой, 1 знак + со снежинкой, 2 знак + без снежинки, 3 нет данных
unsigned char t_r=3, t_a=1, t_b=2, t_c=3;

//переменные расхода топлива, три цифры. Первая цифра 0 не выводится, от 1 до 4, 5 все прочерки
unsigned char r_a=5, r_b=3, r_c=3;

//уровень топлива в баке
unsigned char gazoline = 0;  //0

unsigned long time;

//флаги картинок
unsigned char f_NUOZ = 0, //омывайка
			  f_LF = 0,   //ламы фар
			  f_LS = 0,   //лампы стопов
			  lowVoltagePicture = 0,    //акума
			  lowLevelAntifreezePicture = 0,  //антифриз
			  lowGasolinePicture = 0,    //бензин
			  lowBrakeFluidPicture = 0;    //тормоза
			  TERM = 0;    //холодно двиглу



signed int coolendTemperature=0; //температура двигла -ххх
signed int fuelAir=0; //воздух/топливо  хх.х
signed int throttlePosition=0; //положение дросселя ххх
signed int rpm=0; //обороты хххх
signed int positionRhh=0; //РХХ ххх
signed int injectionTimeCorrection=0; //коррекция времени впрыска х.хх
signed int ignitionTiming=0; //УОЗ -хх в знаковом
signed int injectionPulseTime=0; //время впрыска хх.х
signed int fuelConsumption=0; //путевой расход хх.х





unsigned int mode=0; //режим на считывания с января
unsigned char modeLcd = 0; //режим на отображение в центре, от 0 до 9



typedef struct task
{
   void (*pfunc) (void);
   unsigned int delay;
  unsigned char fl;
}task;

volatile task TaskArray[MAXnTASKS];




void InitScheduler (void);
unsigned char AddTask (void (*taskfunc)(void), unsigned int time);
void DispatchTask (void);


#endif MAIN_H
