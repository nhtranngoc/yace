#include "gtest/gtest.h" 

extern "C" {
	#include "../include/chip8.h"
}

Chip8State *s = InitChip8();

// CLS
TEST(op00E0, ok) {
	// Set all pixels to 1
	for(int i=0; i<64; i++) {
		for(int j=0; j<32; j++) {
			SetPixelChip8(s,i,j,1);
		}
	}
	int curPC = s->PC;
	Op0(s, 0x00E0);
	// Expect all pixels to be 0
	for(int i=0; i<64; i++) {
		for(int j=0; j<32; j++) {
			EXPECT_EQ(GetPixelChip8(s,i,j), 0);
		}
	}
	// Advance PC
	EXPECT_EQ(s->PC, curPC+2);
}

// RET
TEST(op00ee, ok) {

}

// JP addr
TEST(op1nnn, ok) {
	Op1(s, 0x3F);
	EXPECT_EQ(s->PC, 0x3F);
}

// CALL
TEST(op2nnn, ok) {

}

// SE Vx, byte
TEST(op3xkk, ok) {
	// Skip if Vx == kk
	int curPC = s->PC;
	s->V[5] = 30;
	Op3(s,5,30);
	EXPECT_EQ(s->PC, curPC+4);

	// Else increment PC and do nothing
	curPC = s->PC;
	s->V[3] = 2;
	Op3(s,3,0);
	EXPECT_EQ(s->PC, curPC+2);
}

// SNE Vx, byte
TEST(op4xkk, ok) {
	// Skip if Vx != kk
	int curPC = s->PC;
	s->V[5] = 30;
	Op4(s,5,30);
	EXPECT_EQ(s->PC, curPC+2);

	// Else increment PC and do nothing
	curPC = s->PC;
	s->V[3] = 2;
	Op4(s,3,0);
	EXPECT_EQ(s->PC, curPC+4);
}

// SE Vx, Vy
TEST(op5xy0, ok) {
	int curPC = s->PC;
	s->V[0] = 10;
	s->V[1] = 10;
	s->V[2] = 0;

	Op5(s,0,1);
	EXPECT_EQ(s->PC, curPC+4);
	curPC = s->PC;
	Op5(s,0,2);
	EXPECT_EQ(s->PC, curPC+2);
}

// LD Vx, byte
TEST(op6xkk, ok) {
	int curPC = s->PC;

	Op6(s,1,0x80);
	EXPECT_EQ(s->V[1], 0x80);
	EXPECT_EQ(s->PC, curPC+2);
}

// ADD Vx, byte
TEST(op7xkk, ok) {
	int curPC = s->PC;
	int curVx = s->V[2];

	Op7(s,2,30);
	EXPECT_EQ(s->V[2], curVx+30);
	EXPECT_EQ(s->PC, curPC+2);
}

// LD Vx, Vy
TEST(op8xy0, ok) {
	int curPC = s->PC;
	s->V[3] = 10;
	s->V[4] = 50;
	Op8(s,3,4,0);

	EXPECT_EQ(s->V[3], 50);
	EXPECT_EQ(s->V[4], 50);
	EXPECT_EQ(s->PC, curPC+2);
}

// OR Vx, Vy
TEST(op8xy1, ok) {
	int curPC = s->PC;
	s->V[3] = 10;
	s->V[4] = 50;
	Op8(s,3,4,1);

	EXPECT_EQ(s->V[3], 10 | 50);
	EXPECT_EQ(s->V[4], 50);
	EXPECT_EQ(s->PC, curPC+2);
}

// AND Vx, Vy
TEST(op8xy2, ok) {
	int curPC = s->PC;
	s->V[3] = 10;
	s->V[4] = 50;
	Op8(s,3,4,2);

	EXPECT_EQ(s->V[3], 10 & 50);
	EXPECT_EQ(s->V[4], 50);
	EXPECT_EQ(s->PC, curPC+2);
}

// XOR Vx, Vy
TEST(op8xy3, ok) {
	int curPC = s->PC;
	s->V[3] = 10;
	s->V[4] = 50;
	Op8(s,3,4,3);

	EXPECT_EQ(s->V[3], 10 ^ 50);
	EXPECT_EQ(s->V[4], 50);
	EXPECT_EQ(s->PC, curPC+2);
}

// ADD Vx, Vy
TEST(op8xy4, ok) {
	int curPC = s->PC;

	s->V[3] = 10;
	s->V[4] = 50;
	s->V[0xf] = 0;
	Op8(s,3,4,4);
	EXPECT_EQ(s->V[3], 60);
	EXPECT_EQ(s->V[4], 50);
	EXPECT_EQ(s->V[0xf], 0);

	s->V[0xf] = 0;
	s->V[3] = 130;
	s->V[4] = 200;
	Op8(s,3,4,4);

	EXPECT_EQ(s->V[3], (130+200) & 0xff);
	EXPECT_EQ(s->V[4], 200);
	EXPECT_EQ(s->V[0xf], 1);

	EXPECT_EQ(s->PC, curPC+4);
}

// SUB Vx, Vy
TEST(op8xy5, ok){
	int curPC = s->PC;

	s->V[3] = 10;
	s->V[4] = 50;
	s->V[0xf] = 0;
	Op8(s,3,4,5);
	EXPECT_EQ(s->V[3], 256-(50-10));
	EXPECT_EQ(s->V[4], 50);
	EXPECT_EQ(s->V[0xf], 0);

	s->V[3] = 50;
	s->V[4] = 10;
	s->V[0xf] = 0;
	Op8(s,3,4,5);
	EXPECT_EQ(s->V[3], 40);
	EXPECT_EQ(s->V[4], 10);
	EXPECT_EQ(s->V[0xf], 1);

	EXPECT_EQ(s->PC, curPC+4);
}

// SHR Vx, {, Vy} 
TEST(op8xy6, ok) {

}