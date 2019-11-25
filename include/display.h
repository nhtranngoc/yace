#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "SDL2/SDL.h"
#include "chip8.h"
#include <stdio.h>

extern char draw;
extern int scale;

void InitDisplay(void);
void RenderDisplay(Chip8State *s);

#endif // DISPLAY_H_