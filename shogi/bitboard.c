#include <stdio.h>
#include <stdint.h>

#include "bitboard.h"
#include "position.h"

bitboard mask_bb[81];
bitboard file_mask[9];
bitboard rank_mask[9];
bitboard all_one_bb;
bitboard in_front_mask[2][9];
bitboard enemy_field[2];
bitboard lance_attack[2][81][128];
bitboard rook_attack_rank_to_mask[81][2];
__m256i bishop_attack_to_mask[81][2];
bitboard king_attack[81];
bitboard gold_attack[2][81];
bitboard silver_attack[2][81];
bitboard knight_attack[2][81];
bitboard pawn_attack[2][81];
bitboard between_bb[81][81];
const int slide[81] = {
	1,1,1,1,1,1,1,1,1,
	10,10,10,10,10,10,10,10,10,
	19,19,19,19,19,19,19,19,19,
	28,28,28,28,28,28,28,28,28,
	37,37,37,37,37,37,37,37,37,
	46,46,46,46,46,46,46,46,46,
	55,55,55,55,55,55,55,55,55,
	1,1,1,1,1,1,1,1,1,
	10,10,10,10,10,10,10,10,10
};


//座標sqごとにbitが立っている配列を生成している
void new_mask_bb() {
	for (int i = 0; i < 63; i += 1) {
		mask_bb[i].p[0] = (int64_t)1 << i;
		mask_bb[i].p[1] = 0x00;
	}
	for (int i = 63; i < 81; i += 1){
		mask_bb[i].p[0] = 0x00;
		mask_bb[i].p[1] = (int64_t)1 << (i - 63);
	}
}

void new_file_mask() {
	for (int i = 0; i < 7; i += 1) {
		file_mask[i].p[0] = (int64_t)0x1ff << (9 * i);
		file_mask[i].p[1] = 0x00;
	}
	for (int i = 7; i < 9; i += 1) {
		file_mask[i].p[0] = 0x00;
		file_mask[i].p[1] = (int64_t)0x1ff << (9 * (i - 7));
	}
}

void new_rank_mask() {
	for (int i = 0; i < 9; i += 1) {
		rank_mask[i].p[0] = (int64_t)0x40201008040201 << i;
		rank_mask[i].p[1] = (int64_t)0x201 << i;
	}
}

void new_all_one_bb() {
	all_one_bb.p[0] = 0x7fffffffffffffff;
	all_one_bb.p[1] = 0x000000000003ffff;
}

// Rankを指定するとそのRankより前段のRankをbitで埋めていく。カラーによって前段の方向は反対になる
// RankMaskが設定されていることが前提、AllZeroBBが設定されていることが前提
void new_in_front_mask() {
	in_front_mask[black][0].m = _mm_setzero_si128();
	in_front_mask[black][1] = rank_mask[0];
	in_front_mask[black][2].m = _mm_or_si128(in_front_mask[black][1].m, rank_mask[1].m);
	in_front_mask[black][3].m = _mm_or_si128(in_front_mask[black][2].m, rank_mask[2].m);
	in_front_mask[black][4].m = _mm_or_si128(in_front_mask[black][3].m, rank_mask[3].m);
	in_front_mask[black][5].m = _mm_or_si128(in_front_mask[black][4].m, rank_mask[4].m);
	in_front_mask[black][6].m = _mm_or_si128(in_front_mask[black][5].m, rank_mask[5].m);
	in_front_mask[black][7].m = _mm_or_si128(in_front_mask[black][6].m, rank_mask[6].m);
	in_front_mask[black][8].m = _mm_or_si128(in_front_mask[black][7].m, rank_mask[7].m);
	in_front_mask[white][8].m = _mm_setzero_si128();
	in_front_mask[white][7] = rank_mask[8];
	in_front_mask[white][6].m = _mm_or_si128(in_front_mask[white][7].m, rank_mask[7].m);
	in_front_mask[white][5].m = _mm_or_si128(in_front_mask[white][6].m, rank_mask[6].m);
	in_front_mask[white][4].m = _mm_or_si128(in_front_mask[white][5].m, rank_mask[5].m);
	in_front_mask[white][3].m = _mm_or_si128(in_front_mask[white][4].m, rank_mask[4].m);
	in_front_mask[white][2].m = _mm_or_si128(in_front_mask[white][3].m, rank_mask[3].m);
	in_front_mask[white][1].m = _mm_or_si128(in_front_mask[white][2].m, rank_mask[2].m);
	in_front_mask[white][0].m = _mm_or_si128(in_front_mask[white][1].m, rank_mask[1].m);
}

