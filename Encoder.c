/*
 * Encoder.c
 *
 * Created: 05.01.2021 16:41:08
 *  Author: Hendrik
 */ 
  #include <stdio.h>
  #include <avr/io.h>

 void encoder_init(void){
	//PC6 -> Encoder A
	//PC7 -> Encoder B

	// define data direction as inputs
	DDRC &= ~(1<<PORTC6);
	DDRC &= ~(1<<PORTC7);

	// activate Pull-Ups
	PORTC |= (1<<PORTC6);
	PORTC |= (1<<PORTC7);
 }

 static volatile int16_t counter = 0;
 
 void encoder_process(void){
	static enum{
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
	}Status = ONE;
	
	uint8_t input = PINC & 0b11000000; // delete not used PINC input
	input = input >> 6;	// bit shift >> 6 to make interpretaion easier
	PORTB = input;
	

	switch(Status){

		case ONE:
			if (input == 1){
				Status = TWO;
			}
			if(input == 2){
				Status = FIVE;
			}
			break;

		case TWO:
			if (input == 3){
				Status = THREE;
			}
			if (input == 0){
				Status = ONE;
			}
			break;

		case THREE:
			if (input == 2){
				Status = FOUR;
			}
			if (input == 1){
				Status = TWO;
			}
			break;

		case FOUR:
			if (input == 3){
				Status = THREE;
			}
			if (input == 0){
				Status = ONE;
				//increment
				counter--;
			}
			break;

		case FIVE:
			if (input == 3){
				Status = SIX;
			}
			if (input == 0){
				Status = ONE;
			}
			break;

		case SIX:
			if (input == 1){
				Status = SEVEN;
			}
			if (input == 2){
				Status = FIVE;
			}
			break;

		case SEVEN:
			if (input == 0){
				Status = ONE;
				//decrement
				counter++;
			}
			if (input == 3){
				Status = SIX;
			}
			break;
	}
 }

 int16_t encoder_get(void){
	return counter;
 }
