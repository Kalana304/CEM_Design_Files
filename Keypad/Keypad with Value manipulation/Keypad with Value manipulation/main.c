/*
 * GccApplication1.c
 *
 * Created: 4/8/2019 7:32:50 PM
 * Author : Shechem
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#define A 10
#define B 11
#define C 12
#define Enter 13
#define Star 14
#define Hash 15


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lcd.h"

void enable_INT1_interrupt()
{
	MCUCR=(1<<ISC2);
	GICR=(1<<INT1);
	sei();
}

ISR(INT1_vect){
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Switch Pressed");
	_delay_ms(500);
	Lcd4_Clear();
}

int GetKeyPadInput()
{
	int keypressed; int correctkey; int num = -1;
	while (num==-1){
	if (PINB!=0x0F){
		
		_delay_ms(5);
		keypressed = PINB;
		//_delay_ms(1000);
		
		DDRB ^=0xFF;
		_delay_ms(1);
		PORTB ^= 0xFF;
		_delay_ms(1);
		
		//Lcd4_Clear();
		//Lcd4_Set_Cursor(1,1);
		//Lcd4_Write_String("Press again");
		//Lcd4_Set_Cursor(2,1);
		while (PINB==0xF0){}
		
		keypressed |= PINB;
		keypressed=~keypressed;
		
		switch (keypressed){
			case 0xff11: {num = 1;	break;}
			case 0xff21: {num = 2;	break;}
			case 0xff41: {num = 3;	break;}
			case 0xff12: {num = 4;	break;}
			case 0xff22: {num = 5;	break;}
			case 0xff42: {num = 6;	break;}
			case 0xff14: {num = 7;	break;}
			case 0xff24: {num = 8;	break;}
			case 0xff44: {num = 9;	break;}
			case 0xff28: {num = 0;	break;}
			case 0xff81: {num = A;	break;}
			case 0xff82: {num = B;	break;}
			case 0xff84: {num = C;	break;}
			case 0xff88: {num = Enter;	break;}
			case 0xff18: {num = Star;	break;}
			case 0xff48: {num = Hash;	break;}
			default: {
				char str[8];
				sprintf(str, "%x", correctkey);
				Lcd4_Write_String(str);
				num = 99;
				break;
			}
			
	}_delay_ms(1);
	
	DDRB ^=0xFF;
	_delay_ms(1);
	PORTB ^=0xFF;
	Lcd4_Write_Number(num);
	_delay_ms(500);

	}
			}

return num;
}

void Lcd4_EnterNumber()
{
	_delay_ms(1);
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Enter a Number");
	_delay_ms(1);
	Lcd4_Set_Cursor(2,1);
}

void Lcd4_Turn_On(){
	PORTC |= (1<<PC0);
	_delay_ms(50);
}

void Lcd4_Turn_Off(){
	_delay_ms(50);
	PORTC =0x00;
	_delay_ms(50);
}
int Get_Temperature()
{
	int factor = 1;
	int Temp = 0;
	int Input = -1;
	Lcd4_Turn_On();
	Lcd4_Clear();
	Lcd4_Set_Cursor(2,1);
	Lcd4_Write_String("Press Enter");
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Temp: ");
	while (Input!=13){
		
		Input = GetKeyPadInput();
		if (Input==Enter){break;}
		if (Input == -1||Input ==99||Input ==10||Input ==11||Input ==12||Input ==14||Input ==15){
			Lcd4_Write_String("Error - Restart");
			Lcd4_Clear();
			Lcd4_Set_Cursor(2,1);
			Lcd4_Write_String("Press Enter");
			Lcd4_Set_Cursor(1,1);
			Lcd4_Write_String("Temp: ");
			Temp = 0;
			factor = 1;
		}
		Temp *=factor;
		Temp +=Input;
		factor*=10;
		
		
	}
	return Temp;
}

void Lcd4_Write_Number(int num){
	char val[5];
	switch(num){
		case A: {break;}
		case B: {break;}
		case C: {break;}
		case Enter: {break;}
		case Star: {Lcd4_Write_String("*");	break;}
		case Hash: {Lcd4_Write_String("#");	break;}
		default: {
			sprintf(val,"%d", num);
			Lcd4_Write_String(val);
			}
	
}
		}

void Init_Kepyad()
{
	DDRB = 0xF0; //setting the columns as inputs and the rows as outputs
	_delay_ms(1);
	PORTB = 0x0F;
	_delay_ms(1);
}

int main(void)
{	
	
	DDRD = 0xff;
	DDRC = 0xff;
	
	int Temperature = -1;
	_delay_ms(20);
	Lcd4_Turn_On();
	_delay_ms(20);
	enable_INT1_interrupt();
	Lcd4_Init();
	Lcd4_EnterNumber();
	
	Init_Kepyad();
	
	GetKeyPadInput();
	
	Lcd4_Turn_Off();
	_delay_ms(20);
	Lcd4_Turn_On();
	_delay_ms(20);
	Lcd4_Init();
	
	
	while (1){
		Temperature = Get_Temperature();
		_delay_ms(10);
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String("Obtained Temp: ");
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_Number(Temperature);
		
		_delay_ms(20);
		
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String("Obtained TempX2: ");
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_Number(Temperature*2);
		_delay_ms(2000);
		/**Value = GetKeyPadInput();
		if (Value!=-1){
			char val[2];
			Lcd4_Set_Cursor(2,1);
			sprintf(val,"%d",Value);
			Lcd4_Write_String(val);
			Value = -1;
			_delay_ms(500);
			Lcd4_EnterNumber();**/
		}
	}

