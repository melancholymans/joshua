#if !defined(BITBOARD_H_INCLUDE)
#define BITBOARD_H_INCLUDE

#include "types.h"

class BitBoard;		//先行宣言
extern const BitBoard SquareBB[SquareNum];

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
		*this |= SquareBB[sq];
	}
	//指定した座標に駒がいるか判定する
	bool is_bit_on(const Square sq) const
	{
		return !(_mm_testz_si128(m_, SquareBB[sq].m_));
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
		clear_bits(SquareBB[sq]);
	}
	//指定した座標がbit onならbit offにoffならonにする
	void xor_bit(const Square sq)
	{
		*this ^= SquareBB[sq];
	}
	//指定した座標sq1,sq2でxor演算、駒の移動表現
	void xor_bit(const Square sq1, const Square sq2)
	{
		_mm_store_si128(&m_, _mm_xor_si128(m_, _mm_or_si128(SquareBB[sq1].m_, SquareBB[sq2].m_)));
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
	const BitBoard file_i_mask(static_cast<uint64_t>(0x1FF) << (9 * 0), 0x00);
	const BitBoard file_h_mask(static_cast<uint64_t>(0x1FF) << (9 * 1), 0x00);
	const BitBoard file_g_mask(static_cast<uint64_t>(0x1FF) << (9 * 2), 0x00);
	const BitBoard file_f_mask(static_cast<uint64_t>(0x1FF) << (9 * 3), 0x00);
	const BitBoard file_e_mask(static_cast<uint64_t>(0x1FF) << (9 * 4), 0x00);
	const BitBoard file_d_mask(static_cast<uint64_t>(0x1FF) << (9 * 5), 0x00);
	const BitBoard file_c_mask(static_cast<uint64_t>(0x1FF) << (9 * 6), 0x00);
	const BitBoard file_b_mask(0x00, 0x1FF << (9 * 0));
	const BitBoard file_a_mask(0x00, 0x1FF << (9 * 1));
	const BitBoard file_mask[FileNum] = {
		file_i_mask, file_h_mask, file_g_mask, file_f_mask, file_g_mask, file_d_mask, file_c_mask, file_b_mask, file_a_mask
	};
	//指定したrankのマスクを返す
	const BitBoard rank_9_mask(0x40201008040201 << 0, 0x201 << 0);
	const BitBoard rank_8_mask(0x40201008040201 << 1, 0x201 << 1);
	const BitBoard rank_7_mask(0x40201008040201 << 2, 0x201 << 2);
	const BitBoard rank_6_mask(0x40201008040201 << 3, 0x201 << 3);
	const BitBoard rank_5_mask(0x40201008040201 << 4, 0x201 << 4);
	const BitBoard rank_4_mask(0x40201008040201 << 5, 0x201 << 5);
	const BitBoard rank_3_mask(0x40201008040201 << 6, 0x201 << 6);
	const BitBoard rank_2_mask(0x40201008040201 << 7, 0x201 << 7);
	const BitBoard rank_1_mask(0x40201008040201 << 8, 0x201 << 8);
	const BitBoard rank_mask[RankNum] = {
		rank_9_mask, rank_8_mask, rank_7_mask, rank_6_mask, rank_5_mask, rank_4_mask, rank_3_mask, rank_2_mask, rank_1_mask
	};
	//指定したrankより前方（カラーによって変わる）のbitがonになるbitboardを返す、指定したrankのbitは含まれない
	const BitBoard in_front_of_rank9_black(0x00, 0x00);
	const BitBoard in_front_of_rank8_black = rank_9_mask;
	const BitBoard in_front_of_rank7_black = in_front_of_rank8_black | rank_8_mask;
	const BitBoard in_front_of_rank6_black = in_front_of_rank7_black | rank_7_mask;
	const BitBoard in_front_of_rank5_black = in_front_of_rank6_black | rank_6_mask;
	const BitBoard in_front_of_rank4_black = in_front_of_rank5_black | rank_5_mask;
	const BitBoard in_front_of_rank3_black = in_front_of_rank4_black | rank_4_mask;
	const BitBoard in_front_of_rank2_black = in_front_of_rank3_black | rank_3_mask;
	const BitBoard in_front_of_rank1_black = in_front_of_rank2_black | rank_2_mask;
	const BitBoard in_front_of_rank1_white(0x00, 0x00);
	const BitBoard in_front_of_rank2_white = rank_1_mask;
	const BitBoard in_front_of_rank3_white = in_front_of_rank2_white | rank_2_mask;
	const BitBoard in_front_of_rank4_white = in_front_of_rank3_white | rank_3_mask;
	const BitBoard in_front_of_rank5_white = in_front_of_rank4_white | rank_4_mask;
	const BitBoard in_front_of_rank6_white = in_front_of_rank5_white | rank_5_mask;
	const BitBoard in_front_of_rank7_white = in_front_of_rank6_white | rank_6_mask;
	const BitBoard in_front_of_rank8_white = in_front_of_rank7_white | rank_7_mask;
	const BitBoard in_front_of_rank9_white = in_front_of_rank8_white | rank_8_mask;
	const BitBoard in_front_mask[ColorNum][RankNum] = {
		{ in_front_of_rank9_black, in_front_of_rank8_black, in_front_of_rank7_black, in_front_of_rank6_black, in_front_of_rank5_black, in_front_of_rank4_black, in_front_of_rank3_black, in_front_of_rank2_black, in_front_of_rank1_black },
		{ in_front_of_rank9_white, in_front_of_rank8_white, in_front_of_rank7_white, in_front_of_rank6_white, in_front_of_rank5_white, in_front_of_rank4_white, in_front_of_rank3_white, in_front_of_rank2_white, in_front_of_rank1_white }
	};
	static BitBoard bishop_attack[20224];
	static BitBoard rook_attack[495616];
	static BitBoard bishop_mask[81];
	static BitBoard rook_mask[81];
	static int bishop_attack_index[81];
	static int rook_attack_index[81];

	void init();
	void print(BitBoard&);
	BitBoard make_bishop_attack(const Square sq, const BitBoard& occ);
	BitBoard make_rook_attack(const Square sq, const BitBoard& occ);
	static BitBoard sliding_attack(Square sq, BitBoard occ, bool is_bishop);
	static BitBoard index_to_occupied(int index, int attack_num, const BitBoard mask);
	static uint64_t occupied_to_index(const BitBoard& occ, const BitBoard& mask);
	static void init_bishop_attacks();
	static void init_rook_attacks();
}

#endif
