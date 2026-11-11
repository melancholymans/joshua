#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "bitboard.h"
#include "position.h"

/*
*   sq:	sqare 座標
*   f:  file 筋
*   r:  rank 段
*	c:	color 手番
*   pt:	piecetype 駒種
*   pc:	piece 駒番 
*	cap: captured piece 取った駒の駒番
*	drop: drop piece 打った駒の駒番
*/

// ２つの座標の位置関係のテーブル
int square_relation_direct[81][81];
// 座標文字列 これ使っていない、不要では
char* square_usi_string_table[81] = {"1a","1b","1c","1d","1e","1f","1g","1h","1i",
							         "2a","2b","2c","2d","2e","2f","2g","2h","2i",
							         "3a","3b","3c","3d","3e","3f","3g","3h","3i",
							         "4a","4b","4c","4d","4e","4f","4g","4h","4i",
							         "5a","5b","5c","5d","5e","5f","5g","5h","5i",
							         "6a","6b","6c","6d","6e","6f","6g","6h","6i",
							         "7a","7b","7c","7d","7e","7f","7g","7h","7i",
							         "8a","8b","8c","8d","8e","8f","8g","8h","8i",
							         "9a","9b","9c","9d","9e","9f","9g","9h","9i"};
/*
 piece_symbolの意味
 1-8はset_boardでblackの駒を表す番号,17-24はwhiteの駒を表す番号,他はprint_boardで使うための番号
 0 ""
 1 P bpawn
 2 L blance
 3 N bknight
 4 S bsilver
 5 B bbishop
 6 R brook
 7 G bgold
 8 K bking
 9 A propawn
 10 C prolance
 11 V proknight
 12 I prosilver
 13 H horse
 14 D dragon
 15 ""
 16 ""
 17 p wpawn
 18 l wlance
 19 n wknight
 20 s wsilver
 21 b wbishop
 22 r wrook
 23 g wgold
 24 k wking
 25 a propawn
 26 c prolance
 27 v proknight
 28 i prosilver
 29 h horse
 30 d dragon
*/
char piece_symbol[31] = { ' ','P','L','N','S','B','R','G','K','A','C','V','I','H','D',' ',' ', 
		                  'p','l','n','s','b','r','g','k','a','c','v','i','h','d' };


// 2つの位置関係のテーブル,sq1とsq2の関係がdirect_file=縦方向、direct_rank,direct_diag_nesw,direct_diag_nwse方向なのかを即答してくれるテーブル、それ以外はdirect_misc
void new_square_relation_direct(void) {
	for (int sq1 = sq1a; sq1 <= sq9i; sq1 += 1) {
		const f1 = set_file(sq1);
		const r1 = set_rank(sq1);
		for (int sq2 = sq1a; sq2 <= sq9i; sq2 += 1) {
			const f2 = set_file(sq2);
			const r2 = set_rank(sq2);
			square_relation_direct[sq1][sq2] = direct_misc;
			if (sq1 == sq2) {
				continue;
			}
			if (f1 == f2) {
				square_relation_direct[sq1][sq2] = direct_file;	//縦方向
			}
			else if (r1 == r2) {
				square_relation_direct[sq1][sq2] = direct_rank;	//横方向
			}
			else if ((r1 - r2) == (f1 - f2)) {
				square_relation_direct[sq1][sq2] = direct_diag_nesw;	//右上から左下
			}
			else if ((r1 - r2) == (f2 - f1)) {
				square_relation_direct[sq1][sq2] = direct_diag_nwse;	//左上から右下 
			}
		}
	}
}

//fileとrankを指定して座標値を返す
int set_square(int f, int r) {
	return f * 9 + r;
}

// sq座標からfile座標を計算して返している。
int set_file(int sq) {
	return sq / 9;
}

// sq座標からrank座標を計算して返している。
int set_rank(int sq) {
	return sq % 9;
}

//move形式の座標をusi形式の座標に変換する(40->"5e")
void movesq_to_usisq(int sq,char* usi_sq) {
	int f = set_file(sq);
	int r = set_rank(sq);
	usi_sq[0] = '1' + f;
	usi_sq[1] = 'a' + r;
	usi_sq[2] = '\0';
}

//usi形式の座標をmove形式の座標に変換する("5e"->40)
int usisq_to_movesq(char* usi_sq) {
	return (usi_sq[0] - '1') * 9 + (usi_sq[1] - 'a');
}

// 渡されたsq座標同士の位置関係を返す
int square_relation(const int sq1, const int sq2) {
	return square_relation_direct[sq1][sq2];
}

// 駒番をblack->white,white->blackにする(bpawn->wpawn)
int inverse(const int pc) {
	return pc ^ 0x10;
}

