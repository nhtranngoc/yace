#include "keyboard.h"

void PollKeyboard(Chip8State *s) {
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
				case SDLK_1: s->Keys[0x0] = 1; break;
				case SDLK_2: s->Keys[0x1] = 1; break;
				case SDLK_3: s->Keys[0x2] = 1; break;
				case SDLK_4: s->Keys[0x3] = 1; break;

				case SDLK_q: s->Keys[0x4] = 1; break;
				case SDLK_w: s->Keys[0x5] = 1; break;
				case SDLK_e: s->Keys[0x6] = 1; break;
				case SDLK_r: s->Keys[0x7] = 1; break;

				case SDLK_a: s->Keys[0x8] = 1; break;
				case SDLK_s: s->Keys[0x9] = 1; break;
				case SDLK_d: s->Keys[0xa] = 1; break;
				case SDLK_f: s->Keys[0xb] = 1; break;

				case SDLK_z: s->Keys[0xc] = 1; break;
				case SDLK_x: s->Keys[0xd] = 1; break;
				case SDLK_c: s->Keys[0xe] = 1; break;
				case SDLK_v: s->Keys[0xf] = 1; break;
			}
			break;

			case SDL_KEYUP:
			switch(event.key.keysym.sym) {
				case SDLK_1: s->Keys[0x0] = 0; break;
				case SDLK_2: s->Keys[0x1] = 0; break;
				case SDLK_3: s->Keys[0x2] = 0; break;
				case SDLK_4: s->Keys[0x3] = 0; break;

				case SDLK_q: s->Keys[0x4] = 0; break;
				case SDLK_w: s->Keys[0x5] = 0; break;
				case SDLK_e: s->Keys[0x6] = 0; break;
				case SDLK_r: s->Keys[0x7] = 0; break;

				case SDLK_a: s->Keys[0x8] = 0; break;
				case SDLK_s: s->Keys[0x9] = 0; break;
				case SDLK_d: s->Keys[0xa] = 0; break;
				case SDLK_f: s->Keys[0xb] = 0; break;

				case SDLK_z: s->Keys[0xc] = 0; break;
				case SDLK_x: s->Keys[0xd] = 0; break;
				case SDLK_c: s->Keys[0xe] = 0; break;
				case SDLK_v: s->Keys[0xf] = 0; break;
			}
			break;

			case SDL_QUIT:
			printf("Termination signal caught, exiting..\n");
			exit(0);
			break;

		}
	}
}