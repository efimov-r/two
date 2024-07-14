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

//��������� ����������, ��� ����� �� ������� � ���� �����. ���� ����� �� ������� 0 � 0, �� ��������� ��������
//unsigned char v_a=0, v_b=0, v_c=3;
unsigned int voltage = 0;

//���������� ����������� �����, ��� �����. ������ ����� 0 �� ���������, 1 ��� 1, 2 ���� -, 3 ��� ��������, 4 ���� <
//unsigned char tm_a=3, tm_b=3, tm_c=3;
unsigned int tempureOil = 333;

//���������� ����������� �������. t_r �����, 0 ���� - �� ���������, 1 ���� + �� ���������, 2 ���� + ��� ��������, 3 ��� ������
unsigned char t_r=3, t_a=1, t_b=2, t_c=3;

//���������� ������� �������, ��� �����. ������ ����� 0 �� ���������, �� 1 �� 4, 5 ��� ��������
unsigned char r_a=5, r_b=3, r_c=3;

//������� ������� � ����
unsigned char gazoline = 0;  //0

unsigned long time;

//����� ��������
unsigned char f_NUOZ = 0, //��������
			  f_LF = 0,   //���� ���
			  f_LS = 0,   //����� ������
			  lowVoltagePicture = 0,    //�����
			  lowLevelAntifreezePicture = 0,  //��������
			  lowGasolinePicture = 0,    //������
			  lowBrakeFluidPicture = 0;    //�������
			  TERM = 0;    //������� ������



signed int coolendTemperature=0; //����������� ������ -���
signed int fuelAir=0; //������/�������  ��.�
signed int throttlePosition=0; //��������� �������� ���
signed int rpm=0; //������� ����
signed int positionRhh=0; //��� ���
signed int injectionTimeCorrection=0; //��������� ������� ������� �.��
signed int ignitionTiming=0; //��� -�� � ��������
signed int injectionPulseTime=0; //����� ������� ��.�
signed int fuelConsumption=0; //������� ������ ��.�





unsigned int mode=0; //����� �� ���������� � ������
unsigned char modeLcd = 0; //����� �� ����������� � ������, �� 0 �� 9



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
