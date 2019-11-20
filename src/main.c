#include "chip8.h"

int main(int argc, char **argv) {
	// retutns zero on success else non-zero 
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("error initializing SDL: %s\n", SDL_GetError()); 
    } 
    SDL_Window* win = SDL_CreateWindow("GAME", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       32, 64, 0); 

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
	SDL_Event event;
	while(!STOP) {
		// Handle Ctr+C
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT) {
			printf("Termination signal caught, exiting..\n");
			exit(1);
		}

		//Emulat cycle, then move program pointer forward.
		EmulateChip8(s);
		printMem(s, 0);
	}

	return 0;
}