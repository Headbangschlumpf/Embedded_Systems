/*
 * UART.c
 *
 * Created: 25.11.2020 13:46:40
 *  Author: Hendrik
 */ 
  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <stdbool.h>

  #define SIZE_BUFFER 500
  
  	struct{
	  	uint8_t data[SIZE_BUFFER];
	  	uint16_t Readpointer;
	  	uint16_t Writepointer;
	  	volatile uint8_t SendActive;
  	}Ringspeicher_TX;

  	struct{
	  	uint8_t data[SIZE_BUFFER];
	  	uint16_t Readpointer;
	  	uint16_t Writepointer;
  	}Ringspeicher_RX;

  	void uart_init_rs(void){
	  	
	  	UBRR0 = 103;
	  	UCSR0B |= (1<<TXEN0)|(1<<TXCIE0);
	  	UCSR0B |= (1<<RXEN0)|(1<<RXCIE0);
	  	UCSR0C = (3<<UCSZ00);

	  	Ringspeicher_TX.Readpointer=0;
	  	Ringspeicher_TX.Writepointer=0;
	  	Ringspeicher_TX.SendActive=0;

	  	Ringspeicher_RX.Readpointer=0;
	  	Ringspeicher_RX.Writepointer=0;

  	}
  	
  	void uart_send(const char* string){

	  	while(*string!=0){
		  	uart_send_byte(*string++);
	  	}
  	}

  	void uart_send_byte(char x){
	  	//cli();
	  	UCSR0B&=~(1<<TXCIE0);//disable
	  	if(Ringspeicher_TX.SendActive == 0){
		  	UDR0 = x;
		  	Ringspeicher_TX.SendActive = 1;
		  	}else{
		  	Ringspeicher_TX.data[Ringspeicher_TX.Writepointer++] = x;
		  	if(Ringspeicher_TX.Writepointer>=SIZE_BUFFER){
			  	Ringspeicher_TX.Writepointer=0;
		  	}
	  	}
	  	UCSR0B|=(1<<TXCIE0);//enable
	  	//sei();
  	}

  	ISR(USART0_TX_vect){

	  	if(Ringspeicher_TX.Readpointer!=Ringspeicher_TX.Writepointer){
		  	UDR0 = Ringspeicher_TX.data[Ringspeicher_TX.Readpointer++];
		  	if(Ringspeicher_TX.Readpointer>=SIZE_BUFFER){
			  	Ringspeicher_TX.Readpointer = 0;
		  	}
		  	}else{
		  	Ringspeicher_TX.SendActive=0;
	  	}
  	}
  	
  	uint8_t uart_data_available(void){
	  	uint8_t returnval = 0;
		UCSR0B &= ~(1<<RXCIE0);
		if(Ringspeicher_RX.Writepointer != Ringspeicher_RX.Readpointer){
			returnval = 1;
		}
		UCSR0B |= (1<<RXCIE0);
		return returnval;
  	}

  	char uart_get_data(void){
	  	char returnval = 0;
		UCSR0B &= ~(1<<RXCIE0);
		if(Ringspeicher_RX.Writepointer != Ringspeicher_RX.Readpointer){
			returnval = Ringspeicher_RX.data[Ringspeicher_RX.Readpointer++];
			if(Ringspeicher_RX.Readpointer>=SIZE_BUFFER){
				Ringspeicher_RX.Readpointer = 0;
			}
		}
		UCSR0B |= (1<<RXCIE0);
		return returnval;
  	}

  	ISR(USART0_RX_vect){
		uint8_t status = UCSR0A;
		uint8_t data = UDR0;
	  	if((status & ((1<<DOR0)|(1<<FE0)))==0){
		  	Ringspeicher_RX.data[Ringspeicher_RX.Writepointer++]=data;
		  	if(Ringspeicher_RX.Writepointer>=SIZE_BUFFER){
				Ringspeicher_RX.Writepointer=0;
	  		}
		}
  	}
	
  /*
	struct{
		uint8_t data[SIZE_BUFFER];
		uint16_t Readpointer;
		uint16_t Writepointer;
		volatile uint8_t SendActive;
	}Ringspeicher_TX;

	struct{
		uint8_t data[SIZE_BUFFER];
		uint16_t Readpointer;
		uint16_t Writepointer;
		volatile uint8_t if_received;
	}Ringspeicher_RX;

	void uart_init_rs(void){
	
	UBRR0 = 103;
	UCSR0B |= (1<<TXEN0)|(1<<TXCIE0);
	UCSR0B |= (1<<RXEN0)|(1<<RXCIE0);
	UCSR0C = (3<<UCSZ00);

	Ringspeicher_TX.Readpointer=0;
	Ringspeicher_TX.Writepointer=0;
	Ringspeicher_TX.SendActive=0;

	Ringspeicher_RX.Readpointer=0;
	Ringspeicher_RX.Writepointer=0;
	Ringspeicher_RX.if_received=0;

	}
	
	void uart_send(const char* string){

		while(*string!=0){
			uart_send_byte(*string++);
		}
	}

	void uart_send_byte(char x){
		//cli();
		UCSR0B&=~(1<<TXCIE0);//disable
		if(Ringspeicher_TX.SendActive == 0){
			UDR0 = x;
			Ringspeicher_TX.SendActive = 1;
		}else{
			Ringspeicher_TX.data[Ringspeicher_TX.Writepointer++] = x;
			if(Ringspeicher_TX.Writepointer>=SIZE_BUFFER){
				Ringspeicher_TX.Writepointer=0;
			}
		}
		UCSR0B|=(1<<TXCIE0);//enable
		//sei();
	}

	ISR(USART0_TX_vect){

		if(Ringspeicher_TX.Readpointer!=Ringspeicher_TX.Writepointer){
			UDR0 = Ringspeicher_TX.data[Ringspeicher_TX.Readpointer++];
			if(Ringspeicher_TX.Readpointer>=SIZE_BUFFER){
				Ringspeicher_TX.Readpointer = 0;
			}
		}else{
			Ringspeicher_TX.SendActive=0;
		}
	}
	
	uint8_t uart_data_available(void){
		return Ringspeicher_RX.if_received;
	}

	char uart_get_data(void){
		UCSR0B &= ~(1<<RXCIE0);
		uint8_t tmp = Ringspeicher_RX.data[Ringspeicher_RX.Readpointer++];
		if(Ringspeicher_RX.Readpointer>=SIZE_BUFFER){
			Ringspeicher_RX.Readpointer = 0;
		}
		if(Ringspeicher_RX.Readpointer==Ringspeicher_RX.Writepointer){
			Ringspeicher_RX.Writepointer=0;
			Ringspeicher_RX.Readpointer=0;
			Ringspeicher_RX.if_received=0;
		}
		UCSR0B |= (1<<RXCIE0);
		return tmp;
	}

	ISR(USART0_RX_vect){
		if(!Ringspeicher_RX.if_received){
			Ringspeicher_RX.data[Ringspeicher_RX.Writepointer++]=UDR0;
			Ringspeicher_RX.if_received=1;		
			if (Ringspeicher_RX.Writepointer>=SIZE_BUFFER){
				Ringspeicher_RX.Writepointer = 0;
			}
		}
	}
	*/
  /*
  #define SIZE_BUFFER 500

	struct{
		uint8_t data[SIZE_BUFFER];
		uint16_t Readpointer;
		uint16_t Writepointer;
		volatile uint8_t SendActive;
	}Ringspeicher_TX;

	void uart_init_rs(void){
	
	UBRR0 = 103;
	UCSR0B = (1<<TXEN0)|(1<<TXCIE0);
	UCSR0C = (3<<UCSZ00);

	Ringspeicher_TX.Readpointer=0;
	Ringspeicher_TX.Writepointer=0;
	Ringspeicher_TX.SendActive=0;
	}
	
	void uart_send(const char* string){

		while(*string!=0){
			uart_send_byte(*string++);
		}
	}

	void uart_send_byte(char x){
		//cli();
		UCSR0B&=~(1<<TXCIE0);//disable 
		if(Ringspeicher_TX.SendActive == 0){
			UDR0 = x;
			Ringspeicher_TX.SendActive = 1;
		}else{
			Ringspeicher_TX.data[Ringspeicher_TX.Writepointer++] = x;
			if(Ringspeicher_TX.Writepointer>=SIZE_BUFFER){
				Ringspeicher_TX.Writepointer=0;
			}
		}
		UCSR0B|=(1<<TXCIE0);//enable
		//sei();
	}

	ISR(USART0_TX_vect){

		if(Ringspeicher_TX.Readpointer!=Ringspeicher_TX.Writepointer){
			UDR0 = Ringspeicher_TX.data[Ringspeicher_TX.Readpointer++];
			if(Ringspeicher_TX.Readpointer>=SIZE_BUFFER){
				Ringspeicher_TX.Readpointer = 0;
			}
		}else{
			Ringspeicher_TX.SendActive=0;
		}
	}
	*/
	/*
	ISR(USART0_RX_vect){
		PORTB = UDR0;
	}
	*/
