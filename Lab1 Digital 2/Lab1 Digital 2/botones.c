/*
 * botones.c
 *
 * Created: 19/01/2026 01:07:32 a. m.
 *  Author: David Carranza
 */ 

#include "botones.h"
#include <avr/io.h>

// Estructura para manejo de botón con anti-rebote
typedef struct {
	uint8_t pin;				// Pin del botón
	uint8_t estado_anterior;	// Estado anterior (detección de flanco)
	uint8_t estado_actual;		// Estado actual (filtrado)
	uint8_t presionado_flag;	// Bandera de botón presionado
	uint8_t ultimo_cambio;		// Tiempo del último cambio
	} Boton;