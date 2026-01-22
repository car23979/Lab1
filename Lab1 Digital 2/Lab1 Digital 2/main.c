/*
 * Lab1 Digital 2.c
 *
 * Created: 19/01/2026 12:54:50 a. m.
 * Author : David Carranza
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "display7seg.h"
#include "botones.h"

// Configuración de puertos
// LEDs Jugador 1 - PORTB (4 leds para contador binario)
#define J1_LED0		PINB1	// D9	- bit 0
#define J1_LED1		PINB2	// D10	- bit 1
#define J1_LED2		PINB3	// D11	- bit 2
#define J1_LED3		PINB4	// D12	- bit 3

// LEDs Jugador 2 - PORTB y PORTC (4 leds para contador binario)
#define J2_LED0		PINB5	// D13	- bit 0
#define J2_LED1		PINC0	// A0	- bit 1
#define J2_LED2		PINC1	// A1	- bit 2
#define J2_LED3		PINC2	// A2	- bit 3


// Varaibles Globales
// Estados del juego
typedef enum {
	ESPERANDO_INICIO,
	CONTEO_REGRESIVO,
	CARRERA_EN_CURSO,
	CARRERA_TERMINADA
	} EstadoJuego;

EstadoJuego estadoActual = ESPERANDO_INICIO

// Contadores
volatile uint8_t contadorRegresivo = 5;
volatile uint8_t contadorJugador1 = 0;
volatile uint8_t contadorJugador2 = 0;
volatile uint8_t ganador = 0;	// 0 es ninguno, 1 = J1, 2=J2, 3 es empate

// Meta
#define META 9 // 9 es 1001 en binario

// Variables para el problema de inicialización del conteo
volatile uint8_t inicioConteoFlag = 0;
volatile uint8_t tiempoInicioConteo = 0;

// Variables de tiempo
volatile uint32_t milisegundos = 0;
volatile uint8_t segundoCompleto = 0;

// Funciones de Configuración
void configurarPuertos(void) {
	// Los segmentos del display se configuran en display7seg_init()
	
	// Configuración de LEDs J1 como salidas
	DDRB |= (1 << J1_LED0) | (1 << J1_LED1) |
			(1 << J1_LED2) | (1 << J1_LED3);
	
	// Configuración de LEDs J2 como salidas
	DDRB |= (1 << J2_LED0);
	DDRC |= (1 << J1_LED1) | (1 << J1_LED2) | (1 << J1_LED3);
	
	// Los botones se configuran en botones_init()		
}

// Funciones LEDs
void actualizarLEDsJugador1(uint8_t valor) {
	// Mostrar valor binario en LEDs J1
	if (valor & 0x01) PORTB |= (1 << J1_LED0);
		else PORTB &= ~(1 << J1_LED0);
	if (valor & 0x02) PORTB |= (1 << J1_LED1);
		else PORTB &= ~(1 << J1_LED1);
	if (valor & 0x04) PORTB |= (1 << J1_LED2);
		else PORTB &= ~(1 << J1_LED2);
	if (valor & 0x08) PORTB |= (1 << J1_LED3);
		else PORTB &= ~(1 << J1_LED3);
}