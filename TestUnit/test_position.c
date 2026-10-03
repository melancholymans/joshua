#include <string.h>
#include "unity.h"

#include "../shogi/position.h"
#include "../shogi/usi.h"
#include "test_position.h"


void test_set_file(void) {
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

void test_set_rank(void) {
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

bool test_set_square(void) {
	for (int sq = 0; sq < 81; sq++) {
		if (set_square(set_file(sq), set_rank(sq)) != sq) {
			return false;
		}
	}
	return true;
}

void test_new_square_relation_direct(void) {
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

void test_square_relation(void) {
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

void test_opposite_color(void) {
	int color = black;
	TEST_ASSERT_EQUAL_INT(white, opposite_color(color));
	color = white;
	TEST_ASSERT_EQUAL_INT(black, opposite_color(color));
}

void test_inverse(void) {
	for (int i = 0; i < 14; i++) {
		TEST_ASSERT_EQUAL_INT(wpawn+i, inverse(bpawn+i));
	}
	for (int i = 0; i < 14; i++) {
		TEST_ASSERT_EQUAL_INT(bpawn + i, inverse(wpawn + i));
	}
}

void test_piece_to_piecetype(void) {
	for (int i = 0; i < 14; i++) {
		TEST_ASSERT_EQUAL_INT(pawn + i, piece_to_piecetype(bpawn + i));
	}
	for (int i = 0; i < 14; i++) {
		TEST_ASSERT_EQUAL_INT(pawn + i, piece_to_piecetype(wpawn + i));
	}
}

void test_piece_to_color(void) {
	for (int i = 0; i < 14; i++) {
		TEST_ASSERT_EQUAL_INT(black, piece_to_color(bpawn+i));
	}
	for (int i = 0; i < 14; i++) {
		TEST_ASSERT_EQUAL_INT(white, piece_to_color(wpawn + i));
	}
}

void test_piecetype_to_piece(void) {
	for (int i = 0; i < 14; i++) {
		TEST_ASSERT_EQUAL_INT(bpawn + i, piecetype_to_piece(black, pawn + i));
	}
	for (int i = 0; i < 14; i++) {
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
void test_is_jump(void) {
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
void test_rank_usi_string(void) {
	for (int r = rank1; r <= rank9; r++) {
		TEST_ASSERT_EQUAL_INT8('a' + r, rank_usi_string(r));
	}
}

// file1 -> 1,file9 -> 9
void test_file_usi_string(void) {
	for (int f = file1; f <= file9; f++) {
		TEST_ASSERT_EQUAL_INT8('1' + f, file_usi_string(f));
	}
}

// square_usi_string_table[81]の初期値が正しいか確認している
void test_square_usi_string(void) {
	for (int sq = sq1a; sq <= sq9i; sq++) {
		char str[8];		
		square_usi_string(sq, str);
		TEST_ASSERT_EQUAL_STRING(str, square_usi_string_table[sq]);
	}
}

// sq1a = 81-1-0=80=sq9i
// sq5c = 81-1-38=sq5g
// sq6b = 81-1-46=sq4h
// sq9i = 81-1-80=sq1a
void test_square_inverse(void) {
	TEST_ASSERT_EQUAL_INT(sq9i, square_inverse(sq1a));
	TEST_ASSERT_EQUAL_INT(sq5g, square_inverse(sq5c));
	TEST_ASSERT_EQUAL_INT(sq4h, square_inverse(sq6b));
	TEST_ASSERT_EQUAL_INT(sq1a, square_inverse(sq9i));
}

// file1 = 9-1-0=8=file9
// file3 = 9-1-2=6=file7
void test_file_inverse(void) {
	TEST_ASSERT_EQUAL_INT(file9, file_inverse(file1));
	TEST_ASSERT_EQUAL_INT(file7, file_inverse(file3));
}

// rank1 = 9-1-0=8=rank9
// rank3 = 9-1-2=6=rank7
void test_rank_inverse(void) {
	TEST_ASSERT_EQUAL_INT(rank9, rank_inverse(rank1));
	TEST_ASSERT_EQUAL_INT(rank7, rank_inverse(rank3));
}

// black駒
// rank1-3 true(white側エリアであれば成れる）
// white駒
// rank7-9 true(white側エリアであれば成れる）
// enemy_mask[2]というbitboardがあり、同じ趣旨であるが今のところ使い分けの判断は保留にしておく(TODO:)
void test_can_promote(void) {
	for (int r = rank1; r <= rank9; r++) {
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

// test問題1
// position sfen lnsgkgsnl/9/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL w - 1 moves 5a6b 7g7f 3a3b
// test問題2(コンピュータ将棋2問題1)
// position sfen lR1B3nl/2gp5/ngk1+B5pPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b - 1
// test問題3(コンピュータ将棋2問題46を改変)
// position sfen l2g2ks1/4+P3+L/2p1+S2pn/p2p2+r1p/5+B3/P3S2PP/1PPP+b1P2/1rG2P3/LN2KG1+n b - 1
void test_set_board(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	sfen[0] = '\0';
	// test問題1
	strcpy_s(sfen, sizeof(sfen), "lnsgkgsnl/9/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	board_t* b = hd.bd;
	TEST_ASSERT_EQUAL_INT(wlance, b->mb[sq9a]);
	TEST_ASSERT_EQUAL_INT(wknight, b->mb[sq8a]);
	TEST_ASSERT_EQUAL_INT(wsilver, b->mb[sq7a]);
	TEST_ASSERT_EQUAL_INT(wgold, b->mb[sq6a]);
	TEST_ASSERT_EQUAL_INT(wking, b->mb[sq5a]);
	TEST_ASSERT_EQUAL_INT(wgold, b->mb[sq4a]);
	TEST_ASSERT_EQUAL_INT(wsilver, b->mb[sq3a]);
	TEST_ASSERT_EQUAL_INT(wknight, b->mb[sq2a]);
	TEST_ASSERT_EQUAL_INT(wlance, b->mb[sq1a]);

	for (int sq = sq9b; sq >= sq1b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	for (int sq = sq9c; sq >= sq1c; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq]);
	}

	for (int sq = sq9d; sq >= sq1d; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	for (int sq = sq9e; sq >= sq1e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	for (int sq = sq9f; sq >= sq1f; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	for (int sq = sq9g; sq >= sq1g; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq]);
	}

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9h]);
	TEST_ASSERT_EQUAL_INT(bbishop, b->mb[sq8h]);
	for (int sq = sq7h; sq >= sq3h; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(brook, b->mb[sq2h]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq1h]);

	TEST_ASSERT_EQUAL_INT(blance, b->mb[sq9i]);
	TEST_ASSERT_EQUAL_INT(bknight, b->mb[sq8i]);
	TEST_ASSERT_EQUAL_INT(bsilver, b->mb[sq7i]);
	TEST_ASSERT_EQUAL_INT(bgold, b->mb[sq6i]);
	TEST_ASSERT_EQUAL_INT(bking, b->mb[sq5i]);
	TEST_ASSERT_EQUAL_INT(bgold, b->mb[sq4i]);
	TEST_ASSERT_EQUAL_INT(bsilver, b->mb[sq3i]);
	TEST_ASSERT_EQUAL_INT(bknight, b->mb[sq2i]);
	TEST_ASSERT_EQUAL_INT(blance, b->mb[sq1i]);

	TEST_ASSERT_EQUAL_INT(white, b->turn);
	TEST_ASSERT_EQUAL_INT(1, b->move_number);

	// test問題2
	sfen[0] = '\0';
	strcpy_s(sfen, sizeof(sfen), "lR1B3nl/2gp5/ngk1+BspPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	b = hd.bd;
	TEST_ASSERT_EQUAL_INT(wlance, b->mb[sq9a]);
	TEST_ASSERT_EQUAL_INT(brook, b->mb[sq8a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq7a]);
	TEST_ASSERT_EQUAL_INT(bbishop, b->mb[sq6a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq5a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq3a]);
	TEST_ASSERT_EQUAL_INT(wknight, b->mb[sq2a]);
	TEST_ASSERT_EQUAL_INT(wlance, b->mb[sq1a]);

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9b]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq8b]);
	TEST_ASSERT_EQUAL_INT(wgold, b->mb[sq7b]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq6b]);
	for (int sq = sq5b; sq >= sq1b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	TEST_ASSERT_EQUAL_INT(wknight, b->mb[sq9c]);
	TEST_ASSERT_EQUAL_INT(wgold, b->mb[sq8c]);
	TEST_ASSERT_EQUAL_INT(wking, b->mb[sq7c]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq6c]);
	TEST_ASSERT_EQUAL_INT(bhorse, b->mb[sq5c]);
	TEST_ASSERT_EQUAL_INT(wsilver, b->mb[sq4c]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq3c]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq2c]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq1c]);

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9d]);
	TEST_ASSERT_EQUAL_INT(wsilver, b->mb[sq8d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq7d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq6d]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq5d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq3d]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq2d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq1d]);

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9d]);
	TEST_ASSERT_EQUAL_INT(wsilver, b->mb[sq8d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq7d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq6d]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq5d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq3d]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq2d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq1d]);

	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq9e]);
	for (int sq = sq8e; sq >= sq5e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bsilver, b->mb[sq4e]);
	for (int sq = sq3e; sq >= sq1e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9f]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq8f]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq7f]);
	for (int sq = sq6f; sq >= sq1f; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq9g]);
	for (int sq = sq8g; sq >= sq4g; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq3g]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq2g]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq1g]);

	TEST_ASSERT_EQUAL_INT(blance, b->mb[sq9h]);
	TEST_ASSERT_EQUAL_INT(bgold, b->mb[sq8h]);
	TEST_ASSERT_EQUAL_INT(bgold, b->mb[sq7h]);
	for (int sq = sq6h; sq >= sq1h; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	TEST_ASSERT_EQUAL_INT(bking, b->mb[sq9i]);
	TEST_ASSERT_EQUAL_INT(bknight, b->mb[sq8i]);
	for (int sq = sq7i; sq >= sq3i; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bknight, b->mb[sq2i]);
	TEST_ASSERT_EQUAL_INT(blance, b->mb[sq1i]);


	// test問題3
	sfen[0] = '\0';
	strcpy_s(sfen, sizeof(sfen), "l2g2ks1/4+P3+L/2p1+S2pn/p2p2+r1p/5+B3/P3S2PP/1PPP+b1P2/1rG2P3/LN2KG1+n b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	b = hd.bd;
	TEST_ASSERT_EQUAL_INT(wlance, b->mb[sq9a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq8a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq7a]);
	TEST_ASSERT_EQUAL_INT(wgold, b->mb[sq6a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq5a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4a]);
	TEST_ASSERT_EQUAL_INT(wking, b->mb[sq3a]);
	TEST_ASSERT_EQUAL_INT(wsilver, b->mb[sq2a]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq1a]);

	for (int sq = sq9b; sq >= sq6b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bpropawn, b->mb[sq5b]);
	for (int sq = sq4b; sq >= sq2b; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bprolance, b->mb[sq1b]);

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9c]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq8c]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq7c]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq6c]);
	TEST_ASSERT_EQUAL_INT(bprosilver, b->mb[sq5c]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4c]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq3c]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq2c]);
	TEST_ASSERT_EQUAL_INT(wknight, b->mb[sq1c]);

	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq9d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq8d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq7d]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq6d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq5d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4d]);
	TEST_ASSERT_EQUAL_INT(wdragon, b->mb[sq3d]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq2d]);
	TEST_ASSERT_EQUAL_INT(wpawn, b->mb[sq1d]);

	for (int sq = sq9e; sq >= sq5e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bhorse, b->mb[sq4e]);
	for (int sq = sq3e; sq >= sq1e; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq9f]);
	for (int sq = sq8f; sq >= sq6f; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}
	TEST_ASSERT_EQUAL_INT(bsilver, b->mb[sq5f]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4f]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq3f]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq2f]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq1f]);

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9g]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq8g]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq7g]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq6g]);
	TEST_ASSERT_EQUAL_INT(whorse, b->mb[sq5g]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq4g]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq3g]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq2g]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq1g]);

	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq9h]);
	TEST_ASSERT_EQUAL_INT(wrook, b->mb[sq8h]);
	TEST_ASSERT_EQUAL_INT(bgold, b->mb[sq7h]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq6h]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq5h]);
	TEST_ASSERT_EQUAL_INT(bpawn, b->mb[sq4h]);
	for (int sq = sq3h; sq >= sq1h; sq -= 9) {
		TEST_ASSERT_EQUAL_INT(empty, b->mb[sq]);
	}

	TEST_ASSERT_EQUAL_INT(blance, b->mb[sq9i]);
	TEST_ASSERT_EQUAL_INT(bknight, b->mb[sq8i]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq7i]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq6i]);
	TEST_ASSERT_EQUAL_INT(bking, b->mb[sq5i]);
	TEST_ASSERT_EQUAL_INT(bgold, b->mb[sq4i]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq3i]);
	TEST_ASSERT_EQUAL_INT(wproknight, b->mb[sq2i]);
	TEST_ASSERT_EQUAL_INT(empty, b->mb[sq1i]);


	// moveができていないので、hd(持ち駒)のテストはできていない
}

void test_print_board(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	new_board(&hd);
	char sfen[128];
	sfen[0] = '\0';	
	strcpy_s(sfen, sizeof(sfen), "l2g2ks1/4+P3+L/2p1+S2pn/p2p2+r1p/5+B3/P3S2PP/1PPP+b1P2/1rG2P3/LN2KG1+n b - 1");
	set_board(&hd, sfen);
	print_board(&hd);
}