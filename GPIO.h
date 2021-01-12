/*
 * GPIO.h
 *
 * Created: 12.01.2021 18:01:15
 *  Author: Hendrik
 */ 


#ifndef GPIO_H_
#define GPIO_H_

typedef struct GPIO{
	uint8_t pin0 :1;
	uint8_t pin1 :1;
	uint8_t pin2 :1;
	uint8_t pin3 :1;
	uint8_t pin4 :1;
	uint8_t pin5 :1;
	uint8_t pin6 :1;
	uint8_t pin7 :1;
	uint8_t ddr0 :1;
	uint8_t ddr1 :1;
	uint8_t ddr2 :1;
	uint8_t ddr3 :1;
	uint8_t ddr4 :1;
	uint8_t ddr5 :1;
	uint8_t ddr6 :1;
	uint8_t ddr7 :1;
	uint8_t port0 :1;
	uint8_t port1 :1;
	uint8_t port2 :1;
	uint8_t port3 :1;
	uint8_t port4 :1;
	uint8_t port5 :1;
	uint8_t port6 :1;
	uint8_t port7 :1;
}GPIO_t;

#define GPIO_A ((GPIO_t*)(0x20))
#define GPIO_B ((GPIO_t*)(0x23))
#define GPIO_C ((GPIO_t*)(0x26))
#define GPIO_D ((GPIO_t*)(0x29))

#endif /* GPIO_H_ */