#pragma once
#include <immintrin.h>
#include <intrin.h>
#include <stdbool.h>

extern __m128i file_mask[9];
extern __m128i rank_mask[9];
extern __m128i all_one_bb;
extern __m128i in_front_mask[2][9];
extern __m128i enemy_field[2];

void init_tables();
void new_mask_bb();
void new_file_mask();
void new_rank_mask();
void new_all_one_bb();
__m128i all_zero_bb();
void new_in_front_mask();
void new_enemy_field();
__m128i set_board(int64_t idx0, int64_t idx1);
bool is_biton(int sq, __m128i bb);
void set_biton(int sq, __m128i* bb);

void print_bitboard(__m128i bb,const char* msg);