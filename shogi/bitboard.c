#include <stdio.h>
#include <stdint.h>

#include "bitboard.h"
#include "position.h"

__m128i mask_bb[81];
__m128i file_mask[9];
__m128i rank_mask[9];
__m128i all_one_bb;
__m128i in_front_mask[2][9];
__m128i enemy_field[2];

//各種テーブルの初期化
void init_tables() {
	new_mask_bb();
	new_file_mask();
	new_rank_mask();
	new_all_one_bb();
	new_in_front_mask();
	new_enemy_field();
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

void new_rank_mask() {
	for (int i = 0; i < 9; i += 1) {
		int64_t tmp[2] = { (int64_t)0x40201008040201 << i,(int64_t)0x201 << i };
		rank_mask[i] = _mm_loadu_si128((const __m128i*)tmp);
	}
}

void new_all_one_bb() {
	int64_t tmp[2] = { 0x7fffffffffffffff,0x000000000003ffff };
	all_one_bb = _mm_loadu_si128((const __m128i*)tmp);
}

__m128i all_zero_bb() {
	return _mm_setzero_si128();
}

// Rankを指定するとそのRankより前段のRankをbitで埋めていく。カラーによって前段の方向は反対になる
// RankMaskが設定されていることが前提、AllZeroBBが設定されていることが前提
void new_in_front_mask() {
	in_front_mask[black][0] = all_zero_bb();
	in_front_mask[black][1] = rank_mask[0];
	in_front_mask[black][2] = _mm_or_si128(in_front_mask[black][1], rank_mask[1]);
	in_front_mask[black][3] = _mm_or_si128(in_front_mask[black][2], rank_mask[2]);
	in_front_mask[black][4] = _mm_or_si128(in_front_mask[black][3], rank_mask[3]);
	in_front_mask[black][5] = _mm_or_si128(in_front_mask[black][4], rank_mask[4]);
	in_front_mask[black][6] = _mm_or_si128(in_front_mask[black][5], rank_mask[5]);
	in_front_mask[black][7] = _mm_or_si128(in_front_mask[black][6], rank_mask[6]);
	in_front_mask[black][8] = _mm_or_si128(in_front_mask[black][7], rank_mask[7]);
	in_front_mask[white][8] = all_zero_bb();
	in_front_mask[white][7] = rank_mask[8];
	in_front_mask[white][6] = _mm_or_si128(in_front_mask[white][7], rank_mask[7]);
	in_front_mask[white][5] = _mm_or_si128(in_front_mask[white][6], rank_mask[6]);
	in_front_mask[white][4] = _mm_or_si128(in_front_mask[white][5], rank_mask[5]);
	in_front_mask[white][3] = _mm_or_si128(in_front_mask[white][4], rank_mask[4]);
	in_front_mask[white][2] = _mm_or_si128(in_front_mask[white][3], rank_mask[3]);
	in_front_mask[white][1] = _mm_or_si128(in_front_mask[white][2], rank_mask[2]);
	in_front_mask[white][0] = _mm_or_si128(in_front_mask[white][1], rank_mask[1]);
}

// 敵陣を表現する。Black側だとRank1,2,3のbitが立っている、White側だとRank7,8,9のbitが立っている
// RankMaskが設定されていることが前提条件
void new_enemy_field() {
	enemy_field[black] = _mm_or_si128(_mm_or_si128(rank_mask[rank1], rank_mask[rank2]), rank_mask[rank3]);
	enemy_field[white] = _mm_or_si128(_mm_or_si128(rank_mask[rank7], rank_mask[rank8]), rank_mask[rank9]);
}

// new_lance_attack関数のヘルパー関数,sq座標があるfile_maskから1段と9段のbitを除いたbitBoardを返す
// all_one_bbとのAndNotはC/C++のチルダ演算子（bit反転）と同等の演算となる
__m128i lance_block_mask(int sq) {
	__m128i bb = _mm_andnot_si128(all_one_bb, _mm_or_si128(rank_mask[rank1], rank_mask[rank9]));
	return _mm_and_si128(file_mask[set_file(sq)],bb);
}

// 渡されたbitBoardを0から80までスキャンして最初bitが立っていたindexを返す,1つもビットが立っていなかったらfalseを返す
// LSB側からの最初の1bitを0にする
int first_one_from(__m128i* bb) {
	int64_t tmp[2];
	unsigned long sq;
	_mm_storeu_si128((__m128i*)tmp, *bb);
	if (tmp[0] != 0) {
		_BitScanForward(&sq,tmp[0]);
		tmp[0] = tmp[0] & (tmp[0] - 1);
		*bb = _mm_loadu_si128((const __m128i*)tmp);
		return (int)sq;
	}
	if (tmp[1] != 0) {
		_BitScanForward(&sq, tmp[1]);
		tmp[1] = tmp[1] & (tmp[1] - 1);
		*bb = _mm_loadu_si128((const __m128i*)tmp);
		return (int)sq + 63;
	}
	return -1; // ビットが立っていない場合は-1を返す
}

__m128i index_to_occupied(int idx, __m128i block_mask) {
	__m128i tmp = block_mask;
	__m128i result = all_zero_bb();
	for(int i = 0;i < 7;i += 1){
		int sq = first_one_from(&tmp);
		if(idx & (1 << i)){
			__m128i bit = _mm_and_si128(tmp, _mm_sub_epi64(tmp, _mm_set1_epi64x(1)));
			result = _mm_or_si128(result, bit);
			tmp = _mm_andnot_si128(bit, tmp);
		}
	}
}

// lanceの利きbitboardを生成する
void new_lance_attack(){
	for(int c = black;c <= white;c+=1){
		for (int sq = sq1a; sq <= sq9i;sq+=1) {
			__m128i block_mask = lance_block_mask(sq);
			for(int i=0;i<128;i+=1){
				__m128i occ = index_to_occ_upied(i,block_mask);
				lance_attack[c][sq][i] = lance_attack_calc(c,sq,occ);
			}
		}
	}
}

__m128i set_board(int64_t idx0, int64_t idx1) {
	int64_t tmp[2] = { idx0, idx1 };
	return _mm_loadu_si128((const __m128i*)tmp);
}

//引数bbのsq座標にビットが立っていればtrueを返す
bool is_biton(int sq, __m128i bb) {
	return !(bool)_mm_testz_si128(mask_bb[sq],bb);
}

// 引数bbのsq座標にビットを立てる
void set_biton(int sq, __m128i* bb) {
	*bb = _mm_or_si128(*bb, mask_bb[sq]);
}

//debug用のbitboard表示
void print_bitboard(__m128i bb, const char* msg) {
	char rstr[] = "abcdefghi";
	printf("| %s |\n", msg);
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
