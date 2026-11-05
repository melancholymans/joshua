#include <stdbool.h>

#include "stb_ds.h"

#include "init.h"
#include "position.h"
#include "thinking.h"


//合法手（簡略版）生成
int do_thinking(usi_handler_t* hd) {
	int best_move = 0;
	int* pml = NULL;
	pml = possible_moves(hd, pml);
	/*for (int i = 0; i < arrlen(pml); i++) {
		if (Is_suicide_or_check_by_dropping_pawn(hd,pml[i])) {
			arrput(ml, pml[i]);
		}
	}*/
	srand((unsigned)time(NULL));
	int len = arrlen(pml);
	best_move = pml[rand() % len];
	arrfree(pml);
	return best_move;
}

void push_pos(const int move) {
	printf("Hello thinking do_move\n");
}

void pop_pos(const int move){
	printf("Hello thinking undo_move\n");
}

//盤上、駒台をスキャンして着手可能手をリストアップする。ここでは合法手であるかのチエックは少ない。move_list関数で絞り込む
int* possible_moves(usi_handler_t* hd, int* ml) {
	board_t* bd = hd->bd;
	int* sqml = NULL;
	int* empty_square = NULL;
	int move = 0;
	for (int from = sq1a; from <= sq9i; from++) {
		int pc = bd->mb[from];
		if (pc != 0 && ((pc & 0x10) >> 4) == bd->turn) {
			sqml = attacks_from(hd, pc, from, bd->turn);
			for (int i = 0; i < arrlen(sqml); i++) {
				int to = sqml[i];
				if (bd->mb[to] != 0 && ((bd->mb[to] & 0x10) >> 4) == bd->turn) {	//toで移動先の駒が自陣側の駒だったパス
					continue;
				}
				if (canmove_without_promotion(to, pc, bd->turn)) {	//不成の移動
					move = bd->mb[to] << 24 | bd->mb[from] << 16 | from << 7 | to;
					arrput(ml, move);
				}
				if (can_promote(from, pc)) {	//成る移動
					move = bd->mb[to] << 24 | bd->mb[from] << 16 | 1 << 14 | from << 7 | to;
					arrput(ml, move);
				}
			}
		}
		else if (pc == 0) {
			arrput(empty_square, from);
		}
	}
	for (int pt = bpawn; pt <= bgold; pt++) {	//white側の駒台はパス
		if (hd->bd->hb[pt] == 0) {
			continue;
		}
		for (int i = 0; i < arrlen(empty_square); i++) {
			int to = empty_square[i];
			if (canmove_without_promotion(to, pt, black) && !is_double_pawn(hd, to, pt)) {
				move = (1 << 15) | (pt << 7) | to;
				arrput(ml, move);
			}
		}
	}
	return ml;
}

//2歩であればtrueを返す
_Bool is_double_pawn(usi_handler_t* hd, int sq, int pc) {
	if (pc != bpawn && pc != wpawn) {
		return false;
	}
	board_t* bt = hd->bd;
	int f = set_file(sq);
	for (int r = set_rank(sq + 1); r <= rank9; r += 1) {
		int s = set_square(f, r);
		int p = bt->mb[s];
		if (p != 0 && p == pc) {
			return true;
		}
	}
	for (int r = set_rank(sq - 1); r >= rank1; r -= 1) {
		int s = set_square(f, r);
		int p = bt->mb[s];
		if (p != 0 && p == pc) {
			return true;
		}
	}
	return false;
}

//pawn,lance.knightが死駒にならないならtrueを返す 
_Bool canmove_without_promotion(int to, int pt, int c) {
	int r = set_rank(to);
	if (c == black) {
		return
			(pt != bpawn && pt != blance && pt != bknight) ||
			(pt == bpawn && r > rank1) ||
			(pt == blance && r > rank1) ||
			(pt == bknight && r > rank2);
	}
	else {
		return
			(pt != wpawn && pt != wlance && pt != wknight) ||
			(pt == wpawn && r < rank9) ||
			(pt == wlance && r < rank9) ||
			(pt == wknight && r < rank8);
	}
}

//成駒になる可能性のある着手ならtrue この関数はsqがto座標のみで成判定をしているので間違いであるがとりあえずはこれでOK(TODO:)
_Bool can_promote(int sq, int pt) {
	int r = set_rank(sq);
	if (pt >= bpawn && pt <= brook) {
		return r <= rank3;
	}
	else if (pt >= wpawn && pt <= wrook) {
		return r >= rank7;
	}
	return false;
}

