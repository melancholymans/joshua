#include <stdio.h>

#include "bitboard.h"

void init_tables() {
	new_mask_bb();
	new_file_mask();
}

int main() {
	printf("Hello World \n");
	init_tables();
	return 1;
}