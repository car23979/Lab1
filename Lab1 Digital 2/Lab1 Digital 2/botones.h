/*
 * botones.h
 *
 * Created: 19/01/2026 01:08:34 a. m.
 *  Author: David Carranza
 */ 


#ifndef BOTONES_H_
#define BOTONES_H_

#include <stdint.h>

// Definiciones de pines de botones
#define BTN_INICIO		PINC3	// A3
#define BTN_J1			PINC5	// A4
#define BTN_J2			PINC4	// A5

// Prototipos de funciones
void botones_init(void);
void botones_actualizar(uint32_t tiempo_actual);
uint8_t boton_inicio_presionado(void);
uint8_t boton_j1_presionado(void);
uint8_t boton_j2_presionado(void);

#endif /* BOTONES_H_ */