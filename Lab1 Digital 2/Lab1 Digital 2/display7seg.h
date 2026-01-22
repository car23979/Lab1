/*
 * display7seg.h
 *
 * Created: 22/01/2026 01:03:30 a. m.
 *  Author: Admin
 */ 


#ifndef DISPLAY7SEG_H_
#define DISPLAY7SEG_H_

#include "avr/io.h"

// Definiciones de pines de display ánodo común
#define  SEG_A		PIND6	// D6
#define  SEG_B		PIND5	// D5
#define  SEG_C		PIND2	// D2
#define  SEG_D		PIND3	// D3
#define  SEG_E		PIND4	// D4
#define  SEG_F		PIND5	// D7
#define  SEG_G		PINB0	// D8

// Prototipos de funciones
void display_init(void);
void display_mostrar_numero(uint8_t numero);
void display_apagar(void);

#endif /* DISPLAY7SEG.H_H_ */