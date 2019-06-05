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

int main(void)
{	
	
	DDRD = 0xff;
	DDRC = 0xff;
	
	enable_INT1_interrupt();
	short keypressed;
	short correctkey;
	int num;
	Lcd4_Init();
	
	
	
	_delay_ms(1);
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Enter a Number");
	_delay_ms(1);
	Lcd4_Set_Cursor(2,1);
	DDRB = 0xF0; //setting the columns as inputs and the rows as outputs
	_delay_ms(1);
	PORTB = 0x0F;
	_delay_ms(1);
	
	while (1){
    /* Replace with your application code */
		
		
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
				case 0xff11: {
					Lcd4_Write_String("1");
					num = 1;
					break;
					}
					
				case 0xff21: {
					Lcd4_Write_String("2");
					num = 2;
					break;
				}
				case 0xff41: {
					Lcd4_Write_String("3");
					num = 3;
					break;
				}
				case 0xff81: {
					Lcd4_Write_String("A");
					num = 10;
					break;
				}
				case 0xff12: {
					Lcd4_Write_String("4");
					num = 4;
					break;
				}
				case 0xff14: {
					Lcd4_Write_String("7");
					num = 7;
					break;
				}
				case 0xff0F: {
					Lcd4_Write_String("fail");
					break;
				}
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
		_delay_ms(500);
		//Lcd4_Clear();
		//Lcd4_Set_Cursor(1,1);
		//Lcd4_Write_String("Enter a Number");
		//_delay_ms(1);
		//Lcd4_Set_Cursor(2,1);
			
		}
	
	}
}

