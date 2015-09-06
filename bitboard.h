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
		_mm_store_si128(&this->m_, rhs.m_);
		return *this;
	}
	//コピーコンストラクタ
	BitBoard(const BitBoard& bb)
	{
		_mm_store_si128(&this->m_, bb.m_);
	}
	//空コンストラクタ
	BitBoard() {}
	//64bit変数で設定する
	//使用例
	//BitBoard bb(0,0);
	BitBoard(const uint64_t v0, const uint64_t v1)
	{
		this->p_[0] = v0;
		this->p_[1] = v1;
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
		_mm_store_si128(&this->m_, _mm_and_si128(this->m_, rhs.m_));
		return *this;
	}
	//引数rhsとのOR演算
	BitBoard operator |= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_or_si128(this->m_, rhs.m_));
		return *this;
	}
	//引数rhsとのXOR演算
	BitBoard operator ^= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_xor_si128(this->m_, rhs.m_));
		return *this;
	}
	//bitboardを引数iだけ左シフトする
	BitBoard operator <<= (const int i)
	{
		_mm_store_si128(&this->m_, _mm_slli_epi64(this->m_, i));
		return *this;
	}
	//bitboardを引数iだけ右シフトする
	BitBoard operator >>= (const int i)
	{
		_mm_store_si128(&this->m_, _mm_srli_epi64(this->m_, i));
		return *this;
	}
	//bitboardを引数rhsと比較して等しければtrueを返す
	bool operator == (const BitBoard& rhs) const
	{
		return _mm_testc_si128(_mm_cmpeq_epi8(this->m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xFFu))) ? true:false;
	}
	//bitboardを引数rhsと比較して等しくなければtrueを返す
	bool operator != (const BitBoard& rhs) const
	{
		return _mm_testc_si128(_mm_cmpeq_epi8(this->m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xFFu))) ? false : true;
	}

	//_mm_testz_si128は引数同士をbit AND演算して結果がゼロになれば1を返す
	//このis_not_zero関数はBitBoardクラスのbitboardがすべてゼロになったらFALSEを返す
	bool is_not_zero()
	{
		return !(_mm_testz_si128(this->m_, _mm_set1_epi8(static_cast<char>(0xFFu))));
	}
	//局面bitboardと引数のbitboardのAND演算でbitが立っていればtrueを返す
	bool is_biton_inmask(const BitBoard& mask)
	{
		return !(_mm_testz_si128(this->m_, mask.m_));
	}
	int pop_count()
	{
		return int(_mm_popcnt_u64(p_[0]) + _mm_popcnt_u64(p_[1]));
	}
	//指定した座標のbitをon
	void set_bit(const Square sq)
	{
		*this |= SquareBB[sq];
	}
	//指定した座標に駒がいるか判定する
	bool is_bit_on(const Square sq)
	{
		return !(_mm_testz_si128(this->m_, SquareBB[sq].m_));
	}
	//maskがonになっているところをクリアする
	BitBoard clear_bits(const BitBoard& mask)
	{
		_mm_store_si128(&this->m_, _mm_andnot_si128(mask.m_, this->m_));
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
		_mm_store_si128(&this->m_,_mm_xor_si128(this->m_, _mm_or_si128(SquareBB[sq1].m_, SquareBB[sq2].m_)));
	}
	//private:
	union{
		//将棋盤の座標0～62までをp_[0]が表現,座標63から80までをp_[1]が表現
		//座標が若いほど下位bit,p_[0]では座標0が最下位bit,座標63が最上位bit
		//p_[1]では座標63が最下位bitで座標80が最上位bit
		uint64_t p_[2];
		__m128i m_;
	};
};

namespace BitBoards
{
	void init();
	void print(BitBoard&);
}
extern const BitBoard SquareBB[SquareNum];

#endif
