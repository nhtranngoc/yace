#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdio.h>
#include "SDL2/SDL.h"
#include "chip8.h"

SDL_Event event;

void PollKeyboard(Chip8State *s);

#endif // KEYBOARD_H_