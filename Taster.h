#ifndef TASTER_H_
#define TASTER_H_

#include <avr/io.h>

/*
Initialisiert die Hardware für die Taster
*/
void Taster_init(void);

/*
Gibt den aktuellen Status der Tasten aus. 
Rückgabewert 0 = Taster nicht gedrückt, 1 = Taster gedrückt.
*/
uint8_t Taster1_get(void);
uint8_t Taster2_get(void);
uint8_t Taster3_get(void);
uint8_t Taster4_get(void);


#endif /* TASTER_H_ */