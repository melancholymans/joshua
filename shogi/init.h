#pragma once

#include <stdio.h>


//mbはmain board,hbはhand board,turnはターン
/*
* hb[24]の配列の中身は以下の通り
	hb[1] = bpawn = 1,
	hb[2] = blance = 2,
	hb[3] = bknight = 3,
	hb[4] = bsilver = 4,
	hb[5] = bbishop = 5,
	hb[6] = brook = 6,
	hb[7] = bgold = 7,
	hb[17] = wpawn = 17,
	hb[18] = wlance = 18,
	hb[19] = wknight = 19,
	hb[20] = wsilver = 20,
	hb[21] = wbishop = 21,
	hb[22] = wrook = 22,
	hb[23] = wgold = 23,
	hb[24] = wking = 24,
*/
typedef struct {
	int mb[81];
	int hb[25];
	int turn;
	int move_number;
}board_t;

typedef struct {
	board_t* bd;
	FILE* stream;
	char* name;
	char* author;
	int* mv;
}usi_handler_t;


void init_tables(void);
