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

