/*
 * display7seg.c
 *
 * Created: 19/01/2026 01:07:02 a. m.
 *  Author: David Carranza
 */ 

#include "display7seg.h"

// TABLA PARA ÁNODO COMÚN (0=encendido, 1=apagado)
// Orden de segmentos: A, B, C, D, E, F, G
static const uint8_t digitos[10][7] = {
	// A  B  C  D  E  F  G
	{0,0,0,0,0,0,1}, // 0
	{1,0,0,1,1,1,1}, // 1
	{0,0,1,0,0,1,0}, // 2
	{0,0,0,0,1,1,0}, // 3
	{1,0,0,1,1,0,0}, // 4
	{0,1,0,0,1,0,0}, // 5
	{0,1,0,0,0,0,0}, // 6
	{0,0,0,1,1,1,1}, // 7
	{0,0,0,0,0,0,0}, // 8
	{0,0,0,0,1,0,0}  // 9
};

void display_init(void) {
	// Configurar pines como salidas
	DDRD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) |
	(1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F);
	DDRB |= (1 << SEG_G);
	
	// Apagar display inicialmente
	display_apagar();
}

void display_apagar(void) {
	// ÁNODO COMÚN: HIGH = apagado, LOW = encendido
	// Para apagar: poner todos los segmentos en HIGH
	PORTD |= (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) |
	(1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F);
	PORTB |= (1 << SEG_G);
}

void display_mostrar_numero(uint8_t numero) {
	// Validar entrada
	if (numero > 9) {
		display_apagar();
		return;
	}
	
	// Apagar todos los segmentos primero
	display_apagar();
	
	// Segmento A
	if (digitos[numero][0] == 0) PORTD &= ~(1 << SEG_A); else PORTD |= (1 << SEG_A);
	// Segmento B
	if (digitos[numero][1] == 0) PORTD &= ~(1 << SEG_B); else PORTD |= (1 << SEG_B);
	// Segmento C
	if (digitos[numero][2] == 0) PORTD &= ~(1 << SEG_C); else PORTD |= (1 << SEG_C);
	// Segmento D
	if (digitos[numero][3] == 0) PORTD &= ~(1 << SEG_D); else PORTD |= (1 << SEG_D);
	// Segmento E
	if (digitos[numero][4] == 0) PORTD &= ~(1 << SEG_E); else PORTD |= (1 << SEG_E);
	// Segmento F
	if (digitos[numero][5] == 0) PORTD &= ~(1 << SEG_F); else PORTD |= (1 << SEG_F);
	// Segmento G
	if (digitos[numero][6] == 0) PORTB &= ~(1 << SEG_G); else PORTB |= (1 << SEG_G);
}