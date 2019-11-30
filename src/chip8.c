#include "chip8.h"

char STOP = 0;
int icount = 0;
char draw;
uint8_t font4x5[] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, 	// 0
	0x20, 0x60, 0x20, 0x20, 0x70,	// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, 	// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
	0xF0, 0x80, 0xF0, 0x80, 0x80	// F
};

Chip8State *InitChip8(void) {
	Chip8State *s = calloc(sizeof(Chip8State), 1);

	s->memory = calloc(1024*4, 1);
	s->screen = &s->memory[0xf00];
	s->SP = 0xFA0; // Why? The specs didn't mention this.
	s->PC = 0x200;
	s->I = 0;
	s->Wait = false;

	// Copy font to memory, starting at memory[0]
	memcpy(&s->memory[0], font4x5, 5*16);

	return s;
}

void EmulateChip8(Chip8State *s) {
	uint8_t *opcode = &s->memory[s->PC];
	uint8_t firstnib = (opcode[0] >> 4);
	uint8_t lastnib = (opcode[1] & 0x0f);
	uint16_t nnn = ((opcode[0] & 0x0f) << 8) | opcode[1];
	uint8_t nn = nnn & 0xff;
	uint8_t n = nn & 0x0f;
	uint8_t x = opcode[0] & 0x0f;
	uint8_t y = (opcode[1] & 0xf0) >> 4;

	printf("%04x %02x %02x ", s->PC, opcode[0], opcode[1]);
	switch(firstnib) {
		case 0x00: Op0(s, nnn); 			break;
		case 0x01: Op1(s, nnn); 			break;
		case 0x02: Op2(s, nnn); 			break;
		case 0x03: Op3(s, x, nn); 			break;
		case 0x04: Op4(s, x, nn); 			break;
		case 0x05: Op5(s, x, y); 			break;
		case 0x06: Op6(s, x, nn); 			break;
		case 0x07: Op7(s, x, nn); 			break;
		case 0x08: Op8(s, x, y, lastnib); 	break;
		case 0x09: Op9(s, x, y); 			break;
		case 0x0a: OpA(s, nnn); 			break;
		case 0x0b: OpB(s, nnn); 			break;
		case 0x0c: OpC(s, x, nn); 			break;
		case 0x0d: OpD(s, x, y, n); 		break;
		case 0x0e: OpE(s, opcode[1], x); 	break;
		case 0x0f: OpF(s, opcode[1], x); 	break;
		default:
		printf("Invalid instruction.");
	}

	if(s->Delay > 0) {
		s->Delay--;
	}

	printf("\n");
	icount++;
}

void Op0(Chip8State *s, uint16_t nnn) {
	switch(nnn) {
		case 0x00E0:
		printf("%-10s", "CLS"); 
		memset(s->screen, 0, (SCREEN_WIDTH * SCREEN_HEIGHT)/8);
		draw = 1;
		s->PC += 2;
		break;
		case 0x00EE:
		printf("%-10s", "RET"); 
		s->PC = (s->memory[s->SP] << 8) | (s->memory[s->SP+1]);
		s->SP += 2;
		break;
		default:
		STOP = 1; // When it get to this it basically crashes.
		printf("%-10s #$%03x", "SYS", nnn); 
		break;
	}
}

void Op1(Chip8State *s, uint16_t nnn) {
	printf("%-10s #%03x", "JP", nnn); 
	s->PC = nnn;
}

void Op2(Chip8State *s, uint16_t nnn) {
	printf("%-10s #%03x", "CALL", nnn); 
	s->SP -= 2;
	s->memory[s->SP]   = ((s->PC+2) & 0xFF00) >> 8;
	s->memory[s->SP+1] =  (s->PC+2) & 0xFF;
	s->PC = nnn;
}

void Op3(Chip8State *s, uint8_t x, uint8_t nn) {
	printf("%-10s V%01x, #$%02x", "SE", x, nn); 
	s->PC += (s->V[x] == nn) ? 4 : 2;
}

void Op4(Chip8State *s, uint8_t x, uint8_t nn) {
	printf("%-10s V%01x, #$%02x", "SNE", x, nn); 
	s->PC += (s->V[x] != nn) ? 4 : 2;
}

void Op5(Chip8State *s, uint8_t x, uint8_t y) {
	printf("%-10s V%01x, V%01x", "SE", x, y); 
	s->PC += (s->V[x] == s->V[y]) ? 4 : 2;
}

void Op6(Chip8State *s, uint8_t x, uint8_t nn) {
	printf("%-10s V%01x, #$%02x", "LD", x, nn); 
	s->V[x] = nn;
	s->PC += 2;
}

