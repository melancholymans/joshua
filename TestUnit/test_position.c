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