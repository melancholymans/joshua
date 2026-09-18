#include "unity.h"
#include "../shogi/bitboard.h"
#include "../shogi/position.h"
#include "test_bitboard.h"

void test_new_set_mask_bb(bitboard bb) {
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
	TEST_ASSERT_EQUAL_INT(20, _mm_popcnt_u64(bb.p[0]));
	TEST_ASSERT_EQUAL_INT(12, _mm_popcnt_u64(bb.p[1]));
}

void test_new_file_mask() {
	int f = file1;
	for(int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file2;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file3;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file4;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file5;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file6;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file7;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file8;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
	f = file9;
	for (int r = 0; r < 9; r += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, file_mask[f]));
	}
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(file_mask[f].p[0]));
	TEST_ASSERT_EQUAL_INT(9, _mm_popcnt_u64(file_mask[f].p[1]));
	//print_bitboard(file_mask[f], "test_file_mask");
}

void test_new_rank_mask() {
	int r = rank1;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank2;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank3;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank4;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank5;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank6;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank7;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank8;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
	r = rank9;
	for (int f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, rank_mask[r]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(rank_mask[r].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(rank_mask[r].p[1]));
	//print_bitboard(rank_mask[r], "test_rank_mask");
}

void test_new_all_one_bb() {
	TEST_ASSERT_EQUAL_INT(7 * 9, _mm_popcnt_u64(all_one_bb.p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 9, _mm_popcnt_u64(all_one_bb.p[1]));
	//print_bitboard(all_one_bb, "test_all_one_bb");
}

void test_new_in_front_mask() {
	int color = black;
	int rank = rank1;
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank2;
	for (int r=rank1,f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank3;
	for(int r=rank1;r<rank3;r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7*2, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2*2, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank4;
	for (int r = rank1; r < rank4; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank5;
	for (int r = rank1; r < rank5; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 4, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 4, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank6;
	for (int r = rank1; r < rank6; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 5, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 5, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank7;
	for (int r = rank1; r < rank7; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 6, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 6, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank8;
	for (int r = rank1; r < rank8; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 7, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 7, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank9;
	for (int r = rank1; r < rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 8, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 8, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	color = white;
	rank = rank9;
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(0, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank8;
	for (int r = rank9, f = 0; f < 9; f += 1) {
		int sq = set_square(f, r);
		TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
	}
	TEST_ASSERT_EQUAL_INT(7, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank7;
	for (int r = rank8; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 2, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 2, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank6;
	for (int r = rank7; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank5;
	for (int r = rank6; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 4, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 4, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank4;
	for (int r = rank5; r < rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 5, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 5, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank3;
	for (int r = rank4; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 6, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 6, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank2;
	for (int r = rank3; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 7, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 7, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");

	rank = rank1;
	for (int r = rank2; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, in_front_mask[color][rank]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 8, _mm_popcnt_u64(in_front_mask[color][rank].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 8, _mm_popcnt_u64(in_front_mask[color][rank].p[1]));
	//print_bitboard(in_front_mask[color][rank], "test_in_front_mask");
}

void test_new_enemy_field() {
	int color = black;
	for (int r = rank1; r <= rank3; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, enemy_field[color]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(enemy_field[color].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(enemy_field[color].p[1]));
	//print_bitboard(enemy_field[color], "test_enemy_field");

	color = white;
	for (int r = rank7; r <= rank9; r += 1) {
		for (int f = 0; f < 9; f += 1) {
			int sq = set_square(f, r);
			TEST_ASSERT_TRUE(is_biton(sq, enemy_field[color]));
		}
	}
	TEST_ASSERT_EQUAL_INT(7 * 3, _mm_popcnt_u64(enemy_field[color].p[0]));
	TEST_ASSERT_EQUAL_INT(2 * 3, _mm_popcnt_u64(enemy_field[color].p[1]));
	//print_bitboard(enemy_field[color], "test_enemy_field");
}

void test_set_biton() {
	bitboard bb;
	bb.m = _mm_setzero_si128();
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
	//print_bitboard(bb, "test_set_biton");
}

void test_set_board() {
	bitboard bb = set_board(0x298060C0A1121B45, 0x3abad);
	TEST_ASSERT_EQUAL_HEX64(0x298060C0A1121B45, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x3abad, bb.p[1]);
	//print_bitboard(bb, "test_set_board");
}

void test_first_one_from() {
	bitboard bb = set_board(0x298060C0A1121B45, 0x3abad);
	TEST_ASSERT_EQUAL_INT(0,first_one_from(&bb));
	TEST_ASSERT_EQUAL_HEX64(0x298060C0A1121B44, bb.p[0]);	//bbから最初の1bitを0にした値が返ることを確認
	TEST_ASSERT_EQUAL_HEX64(0x3abad, bb.p[1]);
	bb = set_board(0x282d026660282000, 0x28b5b);
	TEST_ASSERT_EQUAL_INT(13, first_one_from(&bb));
	TEST_ASSERT_EQUAL_HEX64(0x282D026660280000, bb.p[0]);	//bbから最初の1bitを0にした値が返ることを確認
	TEST_ASSERT_EQUAL_HEX64(0x28b5b, bb.p[1]);
}

void test_lance_block_mask() {
	bitboard bb = lance_block_mask(sq9a);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq8b);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0xfe, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq7c);
	TEST_ASSERT_EQUAL_HEX64(0x3f80000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq6d);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq5e);
	TEST_ASSERT_EQUAL_HEX64(0xFE000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq4f);
	TEST_ASSERT_EQUAL_HEX64(0x7F0000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq3g);
	TEST_ASSERT_EQUAL_HEX64(0x3F80000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq2h);
	TEST_ASSERT_EQUAL_HEX64(0x1fc00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
	bb = lance_block_mask(sq1i);
	TEST_ASSERT_EQUAL_HEX64(0xfe, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_lance_block_mask");
}

void test_index_to_occupied() {
	int sq = sq5e;
	bitboard lbm = lance_block_mask(sq);
	bitboard occ = index_to_occupied(0, lbm);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");


	occ = index_to_occupied(1, lbm);
	TEST_ASSERT_EQUAL_HEX64(0x2000000000, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");

	occ = index_to_occupied(2, lbm);
	TEST_ASSERT_EQUAL_HEX64(0x4000000000, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");

	occ = index_to_occupied(3, lbm);
	TEST_ASSERT_EQUAL_HEX64(0x6000000000, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");

	occ = index_to_occupied(4, lbm);
	TEST_ASSERT_EQUAL_HEX64(0x8000000000, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");

	occ = index_to_occupied(5, lbm);
	TEST_ASSERT_EQUAL_HEX64(0xA000000000, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");

	occ = index_to_occupied(125, lbm);
	TEST_ASSERT_EQUAL_HEX64(0xFA000000000, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");

	occ = index_to_occupied(127, lbm);
	TEST_ASSERT_EQUAL_HEX64(0xFE000000000, occ.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, occ.p[1]);
	//print_bitboard(occ, "test_index_to_occupied");
}

void test_lance_attack_calc() {
	//black
	//int64_t tmp[2];
	bitboard bm = lance_block_mask(sq5a);
	bitboard occ = index_to_occupied(97,bm);
	bitboard bb = lance_attack_calc(black, sq5a, occ);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(occ, "test_lance_attack_calc");

	bm = lance_block_mask(sq5d);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(black, sq5d, occ);
	TEST_ASSERT_EQUAL_HEX64(0x6000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bm = lance_block_mask(sq5f);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(black, sq5f, occ);
	TEST_ASSERT_EQUAL_HEX64(0x1E000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bm = lance_block_mask(sq5i);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(black, sq5i, occ);
	TEST_ASSERT_EQUAL_HEX64(0x80000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//white
	bm = lance_block_mask(sq5a);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5a, occ);
	TEST_ASSERT_EQUAL_HEX64(0x2000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bm = lance_block_mask(sq5c);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5c, occ);
	TEST_ASSERT_EQUAL_HEX64(0x78000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bm = lance_block_mask(sq5f);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5f, occ);
	TEST_ASSERT_EQUAL_HEX64(0x40000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bm = lance_block_mask(sq5i);
	occ = index_to_occupied(97, bm);
	bb = lance_attack_calc(white, sq5i, occ);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
}

void test_new_lance_attack() {
	bitboard bb = lance_attack_mask[black][sq5a][97];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[black][sq5d][97];
	TEST_ASSERT_EQUAL_HEX64(0x6000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_lance_attack");

	bb = lance_attack_mask[black][sq5f][97];
	TEST_ASSERT_EQUAL_HEX64(0x1E000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[black][sq5i][97];
	TEST_ASSERT_EQUAL_HEX64(0x80000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[white][sq5a][97];
	TEST_ASSERT_EQUAL_HEX64(0x2000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[white][sq5c][97];
	TEST_ASSERT_EQUAL_HEX64(0x78000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[white][sq5f][97];
	TEST_ASSERT_EQUAL_HEX64(0x40000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[white][sq5i][97];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[black][sq9e][35];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1800, bb.p[1]);

	bb = lance_attack_mask[black][sq9a][35];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);

	bb = lance_attack_mask[white][sq9e][35];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0xC000, bb.p[1]);

	bb = lance_attack_mask[white][sq9a][35];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x400, bb.p[1]);
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
	int8_t tmp[16] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10 };
	bitboard bb;
	bb.m = _mm_loadu_si128(tmp);
	bitboard result = byte_reverse(bb);
	TEST_ASSERT_EQUAL_HEX64(0x090a0b0c0d0e0f10, result.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0102030405060708, result.p[1]);
}

//                                     |---- [1]---------|---- [0]---------|      
// hight                               0x090a0b0c0d0e0f10 0x0102030405060708
// low                                 0x191a1b1c1d1e1f20 0x1112131415161718
// hight_result  = hight[1] | low[1] = 0x090a0b0c0d0e0f10 0x191a1b1c1d1e1f20
// low_resuto    = hight[0] | low[0] = 0x0102030405060708 0x1112131415161718
void test_unpack() {
	bitboard hight;
	hight.p[0] = 0x0102030405060708;
	hight.p[1] = 0x090a0b0c0d0e0f10;
	bitboard low;
	low.p[0] = 0x1112131415161718;
	low.p[1] = 0x191a1b1c1d1e1f20;
	bitboard hight_result;
	bitboard low_result;
	unpack(hight, low, &hight_result, &low_result);
	TEST_ASSERT_EQUAL_HEX64(0x191a1b1c1d1e1f20, hight_result.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x090a0b0c0d0e0f10, hight_result.p[1]);
	TEST_ASSERT_EQUAL_HEX64(0x1112131415161718, low_result.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0102030405060708, low_result.p[1]);
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
	bitboard hight;
	hight.p[0] = 0x0000000000100000;
	hight.p[1] = 0x0000000000001000;
	bitboard low;
	low.p[0] = 0x0000000000000000;
	low.p[1] = 0x0000000000000000;
	bitboard hight_result;
	bitboard low_result;
	decrement(hight, low, &hight_result, &low_result);
	TEST_ASSERT_EQUAL_HEX64(0x00000000000FFFFF, hight_result.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0000000000000FFF, hight_result.p[1]);
	TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFFF, low_result.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFFF, low_result.p[1]);
}

// rook_attack_rank_to_mask配列に座標を入力してテストしている
void test_new_rook_attacks() {
	TEST_ASSERT_EQUAL_HEX64(0x0400000000000000, rook_attack_rank_to_mask[sq6e][0].p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, rook_attack_rank_to_mask[sq6e][0].p[1]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, rook_attack_rank_to_mask[sq6e][1].p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1020408000010000, rook_attack_rank_to_mask[sq6e][1].p[1]);

	TEST_ASSERT_EQUAL_HEX64(0x00, rook_attack_rank_to_mask[sq7e][0].p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, rook_attack_rank_to_mask[sq7e][0].p[1]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, rook_attack_rank_to_mask[sq7e][1].p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1020408000010200, rook_attack_rank_to_mask[sq7e][1].p[1]);

	TEST_ASSERT_EQUAL_HEX64(0x402010080400000, rook_attack_rank_to_mask[sq2e][0].p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, rook_attack_rank_to_mask[sq2e][0].p[1]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, rook_attack_rank_to_mask[sq2e][1].p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1000000000000000, rook_attack_rank_to_mask[sq2e][1].p[1]);
	//print_bitboard(rook_attack_rank_to_mask[sq2e][0], "test_new_rook_attacks=2e");
	//print_bitboard(rook_attack_rank_to_mask[sq2e][1], "test_new_rook_attacks=2e");
}

void test_rook_attack_rank() {
	bitboard occ = set_board(0x298060C0A1121B45, 0x3abad);
	//print_bitboard(occ, "test_rook_attack_rank occ");

	bitboard bb = rook_attack_rank(sq5e, occ);
	TEST_ASSERT_EQUAL_HEX64(0x402000080000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack_rank 5e");

	bb = rook_attack_rank(sq5f, occ);
	TEST_ASSERT_EQUAL_HEX64(0x804000100804020, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack_rank 5f");

	bb = rook_attack_rank(sq5g, occ);
	TEST_ASSERT_EQUAL_HEX64(0x1008000201000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x8040, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack_rank 5g");

	bb = rook_attack_rank(sq5h, occ);
	TEST_ASSERT_EQUAL_HEX64(0x2010000402010080, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);	
	//print_bitboard(bb, "test_rook_attack_rank 5h");

	bb = rook_attack_rank(sq5i, occ);
	TEST_ASSERT_EQUAL_HEX64(0x4020000804020000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x100, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack_rank 5i");
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
	bitboard occ = set_board(0x298060C0A1121B45, 0x3abad);
	bitboard bb = rook_attack_file(sq5e, occ);
	TEST_ASSERT_EQUAL_HEX64(0x1E8000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack_file 5e");

	bb = rook_attack_file(sq5f, occ);
	TEST_ASSERT_EQUAL_HEX64(0x1D8000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack_file 5f");

	bb = rook_attack_file(sq9f, occ);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0xA000, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack_file 9f");
}

//rookの縦と横の利きbitboardを合成して返す
void test_rook_attack() {
	int64_t tmp[2];
	bitboard occ = set_board(0x298060C0A1121B45, 0x3abad);
	bitboard bb = rook_attack(sq5e, occ);
	TEST_ASSERT_EQUAL_HEX64(0x4021E8080000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x2010, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack 5e");

	bb = rook_attack(sq2f, occ);
	TEST_ASSERT_EQUAL_HEX64(0x80402010083B020, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_rook_attack 2f");
}

void test_new_bishop_attacks() {
	int64_t tmp[4];
	__m256i bb = bishop_attack_to_mask[sq4e][0];
	_mm256_storeu_si256((__m256i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x80808000000000, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[1]);
	TEST_ASSERT_EQUAL_HEX64(0x2008020000000000, tmp[2]);
	TEST_ASSERT_EQUAL_HEX64(0x00, tmp[3]);
	bb = bishop_attack_to_mask[sq4e][1];
	_mm256_storeu_si256((__m256i*)tmp, bb);
	TEST_ASSERT_EQUAL_HEX64(0x01, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x208200000000000, tmp[1]);
	TEST_ASSERT_EQUAL_HEX64(0x100, tmp[2]);
	TEST_ASSERT_EQUAL_HEX64(0x8080800000000000, tmp[3]);
}

void test_unpack256() {
	int64_t tmp[4];
	__m256i hi = _mm256_set_epi64x(0x14, 0x13, 0x12, 0x11);
	__m256i lo = _mm256_set_epi64x(0x04, 0x03, 0x02, 0x01);
	__m256i t1, t0;
	unpack256(hi, lo, &t1, &t0);
	_mm256_storeu_si256((__m256i*)tmp, t1);
	TEST_ASSERT_EQUAL_HEX64(0x02, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x12, tmp[1]);
	TEST_ASSERT_EQUAL_HEX64(0x04, tmp[2]);
	TEST_ASSERT_EQUAL_HEX64(0x14, tmp[3]);
	_mm256_storeu_si256((__m256i*)tmp, t0);
	TEST_ASSERT_EQUAL_HEX64(0x01, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x11, tmp[1]);
	TEST_ASSERT_EQUAL_HEX64(0x03, tmp[2]);
	TEST_ASSERT_EQUAL_HEX64(0x13, tmp[3]);
}

void test_decrement256() {
	__m256i hi = _mm256_set_epi64x(0x1010101000000, 0x20080, 0x104104000000000, 0x202);
	__m256i lo = _mm256_set_epi64x(0x00, 0x1004000000000000, 0x00, 0x101000000000000);
	__m256i t1,t0;
	decrement256(hi, lo, &t1, &t0);
	int64_t tmp[4];
	_mm256_storeu_si256((__m256i*)tmp, t1);
	TEST_ASSERT_EQUAL_HEX64(0x202, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x104103fffffffff, tmp[1]);
	TEST_ASSERT_EQUAL_HEX64(0x20080, tmp[2]);
	TEST_ASSERT_EQUAL_HEX64(0x1010100ffffff, tmp[3]);
	_mm256_storeu_si256((__m256i*)tmp, t0);
	TEST_ASSERT_EQUAL_HEX64(0x100ffffffffffff, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0xffffffffffffffff, tmp[1]);
	TEST_ASSERT_EQUAL_HEX64(0x1003ffffffffffff, tmp[2]);
	TEST_ASSERT_EQUAL_HEX64(0xffffffffffffffff, tmp[3]);
}

// tmp        
// [0]0x01  [1]0x02  [2]0x03  [3]0x04  [4]0x05  [5]0x06  [6]0x07  [7]0x08  [8]0x09  [9]0x0a  [10]0x0b [11]0x0c [12]0x0d [13]0x0e [14]0x0f [15]0x10
// [16]0x21 [17]0x22 [18]0x23 [19]0x24 [20]0x25 [21]0x26 [22]0x27 [23]0x28 [24]0x29 [25]0x2a [26]0x2b [27]0x2c [28]0x2d [29]0x2e [30]0x2f [31]0x30
// __m256i bb
// | ----[3]----------------|--------[2]------------|--------[1]------------|--------[0]------------] 
// 0x30 2f 2e 2d 2c 2b 2a 29 28 27 26 25 24 23 22 21 10 0f 0e 0d 0c 0b 0a 09 08 07 06 05 04 03 02 01   
// これをリバースすると
// __m256i result 256bit全域でリバースするのではなく上位128bit[255:128] 下位128bit[0:127]範囲でリバースする
// | ----[3]---------------|--------[2]------------|---[1]-------------------|--------[0]----------] 
// 0x21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10
void test_byte_reverse256() {
	int8_t td[32] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,
					   0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30 };
	__m256i bb = _mm256_loadu_si256(td);
	__m256i result = byte_reverse256(bb);
	int64_t tmp[4];
	_mm256_storeu_si256((const __m256i*)tmp, result);
	TEST_ASSERT_EQUAL_HEX64(0x090a0b0c0d0e0f10, tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0102030405060708, tmp[1]);
	TEST_ASSERT_EQUAL_HEX64(0x292a2b2c2d2e2f30, tmp[2]);
	TEST_ASSERT_EQUAL_HEX64(0x2122232425262728, tmp[3]);
}

void test_merge256() {
	//bitboard occ1 = set_board(0x802008000080200, 0x10040);
	//print_bitboard(occ1, "test_merge256");
	//bitboard occ2 = set_board(0x202000202020, 0x00);
	//print_bitboard(occ2, "test_merge256");
	__m256i bb = _mm256_set_epi64x(0x10040, 0x802008000080200, 0x00, 0x202000202020);
	__m128i mg = merge256(bb);
	int64_t tmp[2];
	_mm_storeu_si128((__m128*)tmp, mg);
	TEST_ASSERT_EQUAL_HEX64(0x080220a000282220,tmp[0]);
	TEST_ASSERT_EQUAL_HEX64(0x0000000000010040,tmp[1]);
	//bitboard bd;
	//bd.m = merge256(bb);
	//print_bitboard(bd, "test_merge256");

}

void test_bishop_attack() {
	bitboard occ = set_board(0x298060C0A1121B45, 0x3abad);
	//print_bitboard(occ, "test_bishop_attack");
	bitboard mg = bishop_attack(sq4g, occ);
	TEST_ASSERT_EQUAL_HEX64(0x2220a0002822008, mg.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x04, mg.p[1]);
	//print_bitboard(mg, "test_bishop_attack"); ;
	mg = bishop_attack(sq3b, occ);
	TEST_ASSERT_EQUAL_HEX64(0x28000a00, mg.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, mg.p[1]);
	//print_bitboard(mg, "test_bishop_attack"); ;
	mg = bishop_attack(sq8e, occ);
	TEST_ASSERT_EQUAL_HEX64(0xa00802008000000, mg.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x5000, mg.p[1]);
	//print_bitboard(mg, "test_bishop_attack"); ;
}

void test_new_king_attacks() {
	bitboard bb = king_attack[sq4g];
	TEST_ASSERT_EQUAL_HEX64(0xe0503800000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_king_attacks");
	bb = king_attack[sq7e];
	TEST_ASSERT_EQUAL_HEX64(0xa07000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x38, bb.p[1]);
	//print_bitboard(bb, "test_new_king_attacks");
	bb = king_attack[sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0xa07, bb.p[1]);
	//print_bitboard(bb, "test_new_king_attacks");
}

void test_new_gold_attacks() {
	int color = black;
	bitboard bb = gold_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0x60501800000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_attacks");
	bb = gold_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0xA03000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x18, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_attacks");
	bb = gold_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0xa03, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_attacks");
	color = white;
	bb = gold_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0xC0503000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_attacks");
	bb = gold_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0xA06000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x30, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_attacks");
	bb = gold_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0xa06, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_attacks");
}

void test_new_silver_attacks() {
	int color = black;
	bitboard bb = silver_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0xA0102800000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_silver_attacks");
	bb = silver_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0x205000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x28, bb.p[1]);
	//print_bitboard(bb, "test_new_silver_attacks");
	bb = silver_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x205, bb.p[1]);
	//print_bitboard(bb, "test_new_silver_attacks");
	color = white;
	bb = silver_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0xA0402800000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_silver_attacks");
	bb = silver_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0x805000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x28, bb.p[1]);
	//print_bitboard(bb, "test_new_silver_attacks");
	bb = silver_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x805, bb.p[1]);
	//print_bitboard(bb, "test_new_silver_attacks");
}

void test_first_one_from_nodelete() {
	bitboard bb = set_board(0x298060C0A1121B45, 0x3abad);
	TEST_ASSERT_EQUAL_INT(0, first_one_from_nodelete(bb));
	TEST_ASSERT_EQUAL_HEX64(0x298060C0A1121B45, bb.p[0]);	//bbに変化がないことを確認
	TEST_ASSERT_EQUAL_HEX64(0x3abad, bb.p[1]);
	bb = set_board(0x282d026660282000, 0x28b5b);
	TEST_ASSERT_EQUAL_INT(13, first_one_from_nodelete(bb));
	TEST_ASSERT_EQUAL_HEX64(0x282d026660282000, bb.p[0]);	//bbに変化がないことを確認
	TEST_ASSERT_EQUAL_HEX64(0x28b5b, bb.p[1]);
}

void test_star_attacks() {
	bitboard bb = star_attacks(sq4g);
	TEST_ASSERT_EQUAL_HEX64(0xA0002800000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_star_attacks");
	bb = star_attacks(sq7e);
	TEST_ASSERT_EQUAL_HEX64(0x5000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x28, bb.p[1]);
	//print_bitboard(bb, "test_star_attacks");
	bb = star_attacks(sq9b);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x05, bb.p[1]);
	//print_bitboard(bb, "test_star_attacks");
}

void test_new_knight_attacks() {
	int color = black;
	bitboard bb = knight_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0x10000400000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_knight_attacks");
	bb = knight_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0x800000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x04, bb.p[1]);
	//print_bitboard(bb, "test_new_knight_attacks");
	bb = knight_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_knight_attacks");
	color = white;
	bb = knight_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0x100004000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_knight_attacks");
	bb = knight_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0x8000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x40, bb.p[1]);
	//print_bitboard(bb, "test_new_knight_attacks");
	bb = knight_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x08, bb.p[1]);
	//print_bitboard(bb, "test_new_knight_attacks");
}

void test_new_pawn_attacks() {
	int color = black;
	bitboard bb = pawn_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0x100000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_pawn_attacks");
	bb = pawn_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0x200000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_pawn_attacks");
	bb = pawn_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x200, bb.p[1]);
	//print_bitboard(bb, "test_new_pawn_attacks");
	color = white;
	bb = pawn_attack[color][sq4g];
	TEST_ASSERT_EQUAL_HEX64(0x400000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_pawn_attacks");
	bb = pawn_attack[color][sq7e];
	TEST_ASSERT_EQUAL_HEX64(0x800000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_pawn_attacks");
	bb = pawn_attack[color][sq9b];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x800, bb.p[1]);
	//print_bitboard(bb, "test_new_pawn_attacks");
}

void test_new_between_bb() {
	int sq1 = sq3c;
	int sq2 = sq9i;
	int sq3 = sq9c;
	int sq4 = sq2h;
	bitboard bb = between_bb[sq1][sq3];
	TEST_ASSERT_EQUAL_HEX64(0x100804020000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x04, bb.p[1]);
	//print_bitboard(bb, "test_new_between_bb");
	bb = between_bb[sq1][sq2];
	TEST_ASSERT_EQUAL_HEX64(0x1004010040000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x80, bb.p[1]);
	//print_bitboard(bb, "test_new_between_bb");
	bb = between_bb[sq3][sq2];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1F000, bb.p[1]);
	//print_bitboard(bb, "test_new_between_bb");
	bb = between_bb[sq4][sq2];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_between_bb");
}

//rook_attack_edge[sq3c]の場合
//             x
//             x
// x x x x x x s x x
//             x
//             x
//             x
//             x
//             x
//             x
// bishop,lance[black]lance[white]も同様
void test_new_attack_to_edge() {
	int sq1 = sq3c;
	int sq2 = sq6f;
	int sq3 = sq8b;
	int sq4 = sq9i;
	bitboard bb = rook_attack_to_edge[sq1];
	TEST_ASSERT_EQUAL_HEX64(0x100804027EC0804, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x804, bb.p[1]);
	//print_bitboard(bb, "test_new_attack_to_edge");
	bb = bishop_attack_to_edge[sq2];
	TEST_ASSERT_EQUAL_HEX64(0x1400050444100401, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x20888, bb.p[1]);
	//print_bitboard(bb, "test_new_attack_to_edge");
	bb = lance_attack_to_edge[white][sq3];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1FC, bb.p[1]);
	//print_bitboard(bb, "test_new_attack_to_edge");
	bb = lance_attack_to_edge[black][sq4];
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x1FE00, bb.p[1]);
	//print_bitboard(bb, "test_new_attack_to_edge");
}

//  9 8 7 6 5 4 3 2 1
//a . . . . . . X . .
//b . . . . . X . X .
//c . . . . X . S . X
//d . . . . X . . . X
//e . . . . X X X X X
//f . . . . . . . . .
//g . . . . . . . . .
//h . . . . . . . . .
//i . . . . . . . . .
void test_new_gold_check_table() {
	int sq = sq3c;
	bitboard bb = gold_check_table[black][sq];
	TEST_ASSERT_EQUAL_HEX64(0x1C09044241C, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_check_table");
	sq = sq8f;
	bb = gold_check_table[white][sq];
	TEST_ASSERT_EQUAL_HEX64(0x1207000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x9088, bb.p[1]);
	//print_bitboard(bb, "test_new_gold_check_table");
}

//silver_check_table[black][sq3c]の場合
//  9 8 7 6 5 4 3 2 1
//a . . . . X X X X X
//b . . . . X . X . X
//c . . . . X X S X X
//d . . . . X . . . X
//e . . . . X X X X X
//f . . . . . . . . .
//g . . . . . . . . .
//h . . . . . . . . .
//i . . . . . . . . .
//銀は敵陣内で成ることができるので銀の利きと、成銀の利きが合わさっている
void test_new_silver_check_table() {
	int sq = sq3c;
	bitboard bb = silver_check_table[black][sq];
	TEST_ASSERT_EQUAL_HEX64(0x1F0A84C2A1F, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	print_bitboard(bb, "test_new_silver_check_table");
	sq = sq8f;
	bb = silver_check_table[white][sq];
	TEST_ASSERT_EQUAL_HEX64(0x2A1D000000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x150C8, bb.p[1]);
	print_bitboard(bb, "test_new_silver_check_table");
}

//knight_check_table[black][sq3c]の場合
//  9 8 7 6 5 4 3 2 1
//a . . . . . . . . .
//b . . . . . . . . .
//c . . . . . . S . .
//d . . . . . X . X .
//e . . . . X . X . X
//f . . . . . . . . .
//g . . . . X . X . X
//h . . . . . . . . .
//i . . . . . . . . .
void test_new_knight_check_table() {
	int sq = sq3c;
	bitboard bb = knight_check_table[black][sq];
	TEST_ASSERT_EQUAL_HEX64(0x50041401050, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x00, bb.p[1]);
	print_bitboard(bb, "test_new_knight_check_table");
	sq = sq8f;
	bb = knight_check_table[white][sq];
	TEST_ASSERT_EQUAL_HEX64(0x400400000000000, bb.p[0]);
	TEST_ASSERT_EQUAL_HEX64(0x2002, bb.p[1]);
	print_bitboard(bb, "test_new_silver_check_table");
}