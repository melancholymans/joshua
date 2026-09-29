#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "init.h"
#include "bitboard.h"
#include "position.h"
#include "usi.h"

/*
* errno_t
* 200 エラーなし
* 201 system quit
* 202 しらないコマンドです
* 203 メモリが確保できない
*/ 
int main(void) {
	init_tables();
	usi_handler_t hd = new_usi_handler(stdout);
	//bitboard bb = set_bitboard(0x298060C0A1121B45, 0x3abad);	
	//print_bitboard(bb, "test");
	for (;;) {
		char buf[1024];
		if (fgets(buf, sizeof(buf), stdin) != NULL) {
			buf[strcspn(buf, "\n")] = '\0';
		}
		if (handle(&hd,buf) != 200){
			break;
		}
	}
	return 0;
}