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
__m128i lance_attack[2][81][128];

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
//rank1  0     0    0       rank1とrank9はlanceにとって死駒になるので移動可能範囲から除外する
//       x     x    x　　　　つまりlance_block_maskは座標sq軸のlanceの移動可能範囲（block）を表す
//       ...   ...  ...
//       sq    x    x
//       x     x    x
//       x     sq   x 
//       x     x    sq
// rank9 0     0    0 
__m128i lance_block_mask(int sq) {
	__m128i bb = _mm_andnot_si128(_mm_or_si128(rank_mask[rank1], rank_mask[rank9]), all_one_bb);
	return _mm_and_si128(file_mask[set_file(sq)],bb);
}

// 渡されたbitBoardを0から80までスキャンして最初bitが立っていたindexを返す,1つもビットが立っていなかったらfalseを返す
// LSB側からの最初の1bitを0にする(LSBから削っていくことで、次の１bitが最初に立っているbitになる)
int first_one_from(__m128i* bb) {
	int64_t tmp[2];
	unsigned long sq;
	_mm_storeu_si128((__m128i*)tmp, *bb);
	if (tmp[0] != 0) {
		_BitScanForward64(&sq,tmp[0]);
		tmp[0] = tmp[0] & (tmp[0] - 1);
		*bb = _mm_loadu_si128((const __m128i*)tmp);
		return (int)sq;
	}
	if (tmp[1] != 0) {
		_BitScanForward64(&sq, tmp[1]);
		tmp[1] = tmp[1] & (tmp[1] - 1);
		*bb = _mm_loadu_si128((const __m128i*)tmp);
		return (int)sq + 63;
	}
	return -1; // ビットが立っていない場合は-1を返す
}

// NewLanceAttack関数のヘルパー関数、bbが盤の状態を表すbitBoard
__m128i lance_attack_calc(int color,int square, __m128i occ) {
	int f = set_file(square);
	__m128i bb = all_zero_bb();
	//上方向
	for(int r = set_rank(square); r > rank1; ){
		r -= 1;
		int sq = set_square(f, r);
		set_biton(sq, &bb);
		if (is_biton(sq, occ)) {
			break;
		}
	}
	//下方向
	for (int r = set_rank(square); r >  rank9; ) {
		r += 1;
		int sq = set_square(f, r);
		set_biton(sq, &bb);
		if (is_biton(sq, occ)) {
			break;
		}
	}
	return _mm_or_si128(bb,in_front_mask[color][set_rank(square)]);
}

// NewPawnAttack関数のヘルパー関数
// blockはsq座標に応じたlanceの移動可能範囲を表すbitBoard
// idxはパターン番号0から127までの番号がある。
// sq lanceの移動範囲 idx 
//                   0 1 2 3 4 ... 14 ... 120 ... 127   この128のパターンをresultにbitboardとして写取っている
// 5b x              0 1 0 1 0     0      0       1 
// 5c x              0 0 1 1 0     1      0       1
// 5d x              0 0 0 0 1     1      0       1
// 5e x              0 0 0 0 0     1      1       1
// 5f x              0 0 0 0 0     0      1       1
// 5g x              0 0 0 0 0     0      1       1
// 5h x              0 0 0 0 0     0      0       0
// lanceの移動可能範囲のなかで駒を置けるパターンは2^7=128通りあるので、idxは0から127までの番号がある。そのパターンを
// sq座標ごとに128個の配列に保存しておく
__m128i index_to_occupied(int idx, __m128i block_mask) {
	__m128i tmp = block_mask;
	__m128i result = all_zero_bb();
	for(int i = 0;i < 7;i += 1){
		int sq = first_one_from(&tmp);
		if(sq == -1){
			printf("Error: [%s : %d : %s()]\n", __FILE__, __LINE__, __func__);
			exit(1);
		}
		if((idx & (1 << i))>0){
			set_biton(sq, &result);
		}
	}
	return result;
}

// lanceの利きbitboardを生成する
void new_lance_attack(){
	for(int c = black;c <= white;c+=1){
		for (int sq = sq1a; sq <= sq9i;sq+=1) {
			__m128i block_mask = lance_block_mask(sq);
			for(int i=0;i<128;i+=1){
				__m128i occ = index_to_occupied(i,block_mask);
				lance_attack[c][sq][i] = lance_attack_calc(c,sq,occ);
			}
		}
	}
}

__m128i all_zero_bb() {
	return _mm_setzero_si128();
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
