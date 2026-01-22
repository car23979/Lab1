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
