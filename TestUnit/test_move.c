#include <string.h>
#include <stdlib.h>

#include "unity.h"

#include "../shogi/position.h"
#include "../shogi/usi.h"
#include "test_move.h"

void test_get_move(void) {
	int move;
	move = sq7b | sq6a << 7 | 1 << 14 | 0 << 15 | bbishop << 16 | wgold<< 24;	//6a7b bbishop pmoto cap wgold
	int to, from, ispmoto, morh, piece, cap;
	get_move(move, &to, &from, &ispmoto, &morh, &piece, &cap);
	TEST_ASSERT_EQUAL_INT(sq7b, to);
	TEST_ASSERT_EQUAL_INT(sq6a, from);
	TEST_ASSERT_EQUAL_INT(1, ispmoto);
	TEST_ASSERT_EQUAL_INT(0, morh);
	TEST_ASSERT_EQUAL_INT(bbishop, piece);
	TEST_ASSERT_EQUAL_INT(wgold, cap);
}

void test_strip_pmoto(void) {
	TEST_ASSERT_EQUAL_INT(bpawn, strip_pmoto(bpawn));
	TEST_ASSERT_EQUAL_INT(blance, strip_pmoto(blance));
	TEST_ASSERT_EQUAL_INT(bknight, strip_pmoto(bknight));
	TEST_ASSERT_EQUAL_INT(bsilver, strip_pmoto(bsilver));
	TEST_ASSERT_EQUAL_INT(bbishop, strip_pmoto(bbishop));
	TEST_ASSERT_EQUAL_INT(brook, strip_pmoto(brook));
	TEST_ASSERT_EQUAL_INT(bgold, strip_pmoto(bgold));
	TEST_ASSERT_EQUAL_INT(bking, strip_pmoto(bking));
	TEST_ASSERT_EQUAL_INT(bpawn, strip_pmoto(bpropawn));
	TEST_ASSERT_EQUAL_INT(blance, strip_pmoto(bprolance));
	TEST_ASSERT_EQUAL_INT(bknight, strip_pmoto(bproknight));
	TEST_ASSERT_EQUAL_INT(bsilver, strip_pmoto(bprosilver));
	TEST_ASSERT_EQUAL_INT(bbishop, strip_pmoto(bhorse));
	TEST_ASSERT_EQUAL_INT(brook, strip_pmoto(bdragon));
	TEST_ASSERT_EQUAL_INT(wpawn, strip_pmoto(wpawn));
	TEST_ASSERT_EQUAL_INT(wlance, strip_pmoto(wlance));
	TEST_ASSERT_EQUAL_INT(wknight, strip_pmoto(wknight));
	TEST_ASSERT_EQUAL_INT(wsilver, strip_pmoto(wsilver));
	TEST_ASSERT_EQUAL_INT(wbishop, strip_pmoto(wbishop));
	TEST_ASSERT_EQUAL_INT(wrook, strip_pmoto(wrook));
	TEST_ASSERT_EQUAL_INT(wgold, strip_pmoto(wgold));
	TEST_ASSERT_EQUAL_INT(wking, strip_pmoto(wking));
	TEST_ASSERT_EQUAL_INT(wpawn, strip_pmoto(wpropawn));
	TEST_ASSERT_EQUAL_INT(wlance, strip_pmoto(wprolance));
	TEST_ASSERT_EQUAL_INT(wknight, strip_pmoto(wproknight));
	TEST_ASSERT_EQUAL_INT(wsilver, strip_pmoto(wprosilver));
	TEST_ASSERT_EQUAL_INT(wbishop, strip_pmoto(whorse));
	TEST_ASSERT_EQUAL_INT(wrook, strip_pmoto(wdragon));
}

void test_usisq_to_movesq(void) {
	char usi_sq[8] = "2g2f";
	TEST_ASSERT_EQUAL_INT(sq2g, usisq_to_movesq(usi_sq));	//"2g"
	TEST_ASSERT_EQUAL_INT(sq2f, usisq_to_movesq(usi_sq+2));	//"2f"
	strcpy_s(usi_sq, sizeof(usi_sq), "4a3b");
	TEST_ASSERT_EQUAL_INT(sq4a, usisq_to_movesq(usi_sq));	//"4a"
	TEST_ASSERT_EQUAL_INT(sq3b, usisq_to_movesq(usi_sq+2));	//"3b"
	strcpy_s(usi_sq, sizeof(usi_sq), "2c2d");
	TEST_ASSERT_EQUAL_INT(sq2c, usisq_to_movesq(usi_sq));
	TEST_ASSERT_EQUAL_INT(sq2d, usisq_to_movesq(usi_sq+2));
	strcpy_s(usi_sq, sizeof(usi_sq), "4b5a");
	TEST_ASSERT_EQUAL_INT(sq4b, usisq_to_movesq(usi_sq));
	TEST_ASSERT_EQUAL_INT(sq5a, usisq_to_movesq(usi_sq+2));
}

