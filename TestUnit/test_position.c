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

void test_movesq_to_usisq(void) {
	char usi_sq[4];
	movesq_to_usisq(sq1a, usi_sq);
	TEST_ASSERT_EQUAL_STRING("1a", usi_sq);
	movesq_to_usisq(sq2b, usi_sq);
	TEST_ASSERT_EQUAL_STRING("2b", usi_sq);
	movesq_to_usisq(sq3c, usi_sq);
	TEST_ASSERT_EQUAL_STRING("3c", usi_sq);
	movesq_to_usisq(sq4d, usi_sq);
	TEST_ASSERT_EQUAL_STRING("4d", usi_sq);
	movesq_to_usisq(sq5e, usi_sq);
	TEST_ASSERT_EQUAL_STRING("5e", usi_sq);
	movesq_to_usisq(sq6f, usi_sq);
	TEST_ASSERT_EQUAL_STRING("6f", usi_sq);
	movesq_to_usisq(sq7g, usi_sq);
	TEST_ASSERT_EQUAL_STRING("7g", usi_sq);
	movesq_to_usisq(sq8h, usi_sq);
	TEST_ASSERT_EQUAL_STRING("8h", usi_sq);
	movesq_to_usisq(sq9i, usi_sq);
	TEST_ASSERT_EQUAL_STRING("9i", usi_sq);
	movesq_to_usisq(sq9a, usi_sq);
	TEST_ASSERT_EQUAL_STRING("9a", usi_sq);
	movesq_to_usisq(sq8b, usi_sq);
	TEST_ASSERT_EQUAL_STRING("8b", usi_sq);
	movesq_to_usisq(sq7c, usi_sq);
	TEST_ASSERT_EQUAL_STRING("7c", usi_sq);
	movesq_to_usisq(sq6d, usi_sq);
	TEST_ASSERT_EQUAL_STRING("6d", usi_sq);
	movesq_to_usisq(sq4f, usi_sq);
	TEST_ASSERT_EQUAL_STRING("4f", usi_sq);
	movesq_to_usisq(sq3g, usi_sq);
	TEST_ASSERT_EQUAL_STRING("3g", usi_sq);
	movesq_to_usisq(sq2h, usi_sq);
	TEST_ASSERT_EQUAL_STRING("2h", usi_sq);
	movesq_to_usisq(sq1i, usi_sq);
	TEST_ASSERT_EQUAL_STRING("1i", usi_sq);
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
void test_can_promote_area(void) {
	for (int r = rank1; r <= rank9; r++) {
		if (r <= rank3) {
			TEST_ASSERT_EQUAL_INT(true, can_promote_area(black, r));
		}
		else {
			TEST_ASSERT_EQUAL_INT(false, can_promote_area(black, r));
		}
		if (r >= rank7) {
			TEST_ASSERT_EQUAL_INT(true, can_promote_area(white, r));
		}
		else {
			TEST_ASSERT_EQUAL_INT(false, can_promote_area(white, r));
		}
	}
}

// test問題1
// position sfen lnsgkgsnl/9/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL w - 1 moves 5a6b 7g7f 3a3b
// test問題2(コンピュータ将棋2問題1)
// position sfen lR1B3nl/2gp5/ngk1+B5pPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b - 1
// test問題3(コンピュータ将棋2問題46を改変)
// position sfen l2g2ks1/4+P3+L/2p1+S2pn/p2p2+r1p/5+B3/P3S2PP/1PPP+b1P2/1rG2P3/LN2KG1+n b - 1
// test問題4 創作
//   9  8  7  6  5  4  3  2  1
//a  .  .  .  .  .  .  .  v  l
//b  .  d  .  .  .  h  g  k  .
//c  .  .  A  a  .  g  s  p  p
//d  P  .  .  .  .  p  .  .  .
//e  .  .  .  .  P  .  P  P  .
//f  .  c  .  P  .  .  .  .  .
//g  .  .  V  G  .  i  .  .  P
//h  .  g  .  I  .  .  .  D  .
//i  C  .  .  K  H  .  .  N  L

//P : 0 L : 0 N : 0 S : 0 B : 0 R : 0 G : 0
//p : 0 l : 0 n : 0 s : 0 b : 0 r : 0 g : 0
//a : ppawn c : plance v : knight i : psilver h : horse d : dragon
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

	//TEST_ASSERT_EQUAL_INT(white, b->turn);	ここでは手番を判断できないのでテストはパス
	TEST_ASSERT_EQUAL_INT(0, b->move_number);
	TEST_ASSERT_EQUAL_INT(sq5i, b->king_square[black]);
	TEST_ASSERT_EQUAL_INT(sq5a, b->king_square[white]);
	
	bitboard bb = b->bytype_bb[pawn];
	TEST_ASSERT_TRUE(is_biton(sq9c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq2g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	//print_bitboard(bb,__func__);
	bb = b->bytype_bb[lance];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[knight];
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[silver];
	TEST_ASSERT_TRUE(is_biton(sq7a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[gold];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[gold];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[king];
	TEST_ASSERT_TRUE(is_biton(sq5a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[bishop];
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[rook];
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[rook];
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	//print_bitboard(bb, __func__);

	bb = b->bycolor_bb[black];
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq2g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq7i, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bycolor_bb[white];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq9c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	//print_bitboard(bb, __func__);

	bb = b->bytype_bb[ALL_PIECE];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq9c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq2g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq7i, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[HDK];
	TEST_ASSERT_TRUE(is_biton(sq5a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[BISHOP_HORSE];
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[ROOK_DRAGON];
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[SILVER_HDK];
	TEST_ASSERT_TRUE(is_biton(sq7a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS_HDK];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	//print_bitboard(bb, __func__);

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
	TEST_ASSERT_EQUAL_INT(sq9i, b->king_square[black]);
	TEST_ASSERT_EQUAL_INT(sq7c, b->king_square[white]);

	bb = b->bytype_bb[pawn];
	TEST_ASSERT_TRUE(is_biton(sq6b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5d, bb));
	TEST_ASSERT_TRUE(is_biton(sq2d, bb));
	TEST_ASSERT_TRUE(is_biton(sq9e, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq7f, bb));
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[lance];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq9h, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[knight];
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq9c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[silver];
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[gold];
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[bishop];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[rook];
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[king];
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[horse];
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	//print_bitboard(bb, __func__);
	bb = b->bycolor_bb[black];
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	TEST_ASSERT_TRUE(is_biton(sq9h, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	bb = b->bycolor_bb[white];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6b, bb));
	TEST_ASSERT_TRUE(is_biton(sq9c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8d, bb));
	TEST_ASSERT_TRUE(is_biton(sq5d, bb));
	TEST_ASSERT_TRUE(is_biton(sq2d, bb));
	TEST_ASSERT_TRUE(is_biton(sq9e, bb));
	TEST_ASSERT_TRUE(is_biton(sq7f, bb));

	bb = b->bytype_bb[ALL_PIECE];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6b, bb));
	TEST_ASSERT_TRUE(is_biton(sq9c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8d, bb));
	TEST_ASSERT_TRUE(is_biton(sq5d, bb));
	TEST_ASSERT_TRUE(is_biton(sq2d, bb));
	TEST_ASSERT_TRUE(is_biton(sq9e, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq7f, bb));
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	TEST_ASSERT_TRUE(is_biton(sq9h, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS];
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[HDK];
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[BISHOP_HORSE];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[ROOK_DRAGON];
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[SILVER_HDK];
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS_HDK];
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	//print_bitboard(bb, __func__);

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
	TEST_ASSERT_EQUAL_INT(sq5i, b->king_square[black]);
	TEST_ASSERT_EQUAL_INT(sq3a, b->king_square[white]);

	bb = b->bytype_bb[pawn];
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9d, bb));
	TEST_ASSERT_TRUE(is_biton(sq6d, bb));
	TEST_ASSERT_TRUE(is_biton(sq1d, bb));
	TEST_ASSERT_TRUE(is_biton(sq9f, bb));
	TEST_ASSERT_TRUE(is_biton(sq2f, bb));
	TEST_ASSERT_TRUE(is_biton(sq1f, bb));
	TEST_ASSERT_TRUE(is_biton(sq8g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq4h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[lance];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[knight];
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[silver];
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5f, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[gold];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[rook];
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[king];
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[propawn];
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[prolance];
	TEST_ASSERT_TRUE(is_biton(sq1b, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[proknight];
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[horse];
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[dragon];
	TEST_ASSERT_TRUE(is_biton(sq3d, bb));
	//print_bitboard(bb, __func__);

	bb = b->bycolor_bb[black];
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq1b, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq9f, bb));
	TEST_ASSERT_TRUE(is_biton(sq5f, bb));
	TEST_ASSERT_TRUE(is_biton(sq2f, bb));
	TEST_ASSERT_TRUE(is_biton(sq1f, bb));
	TEST_ASSERT_TRUE(is_biton(sq8g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq4h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	bb = b->bycolor_bb[white];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9d, bb));
	TEST_ASSERT_TRUE(is_biton(sq6d, bb));
	TEST_ASSERT_TRUE(is_biton(sq3d, bb));
	TEST_ASSERT_TRUE(is_biton(sq1d, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, __func__);

	bb = b->bytype_bb[ALL_PIECE];
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq1b, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9d, bb));
	TEST_ASSERT_TRUE(is_biton(sq6d, bb));
	TEST_ASSERT_TRUE(is_biton(sq3d, bb));
	TEST_ASSERT_TRUE(is_biton(sq1d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq9f, bb));
	TEST_ASSERT_TRUE(is_biton(sq5f, bb));
	TEST_ASSERT_TRUE(is_biton(sq2f, bb));
	TEST_ASSERT_TRUE(is_biton(sq1f, bb));
	TEST_ASSERT_TRUE(is_biton(sq8g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq4h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS];
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq1b, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[HDK];
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[BISHOP_HORSE];
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[ROOK_DRAGON];
	TEST_ASSERT_TRUE(is_biton(sq3d, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[SILVER_HDK];
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5f, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS_HDK];
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq1b, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, __func__);

	// test問題4 bytype_bb, bycolor_bbのテストのみ
	sfen[0] = '\0';
	strcpy_s(sfen, sizeof(sfen), "7+nl/1+r3+bgk1/2+P+p1gspp/P4p3/4P1PP1/1+l1P5/2+NG1+s2P/1g1+S3+R1/+L2K+B2NL b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	b = hd.bd;
	bb = b->bytype_bb[pawn];
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4d, bb));
	TEST_ASSERT_TRUE(is_biton(sq5e, bb));
	TEST_ASSERT_TRUE(is_biton(sq3e, bb));
	TEST_ASSERT_TRUE(is_biton(sq2e, bb));
	TEST_ASSERT_TRUE(is_biton(sq6f, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	//print_bitboard(bb, "Q4 pawn");
	bb = b->bytype_bb[lance];
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, "Q4 lance");
	bb = b->bytype_bb[knight];
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, "Q4 knight");
	bb = b->bytype_bb[silver];
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	//print_bitboard(bb, "Q4 silver");
	bb = b->bytype_bb[gold];
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	//print_bitboard(bb, "Q4 gold");
	bb = b->bytype_bb[king];
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	bb = b->bytype_bb[propawn];
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	//print_bitboard(bb, "Q4 propawn");
	bb = b->bytype_bb[prolance];
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	//print_bitboard(bb, "Q4 prolance");
	bb = b->bytype_bb[proknight];
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	bb = b->bytype_bb[prosilver];
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	bb = b->bytype_bb[horse];
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	bb = b->bytype_bb[dragon];
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	//print_bitboard(bb, "Q4");

	bb = b->bycolor_bb[black];
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9d, bb));
	TEST_ASSERT_TRUE(is_biton(sq5e, bb));
	TEST_ASSERT_TRUE(is_biton(sq3e, bb));
	TEST_ASSERT_TRUE(is_biton(sq2e, bb));
	TEST_ASSERT_TRUE(is_biton(sq6f, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	bb = b->bycolor_bb[white];
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4d, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	
	bb = b->bytype_bb[ALL_PIECE];
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq9d, bb));
	TEST_ASSERT_TRUE(is_biton(sq4d, bb));
	TEST_ASSERT_TRUE(is_biton(sq5e, bb));
	TEST_ASSERT_TRUE(is_biton(sq3e, bb));
	TEST_ASSERT_TRUE(is_biton(sq2e, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq6f, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS];
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[HDK];
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[BISHOP_HORSE];
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[ROOK_DRAGON];
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[SILVER_HDK];
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	//print_bitboard(bb, __func__);
	bb = b->bytype_bb[GOLDS_HDK];
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq6c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq6g, bb));
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	//print_bitboard(bb, __func__);
}

// test問題1
// position sfen ln4snl/1rs1gkgb1/1pp1pp1pp/6p2/p8/4P4/PPP2PPPP/L1Sp1gKSR/1NB2+p1NL b - 1
//   9  8  7  6  5  4  3  2  1
//a  l  n  .  .  .  .  s  n  l
//b  .  r  s  .  g  k  g  b  .
//c  .  p  p  .  p  p  .  p  p
//d  .  .  .  .  .  .  p  .  .
//e  p  .  .  .  .  .  .  .  .
//f  .  .  .  .  P  .  .  .  .
//g  P  P  P  .  .  P  P  P  P
//h  L  .  S  p  .  g  K  S  R
//i  .  N  B  .  .  a  .  N  L

//P : 0 L : 0 N : 0 S : 0 B : 0 R : 0 G : 0
//p : 0 l : 0 n : 0 s : 0 b : 0 r : 0 g : 0

// test問題2
// position sfen +Ln1+S2snl/1r2gkg2/p1pppp1p+R/6p2/2NP5/2P1PBP2/+nP+b2PGP1/3K5/+l1+l1G1+s2 b - 1
//   9  8  7  6  5  4  3  2  1
//a  C  n  .  I  .  .  s  n  l
//b  .  r  .  .  g  k  g  .  .
//c  p  .  p  p  p  p  .  p  D
//d  .  .  .  .  .  .  p  .  .
//e  .  .  N  P  .  .  .  .  .
//f  .  .  P  .  P  B  P  .  .
//g  v  P  h  .  .  P  G  P  .
//h  .  .  .  K  .  .  .  .  .
//i  c  .  c  .  G  .  i  .  .

void test_attacks_to(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	sfen[0] = '\0';
	// test問題1
	strcpy_s(sfen, sizeof(sfen), "ln4snl/1rs1gkgb1/1pp1pp1pp/6p2/p8/4P4/PPP2PPPP/L1Sp1gKSR/1NB2+p1NL b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	int sq = sq8a;
	bitboard bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	//print_bitboard(bb, "test_set_state");
	sq = sq3a;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	//print_bitboard(bb, "test_set_state");
	sq = sq2a;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 2a");
	sq = sq1a;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	//print_bitboard(bb, "test_set_state 1a");
	sq = sq7b;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	//print_bitboard(bb, "test_set_state 7b");
	sq = sq5b;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	//print_bitboard(bb, "test_set_state 5b");
	sq = sq3b;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	//print_bitboard(bb, "test_set_state 3b");
	sq = sq2b;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	//print_bitboard(bb, "test_set_state 2b");
	sq = sq8c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	//print_bitboard(bb, "test_set_state 8c");
	sq = sq7c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	//print_bitboard(bb, "test_set_state 7c");
	sq = sq5c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	//print_bitboard(bb, "test_set_state 5c");
	sq = sq4c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	//print_bitboard(bb, "test_set_state 4c");
	sq = sq2c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	//print_bitboard(bb, "test_set_state 2c");
	sq = sq1c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	//print_bitboard(bb, "test_set_state 1c");
	sq = sq9e;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	//print_bitboard(bb, "test_set_state 9e");
	sq = sq6h;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7i, bb));
	//print_bitboard(bb, "test_set_state 6h");
	sq = sq4h;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3h, bb));
	TEST_ASSERT_TRUE(is_biton(sq4i, bb));
	//print_bitboard(bb, "test_set_state 4h");
	sq = sq4i;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4h, bb));
	TEST_ASSERT_TRUE(is_biton(sq3h, bb));
	//print_bitboard(bb, "test_set_state 4i");
	sq = sq5f;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 5f");
	sq = sq9g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq9h, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq7i, bb));
	//print_bitboard(bb, "test_set_state 9g");
	sq = sq8g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	//print_bitboard(bb, "test_set_state 8g");
	sq = sq7g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq2b, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	//print_bitboard(bb, "test_set_state 7g");
	sq = sq4g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4h, bb));
	TEST_ASSERT_TRUE(is_biton(sq3h, bb));
	//print_bitboard(bb, "test_set_state 4g");
	sq = sq3g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, "test_set_state 3g");
	sq = sq2g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	//print_bitboard(bb, "test_set_state 2g");
	sq = sq1g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq1h, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	//print_bitboard(bb, "test_set_state 1g");
	sq = sq9h;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 9h");
	sq = sq7h;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 7h");
	sq = sq3h;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4h, bb));
	//print_bitboard(bb, "test_set_state 3h");
	sq = sq2h;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3h, bb));
	TEST_ASSERT_TRUE(is_biton(sq1h, bb));
	//print_bitboard(bb, "test_set_state 2h");
	sq = sq1h;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	//print_bitboard(bb, "test_set_state 1h");
	sq = sq8i;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	//print_bitboard(bb, "test_set_state 8i");
	sq = sq7i;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 7i");
	sq = sq2i;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3h, bb));
	//print_bitboard(bb, "test_set_state 2i");
	sq = sq1i;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq2h, bb));
	TEST_ASSERT_TRUE(is_biton(sq1h, bb));
	//print_bitboard(bb, "test_set_state 1i");
	// test問題2
	strcpy_s(sfen, sizeof(sfen), "+Ln1+S2snl/1r2gkg2/p1pppp1p+R/6p2/2NP5/2P1PBP2/+nP+b2PGP1/3K5/+l1+l1G1+s2 b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	sq = sq8a;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	//print_bitboard(bb, "test_set_state 8a");
	sq = sq3a;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	//print_bitboard(bb, "test_set_state 3a");
	sq = sq1a;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	//print_bitboard(bb, "test_set_state 1a");
	sq = sq8b;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 8b");
	sq = sq5b;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	//print_bitboard(bb, "test_set_state 5b");
	sq = sq4b;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	//print_bitboard(bb, "test_set_state 4b");
	sq = sq3b;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	//print_bitboard(bb, "test_set_state 3b");
	sq = sq9c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	//print_bitboard(bb, "test_set_state 9c");
	sq = sq7c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4f, bb));
	//print_bitboard(bb, "test_set_state 7c");
	sq = sq6c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7e, bb));
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	//print_bitboard(bb, "test_set_state 6c");
	sq = sq5c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	//print_bitboard(bb, "test_set_state 5c");
	sq = sq4c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq5b, bb));
	TEST_ASSERT_TRUE(is_biton(sq4b, bb));
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	//print_bitboard(bb, "test_set_state 4c");
	sq = sq2c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	//print_bitboard(bb, "test_set_state 2c");
	sq = sq9g;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 9g");
	sq = sq7g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	//print_bitboard(bb, "test_set_state 7g");
	sq = sq9i;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	//print_bitboard(bb, "test_set_state 9i");
	sq = sq7i;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	//print_bitboard(bb, "test_set_state 7i");
	sq = sq3i;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 3i");
	sq = sq9a;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 9a");
	sq = sq6a;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 6a");
	sq = sq1c;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq4f, bb));
	//print_bitboard(bb, "test_set_state 1c");
	sq = sq7e;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7f, bb));
	//print_bitboard(bb, "test_set_state 7e");
	sq = sq6e;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 6e");
	sq = sq7f;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	//print_bitboard(bb, "test_set_state 7f");
	sq = sq5f;
	bb = attacks_to(&hd, sq);
	//print_bitboard(bb, "test_set_state 5f");
	sq = sq4f;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	//print_bitboard(bb, "test_set_state 4f");
	sq = sq3f;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	//print_bitboard(bb, "test_set_state 3f");
	sq = sq8g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq8b, bb));
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	//print_bitboard(bb, "test_set_state 8g");
	sq = sq4g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	//print_bitboard(bb, "test_set_state 4g");
	sq = sq3g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq4f, bb));
	//print_bitboard(bb, "test_set_state 3g");
	sq = sq2g;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	//print_bitboard(bb, "test_set_state 2g");
	sq = sq6h;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	TEST_ASSERT_TRUE(is_biton(sq5i, bb));
	TEST_ASSERT_TRUE(is_biton(sq4f, bb));
	//print_bitboard(bb, "test_set_state 6h");
	sq = sq5i;
	bb = attacks_to(&hd, sq);
	TEST_ASSERT_TRUE(is_biton(sq6h, bb));
	//print_bitboard(bb, "test_set_state 5i");
}