//盤上での着手可能手をリストアップ
int* attacks_from(usi_handler_t* hd, int pc, int sq, int turn) {
	board_t* bd = hd->bd;
	int* m = NULL;
	int to = 0;
	int f, r;
	switch (pc) {
	case bpawn:
		to = sq - 1;
		arrput(m, to);
		return m;
	case blance:
		f = set_file(sq);
		r = set_rank(sq) - 1;
		for (; r >= rank1; r -= 1) {
			int to = set_square(f, r);
			int topc = bd->mb[to];
			if (topc == 0) {
				arrput(m, to);
				continue;
			}
			if (((topc & 0x10) >> 4) != turn) {
				arrput(m, to);
			}
			break;
		}
		return m;
	case bknight:
		/*knightのサーチ
		1   2  +7   -11

		  s        s
		*/
		f = set_file(sq);
		r = set_rank(sq);
		switch (f) {
		case file1:
			to = sq + 7;
			arrput(m, to);
			return m;
		case file9:
			to = sq - 11;
			arrput(m, to);
			return m;
		default:
			to = sq + 7;
			arrput(m, to);
			to = sq - 11;
			arrput(m, to);
			return m;
		}
	case bsilver:
		return attacks_bsilver(sq, m);
	case bbishop:
		return attacks_bbishop(hd, sq, m);
	case brook:
		return attacks_brook(hd, sq, m);
	case bgold:
		return attacks_bgold(sq, m);
	case bking:
		return attaks_bking(sq, m);
	case bpropawn:
		return attacks_bgold(sq, m);
	case bprolance:
		return attacks_bgold(sq, m);
	case bproknight:
		return attacks_bgold(sq, m);
	case bprosilver:
		return attacks_bgold(sq, m);
	case bhorse:
		return attacks_bhorse(hd, sq, m);
	case bdragon:
		return attacks_bdragon(hd, sq, m);
		//white ,blackのbold,propawn,prolance,proknight,prosilver,bishop,rook,house,dragon,kingは今はパス
	}
}

int* attacks_bsilver(int sq, int* m) {
	/*silverのサーチ sはsqの略
	1 2 3    +8 -1 -10
	  s          s
	4   5    +10    -8
	*/
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		return m;
	}
}

//black bishop
int* attacks_bbishop(usi_handler_t* hd, int sq, int* m) {
	/*bishopのサーチ sはsqの略
	  1 2  +8    -10
	   s       s
	  3 4  +10   -8
	*/
	//upper left
	for (int to = sq + 8;; to += 8) {
		if (set_file(to - 8) == file9 || set_rank(to - 8) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//upper right
	for (int to = sq - 10;; to -= 10) {
		if (set_file(to + 10) == file1 || set_rank(to + 10) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down left
	for (int to = sq + 10;; to += 10) {
		if (set_file(to - 10) == file9 || set_rank(to - 10) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down right
	for (int to = sq - 8;; to -= 8) {
		if (set_file(to + 8) == file1 || set_rank(to + 8) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	return m;
}

//black rook
int* attacks_brook(usi_handler_t* hd, int sq, int* m) {
	/*bishopのサーチ sはsqの略
		1         -1
	 2  s  3   +9  s  -9
		4         +1
	*/
	//upper
	for (int to = sq - 1;; to -= 1) {
		if (set_rank(to + 1) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//left
	for (int to = sq + 9;; to += 9) {
		if (set_file(to - 9) == file9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//right
	for (int to = sq - 9;; to -= 9) {
		if (set_file(to + 9) == file1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down
	for (int to = sq + 1;; to += 1) {
		if (set_rank(to - 1) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	return m;
}

int* attacks_bgold(int sq, int* m) {
	/*goldのサーチ sはsqの略
	1 2 3     +8 -1 -10
	4 s 5     +9  s  -9
	  6          +1
	*/
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
		}
		return m;
	}
}

//black king
int* attaks_bking(int sq, int* m) {
	/*kingのサーチ sはsqの略
	1 2 3     +8 -1 -10
	4 s 5     +9  s  -9
	6 7 8     +10 +1 -8
	*/
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		return m;
	}
}

//black horse
int* attacks_bhorse(usi_handler_t* hd, int sq, int* m) {
	/*horseのサーチ sはsqの略
	1   2     +8    -10
	  s           s
	3   4     +10    -8

	+

	  5          -1
	6 s 7     +9  s  -9
	  8          +1
	*/
	//upper left
	for (int to = sq + 8;; to += 8) {
		if (set_file(to - 8) == file9 || set_rank(to - 8) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//upper right
	for (int to = sq - 10;; to -= 10) {
		if (set_file(to + 10) == file1 || set_rank(to + 10) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down left
	for (int to = sq + 10;; to += 10) {
		if (set_file(to - 10) == file9 || set_rank(to - 10) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down right
	for (int to = sq - 8;; to -= 8) {
		if (set_file(to + 8) == file1 || set_rank(to + 8) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
		}
		return m;
	}
}

//dragon black
int* attacks_bdragon(usi_handler_t* hd, int sq, int* m) {
	/*dragonのサーチ sはsqの略
	  1          -1
	2 s 3     +9  s  -9
	  4          +1

	+

	5   6    +8     -10
	  s          s
	7   8    +10    -8
	*/
	//upper
	for (int to = sq - 1;; to -= 1) {
		if (set_rank(to + 1) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//left
	for (int to = sq + 9;; to += 9) {
		if (set_file(to - 9) == file9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//right
	for (int to = sq - 9;; to -= 9) {
		if (set_file(to + 9) == file1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down
	for (int to = sq + 1;; to += 1) {
		if (set_rank(to - 1) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 10;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		return m;
	}
}
