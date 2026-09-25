#include <stdio.h>
#include <windows.h>

#include "unity.h"
#include "../shogi/init.h"
#include "../shogi/bitboard.h"
#include "../shogi/position.h"
#include "test_bitboard.h"
#include "test_position.h"

void setUp(void) {}
void tearDown(void) {}

void test_position() {
	printf("-----------test_position----------------------------------\n");
	init_tables();
	test_set_file();
	test_set_rank();
	TEST_ASSERT_TRUE(test_set_square());
	test_new_square_relation_direct();
	test_square_relation();
	test_opposite_color();
	test_inverse();
	test_piece_to_piecetype();
	test_piece_to_color();
	test_piecetype_to_piece();
	test_is_jump();
	test_rank_usi_string();
	test_file_usi_string();
	test_square_usi_string();
	test_square_inverse();
	test_file_inverse();
	test_rank_inverse();
	test_can_promote();
}

void test_bitboard() {
	printf("-----------test_bitboard----------------------------------\n");
	init_tables();
	test_new_sq_mask();
	test_new_file_mask();
	test_new_rank_mask();
	test_new_all_one_mask();
	test_new_all_zero_mask();
	test_new_in_front_mask();
	test_new_enemy_mask();
	test_set_biton();
	test_set_board();
	test_first_one_from();
	test_lance_block_mask();
	test_index_to_occupied();
	test_lance_attack_calc();
	test_new_lance_attack_mask();
	test_byte_reverse();
	test_unpack();
	test_decrement();
	test_new_rook_attack_rank_to_mask();
	test_rook_attack_rank();
	test_rook_attack_file();
	test_new_bishop_attack_to_mask();
	test_unpack256();
	test_decrement256();
	test_byte_reverse256();
	test_merge256();
	test_new_king_attack_mask();
	test_new_gold_attack_mask();
	test_new_silver_attack_mask();
	test_first_one_from_nodelete();
	test_star_mask();
	test_new_knight_attacks();
	test_new_pawn_attack_mask();
	test_new_between_mask();
	test_new_attack_to_edge();
	test_new_gold_check_table();
	test_new_silver_check_table();
	test_new_knight_check_table();
	test_new_lance_check_table();
	test_new_pawn_check_table();
	test_new_bishop_check_table();
	test_new_horse_check_table();
	test_new_neighbor_5x5_mask();
	test_lance_attack();
	test_bishop_attack();
	test_rook_attack();
	test_horse_attack();
	test_dragon_attack();
}

int main() {
	UNITY_BEGIN();
	//テスト用関数にtest_とつけるのは慣用でありルールではない。テスト関数には引数なし、返り値なしで記述する
	RUN_TEST(test_bitboard);
	RUN_TEST(test_position);
	return UNITY_END();;
}