void test_initial_to_piecetype(void) {
	char p = 'P';
	TEST_ASSERT_EQUAL_INT(pawn, initial_to_piecetype(p));
	p = 'L';
	TEST_ASSERT_EQUAL_INT(lance, initial_to_piecetype(p));
	p = 'N';
	TEST_ASSERT_EQUAL_INT(knight, initial_to_piecetype(p));
	p = 'S';
	TEST_ASSERT_EQUAL_INT(silver, initial_to_piecetype(p));
	p = 'B';
	TEST_ASSERT_EQUAL_INT(bishop, initial_to_piecetype(p));
	p = 'R';
	TEST_ASSERT_EQUAL_INT(rook, initial_to_piecetype(p));
	p = 'G';
	TEST_ASSERT_EQUAL_INT(gold, initial_to_piecetype(p));
	//p = 'K';
	//TEST_ASSERT_EQUAL_INT(gold, initial_to_piecetype(p));
	//[ERROR] F:\joshua\shogi\move.c:122: Array out of bounds
}

void test_do_usi_move() {
	usi_handler_t hd = new_usi_handler(stdout);
	char sfen[128];
	char* moves[8];
	for (int i = 0; i < 3; i++) {
		moves[i] = (char*)malloc(8);
	}
	new_board(&hd);
	strcpy_s(sfen, 63 + 1, "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1");
	set_board(&hd, sfen);
	strcpy_s(moves[0],8, "2g2f");
	strcpy_s(moves[1],8, "4a3b");
	strcpy_s(moves[2],8, "2f2e");
	for (int i = 0; i < 3; i++) {
		do_usi_move(hd, moves[i]);
	}
	for (int i = 0; i < 3; i++) {
		free(moves[i]);
	}
}

/*
 ƒeƒXƒgŠû•ˆ
 26•à 2g2f 
 32‹à 4a3b
 25•à 2f2e
 52‹à 6a5b
 76•à 7g7f
 42‰¤ 5a4b
 78‹à 6i7h
 62‹â 7a6b
 68‹â 7i6h
 94•à 9c9d
 77Šp 8h7g
 93Œj 8a9c
 24•à 2e2d
 “¯•à 2c2d
 “¯”ò 2h2d
 85Œj 9c8e
 23•à‘Å P*2c
 77Œj¬ 8e7g+
 “¯Œj 8i7g
 95•à 9d9e
 38‹â 3i3h
 92”ò 8b9b
 22•à¬ 2c2b+
 “¯‹â 3a2b
 26”ò 2d2f
 23‹â 2b2c
 56Šp‘Å B*5f
 25•à‘Å P*2e
 “¯”ò 2f2e
 24•à‘Å P*2d
 85”ò 2e8e
 28Šp‘Å B*2h
 83”ò¬ 8e8c+
 19Šp¬ 2h1i+
 81—³ 8c8a
 25‘Å L*2e
 21—³ 8a2a
 29¬ 2e2i+
 23Šp¬ 5f2c+
 “¯‹à 3b2c
 41‹â‘Å S*4a
 31Œj‘Å N*3a
 32—³ 2a3b
 51‰¤ 4b5a
 52—³ 3b5b
 “Š—¹

 position startpos moves 2g2f 4a3b 2f2e 6a5b 7g7f 5a4b 6i7h 7a6b 7i6h 9c9d 8h7g 8a9c 2e2d 2c2d 2h2d 9c8e P*2c 8e7g+ 8i7g 9d9e 3i3h 8b9b 2c2b+ 3a2b 2d2f 2b2c B*5f P*2e 2f2e P*2d 2e8e B*2h 8e8c+ 2h1i+ 8c8a L*2e 8a2a 2e2i+ 5f2c+ 3b2c S*4a N*3a 2a3b 4b5a 3b5b
*/