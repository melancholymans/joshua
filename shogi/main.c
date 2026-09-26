#include <stdio.h>
#include <stdint.h>

#include "init.h"
#include "bitboard.h"
#include "position.h"
#include "usi.h"

int main() {
	init_tables();
	usi_handler_t* hd = new_usi_handler(stdin);
	bitboard bb = set_board(0x298060C0A1121B45, 0x3abad);	
	print_bitboard(bb, "test");
	char buf[1048];
	for (;;) {
		if (fgets(buf, sizeof(buf), stdin) != NULL) {
			buf[strcspn(buf, "\n")] = '\0';
		}
		printf("return -> %s\n", buf);
		if (handle(hd,buf) != NULL){
			break;
		}
	}
	return 1;
}