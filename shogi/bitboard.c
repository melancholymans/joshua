#include <stdint.h>
#include <immintrin.h>

#include "bitboard.h"

__m128i mask_bb[81];

//À•Wsq‚²‚Æ‚Ébit‚ª—§‚Á‚Ä‚¢‚é”z—ñ‚ğ¶¬‚µ‚Ä‚¢‚é
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
