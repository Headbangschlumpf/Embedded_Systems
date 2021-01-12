/*
 * Led.c
 *
 * Created: 27.10.2020 16:23:47
 *  Author: Hendrik
 */ 

 #include "Led.h"
 #include "GPIO.h"

 void Led_init(void){
	GPIO_B->ddr0 = 1;
	GPIO_B->ddr1 = 1;
	GPIO_B->ddr2 = 1;
	GPIO_B->ddr3 = 1;
	GPIO_B->ddr4 = 1;
	GPIO_B->ddr5 = 1;
	GPIO_B->ddr6 = 1;
	GPIO_B->ddr7 = 1;
	//DDRB = 0xFF;

	GPIO_B->port0 = 0;
	GPIO_B->port1 = 0;
	GPIO_B->port2 = 0;
	GPIO_B->port3 = 0;
	GPIO_B->port4 = 0;
	GPIO_B->port5 = 0;
	GPIO_B->port6 = 0;
	GPIO_B->port7 = 0;
	//PORTB = 0x00;
 }

 void Led1_On(){
	GPIO_B->port7 = 1;
	//PORTB |= (1<<7);
 }

 void Led1_Off(){
	GPIO_B->port7 = 0;
	//PORTB &= ~(1<<7);
 }

 void Led2_On(){
	GPIO_B->port6 = 1;
	//PORTB |= (1<<6);	
 }

 void Led2_Off(){
	GPIO_B->port6 = 0;
	//PORTB &= ~(1<<6);
 }

 void Led3_On(){
	GPIO_B->port5 = 1;
	//PORTB |= (1<<5);
 }

 void Led3_Off(){
	GPIO_B->port5 = 0;
	//PORTB &= ~(1<<5);
 }

 void Led4_On(){
	GPIO_B->port4 = 1;
	//PORTB |= (1<<4);
 }

 void Led4_Off(){
	GPIO_B->port4 = 0;
	//PORTB &= ~(1<<4);
 }

 void Led5_On(){
	GPIO_B->port3 = 1;
	//PORTB |= (1<<3);
 }

 void Led5_Off(){
	GPIO_B->port3 = 0;
	//PORTB &= ~(1<<3);
 }

 void Led6_On(){
	GPIO_B->port2 = 1;
	//PORTB |= (1<<2);
 }

 void Led6_Off(){
	GPIO_B->port2 = 0;
	//PORTB &= ~(1<<2);
 }

 void Led7_On(){
	GPIO_B->port1 = 1;
	//PORTB |= (1<<1);
 }

 void Led7_Off(){
	GPIO_B->port1 = 0;
	//PORTB &= ~(1<<1);
 }

 void Led8_On(){
	GPIO_B->port0 = 1;
	//PORTB |= (1<<0);
 }

 void Led8_Off(){
	GPIO_B->port0 = 0;
	//PORTB &= ~(1<<0);
 }