// test問題1
// position sfen ln4snl/1rs1gkgb1/1pp1pp1pp/6p2/p8/4P4/PPP2PPPP/L1Sp1gKSR/1NB2+p1NL b - 1
//   9  8  7  6  5  4  3  2  1
//a  l  n  .  .  .  .  s  n  l
//b  .  r  s  .  g  k  g  b  .
//c  .  p  p  .  p  p  .  p  p
//d  .  .  .  .  .  .  p  .  .
//e  p  .  .  .  .  .  .  .  .
//f  .  .  .  .  P  .  .  .  .
//g  P  P  P  .  .  P  P  P  P
//h  L  .  S  p  .  g  K  S  R
//i  .  N  B  .  .  a  .  N  L

// test問題2
// position sfen +Ln1+S2snl/1r2gkg2/p1pppp1p+R/6p2/2NP5/2P1PBP2/+nP+b2PGP1/3K5/+l1+l1G1+s2 b - 1
//   9  8  7  6  5  4  3  2  1
//a  C  n  .  I  .  .  s  n  l
//b  .  r  .  .  g  k  g  .  .
//c  p  .  p  p  p  p  .  p  D
//d  .  .  .  .  .  .  p  .  .
//e  .  .  N  P  .  .  .  .  .
//f  .  .  P  .  P  B  P  .  .
//g  v  P  h  .  .  P  G  P  .
//h  .  .  .  K  .  .  .  .  .
//i  c  .  c  .  G  .  i  .  .

