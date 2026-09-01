#pragma once
#include <immintrin.h>

void new_mask_bb();
void new_file_mask();
__m128i set_bb(int64_t idx0, int64_t idx1);
void print_bitboard(__m128i bb,const char* msg);