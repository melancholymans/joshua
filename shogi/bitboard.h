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
extern __m128i rook_attack_rank_to_mask[81][2];
extern __m256i bishop_attack_to_mask[81][2];
extern const int slide[81];

void init_tables();
void new_mask_bb();
void new_file_mask();
void new_rank_mask();
void new_all_one_bb();
void new_in_front_mask();
void new_enemy_field();
__m128i lance_block_mask(const int sq);
int first_one_from(__m128i* bb);
__m128i lance_attack_calc(const int color, const int square,const __m128i occ);
__m128i index_to_occupied(const int idx,const __m128i block_mask);
void new_lance_attack();
__m128i byte_reverse(__m128i bb);
void unpack(const __m128i hi_in, const __m128i lo_in, __m128i* hi_out, __m128i* lo_out);
void decrement(const __m128i hi_in, const __m128i lo_in, __m128i* hi_out, __m128i* lo_out);
void new_rook_attacks();
__m128i rook_attack_rank(const int sq,const __m128i occ);
__m128i rook_attack_file(const int sq,const __m128i occ);
__m128i rook_attack(const int sq, const __m128i occ);
void new_bishop_attacks();
void unpack256(const __m256i hi_in, const __m256i lo_in, __m256i* hi_out, __m256i* lo_out);
void decrement256(const __m256i hi_in, const __m256i lo_in, __m256i* hi_out, __m256i* lo_out);
__m128i all_zero_bb();
__m128i set_board(const int64_t idx0, const int64_t idx1);
bool is_biton(const int sq,const __m128i bb);
void set_biton(const int sq, __m128i* bb);

void print_bitboard(__m128i bb,const char* msg);