// 駒番から手番（color）を判定する.blackかwhiteが返る
int piece_to_color(const int pc) {
	return pc >> 4;
}

// colorと駒種を指定して駒番を作る
int piecetype_to_piece(const int c, const int pt) { 
	return (c << 4) | pt; 
}

//jump駒かどうかの判定、lance,bishp,rook,horse,dragonならtrue
_Bool is_jump(const int pc) {
	return (0x60646064 & (1 << pc)) != 0;
}

// 座標sqの段の文字を返す
char rank_usi_string(const int r) {
	return 'a' + r;
}

// 座標sqの筋の文字を返す
char file_usi_string(const int f) {
	return '1' + f;
}

// 座標の文字列を返す、これ全然使っていないが不要では？
// C言語での簡単な文字列処理に関するアイデアがないのでとりあえずこれで(TODO:)
void square_usi_string(const int sq, char* str) {
	const int r = set_rank(sq);
	const int f = set_file(sq);
	char ch[] = { file_usi_string(f),rank_usi_string(r),'\0' };
	strcpy_s(str, 3, ch);
}

// 後手の位置を先手の位置へ変換
// sq1a = 81-1-0=80=sq9i
// sq5c = 81-1-38=sq5g
// sq6b = 81-1-46=sq4h
// sq9i = 81-1-80=sq1a
int square_inverse(const int sq) { 
	return 81 - 1 - sq; 
}

// 左右変換
// file1 = 9-1-0=8=file9
// file3 = 9-1-2=6=file7
int file_inverse(const int f) {
	return 9 - 1 - f; 
}

// 上下変換
// rank1 = 9-1-0=8=rank9
// rank3 = 9-1-2=6=rank7
int rank_inverse(const int r) {
	return 9 - 1 - r;
}

// 駒の移動元(from)移動先(to)が成れるエリア（敵陣エリア）かどうかで成りのフラグを返す
_Bool can_promote_area(const int c, const int from_or_to_rank) {
	return (0x1c00007u & (1u << ((c << 4) + from_or_to_rank)));
}

/*
* やねうら王から移植された壁つきの升表現というSquareWithWallがあるがいまのところ実装は後回し(TODO:)
*/

/*
*  やねうら王から移植されたLong Effect Libraryがあるがいまのところ実装は後回し(TODO:)
*/

// handle_is_readyからの対局準備としてboard_tのメモリを確保、設定は別の関数で行う
void new_board(usi_handler_t* hd) {
	hd->bd = (board_t*)malloc(sizeof(board_t));
	if (hd->bd == NULL) {
		fprintf(stderr, "board_tの確保に失敗した");
		return;
	}
	board_t* b = hd->bd;
	hd->st = (StateInfo_t*)malloc(sizeof(StateInfo_t));
	if (hd->st == NULL) {
		fprintf(stderr, "StateInfo_tの確保に失敗した");
		return;
	}
	return;
}

//sfen文字列をパースして局面を設定
void set_board(usi_handler_t* hd, char* sfen) {
	/*
	sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1"のような文字列で渡ってくる
	strtok_s関数でsfen文字列を分割してpartsに切り分ける
	parts[0] =  "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL"
	parts[1] = "b"
	parts[2] = "-"
	parts[4] = "1"
	*/
	char* parts[4];	
	int count = 0;
	char* ptr = NULL;
	char* token = strtok_s(sfen, " ", &ptr);
	for (; token != NULL && count < 4;) {
		parts[count++] = token;
		token = strtok_s(NULL, " ", &ptr);
	}
	board_t* b = hd->bd;
	//局面をクリアにする
	for (int sq = 0; sq < 81; sq++) {
		b->mb[sq] = empty;
	}
	for (int pt = 0; pt < 25; pt++) {
		b->hb[pt] = 0;
	}
	b->king_square[black] = 0;
	b->king_square[white] = 0;
	b->turn = 0;
	b->move_number = 0;
	for (int i = 0; i < 21; i++) {
		b->bytype_bb[i] = all_zero_mask;
	}
	b->bycolor_bb[black] = all_zero_mask;
	b->bycolor_bb[white] = all_zero_mask;
	//局面を設定する
	int sq = sq9a;
	int pmoto = 0;
	for (int i=0; i < strlen(parts[0]); i++) {
		char r = parts[0][i];
		if (isdigit((int)r)) {
			sq += delta_e * (int)(r - 48);
		}
		else if ('+' == r) {
			pmoto = 8;
		}
		else if ('/' == r) {
			sq += (delta_w * 9) + delta_s;
		}
		else {
			for (int pc = bpawn; pc <= wdragon; pc++) {
				if (piece_symbol[pc] == r) {
					if (1 <= pc && 8 >= pc) {	//black
						pc = pc + pmoto;
						b->mb[sq] = pc;
						b->bytype_bb[piece_to_piecetype(pc)].m = _mm_or_si128(b->bytype_bb[piece_to_piecetype(pc)].m,sq_mask[sq].m);
						b->bytype_bb[ALL_PIECE].m = _mm_or_si128(b->bytype_bb[ALL_PIECE].m, sq_mask[sq].m);
						b->bycolor_bb[black].m = _mm_or_si128(b->bycolor_bb[black].m, sq_mask[sq].m);
						if (r == 'K') {
							b->king_square[black] = sq;
						}
					}
					else if (17<=pc && 24>=pc){	//white
						pc = pc + pmoto;
						b->mb[sq] = pc;
						b->bytype_bb[piece_to_piecetype(pc)].m = _mm_or_si128(b->bytype_bb[piece_to_piecetype(pc)].m, sq_mask[sq].m);
						b->bytype_bb[ALL_PIECE].m = _mm_or_si128(b->bytype_bb[ALL_PIECE].m, sq_mask[sq].m);
						b->bycolor_bb[white].m = _mm_or_si128(b->bycolor_bb[white].m, sq_mask[sq].m);
						if (r == 'k') {
							b->king_square[white] = sq;
						}
					}
					break;
				}
			}
			sq += delta_e;
			pmoto = 0;
		}
	}
	b->turn = black;
	update_bytypebb(hd);	//bytype_bb を更新する
	set_state(hd);
	return;
}

