#include <stdio.h>
#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "misc.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

const BitBoard SQUARE_BB[SquareNum] = {		//bitboard index		board square
	BitBoard(1ULL << 0, 0),					//0						I9			
	BitBoard(1ULL << 1, 0),					//1						I8
	BitBoard(1ULL << 2, 0),					//2						I7
	BitBoard(1ULL << 3, 0),					//3						I6
	BitBoard(1ULL << 4, 0),					//4						I5
	BitBoard(1ULL << 5, 0),					//5						I4
	BitBoard(1ULL << 6, 0),					//6						I3
	BitBoard(1ULL << 7, 0),					//7						I2
	BitBoard(1ULL << 8, 0),					//8						I1
	BitBoard(1ULL << 9, 0),					//9						H9
	BitBoard(1ULL << 10, 0),				//10					H8
	BitBoard(1ULL << 11, 0),				//11					H7
	BitBoard(1ULL << 12, 0),				//12					H6
	BitBoard(1ULL << 13, 0),				//13					H5
	BitBoard(1ULL << 14, 0),				//14					H4
	BitBoard(1ULL << 15, 0),				//15					H3
	BitBoard(1ULL << 16, 0),				//16					H2
	BitBoard(1ULL << 17, 0),				//17					H1
	BitBoard(1ULL << 18, 0),				//18					G9
	BitBoard(1ULL << 19, 0),				//19					G8
	BitBoard(1ULL << 20, 0),				//20					G7
	BitBoard(1ULL << 21, 0),				//21					G6
	BitBoard(1ULL << 22, 0),				//22					G5
	BitBoard(1ULL << 23, 0),				//23					G4
	BitBoard(1ULL << 24, 0),				//24					G3
	BitBoard(1ULL << 25, 0),				//25					G2
	BitBoard(1ULL << 26, 0),				//26					G1
	BitBoard(1ULL << 27, 0),				//27					F9
	BitBoard(1ULL << 28, 0),				//28					F8
	BitBoard(1ULL << 29, 0),				//29					F7
	BitBoard(1ULL << 30, 0),				//30					F6
	BitBoard(1ULL << 31, 0),				//31					F5
	BitBoard(1ULL << 32, 0),				//32					F4
	BitBoard(1ULL << 33, 0),				//33					F3
	BitBoard(1ULL << 34, 0),				//34					F2
	BitBoard(1ULL << 35, 0),				//35					F1
	BitBoard(1ULL << 36, 0),				//36					E9
	BitBoard(1ULL << 37, 0),				//37					E8
	BitBoard(1ULL << 38, 0),				//38					E7
	BitBoard(1ULL << 39, 0),				//39					E6
	BitBoard(1ULL << 40, 0),				//40					E5
	BitBoard(1ULL << 41, 0),				//41					E4
	BitBoard(1ULL << 42, 0),				//42					E3
	BitBoard(1ULL << 43, 0),				//43					E2
	BitBoard(1ULL << 44, 0),				//44					E1
	BitBoard(1ULL << 45, 0),				//45					D9
	BitBoard(1ULL << 46, 0),				//46					D8
	BitBoard(1ULL << 47, 0),				//47					D7
	BitBoard(1ULL << 48, 0),				//48					D6
	BitBoard(1ULL << 49, 0),				//49					D5
	BitBoard(1ULL << 50, 0),				//50					D4
	BitBoard(1ULL << 51, 0),				//51					D3
	BitBoard(1ULL << 52, 0),				//52					D2
	BitBoard(1ULL << 53, 0),				//53					D1
	BitBoard(1ULL << 54, 0),				//54					C9
	BitBoard(1ULL << 55, 0),				//55					C8
	BitBoard(1ULL << 56, 0),				//56					C7
	BitBoard(1ULL << 57, 0),				//57					C6
	BitBoard(1ULL << 58, 0),				//58					C5
	BitBoard(1ULL << 59, 0),				//59					C4
	BitBoard(1ULL << 60, 0),				//60					C3
	BitBoard(1ULL << 61, 0),				//61					C2
	BitBoard(1ULL << 62, 0),				//62					C1
	BitBoard(0, 1ULL << 0),					//63					B9
	BitBoard(0, 1ULL << 1),					//64					B8
	BitBoard(0, 1ULL << 2),					//65					B7
	BitBoard(0, 1ULL << 3),					//66					B6
	BitBoard(0, 1ULL << 4),					//67					B5
	BitBoard(0, 1ULL << 5),					//68					B4
	BitBoard(0, 1ULL << 6),					//69					B3
	BitBoard(0, 1ULL << 7),					//70					B2
	BitBoard(0, 1ULL << 8),					//71					B1
	BitBoard(0, 1ULL << 9),					//72					A9
	BitBoard(0, 1ULL << 10),				//73					A8
	BitBoard(0, 1ULL << 11),				//74					A7
	BitBoard(0, 1ULL << 12),				//75					A6
	BitBoard(0, 1ULL << 13),				//76					A5
	BitBoard(0, 1ULL << 14),				//77					A4
	BitBoard(0, 1ULL << 15),				//78					A3
	BitBoard(0, 1ULL << 16),				//79					A2
	BitBoard(0, 1ULL << 17)					//80					A1
};
//局所定数宣言・定義
static const int BISHOP_ATTACK_NUM[SquareNum] = {
	7, 6, 6, 6, 6, 6, 6, 6, 7,
	6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 8, 8, 8, 8, 8, 6, 6,
	6, 6, 8, 10, 10, 10, 8, 6, 6,
	6, 6, 8, 10, 12, 10, 8, 6, 6,
	6, 6, 8, 10, 10, 10, 8, 6, 6,
	6, 6, 8, 8, 8, 8, 8, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 6, 6, 6, 6, 6, 6, 6, 7
};
static const int ROOK_ATTACK_NUM[SquareNum] = {
	14, 13, 13, 13, 13, 13, 13, 13, 14,
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	14, 13, 13, 13, 13, 13, 13, 13, 14,
};
static const int LANCE_ATTACK_NUM[ColorNum][SquareNum] = {
	{	//black
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8,
		0, 1, 2, 3, 4, 5, 6, 7, 8
	},
	{	//white
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0,
		8, 7, 6, 5, 4, 3, 2, 1, 0
	}
};
//lanceはoffsetする必要がないがoccupied_to_index()関数の仕様のためのダミー
static const int LANCE_OFFSET[SquareNum] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int BISHOP_OFFSET[SquareNum] = {
	6, 6, 6, 6, 6, 6, 6, 6, 6,
	5, 5, 6, 6, 6, 6, 6, 5, 5,
	5, 5, 7, 8, 8, 8, 7, 5, 5,
	5, 5, 7, 9, 10, 9, 7, 5, 5,
	5, 5, 7, 9, 10, 9, 7, 5, 5,
	5, 5, 7, 8, 8, 8, 7, 5, 5,
	5, 5, 6, 6, 6, 6, 6, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 5, 4, 4, 4, 4, 4, 5, 6
};
static const int ROOK_OFFSET[SquareNum] = {
	13, 12, 12, 12, 12, 12, 12, 12, 13,
	12, 11, 11, 11, 11, 11, 11, 11, 12,
	12, 11, 11, 11, 11, 11, 11, 11, 12,
	12, 11, 11, 11, 11, 11, 11, 11, 12,
	12, 11, 11, 11, 11, 11, 11, 11, 12,
	12, 11, 11, 11, 11, 11, 11, 11, 12,
	12, 11, 11, 11, 11, 11, 11, 11, 12,
	6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6,
};
//局所変数宣言
static BitBoard bishop_attack[20224];
static BitBoard bishop_mask[81];
static int bishop_attack_index[81];
static BitBoard rook_attack[495616];
static BitBoard rook_mask[81];
static int rook_attack_index[81];
static BitBoard lance_attack[ColorNum][4599];
static BitBoard lance_mask[ColorNum][81];
static int lance_attack_index[ColorNum][81];

