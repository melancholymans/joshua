#include "unity.h"
#include "test_bitboard.h"

void test_set_mask_bb(__m128i bb) {
	//print_bitboard(bb, "test_set_mask_bb");
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
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file2;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file3;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));	
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file4;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file5;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file6;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file7;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file8;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file9;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	_mm_storeu_si128((const __m128i*)tmp, file_mask[f]);
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
}

void test_rank_mask() {
	int r = rank1;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank2;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank3;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank4;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank5;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank6;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank7;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank8;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank9;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	_mm_storeu_si128((const __m128i*)tmp, rank_mask[r]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
}

void test_all_one_bb() {
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, all_one_bb);
	TEST_ASSERT_EQUAL_INT(7*9, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2*9, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(all_one_bb, "test_all_one_bb");
}

void test_all_zero_bb() {
	__m128i bb = all_zero_bb();
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(bb, "test_all_zero_bb");
}

void test_in_front_mask() {
	int color = black;
	int rank = rank1;
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank2;
	for (int r=rank1,f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank3;
	for(int r=rank1;r<rank3;r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7*2, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2*2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank4;
	for (int r = rank1; r < rank4; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank5;
	for (int r = rank1; r < rank5; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 4, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 4, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank6;
	for (int r = rank1; r < rank6; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 5, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 5, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank7;
	for (int r = rank1; r < rank7; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 6, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 6, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank8;
	for (int r = rank1; r < rank8; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 7, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank9;
	for (int r = rank1; r < rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 8, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 8, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	color = white;
	rank = rank9;
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank8;
	for (int r = rank9, f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank7;
	for (int r = rank8; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 2, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 2, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank6;
	for (int r = rank7; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank5;
	for (int r = rank6; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 4, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 4, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank4;
	for (int r = rank5; r < rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 5, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 5, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank3;
	for (int r = rank4; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 6, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 6, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank2;
	for (int r = rank3; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 7, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 7, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank1;
	for (int r = rank2; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, in_front_mask[color][rank]);
	TEST_ASSERT_EQUAL_INT(7 * 8, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 8, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");
}

void test_enemy_field(){
	int color = black;
	int64_t tmp[2];
	for (int r = rank1; r <= rank3; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, enemy_field[color]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, enemy_field[color]);
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(enemy_field[color], "test_enemy_field");

	color = white;
	for (int r = rank7; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, enemy_field[color]));
		}
	}
	_mm_storeu_si128((const __m128i*)tmp, enemy_field[color]);
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(tmp[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(tmp[1]));
	//print_bitboard(enemy_field[color], "test_enemy_field");
}

void test_set_biton() {
	__m128i bb = all_zero_bb();
	set_biton(sq9a, &bb);
	set_biton(sq9i, &bb);
	set_biton(sq1a, &bb);
	set_biton(sq1i, &bb);
	set_biton(sq5e, &bb);
	TEST_ASSERT_TRUE(is_biton(sq9a, bb));
	TEST_ASSERT_TRUE(is_biton(sq9i, bb));
	TEST_ASSERT_TRUE(is_biton(sq1a, bb));
	TEST_ASSERT_TRUE(is_biton(sq1i, bb));
	TEST_ASSERT_TRUE(is_biton(sq5e, bb));
}

void test_first_one_from() {
	__m128i bb = set_board(0x298060C0A1121B45, 0x3abad);
	TEST_ASSERT_EQUAL_INT(0,first_one_from(&bb));
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x298060C0A1121B44, tmp[0]);	//bbから最初の1bitを0にした値が返ることを確認
	TEST_ASSERT_EQUAL_HEX64(0x3abad, tmp[1]);
	bb = set_board(0x282d026660282000, 0x28b5b);
	TEST_ASSERT_EQUAL_INT(13, first_one_from(&bb));
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x282D026660280000, tmp[0]);	//bbから最初の1bitを0にした値が返ることを確認
	TEST_ASSERT_EQUAL_HEX64(0x28b5b, tmp[1]);
}

void test_lance_block_mask() {
	__m128i bb = lance_block_mask(sq9a);
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq8b);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0xfe, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq7c);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x3f80000000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq6d);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq5e);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0xFE000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq4f);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x7F0000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq3g);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x3F80000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq2h);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq1i);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0xfe, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	print_bitboard(bb, "test_lance_block_mask");
}
