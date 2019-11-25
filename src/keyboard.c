#include "keyboard.h"

void PollKeyboard() {
	while(SDL_PollEvent(&event)) {
			// Handle Ctr+C
		if(event.type == SDL_QUIT) {
			printf("Termination signal caught, exiting..\n");
			exit(0);
		}
	}
}