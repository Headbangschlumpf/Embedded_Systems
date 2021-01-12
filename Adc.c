/*
 * Adc.c
 *
 * Created: 15.12.2020 16:59:04
 *  Author: Hendrik
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Adc.h"

// needed registers start at 0x7C: ADMUX(0x7C); ADCL(0x78); ADCH(0x79); ADCSRA(0x7A);
typedef struct adc_t{			// reg

	uint8_t adcl		  ; // ADCL

	uint8_t adch		  ; // ADCH

	uint8_t adps		:3; // ADCSRA
	uint8_t adie		:1; //
	uint8_t adif		:1;	// 
	uint8_t adate		:1;	//
	uint8_t adsc		:1;	// 
	uint8_t aden		:1; // 

	uint8_t adts		:3; // ADCSRB
	uint8_t not_used1	:3;	// 
	uint8_t acme		:1; // 
	uint8_t not_used2	:1;	// 

	uint8_t mux			:5;	// ADMUX
	uint8_t adlar		:1;	//
	uint8_t refs		:2; // 
 
}ADC_t;

volatile ADC_t *ADC_A = (ADC_t*)(0x78);

void adc_init(void){	ADC_A->refs = 0;		// delete REFS0 and REFS1 to get EXT_REF as Reference Voltage	ADC_A->adlar = 1;		// set ADLAR -> left Adjust -> read only ADCH	ADC_A->adps = 0b111;		// set Prescaler to /128 -> 16MHz/128 = 125kHz 111= 1+2+4	ADC_A->aden = 1;		// enable ADC		/*	ADMUX &= ~((1<<REFS0)|(1<<REFS1));	// delete REFS0 and REFS1 to get EXT_REF as Reference Voltage	ADMUX |= (1<<ADLAR);				// set ADLAR -> left Adjust -> read only ADCH	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN)); // set Prescaler to /128 -> 16MHz/128 = 125kHz and enable ADC	*/
}
uint16_t adc_get_poti(void){
	uint16_t val = 0;
	ADC_A->mux = 0;	// delete ADMUX
	ADC_A->mux = 0b001; //select ADC-Pin 1
	ADC_A->adsc = 1;
	while(ADC_A->adsc==1){
		// wait for conversion to be completed
	}
	cli();
	val = ADC_A->adch;
	sei();
	return val;

	/*
	uint16_t val = 0;
	ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)|(1<<MUX4)); // Delete ADMUX
	ADMUX |= (1<<MUX0); // select ADC-Pin 1
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC)) != 0){
		
	}
	cli();
	val = ADCH;
	sei();
	return val;
	*/
}

uint16_t adc_get_LM35(void){
	uint16_t val = 0;
	ADC_A->mux = 0;	// delete ADMUX ald select ADC-Pin 0
	ADC_A->adsc = 1;
	while(ADC_A->adsc==1){
		// wait for conversion to be completed
	}
	cli();
	val = ADC_A->adch;
	sei();
	return val;

	/*
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
	*/
}


// old but working
/*
void adc_init(void){	ADMUX &= ~((1<<REFS0)|(1<<REFS1));	// delete REFS0 and REFS1 to get EXT_REF as Reference Voltage	ADMUX |= (1<<ADLAR);				// set ADLAR -> left Adjust -> read only ADCH	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN)); // set Prescaler to /128 -> 16MHz/128 = 125kHz and enable ADC}uint16_t adc_get_poti(void){
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
*/