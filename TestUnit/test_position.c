#include "unity.h"

#include "../shogi/position.h"
#include "test_position.h"

void test_set_file() {
	TEST_ASSERT_EQUAL_INT(0, set_file(0));
	TEST_ASSERT_EQUAL_INT(1, set_file(10));
	TEST_ASSERT_EQUAL_INT(2, set_file(20));
	TEST_ASSERT_EQUAL_INT(3, set_file(30));
	TEST_ASSERT_EQUAL_INT(4, set_file(40));
	TEST_ASSERT_EQUAL_INT(5, set_file(50));
	TEST_ASSERT_EQUAL_INT(6, set_file(60));
	TEST_ASSERT_EQUAL_INT(7, set_file(70));
	TEST_ASSERT_EQUAL_INT(8, set_file(80));
	TEST_ASSERT_EQUAL_INT(8, set_file(72));
	TEST_ASSERT_EQUAL_INT(7, set_file(64));
	TEST_ASSERT_EQUAL_INT(6, set_file(56));
	TEST_ASSERT_EQUAL_INT(5, set_file(48));
	TEST_ASSERT_EQUAL_INT(3, set_file(32));
	TEST_ASSERT_EQUAL_INT(2, set_file(24));
	TEST_ASSERT_EQUAL_INT(1, set_file(16));
	TEST_ASSERT_EQUAL_INT(0, set_file(8));
}

void test_set_rank() {
	TEST_ASSERT_EQUAL_INT(0, set_rank(0));
	TEST_ASSERT_EQUAL_INT(1, set_rank(10));
	TEST_ASSERT_EQUAL_INT(2, set_rank(20));
	TEST_ASSERT_EQUAL_INT(3, set_rank(30));
	TEST_ASSERT_EQUAL_INT(4, set_rank(40));
	TEST_ASSERT_EQUAL_INT(5, set_rank(50));
	TEST_ASSERT_EQUAL_INT(6, set_rank(60));
	TEST_ASSERT_EQUAL_INT(7, set_rank(70));
	TEST_ASSERT_EQUAL_INT(8, set_rank(80));
	TEST_ASSERT_EQUAL_INT(0, set_rank(72));
	TEST_ASSERT_EQUAL_INT(1, set_rank(64));
	TEST_ASSERT_EQUAL_INT(2, set_rank(56));
	TEST_ASSERT_EQUAL_INT(3, set_rank(48));
	TEST_ASSERT_EQUAL_INT(5, set_rank(32));
	TEST_ASSERT_EQUAL_INT(6, set_rank(24));
	TEST_ASSERT_EQUAL_INT(7, set_rank(16));
	TEST_ASSERT_EQUAL_INT(8, set_rank(8));
}

bool test_set_square() {
	for (int sq = 0; sq < 81; sq += 1) {
		if (set_square(set_file(sq), set_rank(sq)) != sq) {
			return false;
		}
	}
	return true;
}

void test_new_square_relation_direct() {
	int sq1 = sq7b;
	int sq2 = sq2b;
	int sq3 = sq2d;
	int sq4 = sq9i;
	int sq5 = sq7f;
	int sq6 = sq4e;
	TEST_ASSERT_EQUAL_INT(direct_rank, square_relation_direct[sq1][sq2]);
	TEST_ASSERT_EQUAL_INT(direct_misc, square_relation_direct[sq1][sq3]);
	TEST_ASSERT_EQUAL_INT(direct_file, square_relation_direct[sq1][sq5]);
	TEST_ASSERT_EQUAL_INT(direct_file, square_relation_direct[sq2][sq3]);
	TEST_ASSERT_EQUAL_INT(direct_diag_nesw, square_relation_direct[sq2][sq4]);
	TEST_ASSERT_EQUAL_INT(direct_diag_nesw, square_relation_direct[sq4][sq2]);
	TEST_ASSERT_EQUAL_INT(direct_diag_nwse, square_relation_direct[sq1][sq6]);
	TEST_ASSERT_EQUAL_INT(direct_diag_nwse, square_relation_direct[sq6][sq1]);
}

void test_square_relation() {
	int sq1 = sq7b;
	int sq2 = sq2b;
	int sq3 = sq2d;
	int sq4 = sq9i;
	int sq5 = sq7f;
	int sq6 = sq4e;
	TEST_ASSERT_EQUAL_INT(direct_rank, square_relation(sq1,sq2));
	TEST_ASSERT_EQUAL_INT(direct_misc, square_relation(sq1, sq3));
	TEST_ASSERT_EQUAL_INT(direct_file, square_relation(sq1,sq5));
	TEST_ASSERT_EQUAL_INT(direct_file, square_relation(sq2,sq3));
	TEST_ASSERT_EQUAL_INT(direct_diag_nesw, square_relation(sq2,sq4));
	TEST_ASSERT_EQUAL_INT(direct_diag_nesw, square_relation(sq4,sq2));
	TEST_ASSERT_EQUAL_INT(direct_diag_nwse, square_relation(sq1,sq6));
	TEST_ASSERT_EQUAL_INT(direct_diag_nwse, square_relation(sq6, sq1));
}

void test_opposite_color() {
	int color = black;
	TEST_ASSERT_EQUAL_INT(white, opposite_color(color));
	color = white;
	TEST_ASSERT_EQUAL_INT(black, opposite_color(color));
}

void test_inverse() {
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(wpawn+i, inverse(bpawn+i));
	}
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(bpawn + i, inverse(wpawn + i));
	}
}

void test_piece_to_piecetype() {
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(pawn + i, piece_to_piecetype(bpawn + i));
	}
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(pawn + i, piece_to_piecetype(wpawn + i));
	}
}

