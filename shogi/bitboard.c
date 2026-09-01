#include <stdint.h>
#include <stdbool.h>

#include "bitboard.h"

__m128i mask_bb[81];
__m128i file_mask[9];

//座標sqごとにbitが立っている配列を生成している
void new_mask_bb() {
	for (int i = 0; i < 63; i += 1) {
		int64_t tmp[2] = {1 << i,0x00};
		mask_bb[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
	for (int i = 63; i < 81; i += 1){
		int64_t tmp[2] = { 0x00,1 << (i-63)};
		mask_bb[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
}

void new_file_mask() {
	for (int i = 0; i < 7; i += 1) {
		int64_t tmp[2] = { 0x1ff << (9*i),0x00};
		file_mask[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
	for (int i = 7; i < 9; i += 1) {
		int64_t tmp[2] = { 0x00,0x1ff << (9 * (i-7)) };
		file_mask[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
}

//fileとrankを指定して座標値を返す
int set_square(int f, int r) {
	return f * 9 + r;
}

//引数bbのsq座標にビットが立っていればtrueを返す
bool is_biton(int sq, __m128i bb) {
	return !(bool)_mm_testz_si128(mask_bb[sq],bb);
}

//debug用のbitboard表示
void print_bitboard(__m128i bb, const char* msg) {
	char rstr[] = "abcdefghi";
	printf("----------- ", msg, " -----------\n");
	printf("    9  8  7  6  5  4  3  2  1");
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
