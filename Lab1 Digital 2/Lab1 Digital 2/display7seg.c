/*
 * display7seg.c
 *
 * Created: 19/01/2026 01:07:02 a. m.
 *  Author: David Carranza
 */ 

#include "display7seg.h"

// Tabla de conversión para cátodo común
static const uint8_t digitos[10][7] = {
	 {1,1,1,1,1,1,0}, // 0
	 {0,1,1,0,0,0,0}, // 1
	 {1,1,0,1,1,0,1}, // 2
	 {1,1,1,1,0,0,1}, // 3
	 {0,1,1,0,0,1,1}, // 4
	 {1,0,1,1,0,1,1}, // 5
	 {1,0,1,1,1,1,1}, // 6
	 {1,1,1,0,0,0,0}, // 7
	 {1,1,1,1,1,1,1}, // 8
	 {1,1,1,1,0,1,1}  // 9
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
	// Apagar todos los segmentos (low para cátodo común)
	PORTD &= ~ (1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C)	|
			 (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F);
	PORTB &= ~ (1 << SEG_G);
}

void display_mostrar_numero(uint8_t numero) {
	// Validar entrada
	if (numero > 9) {
		display_apagar();
		return;
	}
	
	// Apagar display primero
	display_apagar();
	
	// Segmento A
	if (digitos[numero][0] == 1) PORTD |= (1 << SEG_A); 
		else PORTD &= ~ (1 << SEG_A);
	// Segmento B
	if (digitos[numero][1] == 1) PORTD |= (1 << SEG_B);
		else PORTD &= ~ (1 << SEG_B);
	// Segmento C
	if (digitos[numero][2] == 1) PORTD |= (1 << SEG_C);
		else PORTD &= ~ (1 << SEG_C);
	// Segmento D
	if (digitos[numero][3] == 1) PORTD |= (1 << SEG_D);
		else PORTD &= ~ (1 << SEG_D);
	// Segmento E
	if (digitos[numero][4] == 1) PORTD |= (1 << SEG_E);
		else PORTD &= ~ (1 << SEG_E);
	// Segmento F
	if (digitos[numero][5] == 1) PORTD |= (1 << SEG_F);
		else PORTD &= ~ (1 << SEG_F);
	// Segmento G
	if (digitos[numero][6] == 1) PORTB |= (1 << SEG_F);
		else PORTB &= ~ (1 << SEG_G);
}