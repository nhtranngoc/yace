#include "gtest/gtest.h" 
#include "math.h"

extern "C" {
	#include "../include/chip8.h"
}

TEST(initChip8, ok) {
	Chip8State *s = InitChip8(); 

	EXPECT_EQ(s->memory[0xf00], *s->screen);
	EXPECT_EQ(s->SP, 0xFA0);
	EXPECT_EQ(s->PC, 0x200);
	EXPECT_EQ(s->I, 0);
}

TEST(getBit, ok) {
	uint8_t a = 0xAF; //1010 1111
	uint8_t *tmp = &a;

	EXPECT_EQ(GetBitChip8(tmp,0), 1);
	EXPECT_EQ(GetBitChip8(tmp,1), 0);
	EXPECT_EQ(GetBitChip8(tmp,2), 1);
	EXPECT_EQ(GetBitChip8(tmp,3), 0);

	EXPECT_EQ(GetBitChip8(tmp,4), 1);
	EXPECT_EQ(GetBitChip8(tmp,5), 1);
	EXPECT_EQ(GetBitChip8(tmp,6), 1);
	EXPECT_EQ(GetBitChip8(tmp,7), 1);
}

TEST(setBit, ok) {
	uint8_t *tmp = (uint8_t *)malloc(sizeof(uint8_t));

	// Set bits to 1 from 1000 0000 to 0000 0001
	SetBitChip8(tmp,0,1);	// 1000 0000
	EXPECT_EQ(*tmp,0x80);
	*tmp = 0;				
	SetBitChip8(tmp,1,1);	// 0100 0000
	EXPECT_EQ(*tmp,0x40);	
	*tmp = 0;				
	SetBitChip8(tmp,2,1);	// 0010 0000
	EXPECT_EQ(*tmp,0x20);
	*tmp = 0;
	SetBitChip8(tmp,3,1);	// 0001 0000
	EXPECT_EQ(*tmp,0x10);
	*tmp = 0;

	SetBitChip8(tmp,4,1);	// 0000 1000
	EXPECT_EQ(*tmp,0x08);
	*tmp = 0;
	SetBitChip8(tmp,5,1);	// 0000 0100
	EXPECT_EQ(*tmp,0x04);
	*tmp = 0;
	SetBitChip8(tmp,6,1);	// 0000 0010
	EXPECT_EQ(*tmp,0x02);
	*tmp = 0;
	SetBitChip8(tmp,7,1);	// 0000 0001
	EXPECT_EQ(*tmp,0x01);
	*tmp = 0;

	free(tmp);
}

TEST(setPixel, ok) {
	Chip8State *s = InitChip8();

	SetPixelChip8(s,0,0,1);			// 1000 0000
	EXPECT_EQ(s->screen[0], 0x80);		
	SetPixelChip8(s,1,0,0);			// 1000 0000
	EXPECT_EQ(s->screen[0], 0x80);
	SetPixelChip8(s,2,0,1);			// 1010 0000
	EXPECT_EQ(s->screen[0], 0xA0);
	SetPixelChip8(s,3,0,0);			// 1010 0000
	EXPECT_EQ(s->screen[0], 0xA0);

	SetPixelChip8(s,4,0,1);			// 1010 1000
	EXPECT_EQ(s->screen[0], 0xA8);
	SetPixelChip8(s,5,0,1);			// 1010 1100
	EXPECT_EQ(s->screen[0], 0xAC);
	SetPixelChip8(s,6,0,1);			// 1010 1110
	EXPECT_EQ(s->screen[0], 0xAE);
	SetPixelChip8(s,7,0,1);			// 1010 1111
	EXPECT_EQ(s->screen[0], 0xAF);
}

TEST(getPixel, ok) {
	Chip8State *s = InitChip8();

	s->screen[0] = 0xAF; // 1010 1111 ...
	EXPECT_EQ(GetPixelChip8(s,0,0),1);
	EXPECT_EQ(GetPixelChip8(s,1,0),0);
	EXPECT_EQ(GetPixelChip8(s,2,0),1);
	EXPECT_EQ(GetPixelChip8(s,3,0),0);

	EXPECT_EQ(GetPixelChip8(s,4,0),1);
	EXPECT_EQ(GetPixelChip8(s,5,0),1);
	EXPECT_EQ(GetPixelChip8(s,6,0),1);
	EXPECT_EQ(GetPixelChip8(s,7,0),1);

	EXPECT_EQ(GetPixelChip8(s,8,0),0);
	EXPECT_EQ(GetPixelChip8(s,10,10),0);
}