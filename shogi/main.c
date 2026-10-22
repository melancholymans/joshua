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
	setvbuf(stdout, NULL, _IONBF, 0);
	init_tables();
	usi_handler_t hd = new_usi_handler(stdout);
	if (init_queue(&hd, 16)) {
		fprintf(stderr, "[ERROR] %s:%d: command queue memary out\n", __FILE__, __LINE__);
		exit(1);
	}
	if (!begin_thread(&hd)) {
		fprintf(stderr, "[ERROR] %s:%d: failed to begin thread\n", __FILE__, __LINE__);
		exit(1);
	}
	usi_start(&hd);
	for (;;) {
		char buf[1024];
		wait_command(&hd, buf);
		if (handle(&hd,buf) != 200){
			break;
		}
	}
	return 0;
}