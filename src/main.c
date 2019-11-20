#include "chip8.h"

int main(int argc, char **argv) {
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

	Chip8State *s = InitChip8();
	fread(s->memory+s->PC, fsize, 1, f);
	fclose(f); 

	// Init emulator
	while(!STOP) {
		EmulateChip8(s);
		s->PC+=2;
		printMem(s, 0);
	}

	return 0;
}