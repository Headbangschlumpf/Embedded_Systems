/*
 * Led.c
 *
 * Created: 27.10.2020 16:23:47
 *  Author: Hendrik
 */ 

 #include "Led.h"

 void Led_init(void){
	DDRB = 0xFF;
	PORTB = 0x00;
 }

 void Led1_On(){
	PORTB |= (1<<7);
 }

 void Led1_Off(){
	PORTB &= ~(1<<7);
 }

 void Led2_On(){
	PORTB |= (1<<6);	
 }

 void Led2_Off(){
	PORTB &= ~(1<<6);
 }

 void Led3_On(){
	PORTB |= (1<<5);
 }

 void Led3_Off(){
	PORTB &= ~(1<<5);
 }

 void Led4_On(){
	PORTB |= (1<<4);
 }

 void Led4_Off(){
	PORTB &= ~(1<<4);
 }

 void Led5_On(){
	PORTB |= (1<<3);
 }

 void Led5_Off(){
	PORTB &= ~(1<<3);
 }

 void Led6_On(){
	PORTB |= (1<<2);
 }

 void Led6_Off(){
	PORTB &= ~(1<<2);
 }

 void Led7_On(){
	PORTB |= (1<<1);
 }

 void Led7_Off(){
	PORTB &= ~(1<<1);
 }

 void Led8_On(){
	PORTB |= (1<<0);
 }

 void Led8_Off(){
	PORTB &= ~(1<<0);
 }