//局所関数宣言
static BitBoard sliding_attack(Square sq, BitBoard occ, bool is_bishop);
static BitBoard one_direction_attack(Square square, BitBoard occ, Color c);
static BitBoard index_to_occupied(int index, int attack_num, const BitBoard mask);
static int occupied_to_index(const BitBoard& occ, const BitBoard& mask, const int& offset);
static void init_lance_attacks(Color c);
static void init_bishop_attacks();
static void init_rook_attacks();
static void init_king_attacks();

//bishop,rookの利きを全ての方向にbitboardに記録する。occに他の駒があればそこで停止。盤の端も記録する
static BitBoard sliding_attack(Square square, BitBoard occ,bool is_bishop)
{
	SquareDelta deltas[2][4] = { { DeltaN, DeltaS, DeltaE, DeltaW }, { DeltaNE, DeltaSE, DeltaSW, DeltaNW } };
	BitBoard bb(0x00, 0x00);
	int delta,sq;

	for (int i = 0; i < 4; i++){
		delta = int(deltas[is_bishop][i]);
		for (sq = square + delta; is_square(Square(sq)) && abs(make_rank(Square(sq - delta)) - make_rank(Square(sq))) <= 1; sq += delta){
			bb.set_bit(Square(sq));
			if (occ.is_bit_on(Square(sq))){
				break;
			}
		}
	}
	return bb;
}
//lanceの利きをbitboardに記録する。occに他の駒があればそこで停止。盤の端も記録する,カラーを指定できる
static BitBoard one_direction_attack(Square square, BitBoard occ, Color c)
{
	SquareDelta deltas[2] = { DeltaN, DeltaS };
	BitBoard bb(0x00, 0x00);
	int delta = deltas[c];
	int sq;

	for (sq = square + delta; is_square(Square(sq)) && abs(make_rank(Square(sq - delta)) - make_rank(Square(sq))) <= 1; sq += delta){
		bb.set_bit(Square(sq));
		if (occ.is_bit_on(Square(sq))){
			break;
		}
	}
	return bb;
}

static BitBoard index_to_occupied(int index, int attack_num, const BitBoard mask)
{
	BitBoard tmp_bb = mask;
	BitBoard bb(0x00, 0x00);

	for (int i = 0; i < attack_num; i++){
		const Square sq = tmp_bb.first_one();
		if (index & (1 << i)){
			bb.set_bit(sq);
		}
	}
	return bb;
}

static int occupied_to_index(const BitBoard& occ, const BitBoard& mask,const int& offset)
{
	return static_cast<int>(_pext_u64(occ.p(0), mask.p(0)) | (_pext_u64(occ.p(1), mask.p(1)) << offset));
}

BitBoard BitBoardns::make_lance_attack(const Square sq, const BitBoard& occ,Color c)
{
	const BitBoard line(occ & lance_mask[c][sq]);

	return lance_attack[c][lance_attack_index[c][sq] + occupied_to_index(line, lance_mask[c][sq], LANCE_OFFSET[sq])];
}

BitBoard BitBoardns::make_bishop_attack(const Square sq, const BitBoard& occ)
{
	const BitBoard line(occ & bishop_mask[sq]); 

	return bishop_attack[bishop_attack_index[sq] + occupied_to_index(line,bishop_mask[sq],BISHOP_OFFSET[sq])];
}

BitBoard BitBoardns::make_rook_attack(const Square sq, const BitBoard& occ)
{
	const BitBoard line(occ & rook_mask[sq]);

	return rook_attack[rook_attack_index[sq] + occupied_to_index(line, rook_mask[sq],ROOK_OFFSET[sq])];
}

static void init_lance_attacks(Color c)
{
	int index = 0;
	BitBoard zero_bb(0x00, 0x00);
	BitBoard occ[1 << 8];

	for (int sq = I9; sq < SquareNum; sq++){
		lance_mask[c][sq] = one_direction_attack(Square(sq), zero_bb, c);
		lance_mask[c][sq] &= ~BitBoardns::RANK_MASK[Rank1];		//white側のとき盤の端を消す
		lance_mask[c][sq] &= ~BitBoardns::RANK_MASK[Rank9];		//black側のとき盤の端を消す
		lance_attack_index[c][sq] = index;
		const int attack_num = LANCE_ATTACK_NUM[c][sq];
		for (int i = 0; i < (1 << attack_num); i++){
			occ[i] = index_to_occupied(i, attack_num, lance_mask[c][sq]);
			lance_attack[c][index + occupied_to_index(occ[i] & lance_mask[c][sq], lance_mask[c][sq], LANCE_OFFSET[sq])] = one_direction_attack(Square(sq), occ[i], c);
		}
		index += 1 << LANCE_ATTACK_NUM[c][sq];
	}
}

static void init_bishop_attacks()
{
	int index = 0;
	BitBoard zero_bb(0x00, 0x00);
	BitBoard occ[1 << 14];

	for (int sq = I9; sq < SquareNum; sq++){
		bishop_mask[sq] = sliding_attack(Square(sq), zero_bb, true);
		bishop_mask[sq] &= ~(BitBoardns::RANK_MASK[Rank1] | BitBoardns::RANK_MASK[Rank9] | BitBoardns::FILE_MASK[FileA] | BitBoardns::FILE_MASK[FileI]);	//board edgeを削っている
		bishop_attack_index[sq] = index;
		const int attack_num = BISHOP_ATTACK_NUM[sq];
		for (int i = 0; i < (1 << attack_num); i++){
			occ[i] = index_to_occupied(i, attack_num, bishop_mask[sq]);
			bishop_attack[index + occupied_to_index(occ[i] & bishop_mask[sq], bishop_mask[sq],BISHOP_OFFSET[sq])] = sliding_attack(Square(sq), occ[i], true);
		}
		index += 1 << BISHOP_ATTACK_NUM[sq];
	}
}