// 敵陣を表現する。Black側だとRank1,2,3のbitが立っている、White側だとRank7,8,9のbitが立っている
// RankMaskが設定されていることが前提条件
void new_enemy_field() {
	enemy_field[black].m = _mm_or_si128(_mm_or_si128(rank_mask[rank1].m, rank_mask[rank2].m), rank_mask[rank3].m);
	enemy_field[white].m = _mm_or_si128(_mm_or_si128(rank_mask[rank7].m, rank_mask[rank8].m), rank_mask[rank9].m);
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
bitboard lance_block_mask(const int sq) {
	bitboard bb;
	bb.m = _mm_andnot_si128(_mm_or_si128(rank_mask[rank1].m, rank_mask[rank9].m), all_one_bb.m);
	bb.m = _mm_and_si128(file_mask[set_file(sq)].m, bb.m);
	return bb;
}

// 渡されたbitBoardを0から80までスキャンして最初bitが立っていたindexを返す,1つもビットが立っていなかったらfalseを返す
// LSB側からの最初の1bitを0にする(LSBから削っていくことで、次の１bitが最初に立っているbitになる)
int first_one_from(bitboard* bb) {
	unsigned long sq;
	if (bb->p[0] != 0) {
		_BitScanForward64(&sq, bb->p[0]);
		bb->p[0] = bb->p[0] & (bb->p[0] - 1);
		return (int)sq;
	}
	if (bb->p[1] != 0) {
		_BitScanForward64(&sq, bb->p[1]);
		bb->p[1] = bb->p[1] & (bb->p[1] - 1);
		return (int)sq + 63;
	}
	return -1; // ビットが立っていない場合は-1を返す
}

// NewLanceAttack関数のヘルパー関数、bbが盤の状態を表すbitBoard
// occはindex_to_occupied関数で生成されたlanceの移動可能範囲のなかで駒を置いたパターンを表すbitBoard
// 指定されたcolor,指定された座標sqにlanceが置かれたとして、そのlanceから上、下に利きを記録する
// 最後にcolorでblackなら上向きの利きを、whiteなら下向きの利きにマスクして返す
// sq5eでoccのパターンが97(0x61)の場合
//    sq  occ(97)   lanceの利き in_front_mask[color=black][rank5] andした結果
// 5a     0          0          1                                 0
// 5b     1          1          1                                 1
// 5c     0          1          1                                 1
// 5d     0          1          1                                 1
// 5e x   0          0          0                                 0
// 5f     0          1          0                                 0
// 5g     1          1          0                                 1
// 5h     1          0          0                                 0
// 5i     0          0          0                                 0
bitboard lance_attack_calc(const int color, const int square, const bitboard occ) {
	int f = set_file(square);
	bitboard bb;
	bb.m = _mm_setzero_si128();
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
	for (int r = set_rank(square); r <  rank9; ) {
		r += 1;
		int sq = set_square(f, r);
		set_biton(sq, &bb);
		if (is_biton(sq, occ)) {
			break;
		}
	}
	bb.m = _mm_and_si128(bb.m, in_front_mask[color][set_rank(square)].m);
	return bb;
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
bitboard index_to_occupied(const int idx, const bitboard block_mask) {
	bitboard tmp = block_mask;
	bitboard result;
	result.m = _mm_setzero_si128();
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
void new_lance_attack() {
	for(int c = black;c <= white;c+=1){
		for (int sq = sq1a; sq <= sq9i;sq+=1) {
			bitboard block_mask = lance_block_mask(sq);
			for(int i=0;i<128;i+=1){
				bitboard occ = index_to_occupied(i,block_mask);
				lance_attack[c][sq][i] = lance_attack_calc(c,sq,occ);
			}
		}
	}
}

// 飛車の利きの右方向と角の利きの右上、右下方向を求める時に使う。
// byte単位でリバースする。bit単位ではない
bitboard byte_reverse(bitboard bb) {
	const __m128i shuffle = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	bb.m = _mm_shuffle_epi8(bb.m, shuffle);
	return bb;
}

// hi_in,lo_inの上位64bitを抜き出して１つの128bitレジスタ(hi_out)を構成する
// hi_in,lo_inの下位64bitを抜き出して１つの128bitレジスタ(lo_out)を構成する
void unpack(const bitboard hi_in,const bitboard lo_in, bitboard* hi_out, bitboard* lo_out) {
	hi_out->m = _mm_unpackhi_epi64(lo_in.m, hi_in.m);
	lo_out->m = _mm_unpacklo_epi64(lo_in.m, hi_in.m);
}

// -1を引くことによってsqからの利きを生成する 
void decrement(const bitboard hi_in, const bitboard lo_in, bitboard* hi_out, bitboard* lo_out) {
	hi_out->m = _mm_add_epi64(hi_in.m, _mm_cmpeq_epi64(lo_in.m, _mm_setzero_si128()));
	lo_out->m = _mm_add_epi64(lo_in.m, _mm_set1_epi64x(-1LL));
}

// 座標s	qごとの飛車の横利きをrook_attack_rank_to_mask配列に保存しておく
void new_rook_attacks() {
	for (int f = file1; f <= file9; f+=1) {
		for (int r = rank1; r <= rank9; r+=1) {
			bitboard left;
			left.m = _mm_setzero_si128();
			bitboard right;
			right.m = _mm_setzero_si128();
			//sq座標から左方向
			for (int f2 = f + 1; f2 <= file9; f2+=1) {
				left.m = _mm_or_si128(left.m,mask_bb[set_square(f2, r)].m);
			}
			//sq座標から右方向
			for (int f2 = f - 1; f2 >= file1; f2-=1) {
				right.m = _mm_or_si128(right.m,mask_bb[set_square(f2, r)].m);
			}
			bitboard right_rev = byte_reverse(right);
			bitboard hi, lo;
			unpack(right_rev, left, &hi, &lo);
			rook_attack_rank_to_mask[set_square(f, r)][0] = lo;
			rook_attack_rank_to_mask[set_square(f, r)][1] = hi;
		}
	}
}

// 局面(occ)に応じて飛車の横利きを返す
bitboard rook_attack_rank(const int sq,const bitboard occ) {
	bitboard hi, lo, t1, t0;
	const bitboard mask_lo = rook_attack_rank_to_mask[sq][0];
	const bitboard mask_hi = rook_attack_rank_to_mask[sq][1];
	bitboard rocc = byte_reverse(occ);
	unpack(rocc, occ, &hi, &lo);
	hi.m = _mm_and_si128(hi.m, mask_hi.m);
	lo.m = _mm_and_si128(lo.m, mask_lo.m);
	decrement(hi, lo, &t1, &t0);
	t1.m = _mm_and_si128(_mm_xor_si128(t1.m, hi.m), mask_hi.m);
	t0.m = _mm_and_si128(_mm_xor_si128(t0.m, lo.m), mask_lo.m);
	unpack(t1, t0, &hi, &lo);
	bitboard result;
	result.m = _mm_or_si128(byte_reverse(hi).m, lo.m);
	return result;
}

// sq座標からrook縦の利きを返す
bitboard rook_attack_file(const int sq, const bitboard occ) {
	int part = (int)(sq > sq7i);
	int index = (occ.p[part] >> (slide[sq])) & 127;
	bitboard bb;
	bb.m = _mm_or_si128(lance_attack[black][sq][index].m, lance_attack[white][sq][index].m);
	return bb;
}

//rookの縦と横の利きbitboardを合成して返す
bitboard rook_attack(const int sq, const bitboard occ) {
	bitboard bb;
	bb.m = _mm_or_si128(rook_attack_rank(sq, occ).m, rook_attack_file(sq, occ).m);
	return bb;
}

//nw  ne
// \ /
// / \
//sw  se
void new_bishop_attacks() {
	int bishop_delta[4] = {
		delta_nw,	//左上
		delta_sw,	//左下
		delta_ne,	//右上
		delta_se	//右下
	};
	for (int f = file1; f <= file9; f += 1) {
		for (int r = rank1; r <= rank9; r += 1) {
			int sq = set_square(f, r);
			bitboard bishop_to_bb[4];
			for (int i = 0; i < 4; i += 1) {
				bitboard bb;
				bb.m = _mm_setzero_si128();
				int delta = bishop_delta[i];
				// 四方にそれぞれリーチを伸ばし壁に突き当たるまで歩進する
				int sq2 = sq;
				for (;;) {
					if ((delta == delta_nw || delta == delta_ne) && set_rank(sq2) == rank1) break;
					if ((delta == delta_sw || delta == delta_se) && set_rank(sq2) == rank9) break;
					if ((delta == delta_nw || delta == delta_sw) && set_file(sq2) == file9) break;
					if ((delta == delta_ne || delta == delta_se) && set_file(sq2) == file1) break;
					sq2 += delta;
					bb.m = _mm_or_si128(bb.m, mask_bb[sq2].m);
				}
				bishop_to_bb[i] = bb;
			}
			bishop_to_bb[2] = byte_reverse(bishop_to_bb[2]);
			bishop_to_bb[3] = byte_reverse(bishop_to_bb[3]);
			__m256i bm = _mm256_castsi128_si256(_mm_set_epi64x(bishop_to_bb[2].p[0], bishop_to_bb[0].p[0]));
			bishop_attack_to_mask[sq][0] = _mm256_inserti128_si256(bm, _mm_set_epi64x(bishop_to_bb[3].p[0], bishop_to_bb[1].p[0]), 1);
			bm = _mm256_castsi128_si256(_mm_set_epi64x(bishop_to_bb[2].p[1], bishop_to_bb[0].p[1]));
			bishop_attack_to_mask[sq][1] = _mm256_inserti128_si256(bm, _mm_set_epi64x(bishop_to_bb[3].p[1], bishop_to_bb[1].p[1]), 1);
		}
	}
}

// hi_in,lo_inの上位64bitを抜き出して１つの256bitレジスタ(hi_out)を構成する
// hi_in,lo_inの下位64bitを抜き出して１つの256bitレジスタ(lo_out)を構成する
// lo_in = [a3,a2 | a1,a0]	配列並びと同じように右側がMSB左がLSB 
// hi_in = [b3,b2 | b1,b0]
// hi_out = [a3,b3 | a1,b1]
// lo_out = [a2,b2 | a0,b0]
void unpack256(const __m256i hi_in, const __m256i lo_in, __m256i* hi_out, __m256i* lo_out) {
	*hi_out = _mm256_unpackhi_epi64(lo_in, hi_in);
	*lo_out = _mm256_unpacklo_epi64(lo_in, hi_in);
}

// -1を引くことによってsqからの利きを生成する 
void decrement256(const __m256i hi_in, const __m256i lo_in, __m256i* hi_out, __m256i* lo_out) {
	*hi_out = _mm256_add_epi64(hi_in, _mm256_cmpeq_epi64(lo_in, _mm256_setzero_si256()));
	*lo_out = _mm256_add_epi64(lo_in, _mm256_set1_epi64x(-1LL));
}

// byte単位でリバースする。bit単位ではない
__m256i byte_reverse256(__m256i bb) {
	const __m256i shuffle = _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	return _mm256_shuffle_epi8(bb, shuffle);
}

//保持している２つの盤面を重ね合わせた利きを返す
__m128i merge256(__m256i bb) {
	return _mm_or_si128(_mm256_castsi256_si128(bb), _mm256_extracti128_si256(bb, 1));
}

//　bishopのクロスの利き返す
bitboard bishop_attack(const int sq, const bitboard occ) {
	__m256i mask_lo = bishop_attack_to_mask[sq][0];
	__m256i mask_hi = bishop_attack_to_mask[sq][1];
	__m256i occ2 = _mm256_broadcastsi128_si256(occ.m);
	__m256i rocc2 = _mm256_broadcastsi128_si256(byte_reverse(occ).m);
	__m256i hi, lo, t1, t0;
	unpack256(rocc2, occ2, &hi, &lo);
	hi = _mm256_and_si256(hi, mask_hi);
	lo = _mm256_and_si256(lo, mask_lo);
	decrement256(hi, lo, &t1, &t0);
	t1 = _mm256_and_si256(_mm256_xor_si256(t1 , hi), mask_hi);
	t0 = _mm256_and_si256(_mm256_xor_si256(t0 , lo), mask_lo);
	unpack256(t1, t0, &hi, &lo);
	bitboard bb;
	bb.m = merge256(_mm256_or_si256(byte_reverse256(hi), lo));
	return bb;
}

void new_king_attacks() {
	for (int sq = sq1a; sq <= sq9i; sq += 1){
		king_attack[sq].m = _mm_or_si128(rook_attack(sq, all_one_bb).m, bishop_attack(sq, all_one_bb).m);
	}
}

void new_gold_attacks() {
	for (int c = black; c <= white; c += 1) {
		for (int sq = sq1a; sq <= sq9i; sq += 1) {
			gold_attack[c][sq].m = _mm_or_si128(_mm_and_si128(king_attack[sq].m, in_front_mask[c][set_rank(sq)].m), rook_attack(sq, all_one_bb).m);
		}
	}
}

void new_silver_attacks() {
	for (int c = black; c <= white; c += 1) {
		for (int sq = sq1a; sq <= sq9i; sq += 1) {
			silver_attack[c][sq].m = _mm_or_si128(_mm_and_si128(king_attack[sq].m, in_front_mask[c][set_rank(sq)].m), bishop_attack(sq, all_one_bb).m);
		}
	}
}

// 渡されたbitBoardを0から80までスキャンして最初bitが立っていたindexを返す
int first_one_from_nodelete(bitboard bb) {
	unsigned long sq;
	if (bb.p[0] != 0) {
		_BitScanForward64(&sq, bb.p[0]);
		return (int)sq;
	}
	if (bb.p[1] != 0) {
		_BitScanForward64(&sq, bb.p[1]);
		return (int)sq + 63;
	}
	return -1; // ビットが立っていない場合は-1を返す
}

// sqを中心に右斜め、左斜めの利きbitboardを返す
bitboard star_attacks(int sq) {	
	bitboard bb;
	bb.m = _mm_and_si128(silver_attack[black][sq].m, silver_attack[white][sq].m);
	return bb;
}

void new_knight_attacks() {
	for (int c = black; c <= white; c += 1) {
		for (int sq = sq1a; sq <= sq9i; sq += 1) {
			knight_attack[c][sq].m = _mm_setzero_si128();
			const bitboard bb = pawn_attack[c][sq];
			if (bb.p[0] > 0 || bb.p[1] > 0) {
				knight_attack[c][sq].m = _mm_and_si128(star_attacks(first_one_from_nodelete(bb)).m, in_front_mask[c][set_rank(sq)].m);
			}
		}
	}
}

void new_pawn_attacks() {
	for (int c = black; c <= white; c += 1) {
		for (int sq = sq1a; sq <= sq9i; sq += 1) {
			pawn_attack[c][sq].m = _mm_xor_si128(silver_attack[c][sq].m,bishop_attack(sq,all_one_bb).m);
		}
	}
}

//飛、角の利きの間にあるbitboardをかえす
// sq1 x x x x x sq2 (sq1,sq2のbitは返さない) =>direct_cross
// sq1  => direct_cross 
//  x                            
//  x                             
// sq2       

//sq1   => diect_diag          sq1 =>direct_diag
//   x                        x
//    x                      x
//     x                    x
//      sq2              sq2
void new_between_bb() {
	for (int sq1 = sq1a; sq1 <= sq9i; sq1 += 1) {
		for (int sq2 = sq1a; sq2 <= sq9i; sq2 += 1) {
			between_bb[sq1][sq2].m = _mm_setzero_si128();
			if (sq1 == sq2) {
				continue;
			}
			const int direct = square_relation(sq1, sq2);
			if (direct & direct_cross) {
				between_bb[sq1][sq2].m = _mm_and_si128(rook_attack(sq1, mask_bb[sq2]).m, rook_attack(sq2, mask_bb[sq1]).m);
			}
			else if (direct & direct_diag) {
				between_bb[sq1][sq2].m = _mm_and_si128(bishop_attack(sq1, mask_bb[sq2]).m, bishop_attack(sq2, mask_bb[sq1]).m);
			}
		}
	}
}

bitboard set_board(const int64_t idx0,const int64_t idx1) {
	bitboard bb;
	bb.p[0] = idx0;
	bb.p[1] = idx1;
	return bb;
}

//引数bbのsq座標にビットが立っていればtrueを返す
bool is_biton(const int sq, const bitboard bb) {
	return !(bool)_mm_testz_si128(mask_bb[sq].m,bb.m);
}

// 引数bbのsq座標にビットを立てる
void set_biton(const int sq, bitboard* bb) {
	bb->m = _mm_or_si128(bb->m, mask_bb[sq].m);
}

//debug用のbitboard表示
void print_bitboard(bitboard bb, const char* msg) {
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
