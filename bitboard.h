#if !defined(BITBOARD_H_INCLUDE)
#define BITBOARD_H_INCLUDE

#include "types.h"

class BitBoard;		//先行宣言
extern const BitBoard SQUARE_BB[SquareNum];

class BitBoard{
public:
	//BitBoardを"="演算子で代入する
	BitBoard& operator = (const BitBoard& rhs)
	{
		_mm_store_si128(&m_, rhs.m_);
		return *this;
	}
	//コピーコンストラクタ
	BitBoard(const BitBoard& bb)
	{
		_mm_store_si128(&m_, bb.m_);
	}
	//空コンストラクタ
	BitBoard() {}
	//64bit変数で設定する
	//使用例
	//BitBoard bb(0,0);
	BitBoard(const uint64_t v0, const uint64_t v1)
	{
		p_[0] = v0;
		p_[1] = v1;
	}
	//単項演算子、保持しているbitboardを反転する
	BitBoard operator ~() const
	{
		BitBoard tmp;
		_mm_store_si128(&tmp.m_, _mm_andnot_si128(this->m_, _mm_set1_epi8(static_cast<char>(0xFFu))));
		return tmp;
	}
	//引数rhsとのAND演算
	BitBoard operator &= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_and_si128(m_, rhs.m_));
		return *this;
	}
	//引数rhsとのOR演算
	BitBoard operator |= (const BitBoard& rhs)
	{
		_mm_store_si128(&m_, _mm_or_si128(m_, rhs.m_));
		return *this;
	}
	//引数rhsとのXOR演算
	BitBoard operator ^= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_xor_si128(m_, rhs.m_));
		return *this;
	}
	//bitboardを引数iだけ左シフトする
	BitBoard operator <<= (const int i)
	{
		_mm_store_si128(&m_, _mm_slli_epi64(m_, i));
		return *this;
	}
	//bitboardを引数iだけ右シフトする
	BitBoard operator >>= (const int i)
	{
		_mm_store_si128(&m_, _mm_srli_epi64(m_, i));
		return *this;
	}
	//bitboardを引数rhsと比較して等しければtrueを返す
	bool operator == (const BitBoard& rhs) const
	{
		return _mm_testc_si128(_mm_cmpeq_epi8(m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xFFu))) ? true : false;
	}
	//bitboardを引数rhsと比較して等しくなければtrueを返す
	bool operator != (const BitBoard& rhs) const
	{
		return _mm_testc_si128(_mm_cmpeq_epi8(m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xFFu))) ? false : true;
	}
	BitBoard operator | (const BitBoard& rhs) const
	{
		return BitBoard(*this) |= rhs;
	}
	BitBoard operator & (const BitBoard& rhs) const
	{
		return BitBoard(*this) &= rhs;
	}
	//_mm_testz_si128は引数同士をbit AND演算して結果がゼロになれば1を返す
	//このis_not_zero関数はBitBoardクラスのbitboardがすべてゼロになったらFALSEを返す
	bool is_not_zero() const
	{
		return !(_mm_testz_si128(m_, _mm_set1_epi8(static_cast<char>(0xFFu))));
	}
	//局面bitboardと引数のbitboardのAND演算でbitが立っていればtrueを返す
	bool is_biton_inmask(const BitBoard& mask) const
	{
		return !(_mm_testz_si128(m_, mask.m_));
	}
	int pop_count() const
	{
		return int(_mm_popcnt_u64(p_[0]) + _mm_popcnt_u64(p_[1]));
	}
	//指定した座標のbitをon
	void set_bit(const Square sq)
	{
		*this |= SQUARE_BB[sq];
	}
	//指定した座標に駒がいるか判定する
	bool is_bit_on(const Square sq) const
	{
		return !(_mm_testz_si128(m_, SQUARE_BB[sq].m_));
	}
	//maskがonになっているところをクリアする
	BitBoard clear_bits(const BitBoard& mask)
	{
		_mm_store_si128(&m_, _mm_andnot_si128(mask.m_, m_));
		return *this;
	}
	//指定した座標のbitをoffにする
	void clear_square(const Square sq)
	{
		clear_bits(SQUARE_BB[sq]);
	}
	//指定した座標がbit onならbit offにoffならonにする
	void xor_bit(const Square sq)
	{
		*this ^= SQUARE_BB[sq];
	}
	//指定した座標sq1,sq2でxor演算、駒の移動表現
	void xor_bit(const Square sq1, const Square sq2)
	{
		_mm_store_si128(&m_, _mm_xor_si128(m_, _mm_or_si128(SQUARE_BB[sq1].m_, SQUARE_BB[sq2].m_)));
	}
	//bitboardが完全にゼロでないことを前提としている
	//bitboardのp_[0]をLSB側からスキャンして最初のbit onのindexを返す。indexはゼロから始まる
	//indexのbitをoffにする
	Square first_one_right()
	{
		unsigned long index;

		_BitScanForward64(&index, p_[0]);
		p_[0] &= p_[0] - 1;
		return Square(index);
	}
	//bitboardが完全にゼロでないことを前提としている
	//bitboardのp_[1]をLSB側からスキャンして最初のbit onのindexを返す。indexはゼロから始まる
	//indexのbitをoffにする
	Square first_one_left()
	{
		unsigned long index;

		_BitScanForward64(&index, p_[1]);
		this->p_[1] &= p_[1] - 1;
		return Square(index + 63);
	}
	Square first_one()
	{
		if (this->p_[0]){
			return first_one_right();
		}
		return first_one_left();
	}
	//onになっているbitが１つだけか判定し、そうならtrueを返す
	bool is_one_bit() const
	{
		return (pop_count() == 1);
	}
	uint64_t p(const int index) const
	{
		return p_[index];
	}
	//_p[0],_p[1]をマージする
	uint64_t merge() const
	{
		return this->p_[0] | this->p_[1];
	}
