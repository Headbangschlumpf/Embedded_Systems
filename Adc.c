/*
 * Adc.c
 *
 * Created: 15.12.2020 16:59:04
 *  Author: Hendrik
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Adc.h"

void adc_init(void){
	uint16_t val = 0;
	ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)|(1<<MUX4)); // Delete ADMUX
	ADMUX |= (1<<MUX0); // select ADC-Pin 1
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC)) != 0){
		// wait for conversion to be completed
	}
	cli();
	val = ADCH;
	sei();
	return val;
}

uint16_t adc_get_LM35(void){
	uint16_t val = 0;
	ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)|(1<<MUX4)); // delete ADMUX and select ADC-Pin 0
	ADCSRA |= (1<<ADSC);
	while(ADCSRA&(1<<ADSC)){
		// wait for conversion to be completed
	}
	cli();
	val = ADCH;
	sei();
	return val;
}