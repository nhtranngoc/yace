#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEM_OFFSET 0x200

void disassembly(unsigned char *buffer, int pc);

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "rb");

	if(f == NULL) {
		printf("Error: Can't open file or file not found: %s\n", argv[1]);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	unsigned char *buffer = malloc(fsize + MEM_OFFSET);
	fread(buffer+MEM_OFFSET, fsize, 1, f);
	fclose(f); 

	int pc = MEM_OFFSET;
	while(pc < (fsize + MEM_OFFSET)) {
		disassembly(buffer, pc);
		pc+=2;
	}

	return 0;
}

void disassembly(unsigned char *buffer, int pc) {
	uint8_t *opcode = &buffer[pc];
	uint8_t firstnib = (opcode[0] >> 4);
	uint8_t lastnib = (opcode[1] & 0x0f);
	uint16_t nnn = ((opcode[0] & 0x0f) << 8) | opcode[1];
	uint8_t nn = nnn & 0xff;
	uint8_t n = nn & 0x0f;
	uint8_t x = opcode[0] & 0x0f;
	uint8_t y = (opcode[1] & 0xf0) >> 4;

	printf("%04x %02x %02x ", pc, opcode[0], opcode[1]);
	switch(firstnib) {
		case 0x00:
			switch(nnn) {
				case 0x00E0:
				printf("%-10s", "CLS"); break;
				case 0x00EE:
				printf("%-10s", "RET"); break;
				default:
				printf("%-10s #$%03x", "SYS", nnn); break;
			}
			break;
		case 0x01:
			printf("%-10s #%03x", "JP", nnn); break;
		case 0x02:
			printf("%-10s #%03x", "CALL", nnn); break;
		case 0x03:
			printf("%-10s V%01x, #$%02x", "SE", x, nn); break;
		case 0x04:
			printf("%-10s V%01x, #$%02x", "SNE", x, nn); break;
		case 0x05:
			printf("%-10s V%01x, V%01x", "SE", x, y); break;
		case 0x06:
			printf("%-10s V%01x, #$%02x", "LD", x, nn); break;
		case 0x07:
			printf("%-10s V%01x, #$%02x", "ADD", x, nn); break;
		case 0x08:
			switch(lastnib) {
				case 0x00:
					printf("%-10s V%01x, V%01x", "LD", x, y); break;
				case 0x01:
					printf("%-10s V%01x, V%01x", "OR", x, y); break;
				case 0x02:
					printf("%-10s V%01x, V%01x", "AND", x, y); break;
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
			printf("%-10s %s, #$%03x", "LD", "I", nnn); break;
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