private:
	union{
		//将棋盤の座標0～62までをp_[0]が表現,座標63から80までをp_[1]が表現
		//座標が若いほど下位bit,p_[0]では座標0が最下位bit,座標63が最上位bit
		//p_[1]では座標63が最下位bitで座標80が最上位bit
		uint64_t p_[2];
		__m128i m_;
	};
};

namespace BitBoardns
{
	//指定したfileのマスクを返す
	const BitBoard FILE_I_MASK(static_cast<uint64_t>(0x1FF) << (9 * 0), 0x00);
	const BitBoard FILE_H_MASK(static_cast<uint64_t>(0x1FF) << (9 * 1), 0x00);
	const BitBoard FILE_G_MASK(static_cast<uint64_t>(0x1FF) << (9 * 2), 0x00);
	const BitBoard FILE_F_MASK(static_cast<uint64_t>(0x1FF) << (9 * 3), 0x00);
	const BitBoard FILE_E_MASK(static_cast<uint64_t>(0x1FF) << (9 * 4), 0x00);
	const BitBoard FILE_D_MASK(static_cast<uint64_t>(0x1FF) << (9 * 5), 0x00);
	const BitBoard FILE_C_MASK(static_cast<uint64_t>(0x1FF) << (9 * 6), 0x00);
	const BitBoard FILE_B_MASK(0x00, 0x1FF << (9 * 0));
	const BitBoard FILE_A_MASK(0x00, 0x1FF << (9 * 1));
	const BitBoard FILE_MASK[FileNum] = {
		FILE_I_MASK, FILE_H_MASK, FILE_G_MASK, FILE_F_MASK, FILE_E_MASK, FILE_D_MASK, FILE_C_MASK, FILE_B_MASK, FILE_A_MASK
	};
	//指定したrankのマスクを返す
	const BitBoard RANK_9_MASK(0x40201008040201 << 0, 0x201 << 0);
	const BitBoard RANK_8_MASK(0x40201008040201 << 1, 0x201 << 1);
	const BitBoard RANK_7_MASK(0x40201008040201 << 2, 0x201 << 2);
	const BitBoard RANK_6_MASK(0x40201008040201 << 3, 0x201 << 3);
	const BitBoard RANK_5_MASK(0x40201008040201 << 4, 0x201 << 4);
	const BitBoard RANK_4_MASK(0x40201008040201 << 5, 0x201 << 5);
	const BitBoard RANK_3_MASK(0x40201008040201 << 6, 0x201 << 6);
	const BitBoard RANK_2_MASK(0x40201008040201 << 7, 0x201 << 7);
	const BitBoard RANK_1_MASK(0x40201008040201 << 8, 0x201 << 8);
	const BitBoard RANK_MASK[RankNum] = {
		RANK_9_MASK, RANK_8_MASK, RANK_7_MASK, RANK_6_MASK, RANK_5_MASK, RANK_4_MASK, RANK_3_MASK, RANK_2_MASK, RANK_1_MASK
	};
	//指定したrankより前方（カラーによって変わる）のbitがonになるbitboardを返す、指定したrankのbitは含まれない
	const BitBoard IN_FRONT_OF_RANK9_BLACK(0x00, 0x00);
	const BitBoard IN_FRONT_OF_RANK8_BLACK = RANK_9_MASK;
	const BitBoard IN_FRONT_OF_RANK7_BLACK = IN_FRONT_OF_RANK8_BLACK | RANK_8_MASK;
	const BitBoard IN_FRONT_OF_RANK6_BLACK = IN_FRONT_OF_RANK7_BLACK | RANK_7_MASK;
	const BitBoard IN_FRONT_OF_RANK5_BLACK = IN_FRONT_OF_RANK6_BLACK | RANK_6_MASK;
	const BitBoard IN_FRONT_OF_RANK4_BLACK = IN_FRONT_OF_RANK5_BLACK | RANK_5_MASK;
	const BitBoard IN_FRONT_OF_RANK3_BLACK = IN_FRONT_OF_RANK4_BLACK | RANK_4_MASK;
	const BitBoard IN_FRONT_OF_RANK2_BLACK = IN_FRONT_OF_RANK3_BLACK | RANK_3_MASK;
	const BitBoard IN_FRONT_OF_RANK1_BLACK = IN_FRONT_OF_RANK2_BLACK | RANK_2_MASK;
	const BitBoard IN_FRONT_OF_RANK1_WHITE(0x00, 0x00);
	const BitBoard IN_FRONT_OF_RANK2_WHITE = RANK_1_MASK;
	const BitBoard IN_FRONT_OF_RANK3_WHITE = IN_FRONT_OF_RANK2_WHITE | RANK_2_MASK;
	const BitBoard IN_FRONT_OF_RANK4_WHITE = IN_FRONT_OF_RANK3_WHITE | RANK_3_MASK;
	const BitBoard IN_FRONT_OF_RANK5_WHITE = IN_FRONT_OF_RANK4_WHITE | RANK_4_MASK;
	const BitBoard IN_FRONT_OF_RANK6_WHITE = IN_FRONT_OF_RANK5_WHITE | RANK_5_MASK;
	const BitBoard IN_FRONT_OF_RANK7_WHITE = IN_FRONT_OF_RANK6_WHITE | RANK_6_MASK;
	const BitBoard IN_FRONT_OF_RANK8_WHITE = IN_FRONT_OF_RANK7_WHITE | RANK_7_MASK;
	const BitBoard IN_FRONT_OF_RANK9_WHITE = IN_FRONT_OF_RANK8_WHITE | RANK_8_MASK;
	const BitBoard IN_FRONT_MASK[ColorNum][RankNum] = {
		{ IN_FRONT_OF_RANK9_BLACK, IN_FRONT_OF_RANK8_BLACK, IN_FRONT_OF_RANK7_BLACK, IN_FRONT_OF_RANK6_BLACK, IN_FRONT_OF_RANK5_BLACK, IN_FRONT_OF_RANK4_BLACK, IN_FRONT_OF_RANK3_BLACK, IN_FRONT_OF_RANK2_BLACK, IN_FRONT_OF_RANK1_BLACK },
		{ IN_FRONT_OF_RANK9_WHITE, IN_FRONT_OF_RANK8_WHITE, IN_FRONT_OF_RANK7_WHITE, IN_FRONT_OF_RANK6_WHITE, IN_FRONT_OF_RANK5_WHITE, IN_FRONT_OF_RANK4_WHITE, IN_FRONT_OF_RANK3_WHITE, IN_FRONT_OF_RANK2_WHITE, IN_FRONT_OF_RANK1_WHITE }
	};
	//指定したrankより前方（カラーによって変わる）３列のみbitがonになるbitboardを返す、指定したrankのbitは含まれない
	const BitBoard alloff(0x00, 0x00);
	const BitBoard allon(0x7FFFFFFFFFFFFFFF, 0x000000000003FFFF);
	const BitBoard PASSED_FRONT[ColorNum][SquareNum] = {
		{
			//file I
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			//file H
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]), 
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			//file G
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]), 
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]), 
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]), 
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]), 
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]), 
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]), 
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			//file F
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]), 
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			//file E
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			//file D
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			//file C
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			//file B
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			//file A
			alloff, 
			IN_FRONT_MASK[Black][Rank8] & (FILE_MASK[FileA] | FILE_MASK[FileB]), 
			IN_FRONT_MASK[Black][Rank7] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[Black][Rank6] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[Black][Rank5] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[Black][Rank4] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[Black][Rank3] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[Black][Rank2] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[Black][Rank1] & (FILE_MASK[FileA] | FILE_MASK[FileB])
		},
		{	//White
			//file I
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileH] | FILE_MASK[FileI]),
			alloff,
			//file H
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileG] | FILE_MASK[FileH] | FILE_MASK[FileI]),
			alloff,
			//file G
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileF] | FILE_MASK[FileG] | FILE_MASK[FileH]),
			alloff,
			//file F
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileE] | FILE_MASK[FileF] | FILE_MASK[FileG]),
			alloff,
			//file E
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileD] | FILE_MASK[FileE] | FILE_MASK[FileF]),
			alloff,
			//file D
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileC] | FILE_MASK[FileD] | FILE_MASK[FileE]),
			alloff,
			//file C
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileB] | FILE_MASK[FileC] | FILE_MASK[FileD]),
			alloff,
			//file B
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileA] | FILE_MASK[FileB] | FILE_MASK[FileC]),
			alloff,
			//file A
			IN_FRONT_MASK[White][Rank9] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[White][Rank8] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[White][Rank7] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[White][Rank6] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[White][Rank5] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[White][Rank4] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[White][Rank3] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			IN_FRONT_MASK[White][Rank2] & (FILE_MASK[FileA] | FILE_MASK[FileB]),
			alloff
		}
	};
	void init();
	void print(BitBoard&);
	BitBoard make_pawn_attack(Color c, const Square sq);
	BitBoard make_lance_attack(Color c, const Square sq, const BitBoard& occ);
	BitBoard make_night_attack(Color c, const Square sq);
	BitBoard make_silver_attack(Color c, const Square sq);
	BitBoard make_bishop_attack(const Square sq, const BitBoard& occ);
	BitBoard make_rook_attack(const Square sq, const BitBoard& occ);
	BitBoard make_gold_attack(Color c,const Square sq);
	BitBoard make_king_attack(const Square sq);
}

#endif
