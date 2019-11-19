#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Chip8State {
	uint8_t V[16];
	uint16_t I;
	uint8_t Delay;
	uint8_t Sound;
	uint16_t PC;
	uint16_t SP;

	uint8_t *memory;
	uint8_t *screen;
} Chip8State;


Chip8State *InitChip8(void);
void EmulateChip8(Chip8State *s);
void printMem(Chip8State *s);

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

	printMem(s);

	// Init emulator
	// while(1) {
	for(int i = 0; i < 10; i++) {
		EmulateChip8(s);
		s->PC+=2;
		printMem(s);
	}

	return 0;
}

Chip8State *InitChip8(void) {
	Chip8State *s = calloc(sizeof(Chip8State), 1);

	s->memory = calloc(1024*4, 1);
	s->screen = &s->memory[0xf00];
	s->SP = 0xFA0; // Why? The specs didn't mention this.
	s->PC = 0x200;
	s->I = 0;

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
		case 0x00:
			switch(nnn) {
				case 0x00E0:
				printf("%-10s", "CLS"); break;
				case 0x00EE:
				printf("%-10s", "RET"); break;
				default:
				printf("%-10s #$%03x", "SYS", nnn); 
				break;
			}
			break;
		case 0x01:
			printf("%-10s #%03x", "JP", nnn); 
			s->PC = nnn;
			break;
		case 0x02:
			printf("%-10s #%03x", "CALL", nnn); 
			s->memory[s->SP] = s->PC;
			s->SP++;
			s->PC = nnn;
			break;
		case 0x03:
			printf("%-10s V%01x, #$%02x", "SE", x, nn); 
			if(s->V[x] == nn) {
				s->PC+=2;
			}
			break;
		case 0x04:
			printf("%-10s V%01x, #$%02x", "SNE", x, nn); 
			if(s->V[x] != nn) {
				s->PC+=2;
			}
			break;
		case 0x05:
			printf("%-10s V%01x, V%01x", "SE", x, y); 
			if(s->V[x] == s->V[y]) {
				s->PC+=2;
			}
			break;
		case 0x06:
			printf("%-10s V%01x, #$%02x", "LD", x, nn); 
			s->V[x] = nn;
			break;
		case 0x07:
			printf("%-10s V%01x, #$%02x", "ADD", x, nn); 
			s->V[x] +=nn;
			break;
		case 0x08:
			switch(lastnib) {
				case 0x00:
					printf("%-10s V%01x, V%01x", "LD", x, y); 
					s->V[x] = s->V[y];
					break;
				case 0x01:
					printf("%-10s V%01x, V%01x", "OR", x, y); 
					s->V[x] ^= s->V[y];
					break;
				case 0x02:
					printf("%-10s V%01x, V%01x", "AND", x, y); 
					break;
				case 0x03:
					printf("%-10s V%01x, V%01x", "XOR", x, y); break;
				case 0x04:
					printf("%-10s V%01x, V%01x", "ADD", x, y); break;
				case 0x05:
					printf("%-10s V%01x, V%01x", "SUB", x, y); break;
				case 0x06:
					printf("%-10s V%01x{, V%01x}", "SHR", x, y); break;
				case 0x07:
					printf("%-10s V%01x, V%01x", "SUBN", x, y); break;
				case 0x0e:
					printf("%-10s V%01x{, V%01x}", "SHL", x, y); break;
				default:
					printf("Invalid instruction 8");
			}
			break;
		case 0x09:
			printf("%-10s V%01x, V%01x", "SNE", x, y); break;
		case 0x0a:
			printf("%-10s %s, #$%03x", "LD", "I", nnn); 
			s->I = nnn;
			break;
		case 0x0b:
			printf("%-10s %s, #%03x", "JP", "V0", nnn); break;
		case 0x0c:
			printf("%-10s V%01x, #$%02x", "RND", x, nn); break;			
		case 0x0d:
			printf("%-10s V%01x, V%01x, #$%01x", "DRW", x, y, n); break;
		case 0x0e:
			switch(opcode[1]) {
				case 0x9E:
					printf("%-10s V%01x", "SKP", x); break;
				case 0xA1:
					printf("%-10s V%01x", "SKNP", x); break;
				default:
					printf("Invalid instruction E"); break;
			}
			break;
		case 0x0f:
			switch(opcode[1]) {
				case 0x07:
					printf("%-10s V%01x, %s", "LD", x, "DT"); break;
				case 0x0A:
					printf("%-10s V%01x, %s", "LD", x, "K"); break;
				case 0x15:
					printf("%-10s %s, V%01x", "LD", "DT", x); break;
				case 0x18:
					printf("%-10s %s, V%01x", "LD", "ST", x); break;
				case 0x1E:
					printf("%-10s %s, V%01x", "ADD", "I", x); break;
				case 0x29:
					printf("%-10s %s, V%01x", "LD", "F", x); break;
				case 0x33:
					printf("%-10s %s, V%01x", "LD", "B", x); break;
				case 0x55:
					printf("%-10s %s, V%01x", "LD", "[I]", x); break;
				case 0x65:
					printf("%-10s V%01x, %s", "LD", x, "[I]"); break;
				default:
					printf("Invalid instruction F");
			}
			break;
		default:
			printf("Invalid instruction.");
	}
	printf("\n");
}

// Print memory nicely.
void printMem(Chip8State *s) {
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
	puts("");

	printf("I: %02x\n", s->I);
	for(int i = 0; i < 16; i++) {
		printf("V%-2d: %02x\t", i, s->V[i]);
		if(i % 2 == 1 && i > 0) {
			puts("");
		}
	}

	puts("");
}