void update_bytypebb(usi_handler_t* hd) {
	board_t* b = hd->bd;
	bitboard bishop_bb;
	bitboard horse_bb;
	bitboard rook_bb;
	bitboard dragon_bb;
	bitboard propawn_bb;
	bitboard prolance_bb;
	bitboard proknight_bb;
	bitboard prosilver_bb;
	bishop_bb.m = b->bytype_bb[bishop].m;
	horse_bb.m = b->bytype_bb[horse].m;
	rook_bb.m = b->bytype_bb[rook].m;
	dragon_bb.m = b->bytype_bb[dragon].m;
	propawn_bb.m = b->bytype_bb[propawn].m;
	prolance_bb.m = b->bytype_bb[prolance].m;
	proknight_bb.m = b->bytype_bb[proknight].m;
	prosilver_bb.m = b->bytype_bb[prosilver].m;

	b->bytype_bb[GOLDS].m = _mm_or_si128(_mm_or_si128(_mm_or_si128(_mm_or_si128(b->bytype_bb[gold].m, propawn_bb.m),prolance_bb.m),proknight_bb.m),prosilver_bb.m);
	b->bytype_bb[HDK].m = _mm_or_si128(_mm_or_si128(b->bytype_bb[king].m, horse_bb.m), dragon_bb.m);
	b->bytype_bb[BISHOP_HORSE].m = _mm_or_si128(bishop_bb.m, horse_bb.m);
	b->bytype_bb[ROOK_DRAGON].m = _mm_or_si128(rook_bb.m, dragon_bb.m);
	b->bytype_bb[SILVER_HDK].m = _mm_or_si128(b->bytype_bb[silver].m, b->bytype_bb[HDK].m);
	b->bytype_bb[GOLDS_HDK].m = _mm_or_si128(b->bytype_bb[GOLDS].m, b->bytype_bb[HDK].m);
}

void set_state(usi_handler_t* hd) {
	StateInfo_t* st = hd->st;
	int us = hd->bd->turn;
	st->board_key = 0;
	st->hand_key = 0;
	st->material_key = 0;
	st->pawn_key = 0;
	st->minor_piece_key = 0;
	st->nonpawn_key[black] = 0;
	st->nonpawn_key[white] = 0;
	st->plies_from_null = 0;
	st->continuous_check[black] = 0;
	st->continuous_check[white] = 0;
	//自王に王手している敵駒の位置bitboardを設定する
	int ksq = hd->bd->king_square[us];
	st->checkersBB = attacks_to(hd,ksq, us);
	/*test code start*/
	print_bitboard(st->checkersBB, "checkersBB");
	ksq = hd->bd->king_square[opposite_color(us)];
	st->checkersBB = attacks_to(hd, ksq, opposite_color(us));
	print_bitboard(st->checkersBB, "checkersBB");
	/*test code end*/
	//st->blockers_for_king[black] = ??
	//st->blockers_for_king[white] = ??
	//st->pinners[black] = ??
	//st->pinners[white] = ??
	//for (int i = 0; i < 16; i++) {
	//	st->check_squares[i] = ? ? ;
	//}
	st->captured_piece = 0;
	st->repetition = 0;
	st->repetition_times = 0;
	st->repetition_type = 0;
	st->hand = 0;
	st->material_value = 0;
	//st->previous = NULL;
}