// test問題3
// position sfen +Ln1+S2snl/1r2gkg2/p1pppp1p+R/6p2/2NP5/2P1PBP2/+nP+b2PGP1/3K5/+l1+l1G1+s2 b - 1
//   9  8  7  6  5  4  3  2  1
//a  l  n  s  .  .  .  s  n  l
//b  .  r  .  .  .  k  g  .  .
//c  p  p  p  p  p  g  .  p  p
//d  .  .  .  .  .  .  p  .  .
//e  P  N  .  .  .  p  .  N  .
//f  .  P  .  P  .  .  P  .  P
//g  .  .  P  .  P  P  b  P  .
//h  .  .  .  .  .  .  .  r  h
//i  i  .  S  G  K  G  .  .  .

// test問題4
//   9  8  7  6  5  4  3  2  1
//a  l  n  s  .  .  .  s  n  l
//b  .  r  .  .  .  k  g  b  .
//c  p  .  p  p  p  g  p  .  p
//d  .  .  .  .  .  p  .  p  .
//e  .  .  P  .  .  .  .  .  .
//f  P  .  .  .  P  .  .  P  .
//g  .  .  .  P  .  P  P  .  P
//h  L  .  a  .  K  .  G  R  .
//i  .  .  .  s  .  .  S  N  L

