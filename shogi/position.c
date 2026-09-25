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
// 座標文字列
char* square_usi_string_table[81] = {"1a","1b","1c","1d","1e","1f","1g","1h","1i",
							         "2a","2b","2c","2d","2e","2f","2g","2h","2i",
							         "3a","3b","3c","3d","3e","3f","3g","3h","3i",
							         "4a","4b","4c","4d","4e","4f","4g","4h","4i",
							         "5a","5b","5c","5d","5e","5f","5g","5h","5i",
							         "6a","6b","6c","6d","6e","6f","6g","6h","6i",
							         "7a","7b","7c","7d","7e","7f","7g","7h","7i",
							         "8a","8b","8c","8d","8e","8f","8g","8h","8i",
							         "9a","9b","9c","9d","9e","9f","9g","9h","9i"};

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

// 座標sqの段の文字を返す
char rank_usi_string(const int r) {
	return 'a' + r;
}

// 座標sqの筋の文字を返す
char file_usi_string(const int f) {
	return '1' + f;
}

// 座標の文字列を返す
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
_Bool can_promote(const int c, const int from_or_to_rank) {
	return (0x1c00007u & (1u << ((c << 4) + from_or_to_rank)));
}

/*
* やねうら王から移植された壁つきの升表現というSquareWithWallがあるがいまのところ実装は後回し(TODO:)
*/

/*
*  やねうら王から移植されたLong Effect Libraryがあるがいまのところ実装は後回し(TODO:)
*/