bitboard attacks_to(usi_handler_t* hd,const int sq, const int c) {
	//white側kingに王手をかけているblack駒の位置bitboardを返す
	board_t* bd = hd->bd;
	bitboard bpawne;
	bitboard bknighte;
	bitboard bsilvere;
	bitboard bgolde;
	bitboard black_bb;
	bitboard blacke;
	bpawne.m = _mm_and_si128(pawn_attack_mask[white][sq].m,bd->bytype_bb[pawn].m);
	bknighte.m = _mm_and_si128(knight_attack_mask[white][sq].m,bd->bytype_bb[knight].m);
	bsilvere.m = _mm_and_si128(silver_attack_mask[white][sq].m,bd->bytype_bb[SILVER_HDK].m);
	bgolde.m = _mm_and_si128(gold_attack_mask[white][sq].m,bd->bytype_bb[GOLDS_HDK].m);
	black_bb = bd->bycolor_bb[black];
	blacke.m = _mm_and_si128(_mm_or_si128(_mm_or_si128(_mm_or_si128(bpawne.m, bknighte.m), bsilvere.m), bgolde.m), black_bb.m);
	//black側kingに王手をかけているwhite駒の位置bitboardを返す
	bitboard wpawne;
	bitboard wknighte;
	bitboard wsilvere;
	bitboard wgolde;
	bitboard white_bb;
	bitboard whitee;
	wpawne.m = _mm_and_si128(pawn_attack_mask[black][sq].m, bd->bytype_bb[pawn].m);
	wknighte.m = _mm_and_si128(knight_attack_mask[black][sq].m, bd->bytype_bb[knight].m);
	wsilvere.m = _mm_and_si128(silver_attack_mask[black][sq].m, bd->bytype_bb[SILVER_HDK].m);
	wgolde.m = _mm_and_si128(gold_attack_mask[black][sq].m, bd->bytype_bb[GOLDS_HDK].m);
	white_bb = bd->bycolor_bb[white];
	whitee.m = _mm_and_si128(_mm_or_si128(_mm_or_si128(_mm_or_si128(wpawne.m, wknighte.m), wsilvere.m), wgolde.m), white_bb.m);
	//black又はwhite側kingに王手をかけている角、飛車、香車の位置bitboardを返す
	bitboard bwbishop = bishop_attack(sq, bd->bytype_bb[ALL_PIECE]);
	bitboard bwrook = rook_attack(sq, bd->bytype_bb[ALL_PIECE]);
	bitboard blance = lance_attack(black, sq, bd->bytype_bb[ALL_PIECE]);
	bitboard wlance = lance_attack(white, sq, bd->bytype_bb[ALL_PIECE]);
	bitboard bwbishope;
	bitboard bwrooke;
	bitboard blancee;
	bwbishope.m = _mm_and_si128(bwbishop.m, bd->bytype_bb[BISHOP_HORSE].m);
	bwrooke.m = _mm_and_si128(bwrook.m, bd->bytype_bb[ROOK_DRAGON].m);
	blancee.m = _mm_and_si128(_mm_or_si128(blance.m, wlance.m), bd->bytype_bb[lance].m);
	bitboard result_bb;
	result_bb.m = _mm_or_si128(_mm_or_si128(_mm_or_si128(_mm_or_si128(blacke.m, whitee.m), bwbishope.m), bwrooke.m), blancee.m);
	return result_bb;
}

/*bitboard piece_to_bitboard(const int pc, const board_t* b) {
	return b->bytype_bb[pc];
}*/

void print_board(usi_handler_t* hd) {
	char rstr[] = "abcdefghi";
	printf("   9  8  7  6  5  4  3  2  1\n");
	for (int r = 0; r < 9; r++) {
		printf("%c ", rstr[r]);
		for (int f = 8; f >= 0; f--) {
			int sq = set_square(f, r);
			if (hd->bd->mb[sq] == empty) {
				printf(" . ");
			}
			else {
				printf(" %c ", piece_symbol[hd->bd->mb[sq]]);
			}
		}
		printf("\n");
	}
	for (int pc = bpawn; pc <= bgold; pc++) {
		printf("%c:%d ", piece_symbol[pc], hd->bd->hb[pc]);
	}
	printf("\n");
	for (int pc = wpawn; pc <= wgold; pc++) {
		printf("%c:%d ", piece_symbol[pc], hd->bd->hb[pc]);
	}
	printf("\n");
	printf("a:ppawn c:plance v:knight i:psilver h:horse d:dragon\n");
}

