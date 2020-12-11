/*
 * Timer.c
 *
 * Created: 03.11.2020 17:23:23
 *  Author: Hendrik
 */ 
 #include <avr/io.h>
 #include <avr/interrupt.h>

 void Timer_init(void){

	// init Timer0 in Mode CTC Prescaler = /8 at 16MHz
	TCCR0A &= ~(1<<WGM00)|(1<<WGM01); // clear WGM00 and WGM02 in TCCR0A
	TCCR0A |= (1<<WGM01); // set WGM01 in TCCR0A to enter CTC Mode
	TCCR0B |= (1<<CS01)|(1<<CS00); // set CS00 adn CS01 in TCCR0B to set Prescaler to /64 -> TCNT counts up every 16MHZ / 64 = 250kHz -> 1/250kHz = 4*10^-6s
	OCR0A = 250-1;	// set OCR0A to 249 to set compare value -> 250 * 4*10^-6s = 1ms -> ISR all 1ms
	TIMSK0 |= (1<<OCIE0A); // enable Compare Match A Interrupt and
 }

  static volatile uint16_t tick = 0;

  uint16_t getTick(void){
	  cli();
	  uint16_t tmp = tick;
	  sei();
	  return tmp;
  }
  
  ISR(TIMER0_COMPA_vect){
	  tick++;
  }