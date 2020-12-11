/*
 * Taster.c
 *
 * Created: 27.10.2020 16:36:45
 *  Author: Hendrik
 */ 
 #include <avr/io.h>

 void Taster_init(void){
	// define data direction
	DDRC &= ~(1<<2);
	DDRD &= ~(1<<5);
	DDRD &= ~(1<<6);
	DDRD &= ~(1<<7);

	// activate Pull-Ups
	PORTC |= (1<<2);
	PORTD |= (1<<5);
	PORTD |= (1<<6);
	PORTD |= (1<<7);

	//deactivate JTAG Interface
	MCUCR |= (1<<7);
	MCUCR |= (1<<7);
}

uint8_t Taster1_get(void){
	uint8_t val = 0;
	if ((PIND & (1<<7)) != 0){
		val = 0;
	} else{
		val = 1;
	}
	return val;
}
uint8_t Taster2_get(void){
	uint8_t val = 0;
	if ((PIND & (1<<6)) != 0){
		val = 0;
		} else{
		val = 1;
	}
	return val;
}
uint8_t Taster3_get(void){
	uint8_t val = 0;
	if ((PIND & (1<<5)) != 0){
		val = 0;
		} else{
		val = 1;
	}
	return val;
}
uint8_t Taster4_get(void){
	uint8_t val = 0;
	if ((PINC & (1<<2)) != 0){
		val = 0;
		} else{
		val = 1;
	}
	return val;
}