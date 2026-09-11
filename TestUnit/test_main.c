#include <stdio.h>
#include <windows.h>

#include "unity.h"
#include "simd_learn.h"
#include "../shogi/bitboard.h"
#include "../shogi/position.h"
#include "test_bitboard.h"
#include "test_position.h"

void setUp(void) {}
void tearDown(void) {}


void test_position() {
	printf("-----------test_position----------------------------------\n");
	test_set_file();
	test_set_rank();
	TEST_ASSERT_TRUE(test_set_square());
}

void test_bitboard() {
	printf("-----------test_bitboard----------------------------------\n");
	init_tables();
	//__m128i bb = set_board(0x282d026660282000, 0x28b5b);
	//__m128i bb = set_board(0x298060C0A1121B45, 0x3abad);
	bitboard bb;
	bb.p[0] = 0x298060C0A1121B45;
	bb.p[1] = 0x3abad;
	test_new_set_mask_bb(bb);
	test_new_file_mask();
	test_new_rank_mask();
	test_new_all_one_bb();
	//test_all_zero_bb(); 不要なので削除する
	test_new_in_front_mask();
	test_new_enemy_field();
	test_set_biton();
	test_set_board();
	test_first_one_from();
	test_lance_block_mask();
	//test_index_to_occupied();
	//test_lance_attack_calc();
	//test_new_lance_attack();
	//test_byte_reverse();
	//test_unpack();
	//test_decrement();
	//test_new_rook_attacks();
	//test_rook_attack_rank();
	//test_rook_attack_file();
	//test_rook_attack();
	//test_new_bishop_attacks();
	//test_unpack256();
	//test_decrement256();
	//test_byte_reverse256();
	//test_merge256();
	//test_bishop_attack();
}

void test_simd() {
	printf("-----------test_simd----------------------------------\n");
	TEST_ASSERT_EQUAL_INT(1, simd_128());
	TEST_ASSERT_EQUAL_INT(1, simd_256());
	int a[8] = { 1,2,3,4,5,6,7,8 };
	TEST_ASSERT_EQUAL_INT(72, scalar_multiplication(a, 1, 8, 2));
	TEST_ASSERT_EQUAL_INT(1, simd_info());
}

int main() {
	UNITY_BEGIN();
	//テスト用関数にtest_とつけるのは慣用でありルールではない。テスト関数には引数なし、返り値なしで記述する
	RUN_TEST(test_simd);
	RUN_TEST(test_position);
	RUN_TEST(test_bitboard);
	return UNITY_END();;
}

