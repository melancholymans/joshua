#pragma once

#include <stdio.h>
#include <Windows.h>
#include <immintrin.h>
#include <intrin.h>
#include <stdint.h>


// gameの結果
enum {
	draw = 0,
	black_win = 1,
	white_win = 2,
};

//mbはmain board,hbはhand board,turnはターン
/* hb[24]の配列の中身は以下の通り、番号はとんでいる。
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
	hb[24] = wking = 24,*/

typedef union {
	__m128i m;
	int64_t p[2];
}bitboard;

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
	int king_square[2];
	int turn;
	int move_number;
	bitboard bytype_bb[21];
	bitboard bycolor_bb[2];
}board_t;

typedef struct {
	uint64_t material_key;
	uint64_t pawn_key;
	uint64_t minor_piece_key;
	uint64_t nonpawn_key[2];
	int plies_from_null;
	int continuous_check[2];
	uint64_t board_key;
	uint64_t hand_key;
	//現局面で手番側に対して王手をしている駒のbitboard
	bitboard checkersBB;
	//自玉に対して(敵駒によって)pinされている駒
	bitboard blockers_for_king[2];
	//自玉に対してpinしている(可能性のある)敵の大駒。
	bitboard pinners[2];
	//自駒の駒種Xによって敵玉が王手となる升のbitboard
	bitboard check_squares[16];
	//この局面で捕獲された駒。先後の区別あり。
	int captured_piece;
	//循環局面であることを示す。
	int repetition;
	//繰り返された回数
	int repetition_times;
	//繰り返しの種類
	int repetition_type;
	//この局面における手番側の持ち駒。優等局面の判定のために必要。
	int hand;
	//この局面での評価関数の駒割
	int material_value;
	//前の局面に遡るためのポインタ。
	//StateInfo_t* previous;
}StateInfo_t;

typedef struct {
	board_t* bd;
	comman_queue_t* ucq;
	FILE* stream;
	char* name;
	char* author;
	char book_file_name[32];
	int hash_size;
	unsigned long ponderhit_receive_time;
	_Bool can_ponder;
	_Bool use_book;
	_Bool is_stop;
	StateInfo_t* st;
}usi_handler_t;


void init_tables(void);
