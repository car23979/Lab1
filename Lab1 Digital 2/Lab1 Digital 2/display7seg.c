/*
 * display7seg.c
 *
 * Created: 19/01/2026 01:07:02 a. m.
 *  Author: David Carranza
 */ 

#include "display7seg.h"

// Tabla de conversión para ánodo común
static const uint8_t digitos[10][7] = {
	{1,1,1,1,1,1,0},	// 0
	{0,1,1,0,0,0,0},	// 1
	{1,1,0,1,1,0,1},	// 2
	{1,1,1,1,0,0,1},	// 3
	{0,1,1,0,0,1,1},	// 4
	{1,0,1,1,0,1,1},	// 5
	{1,0,1,1,1,1,1},	// 6
	{1,1,1,0,0,0,0},	// 7
	{1,1,1,1,1,1,1},	// 8
	{1,1,1,1,0,1,1},	// 9
};

void display_init(void) {
	// Configurar pines de segmentos como salidas
	DDRD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) |
			(1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F);
	DDRB |= (1 << SEG_G);
	
	// Apagar display inicialmente
	display_apagar();
}

void display_apagar(void) {
	// Apagar todos los segmentos (High para ánodo común)
	PORTD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C)	|
			 (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F);
	PORTB |= (1 << SEG_G);
}