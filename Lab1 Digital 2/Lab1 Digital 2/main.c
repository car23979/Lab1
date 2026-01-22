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

EstadoJuego estadoActual = ESPERANDO_INICIO;

// Contadores

volatile uint8_t contadorJugador2 = 0;
volatile uint8_t ganador = 0;	// 0 es ninguno, 1 = J1, 2=J2, 3 es empate
volatile uint8_t contadorRegresivo = 5;
volatile uint8_t contadorJugador1 = 0;

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
// LEDs J1
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

// LEDs J2
void actualizarLEDsJugador2(uint8_t valor) {
	// Mostrar valor binario en LEDs J2
	if (valor & 0x01) PORTB |= (1 << J1_LED0);
	else PORTB &= ~(1 << J2_LED0);
	if (valor & 0x02) PORTC |= (1 << J2_LED1);
	else PORTC &= ~(1 << J2_LED1);
	if (valor & 0x04) PORTC |= (1 << J2_LED2);
	else PORTC &= ~(1 << J2_LED2);
	if (valor & 0x08) PORTC |= (1 << J2_LED3);
	else PORTC &= ~(1 << J2_LED3);
}

void apagarTodosLEDs(void) {
	// Apagar LEDs J1
	PORTB &= ~((1 << J1_LED0) | (1 << J1_LED1) | (1 << J1_LED2) | (1 << J1_LED3));
	// Apagar LEDs J2
	PORTB &= ~(1 << J2_LED0);
	PORTC &= ~((1 << J2_LED1) | (1 << J2_LED2) | (1 << J2_LED3));
}

// Configuración De Timers
void configurarTimers(void) {
	// Timer0 para interrupción cada 1ms (base de tiempo)
	TCCR0A = (1 << WGM01);				// Modo CTC
	TCCR0B = (1 << CS01) | (1 << CS00);	// Prescaler 64
	OCR0A = 249;						// 16MHz/64/1000 = 250 -> 250-1 = 249
	TIMSK0 = (1 << OCIE0A);				// Habilitar interrupción
	
	// Timer1 para interrupción cada 1 segundo (conteo regresivo)
	TCCR1B = (1 << WGM12);					// Modo CTC
	TCCR1B |= (1 << CS12) | (1 << CS10);	// Prescaler 1024
	OCR1A = 15624;							// 16MHz/1024 = 15625Hz -> 15625-1 = 15624
	// Timer1 inicialmente desactivado
}

// Interrupciones
// Timer0: Interrupción cada 1ms
ISR(TIMER0_COMPA_vect) {
	milisegundos++;
}

// Timer1: Interrupción cada 1s
ISR(TIMER1_COMPA_vect) {
	segundoCompleto = 1;
}

// Función para tiempo
uint32_t obtenerMillis(void) {
	uint32_t m;
	cli();
	m = milisegundos;
	sei();
	return m;
}

// Maquina de estados
void manejarEsperaInicio(void) {
	// Mostrar 0 en display
	display_mostrar_numero(0);
	
	// Apagar todos los LEDs y resetear contadores
	apagarTodosLEDs();
	contadorJugador1 = 0;
	contadorJugador2 = 0;
	ganador = 0;
	
	// Verificar botón de inicio
	if (boton_inicio_presionado()) {
		estadoActual = CONTEO_REGRESIVO;
		contadorRegresivo = 5;
		tiempoInicioConteo = 0;
		inicioConteoFlag = 1;	// Indicar que el conteo acaba de empezar
		
		// Configurar Timer1 para 1 segundo
		TCNT1 = 0;					// Reiniciar contador
		TIMSK1 |= (1 << OCIE1A);	// Habilitar interrupción
		
		// Mostrar primer número 5
		display_mostrar_numero(5);
	}
}

void manejarConteoRegresivo(void) {
	static uint32_t ultimoCambio = 0;
	uint32_t tiempoActual = obtenerMillis();
	
	// Verificar si ha pasado 1 segundo desde el último cambio
	if (segundoCompleto) {
		segundoCompleto = 0; obtenerMillis();
		
		// Si es el inicio, asegurar que mostramos 5 por al menos 1 segundo
		if (inicioConteoFlag) {
			inicioConteoFlag = 0;
			ultimoCambio = tiempoActual;
			return; // Mantener 5 por un segundo completo
		}
		
		// Decrementar contador
		if (contadorRegresivo > 0) {
			contadorRegresivo--;
			display_mostrar_numero(contadorRegresivo);
			
			if (contadorRegresivo == 0) {
				// Fin del conteo, comenzar carrera
				estadoActual = CARRERA_EN_CURSO;
				display_apagar();	// Apagar display durante carrera
				// Desactivar Timer1 ya que no lo necesitamos más
				TIMSK1 &= ~(1 << OCIE1A);
			}
		}
	}
}

void manejarCarrera(void) {
	// Botón J1
	if (boton_j1_presionado()) {
		if (contadorJugador1 < META) {
			contadorJugador1++;
			actualizarLEDsJugador1(contadorJugador1);
			
			// Verificar si ganó
			if (contadorJugador1 == META) {
				ganador = 1;
				estadoActual = CARRERA_TERMINADA;
			}
		}
	}
	
	// Botón J2
	if (boton_j2_presionado()) {
		if (contadorJugador2) {
			contadorJugador2++;
			actualizarLEDsJugador2(contadorJugador2);
			
			// Verificar si ganó
			if (contadorJugador2 == META) {
				ganador = 2;
				estadoActual = CARRERA_TERMINADA;
			}
		}
	}
	
	// Verificar empate
	if (contadorJugador1 == META && contadorJugador2 == META) {
		ganador = 3;
		estadoActual = CARRERA_TERMINADA;
	}
}

void manejarFinCarrera(void) {
	static uint8_t mostrarGanador = 1;
	static uint32_t ultimoParpadeo = 0;
	
	// Mostrar número del ganador en display
	if (ganador == 1) {
		display_mostrar_numero(1);
		
		// Encender todos los LEDs del jugador 1
		actualizarLEDsJugador1(0x0F);	// Todos los bits en 1
		
		// Apagar LEDs del jugador 2
		actualizarLEDsJugador2(0x00);	
	} else if (ganador == 2) {
		display_mostrar_numero(2);
		
		// Encender todos los LEDs del jugador 2
		actualizarLEDsJugador2(0x0F);	// Todos los bits en 1
		
		// Apagar LEDs del Jugador 1
		actualizarLEDsJugador1(0x00);
	}else if (ganador == 3) {
		
		// Empate se va a mostrar 0 y parpater ambos jugadores
		uint32_t tiempoActual = obtenerMillis();
		
		if (tiempoActual - ultimoParpadeo > 500) {	//Parpadeo cada 500ms
			ultimoParpadeo = tiempoActual;
			mostrarGanador = !mostrarGanador;
		}
		
		if (mostrarGanador) {
			display_mostrar_numero(0)
		}
	}
}