void test_piece_to_color() {
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(black, piece_to_color(bpawn+i));
	}
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(white, piece_to_color(wpawn + i));
	}
}

void test_piecetype_to_piece() {
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(bpawn + i, piecetype_to_piece(black, pawn + i));
	}
	for (int i = 0; i < 14; i += 1) {
		TEST_ASSERT_EQUAL_INT(wpawn + i, piecetype_to_piece(white, pawn + i));
	}
}

// lance,bishop,rook,horse,dragonのみを選別する
// 0x60646064 & (1 << (bpawn + i) iは0から13まで変化してpawnからdragonまで変化する
//pawn		0x00
//lance		0x04
//knight	0x00
//silver	0x00
//bishop	0x20
//rook		0x40
//gold		0x00
//king		0x00
//propawn	0x00
//prolance	0x00
//proknight	0x00
//prosilver	0x00
//horse		0x2000
//dragon	0x4000
//or結合    0x6064
void test_is_jump() {
	TEST_ASSERT_FALSE(is_jump(bpawn));
	TEST_ASSERT_TRUE(is_jump(blance));
	TEST_ASSERT_FALSE(is_jump(bknight));
	TEST_ASSERT_FALSE(is_jump(bsilver));
	TEST_ASSERT_TRUE(is_jump(bbishop));
	TEST_ASSERT_TRUE(is_jump(brook));
	TEST_ASSERT_FALSE(is_jump(bgold));
	TEST_ASSERT_FALSE(is_jump(bking));
	TEST_ASSERT_FALSE(is_jump(bpropawn));
	TEST_ASSERT_FALSE(is_jump(bprolance));
	TEST_ASSERT_FALSE(is_jump(bproknight));
	TEST_ASSERT_FALSE(is_jump(bprosilver));
	TEST_ASSERT_TRUE(is_jump(bhorse));
	TEST_ASSERT_TRUE(is_jump(bdragon));
	TEST_ASSERT_FALSE(is_jump(wpawn));
	TEST_ASSERT_TRUE(is_jump(wlance));
	TEST_ASSERT_FALSE(is_jump(wknight));
	TEST_ASSERT_FALSE(is_jump(wsilver));
	TEST_ASSERT_TRUE(is_jump(wbishop));
	TEST_ASSERT_TRUE(is_jump(wrook));
	TEST_ASSERT_FALSE(is_jump(wgold));
	TEST_ASSERT_FALSE(is_jump(wking));
	TEST_ASSERT_FALSE(is_jump(wpropawn));
	TEST_ASSERT_FALSE(is_jump(wprolance));
	TEST_ASSERT_FALSE(is_jump(wproknight));
	TEST_ASSERT_FALSE(is_jump(wprosilver));
	TEST_ASSERT_TRUE(is_jump(whorse));
	TEST_ASSERT_TRUE(is_jump(wdragon));
}

// rank1 -> a,rank9 -> i
void test_rank_usi_string() {
	for (int r = rank1; r <= rank9; r += 1) {
		TEST_ASSERT_EQUAL_INT8('a' + r, rank_usi_string(r));
	}
}

// file1 -> 1,file9 -> 9
void test_file_usi_string() {
	for (int f = file1; f <= file9; f += 1) {
		TEST_ASSERT_EQUAL_INT8('1' + f, file_usi_string(f));
	}
}

// square_usi_string_table[81]の初期値が正しいか確認している
void test_square_usi_string() {
	for (int sq = sq1a; sq <= sq9i; sq += 1) {
		char str[8];		
		square_usi_string(sq, str);
		TEST_ASSERT_EQUAL_STRING(str, square_usi_string_table[sq]);
	}
}

// sq1a = 81-1-0=80=sq9i
// sq5c = 81-1-38=sq5g
// sq6b = 81-1-46=sq4h
// sq9i = 81-1-80=sq1a
void test_square_inverse() {
	TEST_ASSERT_EQUAL_INT(sq9i, square_inverse(sq1a));
	TEST_ASSERT_EQUAL_INT(sq5g, square_inverse(sq5c));
	TEST_ASSERT_EQUAL_INT(sq4h, square_inverse(sq6b));
	TEST_ASSERT_EQUAL_INT(sq1a, square_inverse(sq9i));
}

// file1 = 9-1-0=8=file9
// file3 = 9-1-2=6=file7
void test_file_inverse() {
	TEST_ASSERT_EQUAL_INT(file9, file_inverse(file1));
	TEST_ASSERT_EQUAL_INT(file7, file_inverse(file3));
}

// rank1 = 9-1-0=8=rank9
// rank3 = 9-1-2=6=rank7
void test_rank_inverse() {
	TEST_ASSERT_EQUAL_INT(rank9, rank_inverse(rank1));
	TEST_ASSERT_EQUAL_INT(rank7, rank_inverse(rank3));
}

// black駒
// rank1-3 true(white側エリアであれば成れる）
// white駒
// rank7-9 true(white側エリアであれば成れる）
// enemy_mask[2]というbitboardがあり、同じ趣旨であるが今のところ使い分けの判断は保留にしておく(TODO:)
void test_can_promote() {
	for (int r = rank1; r <= rank9; r += 1) {
		if (r <= rank3) {
			TEST_ASSERT_EQUAL_INT(true, can_promote(black, r));
		}
		else {
			TEST_ASSERT_EQUAL_INT(false, can_promote(black, r));
		}
		if (r >= rank7) {
			TEST_ASSERT_EQUAL_INT(true, can_promote(white, r));
		}
		else {
			TEST_ASSERT_EQUAL_INT(false, can_promote(white, r));
		}
	}
}

