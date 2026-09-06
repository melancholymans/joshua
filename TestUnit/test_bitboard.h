#pragma once
#include "../shogi/bitboard.h"
#include "../shogi/position.h"

void test_set_mask_bb(__m128i bb);
void test_file_mask();
void test_rank_mask();
void test_all_one_bb();
void test_all_zero_bb();
void test_in_front_mask();
void test_enemy_field();
void test_set_biton();
void test_first_one_from();
void test_lance_block_mask();
void test_index_to_occupied();
void test_lance_attack_calc();
void test_lance_attack();
