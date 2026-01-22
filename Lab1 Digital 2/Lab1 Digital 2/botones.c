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

// Varaibles para cada botón
static Boton btn_inicio = {BTN_INICIO, 1, 1, 0, 0};
static Boton btn_j1 = {BTN_J1, 1, 1, 0, 0};
static Boton btn_j2 = {BTN_J2, 1, 1, 0, 0};

void botones_init(void) {
	// Configurar botones como entradas con pull-up interno
	DDRC &= ~((1 << BTN_INICIO) | (1 << BTN_J1) | (1 << BTN_J2));
	PORTC |= (1 << BTN_INICIO) | (1 << BTN_J1) | (1 << BTN_J2);
}

void botones_actualizar(uint32_t tiempo_actual) {
	Boton *botones[] = {&btn_inicio, &btn_j1, &btn_j2};
	for (uint8_t i = 0; i < 3; i++)	{
		Boton *btn = botones[i];
		
		// Leer estado actual del botón
		uint8_t estado_leido = (PINC & (1 << btn->pin)) ? 1 : 0;
		
		// Si el estado cambió, actualizar tiempo del último cambio
		if (estado_leido != btn->estado_anterior) {
			btn->ultimo_cambio = tiempo_actual;
			btn->estado_anterior = estado_leido;
		}
		
		// Si ha pasado más de 50ms desde el último cambio
		if ((tiempo_actual - btn->ultimo_cambio) > 50) {
			// Si el estado estable es diferente al estado actual
			if (estado_leido != btn->estado_actual) {
				btn->estado_actual = estado_leido;
				
				// Detectar flanco descendente (presionado)
				if (btn->estado_actual == 0) {
					btn->presionado_flag = 1;
				}
			}
		}
	}
}