// test問題5
//   9  8  7  6  5  4  3  2  1
//a  l  n  .  .  .  .  s  n  l
//b  .  r  .  .  g  k  g  .  .
//c  p  p  .  .  p  p  .  p  p
//d  .  s  .  .  .  .  p  .  P
//e  .  .  .  .  .  .  .  .  .
//f  .  P  .  .  .  .  .  .  .
//g  P  .  a  .  h  P  P  P  N
//h  .  .  .  .  .  K  S  .  R
//i  v  .  P  .  .  G  .  .  L
// test問題6
//   9  8  7  6  5  4  3  2  1
//a  H  n  k  g  . R   .  .  l
//b  .  .  s  S  G  p  c  .  .
//c  p  p  p  p  p  .  .  .  p
//d  .  .  .  .  .  .  .  p  .
//e  .  .  .  .  .  .  p  .  .
//f  .  .  P  .  .  .  .  .  .
//g  P  P  .  P  P  P  S  P  P
//h  .  .  G  K  .  .  .  R  b
//i  L  N  S  .  .  G  .  N  L
// test問題7
//   9  8  7  6  5  4  3  2  1
//a  C  n  I  .  k  .  R  .  .
//b  .  .  .  s  g  .  .  I  .
//c  A  p  p  g  .  .  p  .  V
//d  .  .  .  p  p  .  .  .  .
//e  .  .  .  .  .  p  .  .  p
//f  .  .  P  .  .  .  P  .  .
//g  .  P  S  P  P  P  .  P  P
//h  .  B  G  K  G  .  .  R  .
//i  L  N  .  .  .  .  .  N  L
// test問題8
// 忘れている
// test問題9
//   9  8  7  6  5  4  3  2  1
//a  .  H  s  g  .  k  .  C  .
//b  .  .  r  .  g  s  .  .  l
//c  p  .  .  p  .  p  p  B  .
//d  .  p  .  .  p  .  .  .  p
//e  .  .  p  .  .  .  .  .  .
//f  .  .  P  .  .  .  .  .  .
//g  P  P  .  P  P  P  P  P  P
//h  .  .  G  S  G  .  S  R  .
//i  L  N  .  .  K  .  .  N  L
void test_checker(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	sfen[0] = '\0';
	// test問題1(black)
	strcpy_s(sfen, sizeof(sfen), "ln4snl/1rs1gkgb1/1pp1pp1pp/6p2/p8/4P4/PPP2PPPP/L1Sp1gKSR/1NB2+p1NL b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	bitboard bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq4h, bb));
	// test問題2(black)
	strcpy_s(sfen, sizeof(sfen), "+Ln1+S2snl/1r2gkg2/p1pppp1p+R/6p2/2NP5/2P1PBP2/+nP+b2PGP1/3K5/+l1+l1G1+s2 b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq7g, bb));
	// test問題3(black)
	strcpy_s(sfen, sizeof(sfen), "lns3snl/1r3kg2/pppppg1pp/6p2/PN3p1N1/1P1P2P1P/2P1PPbP1/7r+b/+s1SGKG3 b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	// test問題4(black)
	strcpy_s(sfen, sizeof(sfen), "lns3snl/1r3kgb1/p1pppgp1p/5p1p1/2P6/P3P2P1/3P1PP1P/L1+p1K1GR1/3s2SNL b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq6i, bb));
	// test問題5(black)
	strcpy_s(sfen, sizeof(sfen), "ln4snl/1r2gkg2/pp2pp1pp/1s4p1P/9/1P7/P1+p1+bPPPN/5KS1R/+n1P2G2L b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq5g, bb));
	// test問題6(white)
	strcpy_s(sfen, sizeof(sfen), "+Bnkg1R2l/2sSGp+l2/ppppp3p/7p1/6p2/2P6/PP1PPPSPP/2GK3Rb/LNS2G1NL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	(&hd)->bd->turn = white;	//ここで無理やりtrueをwhiteにして,white側kingのcheckersを計算させている
	set_state((&hd));	
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq6b, bb));
	// test問題7(white)
	strcpy_s(sfen, sizeof(sfen), "+Ln+S1k1R2/3sg2+S1/+Pppg2p1+N/3pp4/5p2p/2P3P2/1PSPPP1PP/1BGKG2R1/LN5NL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	(&hd)->bd->turn = white;	//ここで無理やりtrueをwhiteにして,white側kingのcheckersを計算させている
	set_state((&hd));	
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq3a, bb));
	// test問題8(white)
	strcpy_s(sfen, sizeof(sfen), "1n3k1n1/l1gsrg+P2/pppp2p2/4pp2l/P1PP4p/9/1PS1PPP1P/1BGKG1SR1/LN5NL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	(&hd)->bd->turn = white;	//ここで無理やりtrueをwhiteにして,white側kingのcheckersを計算させている
	set_state((&hd));
	bb = checkers(&hd);
	//print_board(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq3b, bb));
	// test問題9(white)
	strcpy_s(sfen, sizeof(sfen), "1+Bsg1k1+L1/2r1gs2l/p2p1ppB1/1p2p3p/2p6/2P6/PP1PPPPPP/2GSG1SR1/LN2K2NL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	(&hd)->bd->turn = white;	//ここで無理やりtrueをwhiteにして,white側kingのcheckersを計算させている
	set_state((&hd));
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	// test問題10(white)
	strcpy_s(sfen, sizeof(sfen), "lGsk1R+P2/3sg2+P+R/p1pp1p2p/1p2p4/2PP2p1P/5P3/PPS1P1P2/1KG1G1S2/LNB4NL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	(&hd)->bd->turn = white;	//ここで無理やりtrueをwhiteにして,white側kingのcheckersを計算させている
	set_state((&hd));
	bb = checkers(&hd);
	//print_board(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_biton(sq4a, bb));

}

void test_is_check(void) {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	sfen[0] = '\0';
	// test問題?(black)
	strcpy_s(sfen, sizeof(sfen), "ln4snl/1rs1gkgb1/1pp1pp1pp/6p2/p8/4P4/PPP2PPPP/L1Sp1gKSR/1NB2+p1NL b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	bitboard bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_check(&hd));
	// test問題?(black) no check
	strcpy_s(sfen, sizeof(sfen), "ln4snl/1rs1gkgb1/1pp1pp1pp/6p2/p8/4P4/PPP2PPPP/L1Spg1KSR/1NB2+p1NL b - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_FALSE(is_check(&hd));
	// test問題?(white)
	strcpy_s(sfen, sizeof(sfen), "+Bnkg1R2l/2sSGp+l2/ppppp3p/7p1/6p2/2P6/PP1PPPSPP/2GK3Rb/LNS2G1NL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	(&hd)->bd->turn = white;	//ここで無理やりtrueをwhiteにして,white側kingのcheckersを計算させている
	set_state((&hd));
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_TRUE(is_check(&hd));
	// test問題?(white) no check
	strcpy_s(sfen, sizeof(sfen), "+Bnkg1R2l/2s1Gp+l2/ppppp3p/7p1/6p2/2P6/PP1PPPSPP/2GK3Rb/LNS2G1NL w - 1");
	new_board(&hd);
	set_board(&hd, sfen);
	(&hd)->bd->turn = white;	//ここで無理やりtrueをwhiteにして,white側kingのcheckersを計算させている
	set_state((&hd));
	bb = checkers(&hd);
	//print_bitboard(bb, __func__);
	TEST_ASSERT_FALSE(is_check(&hd));
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