/*
 * Applications.c
 *
 * Created: 17.11.2020 17:49:21
 *  Author: Hendrik
 */ 
 #define F_CPU 16000000

 #include <avr/io.h>
 #include <stdbool.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>

 #include "Applications.h"
 #include "Led.h"
 #include "Taster.h"
 #include "Timer.h"
 #include "UART.h"
 #include "cli.h"
 #include "Adc.h"
 #include "Encoder.h"

 void drehgeber1(void){
	uart_init_rs();
	Led_init();
	Taster_init();
	Timer_init();
	line_interpreter_init(&uart_send_byte);
	adc_init();
	encoder_init();
	sei();
	uart_send(" Hallo INIT \r\n");

	while(1){
		if(uart_data_available()){
			line_interpreter_get_data(uart_get_data());
		}
		encoder_process();
	}
 }

 void blink4(void){
	 uart_init_rs();
	 Led_init();
	 Taster_init();
	 Timer_init();
	 line_interpreter_init(&uart_send_byte);
	 adc_init();
	 sei();
	 static enum{
		 INIT = 0,
		 ON = 1,
		 OFF = 2,
	 }Status = INIT;
	 static uint16_t timer = 0;
	 int timeout = 500;
	 while(1){
		 
		 if(uart_data_available()){
			 line_interpreter_get_data(uart_get_data());
		 }
		 switch(Status){
			 case INIT:
			 Led1_On();
			 uart_send(" Hallo INIT \r\n");
			 Status = ON;
			 break;

			 case ON:
			 if ((getTick()-timer)>=timeout){
				 timer = getTick();
				 Led1_Off();
				 //uart_send("OFF\r\n");
				 Status = OFF;
			 }
			 break;

			 case OFF:
			 if ((getTick()-timer)>=timeout){
				 timer = getTick();
				 Led1_On();
				 //uart_send("ON\r\n");
				 Status = ON;
			 }
			 break;
		 }
	 }
 }

  void blink3(void){
	  uart_init_rs();
	  Led_init();
	  Taster_init();
	  Timer_init();
	  line_interpreter_init(&uart_send_byte);
	  sei();
	  static enum{
		  INIT = 0,
		  ON = 1,
		  OFF = 2,
	  }Status = INIT;
	  static uint16_t timer = 0;
	  int timeout = 500;
	  while(1){
		  
		  if(uart_data_available()){
			line_interpreter_get_data(uart_get_data());
		  }
		  switch(Status){
			  case INIT:
			  Led1_On();
			  uart_send(" Hallo INIT \r\n");
			  Status = ON;
			  break;

			  case ON:
			  if ((getTick()-timer)>=timeout){
				  timer = getTick();
				  Led1_Off();
				  //uart_send("OFF\r\n");
				  Status = OFF;
			  }
			  break;

			  case OFF:
			  if ((getTick()-timer)>=timeout){
				  timer = getTick();
				  Led1_On();
				  //uart_send("ON\r\n");
				  Status = ON;
			  }
			  break;
		  }
	  }
  }

 
 void blink2(void){
	uart_init_rs();
	Led_init();
	Timer_init();
	sei();
	static enum{
		INIT = 0,
		ON = 1,
		OFF = 2,
	}Status = INIT;
	static uint16_t timer = 0;
	int timeout =500;
	while(1){
	
		if(uart_data_available()){
			uart_send_byte(uart_get_data());
		}
		
		switch(Status){
			case INIT:
				Led1_On();
				uart_send(" Hallo INIT \r\n");
				Status = ON;
				break;

			case ON:
				if ((getTick()-timer)>=timeout){
					timer = getTick();
					Led1_Off();
					//uart_send("OFF\r\n");
					Status = OFF;
				}
				break;

			case OFF:
				if ((getTick()-timer)>=timeout){
					timer = getTick();
					Led1_On();
					//uart_send("ON\r\n");
					Status = ON;
				}
				break;
		}
	}
 }
 
 
 void blink1(void){
	uart_init();
	uart_send_string("START_");

	Led_init();
	Timer_init();
	static enum{
		INIT = 0,
		ON = 1,
		OFF = 2,
	}Status = INIT;
	static uint16_t timer = 0;
	int timeout = 500;
	while(1){
		switch(Status){
			case INIT:
			Led1_On();
			Status = ON;
			uart_send_string("ON_");
			break;

			case ON:
				if ((getTick()-timer)>=timeout){
					timer = getTick();
					Led1_Off();
					uart_send_string("OFF_");
					Status = OFF;
				}
				break;	

			case OFF:
				if ((getTick()-timer)>=timeout){
					timer = getTick();
					Led1_On();
					uart_send_string("ON_");
					Status = ON;
				}
			break;
			}
		}
	}
	
	
 void Ampelschaltung(void){

	/* Replace with your application code */
	Led_init();
	Timer_init();
	static uint16_t timer = 0;
	int gn_phase = 5000; // 30000
	int other_phases = 2000; // 5000
	static enum{
		INIT = 0,
		TL1_Y = 2,
		TL1_R = 3,
		TL2_R2Y = 4,
		TL2_G = 5,
		TL2_Y = 6,
		TL2_R = 7,
		TL1_R2Y = 8,
		TL1_G = 1, 
	}Status = INIT;
	sei();
	/*
	// Leds used for traffic light
	TL1_Rd = LED1
	TL1_Ye = LED2
	TL1_Gn = Led3

	TL2_Rd = LED4
	TL2_Ye = LED5
	TL2_Gn = Led6
	*/

    while (1) 
    {
		switch(Status){
			case INIT:
				Led3_On(); // TL1_Gn
				Led4_On(); // TL2_Rd
				Status = TL1_Y;
				break;

			case TL1_Y:
				if ((getTick()-timer)>=gn_phase){
					timer = getTick();
					Led3_Off();
					Led2_On();
					Status = TL1_R;
				}
				break;

			case TL1_R:
				if ((getTick()-timer)>=other_phases){
					timer = getTick();
					Led2_Off();
					Led1_On();
					Status = TL2_R2Y;
				}
				break;

			case TL2_R2Y:
				if ((getTick()-timer)>=other_phases){
					timer = getTick();
					Led4_Off();
					Led5_On();
					Status = TL2_G;
				}
				break;
				
			case TL2_G:
				if ((getTick()-timer)>=other_phases){
					timer = getTick();
					Led5_Off();
					Led6_On();
					Status = TL2_Y;
				}
				break;
				
			case TL2_Y:
				if ((getTick()-timer)>=gn_phase){
					timer = getTick();
					Led6_Off();
					Led5_On();
					Status = TL2_R;
				}
				break;
				
			case TL2_R:
				if ((getTick()-timer)>=other_phases){
					timer = getTick();
					Led5_Off();
					Led4_On();
					Status = TL1_R2Y;
				}
				break;
				
			case TL1_R2Y:
				if ((getTick()-timer)>=other_phases){
					timer = getTick();
					Led1_Off();
					Led2_On();
					Status = TL1_G;
				}
				break;
				
			case TL1_G:
				if ((getTick()-timer)>=other_phases){
					timer = getTick();
					Led2_Off();
					Led3_On();
					Status = TL1_Y;
				}
				break;
			
		}	
    }
 }

 void Ampelschaltung_Fussgaenger(void){
	/* Replace with your application code */
	Led_init();
	Taster_init();
	Timer_init();
	static uint16_t timer = 0;
	static uint16_t timer1 = 0;
	int gn_timeout = 5000;
	int other_timeout = 1500;
	int veto_timeout = 10000;
	static enum{
		INIT = 0,
		TL1_G = 1,
		TL1_Y = 2,
		TL1_R = 3,
		PTL_G = 4,
		PTL_R = 5,
		TL1_R2Y = 6,
	}Status = INIT;
	sei();
	/*
	// Leds used for traffic light
	TL1_Rd = LED1
	TL1_Ye = LED2
	TL1_Gn = Led3

	PTL_Rd = Led7
	PTL_Gn = Led8
	*/
	while(1){
		switch(Status){
			case INIT:
				Led3_On(); // TL1_Gn
				Led7_On(); // PTL_Rd
				Status = TL1_G;
				break;

			case TL1_G:
				if ((getTick()-timer)>=other_timeout){
					timer = getTick();
					Led3_On();
					Led2_Off();
				}
				if(Taster1_get()){
					if ((getTick()-timer1)>=veto_timeout){	// veto pedestrian request -> min 10s green
						timer1 = getTick();
						Status = TL1_Y;
					}
				}
				break;

			case TL1_Y:
				if ((getTick()-timer)>=other_timeout){
					timer = getTick();
					Led3_Off();
					Led2_On();
					Status = TL1_R;
					}
				break;

			case TL1_R:
				if ((getTick()-timer)>=other_timeout){
					timer = getTick();
					Led2_Off();
					Led1_On();
					Status = PTL_G;
				}
				break;

			case PTL_G:
			if ((getTick()-timer)>=other_timeout){
					timer = getTick();
					Led7_Off();
					Led8_On();
					Status=PTL_R;
			}
			break;

			case PTL_R:
			if ((getTick()-timer)>=gn_timeout){
				timer = getTick();
				Led8_Off();
				Led7_On();
				Status = TL1_R2Y;
			}
			break;

			case TL1_R2Y:
			if ((getTick()-timer)>=other_timeout){
				timer = getTick();
				Led1_Off();
				Led2_On();
				Status = TL1_G;
			}
			break;
		}
	}
 }