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