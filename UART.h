/*
 * UART.h
 *
 * Created: 25.11.2020 13:46:56
 *  Author: Hendrik
 */ 


#ifndef UART_H_
#define UART_H_


void uart_init(void);
void uart_send_char(unsigned char c);
void uart_send_string(char* s);

void uart_init_isr(void);
void uart_send_isr(char* s);

void uart_send_char_isr(unsigned char c);
void uart_send_string_isr(char* s);

void uart_init_rs(void);
void uart_send_byte(char x);
void uart_send(const char* string);

uint8_t uart_data_available(void);
char uart_get_data(void);


#endif /* UART_H_ */