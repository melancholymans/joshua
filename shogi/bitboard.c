#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "bitboard.h"

__m128i mask_bb[81];
__m128i file_mask[9];

//各種テーブルの初期化
void init_tables() {
	new_mask_bb();
	new_file_mask();
}

//座標sqごとにbitが立っている配列を生成している
void new_mask_bb() {
	for (int i = 0; i < 63; i += 1) {
		int64_t tmp[2] = {(int64_t)1 << i,0x00};
		mask_bb[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
	for (int i = 63; i < 81; i += 1){
		int64_t tmp[2] = { 0x00,(int64_t)1 << (i-63)};
		mask_bb[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
}

void new_file_mask() {
	for (int i = 0; i < 7; i += 1) {
		int64_t tmp[2] = { (int64_t)0x1ff << (9*i),0x00};
		file_mask[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
	for (int i = 7; i < 9; i += 1) {
		int64_t tmp[2] = { 0x00,(int64_t)0x1ff << (9 * (i-7)) };
		file_mask[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
}

__m128i set_bb(int64_t idx0, int64_t idx1) {
	int64_t tmp[2] = { idx0, idx1 };
	return _mm_loadu_si128((const __m128i*)tmp);
}

//引数bbのsq座標にビットが立っていればtrueを返す
bool is_biton(int sq, __m128i bb) {
	return !(bool)_mm_testz_si128(mask_bb[sq],bb);
}

//debug用のbitboard表示
void print_bitboard(__m128i bb, const char* msg) {
	char rstr[] = "abcdefghi";
	printf("----------- %s ------------\n", msg);
	printf("    9  8  7  6  5  4  3  2  1\n");
	for (int r = 0; r < 9; r += 1) {
		printf("%c ", rstr[r]);
		for (int f = 8; f >= 0; f -= 1) {
			int sq = set_square(f, r);
			if (is_biton(sq, bb)==true) {
				printf("  X");
			}
			else {
				printf("  .");
			}
		}
		printf("\n");
	}
}
