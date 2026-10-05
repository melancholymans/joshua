#include "unity.h"

#include "../shogi/position.h"

void test_get_move() {
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

void test_strip_pmoto() {
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