void Op7(Chip8State *s, uint8_t x, uint8_t nn) {
	printf("%-10s V%01x, #$%02x", "ADD", x, nn); 
	s->V[x] +=nn;
	s->PC += 2;
}

void Op8(Chip8State *s, uint8_t x, uint8_t y, uint8_t lastnib) {
	switch(lastnib) {
		case 0x00:
		printf("%-10s V%01x, V%01x", "LD", x, y); 
		s->V[x] = s->V[y];
		s->PC += 2;
		break;
		case 0x01:
		printf("%-10s V%01x, V%01x", "OR", x, y); 
		s->V[x] |= s->V[y];
		s->PC += 2;
		break;
		case 0x02:
		printf("%-10s V%01x, V%01x", "AND", x, y); 
		s->V[x] &= s->V[y];
		s->PC += 2;
		break;
		case 0x03:
		printf("%-10s V%01x, V%01x", "XOR", x, y); 
		s->V[x] ^= s->V[y];
		s->PC += 2;
		break;
		case 0x04:
		printf("%-10s V%01x, V%01x", "ADD", x, y); 
		uint16_t sum = s->V[x] + s->V[y];
		s->V[0xf] = (sum > 255) ? 1 : 0;
		s->V[x] = sum & 0xff;
		s->PC += 2;
		break;
		case 0x05:
		printf("%-10s V%01x, V%01x", "SUB", x, y); 
		s->V[0xf] = (s->V[x] > s->V[y]) ? 1 : 0;
		s->V[x] -= s->V[y];
		s->PC += 2;
		break;
		case 0x06:
		printf("%-10s V%01x{, V%01x}", "SHR", x, y); 
		s->V[0xf] = (s->V[x] & 1) ? 1 : 0;
		s->V[x] /= 2;
		s->PC += 2;
		break;
		case 0x07:
		printf("%-10s V%01x, V%01x", "SUBN", x, y); 
		s->V[0xf] = (s->V[y] > s->V[x]) ? 1 : 0;
		s->V[y] -= s->V[x];
		s->PC += 2;
		break;
		case 0x0e:
		printf("%-10s V%01x{, V%01x}", "SHL", x, y); 
		s->V[0xf] = (s->V[x] & (1 << 8)) ? 1 : 0;
		s->V[x] *= 2;
		s->PC += 2;
		break;
		default:
		printf("Invalid instruction 8");
	}
}

void Op9(Chip8State *s, uint8_t x, uint8_t y) {
	printf("%-10s V%01x, V%01x", "SNE", x, y);
	s->PC += (s->V[x] != s->V[y]) ? 4 : 2;
}

void OpA(Chip8State *s, uint16_t nnn) {
	printf("%-10s %s, #$%03x", "LD", "I", nnn); 
	s->I = nnn;
	s->PC += 2;
}

void OpB(Chip8State *s, uint16_t nnn) {
	printf("%-10s %s, #%03x", "JP", "V0", nnn); 
	s->PC = nnn + s->V[0x0];
}

void OpC(Chip8State *s, uint8_t x, uint8_t nn) {
	printf("%-10s V%01x, #$%02x", "RND", x, nn); 
	srand(time(0));
	uint8_t rng = (rand() % 256);
	s->V[x] = rng & nn;	
	s->PC += 2;		
}

void OpD(Chip8State *s, uint8_t x, uint8_t y, uint8_t n) {
	printf("%-10s V%01x, V%01x, #$%01x", "DRW", x, y, n); 
	//Draw
	s->V[0xf] = 0;
	for(int i=0;i<n;i++) {
		for(int j=0;j<8;j++) {
			bool src_pixel = GetBitChip8(&s->memory[s->I+i], j);
			bool des_pixel = GetPixelChip8(s, s->V[x]+j, s->V[y]+i);

			if(src_pixel && des_pixel) {
				s->V[0xf] = 1;
			}

			SetPixelChip8(s, s->V[x]+j, s->V[y]+i, src_pixel ^ des_pixel);
		}
	}
	draw = 1;
	s->PC += 2;
}

void OpE(Chip8State *s, uint8_t opcode, uint8_t x) {
	switch(opcode) {
		case 0x9E:
		printf("%-10s V%01x", "SKP", x); 
		s->PC += (s->Keys[s->V[x]] != 0) ? 4 : 2;
		break;
		case 0xA1:
		printf("%-10s V%01x", "SKNP", x); 
		s->PC += (s->Keys[s->V[x]] == 0) ? 4 : 2;
		break;
		default:
		printf("Invalid instruction E"); break;
	}
}

