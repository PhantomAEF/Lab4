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

const uint8_t lista[]= {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78,  0x00, 0x10, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E};
volatile uint8_t iBina = 0;
volatile uint8_t idis1 = 0;
volatile uint8_t idis2 = 0;
volatile uint8_t iADC = 0;

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
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT10);

	sei();

	
}

void initADC(){
	ADMUX=0;
	ADMUX |= (1<<REFS0);
	ADMUX |= (1<<ADLAR);
	ADMUX |= ((1<<MUX2) | (1<<MUX1));
	ADCSRA = 0;
	ADCSRA |= ((1<<ADEN) | (1<<ADIE));
	ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
	DIDR0 =0;
	
	
	
}

ISR(ADC_vect){
	
	ADCSRA &= ~(1<<ADIF);
	iADC = ADCH;
	idis1 = iADC & 0b00001111;
	idis2 = iADC >> 4;

}

ISR(PCINT1_vect){
	if (!(PINC & (1 << PC2))) {
		iBina++;
	}
	if (!(PINC & (1 << PC3))) {
		iBina--;
	}
}

int main(void)
{
    cli();
	setup();
	initADC();
	sei();
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);
		PORTB = iBina;
		PORTC = (PORTC & 0b11111100) | ((iBina & 0b11000000) >> 6);
		
		PORTC |= ((1<<PC4));
		PORTC &= ~(1<<PC5);
		PORTD = lista[idis1];
		_delay_ms(4);
		PORTC |= ((1<<PC5));
		PORTC &= ~(1<<PC4);
		PORTD = lista[idis2];
		_delay_ms(4);
    }
	return 0;
}
