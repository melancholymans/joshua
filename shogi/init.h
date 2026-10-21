#pragma once

#include <stdio.h>
#include <Windows.h>

//mbはmain board,hbはhand board,turnはターン
/*
 hb[24]の配列の中身は以下の通り、番号はとんでいる。
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
	char** buffer;
	int capacity;
	int head;
	int tail;
	int size;
	CRITICAL_SECTION cs;
	CONDITION_VARIABLE cv_not_full;	//キューの空き待ち
	CONDITION_VARIABLE cv_not_empty;	//キューのデータ追加待ち
}comman_queue_t;

typedef struct {
	int mb[81];
	int hb[25];
	int turn;
	int move_number;
}board_t;

typedef struct {
	board_t* bd;
	comman_queue_t* ucq;
	FILE* stream;
	char* name;
	char* author;
	char book_file_name[32];
	int* ml;
	int hash_size;
	unsigned long ponderhit_receive_time;
	_Bool can_ponder;
	_Bool use_book;
	_Bool is_stop;
}usi_handler_t;


void init_tables(void);