/*
//UART1
 void uart_init(void){
	// use UART 0
	UBRR0 = 103;							// write 103 to UBRR0 to set BAUD Rate to 9600
	UCSR0C &= ~(1<<UMSEL00)|(1<<UMSEL01);	// clear UMSEL00 and UMSEL01 in UCSR0C to enter Mode: Asynchronous USART
	UCSR0C &= ~(1<<UPM00)|(1<<UPM01);		// clear UPM00 and UPM01 to disable Parity Mode
	UCSR0C &= ~(1<<USBS0);					// clear USBS0 in UCSR0C to set amount of Stop Bits to 1
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);		// set UCSZ00 and UCSZ01 in UCSR0C to set Character Size to 8
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);		// set RXEN0 and TXEN0 to enable Receiver and Transmitter Mode
 }

 void uart_send_char(unsigned char c){
 
	while (!(UCSR0A&(1<<UDRE0))){
		// wait to send
	};
	UDR0 = c;

	
	// does not work with string
	if(UCSR0A & (1<<UDRE0)){
		UDR0 = c;
	}
	
}
void uart_send_string(char* s){
	
	while(*s != '\0'){
		uart_send_char(*s);
		s++;
	}
}
*/

/*
//better UART2
void uart_init_isr(void){
	UBRR0 = 103;
	UCSR0B = (1<<TXEN0)|(1<<TXCIE0);
	UCSR0C = (3<<UCSZ00);
}

volatile const char* senddata;

void uart_send_isr(const char* string){
	senddata = string;
	UDR0 = *senddata++;
}

ISR(USART0_TX_vect){
	if(*senddata){
		UDR0 = *senddata++;
	}
}
*/

