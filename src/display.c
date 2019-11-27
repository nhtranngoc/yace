#include "display.h"

int scale;
char draw;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *screen = NULL;
SDL_Texture *texture = NULL;

void InitDisplay(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
		printf("error initializing SDL: %s\n", SDL_GetError()); 
	} 

	scale = 10;

	window = SDL_CreateWindow("YACE - Yet another CHIP-8 Emulator",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH * scale,
		SCREEN_HEIGHT * scale,
		SDL_WINDOW_RESIZABLE);

	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_INDEX1MSB, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale);
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale, 8, 0,0,0,0);
}

void RenderDisplay(Chip8State *s) {
	if(draw) {
			//Draw stuff here
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.h = SCREEN_WIDTH * scale;
		rect.w = SCREEN_HEIGHT * scale;

		for(int i=0;i<256;i++) {
			int display_byte = s->screen[i];
			int coord_x = i % 8;
			int coord_y = i / 8;
			for(int j=0;j<8;j++) {
				rect.x = (coord_x*8 + j) * scale;
				rect.y = coord_y * scale;
				rect.w = scale;
				rect.h = scale;

				int display_bit = (s->screen[i] >> (7-j)) & 0x01;

				if(display_bit == 1) {
					SDL_SetRenderDrawColor(renderer, 255,255,255,0);
				} else if(display_bit == 0){
					SDL_SetRenderDrawColor(renderer, 0,0,0,0);
				} else {
					SDL_SetRenderDrawColor(renderer, 255,0,0,0);
				}
				SDL_RenderFillRect(renderer, &rect);
			}
		}
		SDL_RenderPresent(renderer);
	}

	draw = 0;
}

// Set bit at coordinate (x,y) - SCREEN_WIDTH x SCREEN_HEIGHT 
 void setBit(Chip8State *s, int x, int y) {
	// Get byte pos
	int _x = x/8;
	// Get bit pos
	int _x1 = x%8;
	// Get index - 8xSCREEN_HEIGHT 
 	int index = _x + y*8;

	s->screen[index] = 1 << _x1;

}