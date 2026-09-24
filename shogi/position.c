#include <stdint.h>

#include "position.h"

/*
*   sq:	sqare 座標
*   f:  file 筋
*   r:  rank 段
*	c:	color 手番
*   pt:	piecetype 駒種
*   pc:	piece 駒番 
*/

// ２つの座標の位置関係のテーブル
int square_relation_direct[81][81];

// 2つの位置関係のテーブル,sq1とsq2の関係がdirect_file=縦方向、direct_rank,direct_diag_nesw,direct_diag_nwse方向なのかを即答してくれるテーブル、それ以外はdirect_misc
void new_square_relation_direct() {
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

// 渡されたsq座標同士の位置関係を返す
int square_relation(const int sq1, const int sq2) {
	return square_relation_direct[sq1][sq2];
}

// 手番を切替える
int opposite_color(const int c) {
	return c ^ 1;
}

// 駒番をblack->white,white->blackにする(bpawn->wpawn)
int inverse(const int pc) {
	return pc ^ 0x10;
}

// 駒番を駒種に変換
int piece_to_piecetype(const int pc) {
	return pc & 15;
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

