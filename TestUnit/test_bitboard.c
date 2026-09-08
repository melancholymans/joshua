#include "unity.h"
#include "test_bitboard.h"

void test_new_set_mask_bb(__m128i bb) {
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

void test_new_file_mask() {
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

void test_new_rank_mask() {
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

void test_new_all_one_bb() {
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

void test_new_in_front_mask() {
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

void test_new_enemy_field(){
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
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq8b);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0xfe, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq7c);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x3f80000000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq6d);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq5e);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0xFE000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq4f);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x7F0000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq3g);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x3F80000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq2h);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq1i);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0xfe, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(bb, "test_lance_block_mask");
}

void test_index_to_occupied() {
	int sq = sq5e;
	__m128i lbm = lance_block_mask(sq);
	int64_t tmp[2];

	__m128i occ = index_to_occupied(0, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	occ = index_to_occupied(1, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0x2000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	occ = index_to_occupied(2, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0x4000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	occ = index_to_occupied(3, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0x6000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	occ = index_to_occupied(4, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0x8000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	occ = index_to_occupied(5, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0xA000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	occ = index_to_occupied(125, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0xFA000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(occ, "test_index_to_occupied");

	occ = index_to_occupied(127, lbm);
	_mm_storeu_si128((const __m128i*)tmp, occ);
	TEST_ASSERT_EQUAL_HEX64(0xFE000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(occ, "test_index_to_occupied");
}

void test_lance_attack_calc(){
	//black
	int64_t tmp[2];
	__m128i bm = lance_block_mask(sq5a);
	__m128i occ = index_to_occupied(97,bm);
	__m128i bb = lance_attack_calc(black, sq5a, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(occ, "test_lance_attack_calc");

	bm = lance_block_mask(sq5d);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(black, sq5d, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x6000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bm = lance_block_mask(sq5f);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(black, sq5f, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x1E000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bm = lance_block_mask(sq5i);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(black, sq5i, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x80000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//white
	bm = lance_block_mask(sq5a);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5a, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x2000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bm = lance_block_mask(sq5c);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5c, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x78000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bm = lance_block_mask(sq5f);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5f, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x40000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bm = lance_block_mask(sq5i);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5i, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
}

void test_new_lance_attack() {
	int64_t tmp[2];

	__m128i bb = lance_attack[black][sq5a][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[black][sq5d][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x6000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[black][sq5f][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x1E000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[black][sq5i][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x80000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[white][sq5a][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x2000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[white][sq5c][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x78000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[white][sq5f][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x40000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[white][sq5i][97];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[black][sq9e][35];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1800, tmp[1]);

	bb = lance_attack[black][sq9a][35];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = lance_attack[white][sq9e][35];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0xC000, tmp[1]);

	bb = lance_attack[white][sq9a][35];
	_mm_storeu_si128((const __m128i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x400, tmp[1]);
}

// tmp        
// [0]0x01 [1]0x02 [2]0x03 [3]0x04 [4]0x05 [5]0x06 [6]0x07 [7]0x08 [8]0x09 [9]0x0a [10]0x0b [11]0x0c [12]0x0d [13]0x0e [14]0xf [15]0x10
// __m128i bb
// | ----[1]---------------|--------[0]------------] 
// 0x10 0f 0e 0d 0c 0b 0a 09 08 07 06 05 04 03 02 01   
// これをリバースすると
// __m128i result
// | ----[1]---------------|--------[0]------------] 
// 0x01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10
void test_byte_reverse() {
	int8_t tmp[16] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10 };	//見やすいので16進数にしている
	__m128i bb = _mm_loadu_si128(tmp);
	__m128i result = byte_reverse(bb);
	int64_t tmp2[2];
	_mm_storeu_si128((const __m128i*)tmp2, result);
	TEST_ASSERT_EQUAL_HEX64(0x090a0b0c0d0e0f10, tmp2[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0102030405060708, tmp2[1]);
}
//                                     |---- [1]---------|---- [0]---------|      
// hight                               0x090a0b0c0d0e0f10 0x0102030405060708
// low                                 0x191a1b1c1d1e1f20 0x1112131415161718
// hight_result  = hight[1] | low[1] = 0x090a0b0c0d0e0f10 0x191a1b1c1d1e1f20
// low_resuto    = hight[0] | low[0] = 0x0102030405060708 0x1112131415161718
void test_unpack() {
	int64_t hight_tmp[2] = { 0x0102030405060708,0x090a0b0c0d0e0f10 };	//見やすいので16進数にしている
	int64_t low_tmp[2] = { 0x1112131415161718,0x191a1b1c1d1e1f20 };	//見やすいので16進数にしている
	__m128i hight = _mm_loadu_si128(hight_tmp);
	__m128i low = _mm_loadu_si128(low_tmp);
	__m128i hight_result;
	__m128i low_result;
	unpack(hight, low, &hight_result, &low_result);
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, hight_result);
	TEST_ASSERT_EQUAL_HEX64(0x191a1b1c1d1e1f20, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x090a0b0c0d0e0f10, tmp[1]);
	_mm_storeu_si128((const __m128i*)tmp, low_result);
	TEST_ASSERT_EQUAL_HEX64(0x1112131415161718, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0102030405060708, tmp[1]);
}

// 最初にlo_inがオールゼロであるかどうかを確認している
// hiにはsq座標より右側の駒の状態が(occ)が入っている
// loにはsq座標より左側の駒の状態が(occ)が入っている
// 101100を右側のoccとするとここから-1を減算（デクリメント）すると
// 101011となり下位から2bitがsqからの利きbitを表現している
// 10 0000000がそれぞれ9筋8筋 7筋～1筋のoccを表現しているとして
// 128bitの上位64bitが9,8筋、下位64bitが7筋～1筋を担当している
// この説明間違っているので理解できたところで書き直す
void test_decrement() {
	int64_t hight_tmp[2] = { 0x0000000000100000,0x0000000000001000 };	//見やすいので16進数にしている
	int64_t low_tmp[2] = { 0x0000000000000000,0x0000000000000000 };	//見やすいので16進数にしている
	__m128i hight = _mm_loadu_si128(hight_tmp);
	__m128i low = _mm_loadu_si128(low_tmp);
	__m128i hight_result;
	__m128i low_result;
	decrement(hight, low, &hight_result, &low_result);
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, hight_result);
	TEST_ASSERT_EQUAL_HEX64(0x00000000000FFFFF, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0000000000000FFF, tmp[1]);
	_mm_storeu_si128((const __m128i*)tmp, low_result);
	TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFFF, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFFF, tmp[1]);
}

// rook_attack_rank_to_mask配列に座標を入力してテストしている
void test_new_rook_attacks() {
	int64_t tmp[2];
	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank_to_mask[sq6e][0]);
	TEST_ASSERT_EQUAL_HEX64(0x0400000000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank_to_mask[sq6e][1]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1020408000010000, tmp[1]);

	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank_to_mask[sq7e][0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank_to_mask[sq7e][1]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1020408000010200, tmp[1]);

	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank_to_mask[sq2e][0]);
	TEST_ASSERT_EQUAL_HEX64(0x402010080400000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank_to_mask[sq2e][1]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1000000000000000, tmp[1]);
	//print_bitboard(rook_attack_rank_to_mask[sq2e][0], "test_new_rook_attacks=2e");
	//print_bitboard(rook_attack_rank_to_mask[sq2e][1], "test_new_rook_attacks=2e");
}

void test_rook_attack_rank() {
	int64_t tmp[2];
	__m128i bb = set_board(0x298060C0A1121B45, 0x3abad);
	//print_bitboard(bb, "test_rook_attack_rank bb");

	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank(sq5e, bb));
	TEST_ASSERT_EQUAL_HEX64(0x402000080000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, tmp[1]);
	//print_bitboard(rook_attack_rank(sq5e, bb), "test_rook_attack_rank 5e");

	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank(sq5f, bb));
	TEST_ASSERT_EQUAL_HEX64(0x804000100804020, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	//print_bitboard(rook_attack_rank(sq5f, bb), "test_rook_attack_rank 5f");

	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank(sq5g, bb));
	TEST_ASSERT_EQUAL_HEX64(0x1008000201000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x8040, tmp[1]);
	//print_bitboard(rook_attack_rank(sq5g, bb), "test_rook_attack_rank 5g");

	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank(sq5h, bb));
	TEST_ASSERT_EQUAL_HEX64(0x2010000402010080, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);	
	//print_bitboard(rook_attack_rank(sq5h, bb), "test_rook_attack_rank 5h");

	_mm_storeu_si128((const __m128i*)tmp, rook_attack_rank(sq5i, bb));
	TEST_ASSERT_EQUAL_HEX64(0x4020000804020000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x100, tmp[1]);
	//print_bitboard(rook_attack_rank(sq5i, bb), "test_rook_attack_rank 5i");
}

// occを1筋～7筋と8,9筋に分ける(partで計算している)
// occに例として問題１の局面を与える。sqはsq5eにする（図中でアスタリスクになっている）
// 5筋なのでpartは0になる。
//   987654321
// a XX.X...XX
// b ..XX.....
// c XXX.XXXXX
// d .X..X..X.
// e X...*X...
// f .XX......
// g X.....X.X
// h XXX......
// i XX.....XX
// (tmp[part] >> (slide[sq]))は5筋のoccだけを抽出している。slide[sq]はスライド数が定数で記録されている
// 5     その結果飛車の利きはこのようになる
// .     
// .     
// X     
// X     1
// *     *
// .     1
// .     1
// .	 1
// .     1
// 最後の & 127は2段～8段だけを抽出する。抽出したパターンはlance_attack[color][sq][index]のどれかに該当する
// blackとwhiteをor結合すればrookの縦利きになる
void test_rook_attack_file() {
	int64_t tmp[2];
	__m128i occ = set_board(0x298060C0A1121B45, 0x3abad);
	__m128i bb = rook_attack_file(sq5e, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	//print_bitboard(bb, "test_rook_attack_file 5e");
	TEST_ASSERT_EQUAL_HEX64(0x1E8000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = rook_attack_file(sq5f, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	//print_bitboard(bb, "test_rook_attack_file 5f");
	TEST_ASSERT_EQUAL_HEX64(0x1D8000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);

	bb = rook_attack_file(sq9f, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	//print_bitboard(bb, "test_rook_attack_file 9f");
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0xA000, tmp[1]);
}

//rookの縦と横の利きbitboardを合成して返す
void test_rook_attack() {
	int64_t tmp[2];
	__m128i occ = set_board(0x298060C0A1121B45, 0x3abad);
	__m128i bb = rook_attack(sq5e, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	print_bitboard(bb, "test_rook_attack 5e");
	TEST_ASSERT_EQUAL_HEX64(0x4021E8080000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, tmp[1]);

	bb = rook_attack(sq2f, occ);
	_mm_storeu_si128((const __m128i*)tmp, bb);
	print_bitboard(bb, "test_rook_attack 2f");
	TEST_ASSERT_EQUAL_HEX64(0x80402010083B020, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
}
