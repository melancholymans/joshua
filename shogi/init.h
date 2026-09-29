#pragma once

#include <stdio.h>

//mb‚Ímain board,hb‚Íhand board,turn‚Íƒ^[ƒ“
typedef struct {
	int mb[81];
	int hb[2][8];
	int turn;
}board_t;

typedef struct {
	int from;
	int to;
	int drop_piece;
	_Bool promotion;
}move_t;

typedef struct {
	board_t* bd;
	FILE* stream;
	char* name;
	char* author;
	move_t* mv;
}usi_handler_t;


void init_tables(void);