void OpF(Chip8State *s, uint8_t opcode, uint8_t x) {
	switch(opcode) {
		case 0x07:
		printf("%-10s V%01x, %s", "LD", x, "DT"); 
		s->V[x] = s->Delay;
		s->PC += 2;
		break;
		case 0x0A:
		printf("%-10s V%01x, %s", "LD", x, "K"); 
		s->Wait = true; 

		for(int i=0; i<16; i++) {
			if(s->Keys[i] == 1) {
				s->Keys[i] = 0;
				s->V[x] = i;
				s->Wait = false;
			}
		}

		if(s->Wait == true) {
			return;
		}

		s->PC += 2;

		break;
		case 0x15:
		printf("%-10s %s, V%01x", "LD", "DT", x); 
		s->Delay = s->V[x];
		s->PC += 2;
		break;
		case 0x18:
		printf("%-10s %s, V%01x", "LD", "ST", x); 
		s->Sound = s->V[x];
		s->PC += 2;
		break;
		case 0x1E:
		printf("%-10s %s, V%01x", "ADD", "I", x); 
		s->I += s->V[x];
		s->PC += 2;
		break;
		case 0x29:
		printf("%-10s %s, V%01x", "LD", "F", x); 
		s->I = s->V[x] * 5;
		s->PC += 2;
		break;
		case 0x33:
		printf("%-10s %s, V%01x", "LD", "B", x);
		uint8_t bcd = s->V[x];
		s->memory[s->I]   =  bcd / 100;
		s->memory[s->I+1] = (bcd / 10) % 10;
		s->memory[s->I+2] = (bcd % 100) % 10;
		s->PC += 2;
		break;
		case 0x55:
		printf("%-10s %s, V%01x", "LD", "[I]", x); 
		for(int i = 0; i <= x; i++) {
			s->memory[s->I+i] = s->V[i];
		}
		s->PC += 2;
		break;
		case 0x65:
		printf("%-10s V%01x, %s", "LD", x, "[I]"); 
		for(int i = 0; i <= x; i++) {
			s->V[i] = s->memory[s->I+i];
		}
		s->PC += 2;
		break;
		default:
		printf("Invalid instruction F");
	}
}

// Get current screen pixel, 64x32 coordinate
bool GetPixelChip8(Chip8State *s, int x, int y) {
	int byteIndex = x/8 + y*8; // 8 bytes per line
	int bitIndex = x % 8; 	 // 8 bits per byte
	return GetBitChip8(&s->screen[byteIndex], bitIndex);
}

// Set pixel to screen
void SetPixelChip8(Chip8State *s, int x, int y, bool val) {
	int byteIndex = x/8 + y*8;
	int bitIndex = x % 8;

	SetBitChip8(&s->screen[byteIndex], bitIndex, val);
}

// Get the nth bix of a byte
bool GetBitChip8(uint8_t *bytes, int index) {
	return (*bytes >> (7 - index)) & 1;
}

void SetBitChip8(uint8_t *byte, int index, bool val) {
	*byte = (*byte & ~(1 << (7 - index))) | (val << (7 - index));
}

// Print memory nicely.
void printMem(Chip8State *s, int printFull) {
	printf("Instruction count: %d\n", icount);
	printf("SP:  %04x\n", s->SP);
	printf("STK: %04x\n", s->memory[s->SP]);
	printf("PC:  %04x\n", s->PC);
	printf("I:   %02x\n", s->I);
	printf("DT:  %d\n"  , s->Delay);
	printf("ST:  %d\n"  , s->Sound);
	for(int i = 0; i < 16; i++) {
		printf("V%-2d: %02x\t", i, s->V[i]);
		if(i % 2 == 1 && i > 0) {
			puts("");
		}
	}

	puts("");

	switch(printFull) {
		case 1:
		for(int i = 0; i < 4096; i++) {
			if(i % 16 == 0) {
				puts("");
				printf("%04x\t", i);
			}

			printf("%02X", s->memory[i]);
			if(s->PC == i) {
				printf("*");
			} else printf(" ");
			if(i == 0x1FF) {
				puts("");
			}
		}	
		break;
		case 2:
		for(int i = 0; i < 256; i++) {
			if(i % 8 == 0) {
				puts("");
				printf("%04x\t", i+0xf00);
			}

			printf("%02X ", s->screen[i]);

			// for(int j = 0; j < 8; j++) {
			// 	int bit = (s->screen[i] >> (7-j)) & 0x01;
			// 	if(bit) {
			// 		printf("_");
			// 	} else printf("X");
			// }
		}
		break;
		default:
		return;
	}

	puts("");
}