// old but working  UART2
/*
 void uart_send_char_isr(unsigned char c){
	UDR0 = c;
 }

void uart_send_string_isr(char* s){
	while(*s != '\0'){
		uart_send_char(*s);
		s++;
	}
}


void uart_init_isr(void){
	// old but working
	// init UART 0
	UBRR0 = 103;								// write 103 to UBRR0 to set BAUD Rate to 9600
	UCSR0C &= ~(1<<UMSEL00)|(1<<UMSEL01);		// clear UMSEL00 and UMSEL01 in UCSR0C to enter Mode: Asynchronous USART
	UCSR0C &= ~(1<<UPM00)|(1<<UPM01);			// clear UPM00 and UPM01 to disable Parity Mode
	UCSR0C &= ~(1<<USBS0);						// clear USBS0 in UCSR0C to set amount of Stop Bits to 1
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);			// set UCSZ00 and UCSZ01 in UCSR0C to set Character Size to 8
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);			// set RXEN0 and TXEN0 to enable Receiver and Transmitter Mode
	UCSR0B |= (1<<UDRIE0);						// set UDRIE0 in UCSR0B to enable Data Register Empty Interrupt Flag

}

static volatile int ptr_output;
static volatile bool sent = false;


void uart_send_isr(char* s){
	// old but working
	sent = false;
	ptr_output = s;
}

ISR(USART0_UDRE_vect){
	// old but working
	if (sent == false){
		uart_send_string_isr(ptr_output);
		sent = true;
	}
}
*/
	