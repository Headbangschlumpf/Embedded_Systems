/*
 * Ampelschaltung_16MHz.c
 *
 * Created: 25.11.2020 08:30:50
 * Author : Hendrik
 */ 

#include <avr/io.h>
#include "Applications.h"
#include <string.h>
#define F_CPU 16000000

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		drehgeber1();
    }
}

