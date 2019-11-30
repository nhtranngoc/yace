#include "main.h"

int main(int argc, char **argv) {
	int clockFreq = 60; //Hz
	// Initialize screen

	InitDisplay();
	Chip8State *s = InitChip8();

	// Load program
	FILE *f = fopen(argv[1], "rb");

	if(f == NULL) {
		printf("Error: Can't open file or file not found: %s\n", argv[1]);
		exit(1);
	}

	// Copy program into memory
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	fread(s->memory+s->PC, fsize, 1, f);
	fclose(f); 

	draw = 0;

	// Init emulator
	while(1) {
		PollKeyboard(s);
		RenderDisplay(s);

		//Emulate cycle, print debug info
		EmulateChip8(s);
		// printMem(s, 2);
		// 
		//Emulate clock.
		SDL_Delay(1000/clockFreq);
	}

	return 0;
}