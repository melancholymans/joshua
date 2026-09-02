#include "unity.h"
#include "test_bitboard.h"

void test_set_mask_bb(__m128i bb) {
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq8a, bb));
	TEST_ASSERT_TRUE(is_biton(sq6a, bb));
	TEST_ASSERT_TRUE(is_biton(sq2a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq7b, bb));
	TEST_ASSERT_TRUE(is_biton(sq6b, bb));
	TEST_ASSERT_TRUE(is_biton(sq9c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8c, bb));
	TEST_ASSERT_TRUE(is_biton(sq7c, bb));
	TEST_ASSERT_TRUE(is_biton(sq5c, bb));
	TEST_ASSERT_TRUE(is_biton(sq4c, bb));
	TEST_ASSERT_TRUE(is_biton(sq3c, bb));
	TEST_ASSERT_TRUE(is_biton(sq2c, bb));
	TEST_ASSERT_TRUE(is_biton(sq1c, bb));
	TEST_ASSERT_TRUE(is_biton(sq8d, bb));
	TEST_ASSERT_TRUE(is_biton(sq5d, bb));
	TEST_ASSERT_TRUE(is_biton(sq2d, bb));
	TEST_ASSERT_TRUE(is_biton(sq9e, bb));
	TEST_ASSERT_TRUE(is_biton(sq4e, bb));
	TEST_ASSERT_TRUE(is_biton(sq8f, bb));
	TEST_ASSERT_TRUE(is_biton(sq7f, bb));
	TEST_ASSERT_TRUE(is_biton(sq9g, bb));
	TEST_ASSERT_TRUE(is_biton(sq3g, bb));
	TEST_ASSERT_TRUE(is_biton(sq1g, bb));
	TEST_ASSERT_TRUE(is_biton(sq9h, bb));
	TEST_ASSERT_TRUE(is_biton(sq8h, bb));
	TEST_ASSERT_TRUE(is_biton(sq7h, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq8i, bb));
	TEST_ASSERT_TRUE(is_biton(sq2i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_INT(20, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(12, _mm_popcnt_u64(tmp[1]));
}

void test_file_mask() {
	int f = file1;
	for(int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	f = file2;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	f = file3;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	f = file4;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	f = file5;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	f = file6;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	f = file7;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	f = file8;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[1]));
	f = file9;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[1]));
}
