#include "gtest/gtest.h" 

extern "C" {
	#include "../include/chip8.h"
}

TEST(bitwise, ok) {
	uint8_t a = 0xA0; //1010 0000
	uint8_t *tmp = &a;

	ASSERT_EQ(GetBitChip8(tmp,6), 0);
}