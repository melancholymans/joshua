#pragma once
#include <immintrin.h>
#include <intrin.h>
#include <stdbool.h>
#include <stdint.h>

typedef union {
	__m128i m;
	int64_t p[2];
}bitboard;

extern bitboard file_mask[9];
extern bitboard rank_mask[9];
extern bitboard all_one_bb;
extern bitboard in_front_mask[2][9];
extern bitboard enemy_field[2];
extern bitboard lance_attack_mask[2][81][128];
extern bitboard rook_attack_rank_to_mask[81][2];
extern __m256i bishop_attack_to_mask[81][2];
extern bitboard king_attack[81];
extern bitboard gold_attack[2][81];
extern bitboard silver_attack[2][81];
extern bitboard knight_attack[2][81];
extern bitboard pawn_attack[2][81];
extern bitboard between_bb[81][81];
extern bitboard star_bb[81];
extern bitboard lance_attack_to_edge[2][81];
extern bitboard bishop_attack_to_edge[81];
extern bitboard rook_attack_to_edge[81];
extern bitboard horse_attack_to_edge[81];
extern bitboard dragon_attack_to_edge[81];
extern bitboard gold_check_table[2][81];
extern bitboard silver_check_table[2][81];
extern bitboard knight_check_table[2][81];
extern bitboard lance_check_table[2][81];
extern bitboard pawn_check_table[2][81];
extern bitboard bishop_check_table[2][81];
extern bitboard horse_check_table[2][81];
extern bitboard neighbor5x5[81];
extern const int slide[81];

void new_mask_bb();
void new_file_mask();
void new_rank_mask();
void new_all_one_bb();
void new_in_front_mask();
void new_enemy_field();
bitboard lance_block_mask(const int sq);
int first_one_from(bitboard* bb);
bitboard lance_attack_calc(const int color, const int square,const bitboard occ);
bitboard index_to_occupied(const int idx,const bitboard block_mask);
void new_lance_attack();
bitboard byte_reverse(bitboard bb);
void unpack(const bitboard hi_in, const bitboard lo_in, bitboard* hi_out, bitboard* lo_out);
void decrement(const bitboard hi_in, const bitboard lo_in, bitboard* hi_out, bitboard* lo_out);
void new_rook_attacks();
bitboard rook_attack_rank(const int sq,const bitboard occ);
bitboard rook_attack_file(const int sq,const bitboard occ);
void new_bishop_attacks();
void unpack256(const __m256i hi_in, const __m256i lo_in, __m256i* hi_out, __m256i* lo_out);
void decrement256(const __m256i hi_in, const __m256i lo_in, __m256i* hi_out, __m256i* lo_out);
__m256i byte_reverse256(__m256i bb);
__m128i merge256(__m256i bb);
bitboard lance_attack(const int c, const int sq, const bitboard occ);
bitboard bishop_attack(const int sq, const bitboard occ);
bitboard horse_attack(const int sq, const bitboard occ);
bitboard rook_attack(const int sq, const bitboard occ);
bitboard dragon_attack(const int sq, const bitboard occ);
void new_king_attacks();
void new_gold_attacks();
void new_silver_attacks();
int first_one_from_nodelete(bitboard bb);
void new_knight_attacks();
void new_pawn_attacks();
void new_star_bb();
void new_between_bb();
void new_attack_to_edge();
void new_gold_check_table();
void new_silver_check_table();
void new_knight_check_table();
void new_lance_check_table();
void new_pawn_check_table();
void new_bishop_check_table();
void new_horse_check_table();
void new_neighbor5x5();
bitboard set_board(const int64_t idx0, const int64_t idx1);
bool is_biton(const int sq,const bitboard bb);
void set_biton(const int sq, bitboard* bb);
void print_bitboard(bitboard bb,const char* msg);