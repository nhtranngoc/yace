#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "SDL2/SDL.h"

extern char STOP;
extern int icount;

typedef struct Chip8State {
	uint8_t V[16];
	uint16_t I;
	uint8_t Delay;
	uint8_t Sound;
	uint16_t PC;
	uint16_t SP;

	uint8_t *memory;
	uint8_t *screen;
} Chip8State;

Chip8State *InitChip8(void);
void EmulateChip8(Chip8State *s);
void printMem(Chip8State *s, int printFull);

#endif