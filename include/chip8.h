#ifndef CHIP8_H_
#define CHIP8_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

extern char STOP;
extern int icount;

typedef struct Chip8State {
	uint8_t V[16];
	uint16_t I;
	uint8_t Delay;
	uint8_t Sound;
	uint16_t PC;
	uint16_t SP;
	uint8_t Keys[16];

	uint8_t *memory;
	uint8_t *screen;
} Chip8State;

Chip8State *InitChip8(void);
void EmulateChip8(Chip8State *s);

bool GetPixelChip8(Chip8State *s, int x, int y);
void SetPixelChip8(Chip8State *s, int x, int y, bool val);
bool GetBitChip8(uint8_t *bytes, int index);
void SetBitChip8(uint8_t *bytes, int index, bool val);

void printMem(Chip8State *s, int printFull);

#endif // CHIP8_H_