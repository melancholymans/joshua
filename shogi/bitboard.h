#pragma once
#include <immintrin.h>
#include <intrin.h>
#include <stdbool.h>

extern __m128i file_mask[9];
extern __m128i rank_mask[9];
extern __m128i all_one_bb;
extern __m128i in_front_mask[2][9];
extern __m128i enemy_field[2];
extern __m128i lance_attack[2][81][128];

void init_tables();
void new_mask_bb();
void new_file_mask();
void new_rank_mask();
void new_all_one_bb();
void new_in_front_mask();
void new_enemy_field();
__m128i lance_block_mask(int sq);
int first_one_from(__m128i* bb);
__m128i lance_attack_calc(int color, int square, __m128i occ);
__m128i index_to_occupied(int idx, __m128i block_mask);
void new_lance_attack();
__m128i all_zero_bb();
__m128i set_board(int64_t idx0, int64_t idx1);
bool is_biton(int sq, __m128i bb);
void set_biton(int sq, __m128i* bb);

void print_bitboard(__m128i bb,const char* msg);