static void init_rook_attacks()
{
	int index = 0;
	BitBoard alloff_bb(0x00, 0x00);
	BitBoard occ[1 << 14];

	for (int sq = I9; sq < SquareNum; sq++){
		rook_mask[sq] = sliding_attack(Square(sq), alloff_bb, false);
		if (SQUARE_FILE[sq] != FileA){ rook_mask[sq] &= ~BitBoardns::FILE_MASK[FileA]; }	//board edgeを削っている
		if (SQUARE_FILE[sq] != FileI){ rook_mask[sq] &= ~BitBoardns::FILE_MASK[FileI]; }
		if (SQUARE_RANK[sq] != Rank1){ rook_mask[sq] &= ~BitBoardns::RANK_MASK[Rank1]; }
		if (SQUARE_RANK[sq] != Rank9){ rook_mask[sq] &= ~BitBoardns::RANK_MASK[Rank9]; }
		rook_attack_index[sq] = index;
		const int attack_num = ROOK_ATTACK_NUM[sq];
		for (int i = 0; i < (1 << attack_num); i++){
			occ[i] = index_to_occupied(i, attack_num, rook_mask[sq]);
			rook_attack[index + occupied_to_index(occ[i] & rook_mask[sq], rook_mask[sq],ROOK_OFFSET[sq])] = sliding_attack(Square(sq),occ[i], false);
		}
		index += 1 << ROOK_ATTACK_NUM[sq];
	}
}
static void init_king_attacks()
{
	/*
	BitBoard allon_bb(0x7FFFFFFFFFFFFFFF, 0x000000000003FFFF);
	for ()
	*/
}

void BitBoardns::init()
{
	init_lance_attacks(Black);
	init_lance_attacks(White);
	init_bishop_attacks();
	init_rook_attacks();
	init_king_attacks();
}

