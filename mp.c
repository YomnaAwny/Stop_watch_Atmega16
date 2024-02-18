/*
 * mp.c
 *
 *  Created on: Oct 24, 2023
*      Author: Yomna Awny Abd El Menam Salh
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define STOP PD2
#define START PB2 //INT2
#define RESET PD3 //INT1
#define SEC1 PA0
#define SEC2 PA1
#define MIN1 PA2
#define MIN2 PA3
#define HOUR1 PA4
#define HOUR2 PA5

char count=0;
char sec2=0;
char min1=0;
char min2=0;
char hour1=0;
char hour2=0;
char n=5;

void print(char count){
	switch(count){
	case 0:
			PORTC=0x00;
			break;
	case 1:

		PORTC=0x01;
		break;

	case 2:
		PORTC=0x02;
			break;

	case 3:
		PORTC=0x03;
			break;
	case 4:
		PORTC=0x04;
			break;
	case 5:
		PORTC=0x05;
			break;
	case 6:
		PORTC=0x06;
				break;
	case 7:
		PORTC=0x07;
				break;
	case 8:
		PORTC=0x08;
				break;
	case 9:
		PORTC=0x09;
				break;
	}
}

ISR(TIMER1_COMPA_vect){
	count++;
	if(count==10)
	{count=0;
	sec2++;}
	if(count==0&&sec2==6)
	{sec2=0;
	min1++;}
	if(min1==10){
		min1=0;
		min2++;
	}
	if(min1==0&&min2==6){
		min2=0;
		hour1++;
	}
	if(hour1==9)
	{hour1=0;
	hour2++;}
	if(hour1==2&&hour2==1)
	{hour1=0;
	hour2=0;}
}

ISR(INT2_vect){
	PORTA|=(1<<SEC1);
	TIMSK |=(1<<OCIE1A);
}

ISR(INT1_vect){
	TIMSK &=~(1<<OCIE1A);
	PORTA=0x00;
	count=0;
	sec2=0;
	min1=0;
	min2=0;
	hour1=0;
	hour2=0;
	TIFR|=(1<<OCF1A);

}

ISR(INT0_vect){
	TIMSK &=~(1<<OCIE1A);

}


int main(){
	//BUTTONS
   DDRD&=~(1<<PD3); //1 MEANS NOT CLICKED
   DDRB&=~(1<<PB2);
   PORTB|=(1<<PB2);
   PORTD|=(1<<PD3);
   DDRD&=~(1<<PD2);
   PORTD|=(1<<PD2);
   //BCD
   DDRC=0xFF;


   //ENABLE
   DDRA|=(1<<PA0);
   DDRA|=(1<<PA1);
   DDRA|=(1<<PA2);
   DDRA|=(1<<PA3);
   DDRA|=(1<<PA4);
   DDRA|=(1<<PA5);

   //Inter
   GICR   |= (1<<INT2)|(1<<INT0)|(1<<INT1);
   MCUCSR &= ~(1<<ISC2);    // Trigger INT2 with the falling edge
   MCUCSR |= (1<<ISC11)|(1<<ISC01);

   TCCR1A |=(1<<FOC1A);
   TCCR1A &=~(1<<0)&~(1<<1);

   TCCR1B |=(1<<CS11)|(1<<CS10)|(1<<WGM12);
   TCCR1B &=~(1<<CS12)&~(WGM13);

   TIMSK &=~(1<<OCIE1A);
   TCNT1=0;
   OCR1A=15625;
   SREG   |= (1<<7);




	while(1){
		PORTA&=~0xFF;
		PORTA|=(1<<SEC1);
		print(count);
		_delay_ms(n);

		PORTA&=~0xFF;
		PORTA|=(1<<SEC2);
		print(sec2);
		_delay_ms(n);


		PORTA&=~0xFF;
		PORTA|=(1<<MIN1);
		print(min1);
		_delay_ms(n);

		PORTA&=~0xFF;
		PORTA|=(1<<MIN2);
		print(min2);
		_delay_ms(n);


		PORTA&=~0xFF;
		PORTA|=(1<<HOUR1);
		print(hour1);
		_delay_ms(n);

		PORTA&=~0xFF;
		PORTA|=(1<<HOUR2);
		print(hour2);
		_delay_ms(n);

	}
	return 0;
}




