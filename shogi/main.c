#include <stdio.h>
#include <stdint.h>

#include "bitboard.h"


int main() {
	init_tables();
	__m128i bb = set_board(0x298060C0A1121B45, 0x3abad);	
	print_bitboard(bb, "test");
	return 1;
}