void BitBoardns::print(BitBoard &bb)
{
	int sq;
	__m128i m = _mm_set_epi64x((bb.p(1)), bb.p(0));

	sync_cout;
	std::cout << "-+--+--+--+--+--+--+--+--+--+" << std::endl;
	std::cout << "   A  B  C  D  E  F  G  H  I " << std::endl;
	for (int r = Rank9; r < RankNum; r++){
		std::cout << (9 - r) << " ";
		for (int f = FileA; FileI <= f; f--){
			sq = make_square(f, r);
			__m128i sq_m = _mm_set_epi64x((SQUARE_BB[sq].p(1)), SQUARE_BB[sq].p(0));
			std::cout << (!(_mm_testz_si128(m, sq_m)) ? " *" : " .") << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "-+--+--+--+--+--+--+--+--+--+" << sync_endl;
}

#ifdef _DEBUG
TEST(bitboard, lance_attack)
{
	//lance_attack配列はlance_mask配列,lance_attack_index配列から作られる最終成果物で
	//局面bitboardの指定した座標にlanceをおいた時得られる利きbitboardを持っている
	//配列数は4,599個あるので全数テストするのは困難であるのでサンプリングしてテストする
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	using BitBoardns::make_lance_attack;
	using BitBoardns::print;
	int sq;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	BitBoard ack;

	BitBoardns::init();
	//lance black
	sq = C9;
	ack = make_lance_attack(Square(sq), occ,Black);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = I8;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0x01);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0x18000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0x100000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0xC000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G4;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0x400000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A3;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x4000);
	sq = C2;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0x1800000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F1;
	ack = make_lance_attack(Square(sq), occ, Black);
	EXPECT_EQ(ack.p(0), 0x7C0000000);
	EXPECT_EQ(ack.p(1), 0x00);
	//lance white
	sq = C9;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x180000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = I8;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x04);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x40000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x400000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x20000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G4;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x1000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A3;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x30000);
	sq = C2;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x4000000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F1;
	ack = make_lance_attack(Square(sq), occ, White);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(bitboard, one_direction_attack)
{
	BitBoard occ(0x00, 0x00), bb;
	using BitBoardns::print;

	//lance black
	bb = one_direction_attack(I1, occ, Black);
	EXPECT_EQ(bb.p(0), 0xFF);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(H2, occ, Black);
	EXPECT_EQ(bb.p(0), 0xFE00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(G3, occ, Black);
	EXPECT_EQ(bb.p(0), 0xFC0000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(F4, occ, Black);
	EXPECT_EQ(bb.p(0), 0xF8000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(E5, occ, Black);
	EXPECT_EQ(bb.p(0), 0xF000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(D6, occ, Black);
	EXPECT_EQ(bb.p(0), 0xE00000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(C7, occ, Black);
	EXPECT_EQ(bb.p(0), 0xC0000000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(B8, occ, Black);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x01);
	bb = one_direction_attack(A9, occ, Black);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(I9, occ, Black);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(H8, occ, Black);
	EXPECT_EQ(bb.p(0), 0x200);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(G7, occ, Black);
	EXPECT_EQ(bb.p(0), 0xC0000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(F6, occ, Black);
	EXPECT_EQ(bb.p(0), 0x38000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(E5, occ, Black);
	EXPECT_EQ(bb.p(0), 0xF000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(D4, occ, Black);
	EXPECT_EQ(bb.p(0), 0x3E00000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(C3, occ, Black);
	EXPECT_EQ(bb.p(0), 0xFC0000000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(B2, occ, Black);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x7F);
	bb = one_direction_attack(A1, occ, Black);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x1FE00);
	//lance white
	bb = one_direction_attack(I9, occ, White);
	EXPECT_EQ(bb.p(0), 0x1FE);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(H8, occ, White);
	EXPECT_EQ(bb.p(0), 0x3F800);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(G7, occ, White);
	EXPECT_EQ(bb.p(0), 0x7E00000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(F6, occ, White);
	EXPECT_EQ(bb.p(0), 0xF80000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(E5, occ, White);
	EXPECT_EQ(bb.p(0), 0x1E0000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(D4, occ, White);
	EXPECT_EQ(bb.p(0), 0x38000000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(C3, occ, White);
	EXPECT_EQ(bb.p(0), 0x6000000000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(B2, occ, White);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x100);
	bb = one_direction_attack(A1, occ, White);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(I1, occ, White);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(H2, occ, White);
	EXPECT_EQ(bb.p(0), 0x20000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(G3, occ, White);
	EXPECT_EQ(bb.p(0), 0x6000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(F4, occ, White);
	EXPECT_EQ(bb.p(0), 0xE00000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(E5, occ, White);
	EXPECT_EQ(bb.p(0), 0x1E0000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(D6, occ, White);
	EXPECT_EQ(bb.p(0), 0x3E000000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(C7, occ, White);
	EXPECT_EQ(bb.p(0), 0x7E00000000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = one_direction_attack(B8, occ, White);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x1FC);
	bb = one_direction_attack(A9, occ, White);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x3FC00);
}
TEST(bitboard, rook_attack)
{
	//rook_attack配列はrook_mask配列,rook_attack_index配列から作られる最終成果物で
	//局面bitboardの指定した座標にrookをおいた時得られる利きbitboardを持っている
	//配列数は495,616個あるので全数テストするのは困難であるのでサンプリングしてテストする
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	using BitBoardns::make_rook_attack;
	using BitBoardns::print;
	int sq;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	BitBoard ack;

	BitBoardns::init();
	sq = C9;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x180200000000000);
	EXPECT_EQ(ack.p(1), 0x01);
	sq = I8;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80405);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x4058100000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x501000000000000);
	EXPECT_EQ(ack.p(1), 0x1008);
	sq = E5;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x40202C080400000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G4;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x20101404020);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A3;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x34040);
	sq = C2;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x5810080000000000);
	EXPECT_EQ(ack.p(1), 0x10080);
	sq = F1;
	ack = make_rook_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x40201007C4000000);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(bitboard, bishop_attack)
{
	//bishop_attack配列はbishop_mask配列,bishop_attack_index配列から作られる最終成果物で
	//局面bitboardの指定した座標にbishopをおいた時得られる利きbitboardを持っている
	//配列数は20,224個あるので全数テストするのは困難であるのでサンプリングしてテストする
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	/*using BitBoardns::index_to_occupied;*/
	/*using BitBoardns::occupied_to_index;*/
	using BitBoardns::make_bishop_attack;
	using BitBoardns::print;
	int sq;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	BitBoard ack;

	BitBoardns::init();
	sq = C9;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x400000000000);
	EXPECT_EQ(ack.p(1), 0x802);
	sq = I8;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x20080200A00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80200A000282020);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x2822000000000);
	EXPECT_EQ(ack.p(1), 0x414);
	sq = E5;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x1005000141000000);
	EXPECT_EQ(ack.p(1), 0x20080);
	sq = G4;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x8088828000A088);
	EXPECT_EQ(ack.p(1), 0x01);
	sq = A3;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x4000000000000000);
	EXPECT_EQ(ack.p(1), 0xA0);
	sq = C2;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x28000000000000);
	EXPECT_EQ(ack.p(1), 0x140);
	sq = F1;
	ack = make_bishop_attack(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80002008020);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(bitboard, occupied_to_index)
{
	//occupied_to_indexは局面bitboardを見てそれに対応するindexを返す関数
	//BMI2命令を使っている。index_to_occupied()の逆関数
	/*using BitBoardns::index_to_occupied;*/
	/*using BitBoardns::occupied_to_index;*/
	using BitBoardns::print;
	BitBoard occ;
	uint64_t index;

	BitBoardns::init();
	//bishop
	for (int sq = I9; sq < SquareNum; sq++){
		for (int i = 0; i < (1 << BISHOP_ATTACK_NUM[sq]); i++){
			occ = index_to_occupied(i, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
			index = occupied_to_index(occ, bishop_mask[sq],BISHOP_OFFSET[sq]);
			EXPECT_EQ(i, index);
		}
	}
	//rook
	for (int sq = I9; sq < SquareNum; sq++){
		for (int i = 0; i < (1 << ROOK_ATTACK_NUM[sq]); i++){
			occ = index_to_occupied(i, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
			index = occupied_to_index(occ, rook_mask[sq], ROOK_OFFSET[sq]);
			EXPECT_EQ(i, index);
		}
	}
}

TEST(bitboard, index_to_occupied)
{
	//引数：指定したindex値(i変数)と利きを利かしている駒がその座標で可能な最大利き数(attack_num)、
	//その駒がその座標から利かせられる利きbitboard(bishop_mask[sq])
	//index_to_occupiedは指定した座標から、指定した駒種の利き座標に、他の駒のがいるパターンを生成しそのbitboardを返す
	//例　３座標しか利かない香車があったとするとindex=0は３座標に一切他の駒がいないパターン 000を生成
	//index=1は001,index=2は010,index=3は011index=4は100となる全パターンを書き並べると下のようになる
	//index  occパターン
	//    0  000
	//    1  001
	//    2  010
	//    3  011
	//    4  100
	//    5  101
	//    6  110
	//    7  111
	//occすべてのパターンが漏れなくダブりなく生成されるこのoccパターンに対して
	//とび駒がどこまで利きを伸ばせるかを設定しているのがinit_bishop_attacks、init_rook_attacks関数
	//occ[i] = index_to_occupied(i, attack_num, bishop_mask[sq]);
	/*using BitBoardns::index_to_occupied;*/
	using BitBoardns::print;
	BitBoard occ;
	Square sq;

	BitBoardns::init();
	//bishop
	//I9 attack_numは７箇所、7bitあるのでパターンとしては127ケースある、全数テストするのは困難なため抜き取りでテストする
	sq = I9;
	occ = index_to_occupied(0, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x00);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(1, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x400);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(2, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x100000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x100400);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x40000000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(5, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x40000400);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(6, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x40100000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x40100400);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(109, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x1000010040000400);
	EXPECT_EQ(occ.p(1), 0x80);
	sq = I4;
	occ = index_to_occupied(1, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x2000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0xA000);
	EXPECT_EQ(occ.p(1), 0x00);	
	occ = index_to_occupied(4, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x200000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x20A000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(63, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x202220A000);
	EXPECT_EQ(occ.p(1), 0x00);
	sq = E5;
	occ = index_to_occupied(1, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x400);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x10400);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x100000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x110400);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4095, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x1105000141110400);
	EXPECT_EQ(occ.p(1), 0x82);
	sq = F4;
	occ = index_to_occupied(1, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x1000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x11000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x400000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x411000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(1023, BISHOP_ATTACK_NUM[sq], bishop_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x111050001411000);
	EXPECT_EQ(occ.p(1), 0x02);
	//rook
	sq = I9;
	occ = index_to_occupied(0, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x00);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(1, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x02);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(2, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x04);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x06);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x08);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(5, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x0A);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(6, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x0C);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x0E);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(16383, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x402010080402FE);
	EXPECT_EQ(occ.p(1), 0x01);
	sq = I4;
	occ = index_to_occupied(1, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x02);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x06);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x08);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x0E);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(8191, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x8040201008040DE);
	EXPECT_EQ(occ.p(1), 0x20);
	sq = E5;
	occ = index_to_occupied(1, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x2000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x402000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x80000000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x80402000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4095, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x4020EE080402000);
	EXPECT_EQ(occ.p(1), 0x10);
	sq = F5;
	occ = index_to_occupied(1, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x2000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(3, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x402000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x10000000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(7, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x10402000);
	EXPECT_EQ(occ.p(1), 0x00);
	occ = index_to_occupied(4095, ROOK_ATTACK_NUM[sq], rook_mask[sq]);
	EXPECT_EQ(occ.p(0), 0x402010770402000);
	EXPECT_EQ(occ.p(1), 0x10);
}
TEST(bitboard, sliding_attack)
{
	BitBoard occ(0x00,0x00);
	BitBoard bb(0x00, 0x00);
	using BitBoardns::print;

	//bishop
	bb = sliding_attack(I9,occ,true);
	EXPECT_EQ(bb.p(0), 0x1004010040100400);
	EXPECT_EQ(bb.p(1), 0x20080);
	bb = sliding_attack(I4, occ, true);
	EXPECT_EQ(bb.p(0), 0x20282220A000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = sliding_attack(I1, occ, true);
	EXPECT_EQ(bb.p(0), 0x101010101010000);
	EXPECT_EQ(bb.p(1), 0x202);
	bb = sliding_attack(G9, occ, true);
	EXPECT_EQ(bb.p(0), 0x401004010000404);
	EXPECT_EQ(bb.p(1), 0x8020);
	bb = sliding_attack(A9, occ, true);
	EXPECT_EQ(bb.p(0), 0x101010101010100);
	EXPECT_EQ(bb.p(1), 0x02);
	bb = sliding_attack(A7, occ, true);
	EXPECT_EQ(bb.p(0), 0x444040404000000);
	EXPECT_EQ(bb.p(1), 0x0A);
	bb = sliding_attack(A1, occ, true);
	EXPECT_EQ(bb.p(0), 0x1004010040100401);
	EXPECT_EQ(bb.p(1), 0x80);
	bb = sliding_attack(E7, occ, true);
	EXPECT_EQ(bb.p(0), 0x441400050444040);
	EXPECT_EQ(bb.p(1), 0x8020);
	bb = sliding_attack(E5, occ, true);
	EXPECT_EQ(bb.p(0), 0x1105000141110501);
	EXPECT_EQ(bb.p(1), 0x20282);
	bb = sliding_attack(F4, occ, true);
	EXPECT_EQ(bb.p(0), 0x4111050001411104);
	EXPECT_EQ(bb.p(1), 0x202);
	//rook
	bb = sliding_attack(I9, occ, false);
	EXPECT_EQ(bb.p(0), 0x402010080403FE);
	EXPECT_EQ(bb.p(1), 0x201);
	bb = sliding_attack(I4, occ, false);
	EXPECT_EQ(bb.p(0), 0x8040201008041DF);
	EXPECT_EQ(bb.p(1), 0x4020);
	bb = sliding_attack(I1, occ, false);
	EXPECT_EQ(bb.p(0), 0x40201008040200FF);
	EXPECT_EQ(bb.p(1), 0x20100);
	bb = sliding_attack(G9, occ, false);
	EXPECT_EQ(bb.p(0), 0x4020100FF80201);
	EXPECT_EQ(bb.p(1), 0x201);
	bb = sliding_attack(A9, occ, false);
	EXPECT_EQ(bb.p(0), 0x40201008040201);
	EXPECT_EQ(bb.p(1), 0x3FC01);
	bb = sliding_attack(A7, occ, false);
	EXPECT_EQ(bb.p(0), 0x100804020100804);
	EXPECT_EQ(bb.p(1), 0x3F604);
	bb = sliding_attack(A1, occ, false);
	EXPECT_EQ(bb.p(0), 0x4020100804020100);
	EXPECT_EQ(bb.p(1), 0x1FF00);
	bb = sliding_attack(E7, occ, false);
	EXPECT_EQ(bb.p(0), 0x1009FB020100804);
	EXPECT_EQ(bb.p(1), 0x804);
	bb = sliding_attack(E5, occ, false);
	EXPECT_EQ(bb.p(0), 0x4021EF080402010);
	EXPECT_EQ(bb.p(1), 0x2010);
	bb = sliding_attack(F4, occ, false);
	EXPECT_EQ(bb.p(0), 0x804020EF8804020);
	EXPECT_EQ(bb.p(1), 0x4020);
}
TEST(bitboard, in_front_of_rank)
{
	using namespace BitBoardns;

	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank9].pop_count() == 0);
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank8].pop_count() == 9);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank8].is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank7].pop_count() == 9 * 2);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank7].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank7].is_bit_on(Square(sq + 1)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank6].pop_count() == 9 * 3);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank6].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank6].is_bit_on(Square(sq + 1)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank6].is_bit_on(Square(sq + 2)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank5].pop_count() == 9 * 4);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank5].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank5].is_bit_on(Square(sq + 1)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank5].is_bit_on(Square(sq + 2)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank5].is_bit_on(Square(sq + 3)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank4].pop_count() == 9 * 5);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank4].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank4].is_bit_on(Square(sq + 1)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank4].is_bit_on(Square(sq + 2)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank4].is_bit_on(Square(sq + 3)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank4].is_bit_on(Square(sq + 4)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank3].pop_count() == 9 * 6);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank3].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank3].is_bit_on(Square(sq + 1)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank3].is_bit_on(Square(sq + 2)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank3].is_bit_on(Square(sq + 3)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank3].is_bit_on(Square(sq + 4)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank3].is_bit_on(Square(sq + 5)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].pop_count() == 9 * 7);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].is_bit_on(Square(sq + 1)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].is_bit_on(Square(sq + 2)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].is_bit_on(Square(sq + 3)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].is_bit_on(Square(sq + 4)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].is_bit_on(Square(sq + 5)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank2].is_bit_on(Square(sq + 6)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].pop_count() == 9 * 8);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq + 1)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq + 2)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq + 3)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq + 4)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq + 5)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq + 6)));
		EXPECT_TRUE(IN_FRONT_MASK[Black][Rank1].is_bit_on(Square(sq + 7)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank1].pop_count() == 0);
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank2].pop_count() == 9);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank2].is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank3].pop_count() == 9 * 2);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank3].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank3].is_bit_on(Square(sq - 1)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank4].pop_count() == 9 * 3);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank4].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank4].is_bit_on(Square(sq - 1)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank4].is_bit_on(Square(sq - 2)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank5].pop_count() == 9 * 4);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank5].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank5].is_bit_on(Square(sq - 1)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank5].is_bit_on(Square(sq - 2)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank5].is_bit_on(Square(sq - 3)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank6].pop_count() == 9 * 5);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank6].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank6].is_bit_on(Square(sq - 1)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank6].is_bit_on(Square(sq - 2)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank6].is_bit_on(Square(sq - 3)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank6].is_bit_on(Square(sq - 4)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank7].pop_count() == 9 * 6);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank7].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank7].is_bit_on(Square(sq - 1)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank7].is_bit_on(Square(sq - 2)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank7].is_bit_on(Square(sq - 3)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank7].is_bit_on(Square(sq - 4)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank7].is_bit_on(Square(sq - 5)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].pop_count() == 9 * 7);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].is_bit_on(Square(sq - 1)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].is_bit_on(Square(sq - 2)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].is_bit_on(Square(sq - 3)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].is_bit_on(Square(sq - 4)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].is_bit_on(Square(sq - 5)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank8].is_bit_on(Square(sq - 6)));
	}
	EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].pop_count() == 9 * 8);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq - 1)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq - 2)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq - 3)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq - 4)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq - 5)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq - 6)));
		EXPECT_TRUE(IN_FRONT_MASK[White][Rank9].is_bit_on(Square(sq - 7)));
	}
}
TEST(bitboard, rank_mask)
{
	using namespace BitBoardns;

	EXPECT_TRUE(RANK_9_MASK.pop_count() == 9);
	for (int sq = I9; sq <= A9; sq += 9){
		EXPECT_TRUE(RANK_9_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_8_MASK.pop_count() == 9);
	for (int sq = I8; sq <= A8; sq += 9){
		EXPECT_TRUE(RANK_8_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_7_MASK.pop_count() == 9);
	for (int sq = I7; sq <= A7; sq += 9){
		EXPECT_TRUE(RANK_7_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_6_MASK.pop_count() == 9);
	for (int sq = I6; sq <= A6; sq += 9){
		EXPECT_TRUE(RANK_6_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_5_MASK.pop_count() == 9);
	for (int sq = I5; sq <= A5; sq += 9){
		EXPECT_TRUE(RANK_5_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_4_MASK.pop_count() == 9);
	for (int sq = I4; sq <= A4; sq += 9){
		EXPECT_TRUE(RANK_4_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_3_MASK.pop_count() == 9);
	for (int sq = I3; sq <= A3; sq += 9){
		EXPECT_TRUE(RANK_3_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_2_MASK.pop_count() == 9);
	for (int sq = I2; sq <= A2; sq += 9){
		EXPECT_TRUE(RANK_2_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(RANK_1_MASK.pop_count() == 9);
	for (int sq = I1; sq <= A1; sq += 9){
		EXPECT_TRUE(RANK_1_MASK.is_bit_on(Square(sq)));
	}
}
TEST(bitboard, file_mask)
{
	using namespace BitBoardns;

	EXPECT_TRUE(FILE_A_MASK.pop_count() == 9);
	for (int sq = A9; sq <= A1; sq++){
		EXPECT_TRUE(FILE_A_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(FILE_B_MASK.pop_count() == 9);
	for (int sq = B9; sq <= B1; sq++){
		EXPECT_TRUE(FILE_B_MASK.is_bit_on(Square(sq)));
	}
	int count = FILE_C_MASK.pop_count();
	EXPECT_TRUE(FILE_C_MASK.pop_count() == 9);
	for (int sq = C9; sq <= C1; sq++){
		EXPECT_TRUE(FILE_C_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(FILE_D_MASK.pop_count() == 9);
	for (int sq = D9; sq <= D1; sq++){
		EXPECT_TRUE(FILE_D_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(FILE_E_MASK.pop_count() == 9);
	for (int sq = E9; sq <= E1; sq++){
		EXPECT_TRUE(FILE_E_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(FILE_F_MASK.pop_count() == 9);
	for (int sq = F9; sq <= F1; sq++){
		EXPECT_TRUE(FILE_F_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(FILE_G_MASK.pop_count() == 9);
	for (int sq = G9; sq <= G1; sq++){
		EXPECT_TRUE(FILE_G_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(FILE_H_MASK.pop_count() == 9);
	for (int sq = H9; sq <= H1; sq++){
		EXPECT_TRUE(FILE_H_MASK.is_bit_on(Square(sq)));
	}
	EXPECT_TRUE(FILE_I_MASK.pop_count() == 9);
	for (int sq = I9; sq <= I1; sq++){
		EXPECT_TRUE(FILE_I_MASK.is_bit_on(Square(sq)));
	}
}

TEST(bitboard, is_one_bit)
{
	BitBoard bb1(0x20100804120104D, 0x11008);	//0x11008=10001000000001000
	BitBoard bb2(0x20, 0x00);
	BitBoard bb3(0x00, 0x20);
	EXPECT_FALSE(bb1.is_one_bit());
	EXPECT_TRUE(bb2.is_one_bit());
	EXPECT_TRUE(bb3.is_one_bit());
}
TEST(bitboard, first_one)
{
	BitBoard bb(0x20100804120104D, 0x11008);	//0x11008=10001000000001000
	int ans[] = { 0, 2, 3, 6, 12, 21, 24, 30, 39, 48, 57, 63 + 3, 63 + 12, 63 + 16 };
	int len = bb.pop_count();
	for (int i = 0; i < len; i++){
		int index = bb.first_one();
		EXPECT_EQ(ans[i], index);
	}
}
TEST(bitboard, first_one_left)
{
	BitBoard bb(0x20100804120104D, 0x11008);	//0x11008=10001000000001000
	int ans[] = { 63 + 3, 63 + 12, 63 + 16 };
	for (int i = 0; i < 3; i++){
		int index = bb.first_one_left();
		EXPECT_EQ(ans[i], index);
	}
}
TEST(bitboard, first_one_right)
{
	BitBoard bb(0x20100804120104D, 0x11008);	//0x20100804120104D=1000000001000000001000000001000001001000000001000001001101
	int ans[] = { 0, 2, 3, 6, 12, 21, 24, 30, 39, 48, 57 };
	for (int i = 0; i < 11; i++){
		int index = bb.first_one_right();
		EXPECT_EQ(ans[i], index);
	}
}
TEST(bitboard, xor_bit)
{
	BitBoard bb1(0x20100804120104D, 0x11008);
	BitBoard ans1(0x20100804120104C, 0x11000);

	bb1.xor_bit(Square(I9));
	bb1.xor_bit(Square(B6));
	
	EXPECT_TRUE(bb1 == ans1);

	bb1.xor_bit(Square(I7), Square(H1));
	BitBoard ans2(0x20100804120104C - 0x04 + 0x20000, 0x11000);
	EXPECT_TRUE(bb1 == ans2);
}
TEST(bitboard, clear_bits)
{
	BitBoard bb1(0x20100804120104D, 0x11008);
	BitBoard bb2(0xCB87, 0x3b);
	BitBoard ans(0x201008041201048, 0x11000);
	//BitBoardns::print(bb2);
	/*
	bb1のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	   A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  *
	8  .  .  .  .  .  .  .  .  .
	7  .  .  .  .  .  .  .  .  *
	6  *  *  *  *  *  *  *  *  *
	5  .  .  .  .  .  .  .  .  .
	4  .  .  .  .  .  .  .  .  .
	3  .  .  .  .  .  .  *  .  *
	2  *  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+
	bb2のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	   A  B  C  D  E  F  G  H  I
	9  .  *  .  .  .  .  .  *  *
	8  .  *  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  *  *
	6  .  *  .  .  .  .  .  .  .
	5  .  *  .  .  .  .  .  .  .
	4  .  *  .  .  .  .  .  *  .
	3  .  .  .  .  .  .  .  *  .
	2  .  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+--+--+--+--+--+--+--+--+--+
	clear_bitの結果
	-+--+--+--+--+--+--+--+--+--+
	   A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  .
	8  .  .  .  .  .  .  .  .  .
	7  .  .  .  .  .  .  .  .  .
	6  *  .  *  *  *  *  *  *  *
	5  .  .  .  .  .  .  .  .  .
	4  .  .  .  .  .  .  .  .  .
	3  .  .  .  .  .  .  *  .  *
	2  *  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+
	*/
	EXPECT_TRUE(bb1.clear_bits(bb2) == ans);
}
TEST(bitboard, pop_count)
{
	BitBoard bb(0x20100804120104D, 0x11008);
	
	EXPECT_EQ(bb.pop_count(), 14);
}
TEST(bitboard, is_not_zero_clear_square)
{
	BitBoard bb(0x20100804120104D, 0x11008);
	int count = 0;

	EXPECT_TRUE(bb.is_not_zero());
	for (int sq = I9; sq < SquareNum; sq++){
		if (bb.is_bit_on(Square(sq))){
			bb.clear_square(Square(sq));
			count++;
		}
	}
	EXPECT_FALSE(bb.is_not_zero());
	EXPECT_EQ(count, 14);

}
TEST(bitboard, not_bit_equal)
{
	BitBoard bb(0x20100804120104D, 0x11008);
	BitBoard ans(0x80402010480413, 0x4402);
	
	EXPECT_TRUE(bb != ans);
}
TEST(bitboard, roght_shift)
{
	BitBoard bb(0x20100804120104D, 0x11008);
	BitBoard ans(0x80402010480413, 0x4402);

	EXPECT_TRUE((bb >>= 2) == ans);

}
TEST(bitboard, left_shift)
{
	BitBoard bb(0x20100804120104D, 0x11008);
	BitBoard ans(0x40201008240209A, 0x22010);
	//bb <<= 1;
	EXPECT_TRUE((bb <<= 1) == ans);
}
TEST(bitboard, xor)
{
	BitBoard bb1(0xCB87, 0x3b);
	BitBoard bb2(0x20100804120104D, 0x11008);
	BitBoard ans(0x20100804120DBCA, 0x11033);
	/*
	bb1のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  *  .  .  .  .  .  *  *
	8  .  *  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  *  *
	6  .  *  .  .  .  .  .  .  .
	5  .  *  .  .  .  .  .  .  .
	4  .  *  .  .  .  .  .  *  .
	3  .  .  .  .  .  .  .  *  .
	2  .  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+--+--+--+--+--+--+--+--+--+
	bb2のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  *
	8  .  .  .  .  .  .  .  .  .
	7  .  .  .  .  .  .  .  .  *
	6  *  *  *  *  *  *  *  *  *
	5  .  .  .  .  .  .  .  .  .
	4  .  .  .  .  .  .  .  .  .
	3  .  .  .  .  .  .  *  .  *
	2  *  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+
	xorの結果
	-+--+--+--+--+--+--+--+--+--+
	   A  B  C  D  E  F  G  H  I
	9  .  *  .  .  .  .  .  *  .
	8  .  *  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  *  .
	6  *  .  *  *  *  *  *  *  *
	5  .  *  .  .  .  .  .  .  .
	4  .  *  .  .  .  .  .  *  .
	3  .  .  .  .  .  .  *  *  *
	2  *  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+--+--+--+--+--+--+--+--+--+
	*/
	EXPECT_TRUE((bb1 ^= bb2) == ans);
}
TEST(bitboard, or)
{
	BitBoard bb1(0xCB87, 0x3b);
	BitBoard bb2(0x20100804120104D, 0x11008);
	BitBoard ans(0x20100804120DBCF, 0x1103B);
	/*
	bb1のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  *  .  .  .  .  .  *  *
	8  .  *  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  *  *
	6  .  *  .  .  .  .  .  .  .
	5  .  *  .  .  .  .  .  .  .
	4  .  *  .  .  .  .  .  *  .
	3  .  .  .  .  .  .  .  *  .
	2  .  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+--+--+--+--+--+--+--+--+--+
	bb2のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  *
	8  .  .  .  .  .  .  .  .  .
	7  .  .  .  .  .  .  .  .  *
	6  *  *  *  *  *  *  *  *  *
	5  .  .  .  .  .  .  .  .  .
	4  .  .  .  .  .  .  .  .  .
	3  .  .  .  .  .  .  *  .  *
	2  *  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+
	orの結果
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  *  .  .  .  .  .  *  *
	8  .  *  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  *  *
	6  *  *  *  *  *  *  *  *  *
	5  .  *  .  .  .  .  .  .  .
	4  .  *  .  .  .  .  .  *  .
	3  .  .  .  .  .  .  *  *  *
	2  *  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+--+--+--+--+--+--+--+--+--+
	*/
	//BitBoardns::print(bb1 |= bb2);
	EXPECT_TRUE((bb1 |= bb2) == ans);

}
TEST(bitboard, and)
{
	BitBoard bb1(0xCB87, 0x3b);
	BitBoard bb2(0x20100804120104D, 0x11008);
	/*
	bb1のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  *  .  .  .  .  .  *  *
	8  .  *  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  *  *
	6  .  *  .  .  .  .  .  .  .
	5  .  *  .  .  .  .  .  .  .
	4  .  *  .  .  .  .  .  *  .
	3  .  .  .  .  .  .  .  *  .
	2  .  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+--+--+--+--+--+--+--+--+--+
	bb2のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  *
	8  .  .  .  .  .  .  .  .  .
	7  .  .  .  .  .  .  .  .  *
	6  *  *  *  *  *  *  *  *  *
	5  .  .  .  .  .  .  .  .  .
	4  .  .  .  .  .  .  .  .  .
	3  .  .  .  .  .  .  *  .  *
	2  *  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+
	andの結果
  	   A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  *
	8  .  .  .  .  .  .  .  .  .
	7  .  .  .  .  .  .  .  .  *
	6  .  *  .  .  .  .  .  .  .
	5  .  .  .  .  .  .  .  .  .
	4  .  .  .  .  .  .  .  .  .
	3  .  .  .  .  .  .  .  .  .
	2  .  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+

	*/
	//BitBoardns::print(bb1 &= bb2);

	BitBoard ans(0x05, 0x08);
	EXPECT_TRUE((bb1 &= bb2) == ans);
}
TEST(bitboard, bit_Inversion_bit_equal)
{
	BitBoard bb(0xCB87, 0x3b);
	BitBoard bb1(0xFFFFFFFFFFFF3478, 0xFFFFFFFFFFFFFFC4);
	//BitBoardns::print(~bb);

	EXPECT_TRUE(~bb == bb1);
}
TEST(bitboard, is_biton_inmask)
{
	uint64_t b0 = 0xCB87;
	uint64_t b1 = 0x3b;
	BitBoard bb(b0, b1);
	//BitBoardns::print(bb);
	/*
	bbのbitパターン
	-+--+--+--+--+--+--+--+--+--+
  	   A  B  C  D  E  F  G  H  I
	9  .  *  .  .  .  .  .  *  *
	8  .  *  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  *  *
	6  .  *  .  .  .  .  .  .  .
	5  .  *  .  .  .  .  .  .  .
	4  .  *  .  .  .  .  .  *  .
	3  .  .  .  .  .  .  .  *  .
	2  .  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+--+--+--+--+--+--+--+--+--+
	*/
	BitBoard bb1(0x1FF, 0);
	//BitBoardns::print(bb1);
	/*
	bb1のbitパターン
	-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	   A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  *
	8  .  .  .  .  .  .  .  .  *
	7  .  .  .  .  .  .  .  .  *
	6  .  .  .  .  .  .  .  .  *
	5  .  .  .  .  .  .  .  .  *
	4  .  .  .  .  .  .  .  .  *
	3  .  .  .  .  .  .  .  .  *
	2  .  .  .  .  .  .  .  .  *
	1  .  .  .  .  .  .  .  .  *
	-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	*/
	BitBoard bb2(0x20100804120104D, 0x11008);
	//BitBoardns::print(bb2);
	/*
	bb2のbitパターン
	-+--+--+--+--+--+--+--+--+--+
	A  B  C  D  E  F  G  H  I
	9  .  .  .  .  .  .  .  .  *
	8  .  .  .  .  .  .  .  .  .
	7  .  .  .  .  .  .  .  .  *
	6  *  *  *  *  *  *  *  *  *
	5  .  .  .  .  .  .  .  .  .
	4  .  .  .  .  .  .  .  .  .
	3  .  .  .  .  .  .  *  .  *
	2  *  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+
	*/
	BitBoard bb3(0x00, 0x1FE00);
	//BitBoardns::print(bb3);
	/*
	-+--+--+--+--+--+--+--+--+--+
	   A  B  C  D  E  F  G  H  I
	9  *  .  .  .  .  .  .  .  .
	8  *  .  .  .  .  .  .  .  .
	7  *  .  .  .  .  .  .  .  .
	6  *  .  .  .  .  .  .  .  .
	5  *  .  .  .  .  .  .  .  .
	4  *  .  .  .  .  .  .  .  .
	3  *  .  .  .  .  .  .  .  .
	2  *  .  .  .  .  .  .  .  .
	1  .  .  .  .  .  .  .  .  .
	-+--+--+--+--+--+--+--+--+--+
	*/
	EXPECT_TRUE(bb.is_biton_inmask(bb1));
	EXPECT_TRUE(bb.is_biton_inmask(bb2));
	EXPECT_TRUE(bb1.is_biton_inmask(bb2));
	EXPECT_FALSE(bb.is_biton_inmask(bb3));
	EXPECT_FALSE(bb1.is_biton_inmask(bb3));
	EXPECT_TRUE(bb2.is_biton_inmask(bb3));
}

TEST(bitboard, is_bit_on)
{
	uint64_t b0 = 0xCB87;
	uint64_t b1 = 0x3b;
	BitBoard bb(b0, b1);
	//p[0]
	EXPECT_TRUE(bb.is_bit_on(I9));
	EXPECT_TRUE(bb.is_bit_on(I8));
	EXPECT_TRUE(bb.is_bit_on(I7));
	EXPECT_FALSE(bb.is_bit_on(I6));
	EXPECT_FALSE(bb.is_bit_on(I5));
	EXPECT_FALSE(bb.is_bit_on(I4));
	EXPECT_FALSE(bb.is_bit_on(I3));
	EXPECT_TRUE(bb.is_bit_on(I2));
	EXPECT_TRUE(bb.is_bit_on(I1));
	EXPECT_TRUE(bb.is_bit_on(H9));
	EXPECT_FALSE(bb.is_bit_on(H8));
	EXPECT_TRUE(bb.is_bit_on(H7));
	EXPECT_FALSE(bb.is_bit_on(H6));
	EXPECT_FALSE(bb.is_bit_on(H5));
	EXPECT_TRUE(bb.is_bit_on(H4));
	EXPECT_TRUE(bb.is_bit_on(H3));
	EXPECT_FALSE(bb.is_bit_on(H2));
	EXPECT_FALSE(bb.is_bit_on(H1));
	EXPECT_FALSE(bb.is_bit_on(G9));
	EXPECT_FALSE(bb.is_bit_on(G8));
	EXPECT_FALSE(bb.is_bit_on(G7));
	EXPECT_FALSE(bb.is_bit_on(G6));
	EXPECT_FALSE(bb.is_bit_on(G5));
	EXPECT_FALSE(bb.is_bit_on(G4));
	EXPECT_FALSE(bb.is_bit_on(G3));
	EXPECT_FALSE(bb.is_bit_on(G2));
	EXPECT_FALSE(bb.is_bit_on(G1));
	EXPECT_FALSE(bb.is_bit_on(F9));
	EXPECT_FALSE(bb.is_bit_on(F8));
	EXPECT_FALSE(bb.is_bit_on(F7));
	EXPECT_FALSE(bb.is_bit_on(F6));
	EXPECT_FALSE(bb.is_bit_on(F5));
	EXPECT_FALSE(bb.is_bit_on(F4));
	EXPECT_FALSE(bb.is_bit_on(F3));
	EXPECT_FALSE(bb.is_bit_on(F2));
	EXPECT_FALSE(bb.is_bit_on(F1));
	EXPECT_FALSE(bb.is_bit_on(E9));
	EXPECT_FALSE(bb.is_bit_on(E8));
	EXPECT_FALSE(bb.is_bit_on(E7));
	EXPECT_FALSE(bb.is_bit_on(E6));
	EXPECT_FALSE(bb.is_bit_on(E5));
	EXPECT_FALSE(bb.is_bit_on(E4));
	EXPECT_FALSE(bb.is_bit_on(E3));
	EXPECT_FALSE(bb.is_bit_on(E2));
	EXPECT_FALSE(bb.is_bit_on(E1));
	EXPECT_FALSE(bb.is_bit_on(D9));
	EXPECT_FALSE(bb.is_bit_on(D8));
	EXPECT_FALSE(bb.is_bit_on(D7));
	EXPECT_FALSE(bb.is_bit_on(D6));
	EXPECT_FALSE(bb.is_bit_on(D5));
	EXPECT_FALSE(bb.is_bit_on(D4));
	EXPECT_FALSE(bb.is_bit_on(D3));
	EXPECT_FALSE(bb.is_bit_on(D2));
	EXPECT_FALSE(bb.is_bit_on(D1));
	EXPECT_FALSE(bb.is_bit_on(C9));
	EXPECT_FALSE(bb.is_bit_on(C8));
	EXPECT_FALSE(bb.is_bit_on(C7));
	EXPECT_FALSE(bb.is_bit_on(C6));
	EXPECT_FALSE(bb.is_bit_on(C5));
	EXPECT_FALSE(bb.is_bit_on(C4));
	EXPECT_FALSE(bb.is_bit_on(C3));
	EXPECT_FALSE(bb.is_bit_on(C2));
	EXPECT_FALSE(bb.is_bit_on(C1));
	//p[1]
	EXPECT_TRUE(bb.is_bit_on(B9));
	EXPECT_TRUE(bb.is_bit_on(B8));
	EXPECT_FALSE(bb.is_bit_on(B7));
	EXPECT_TRUE(bb.is_bit_on(B6));
	EXPECT_TRUE(bb.is_bit_on(B5));
	EXPECT_TRUE(bb.is_bit_on(B4));
	EXPECT_FALSE(bb.is_bit_on(B3));
	EXPECT_FALSE(bb.is_bit_on(B2));
	EXPECT_FALSE(bb.is_bit_on(B1));
}
TEST(bitboard_case, bitboard)
{
	//pop_count function
	uint64_t b0 = 0xCB87;
	uint64_t b1 = 0x3b;

	BitBoard bb(b0, b1);
	EXPECT_EQ(bb.pop_count(), 14);
}
/*
WHILE(後手)								BLACK(先手)
+--+--+--+--+--+--+--+--+--+
FILEI		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+
FILEH		|H9|H8|H7|H6|H5|H4|H3|H2|H1|
|09|10|11|12|13|14|15|16|17|
+--+--+--+--+--+--+--+--+--+
FILEG		|G9|G8|I7|I6|I5|I4|I3|I2|I1|
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+
FILEF		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+
FILEE		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+
FILED		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+
FILEC		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+  ↑ BitBoard.p_[0]	63座標
FILEB		|I9|I8|I7|I6|I5|I4|I3|I2|I1|  ↓ BitBoard.p_[1]	18座標
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+
FILEA		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
|00|01|02|03|04|05|06|07|08|
+--+--+--+--+--+--+--+--+--+
RANK	  9  8  7  6  5  4  3  2  1

将棋盤を真横から見ているような配置にしてある、将棋は先手側から後手側に、後手側から先手側に縦に移動する手が多いので
このような配置にしてある、と思う。
*/

#endif