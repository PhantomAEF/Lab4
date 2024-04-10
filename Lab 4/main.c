/*
 * Lab 4.c
 *
 * Created: 9/4/2024 19:05:31
 * Author : alane
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


volatile uint8_t iBina = 0;
void initADC(void);
void setup(void);

void setup(){
	DDRC = 0xFF;
	DDRC &= ~((1 << PC2) | (1 << PC3));
	PORTC |= (1 << PC2) | (1 << PC3);
	DDRD = 0xFF;
	PORTD = 0;
	DDRB = 0xFF;
	UCSR0B = 0;
	sei();

	
}

ISR(PCINT1_vect){
	if (!(PINC & (1 << PC2))) {
		iBina++;
	}
	if (!(PINC & (1 << PC5))) {
		iBina--;
	}
}

int main(void)
{
    cli();
	setup();
	sei();
	
    while (1) 
    {
		PORTB = iBina;
		PORTC = (PORTC & 0b11111100) | ((iBina & 0b11000000) >> 6);
    }
	return 0;
}
