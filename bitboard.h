#if !defined(BITBOARD_H_INCLUDE)
#define BITBOARD_H_INCLUDE

#include "types.h"

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
	BitBoard(const uint64_t v0, const uint64_t v1)
	{
		this->p_[0] = v0;
		this->p_[1] = v1;
	}
	//_mm_testz_si128は引数同士をbit AND演算して結果がゼロになれば1を返す
	//このis_not_zero関数はこのBitBoardクラスのbitboardがずべてゼロになったらFALSEを返す
	bool is_not_zero()
	{
		return !(_mm_testz_si128(this->m_, _mm_set1_epi8(static_cast<char>(0xFFu))));
	}
	BitBoard operator |= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_or_si128(this->m_, rhs.m_));
	}
	int pop_count()
	{
		return _mm_popcnt_u64(p_[0]) + _mm_popcnt_u64(p_[1]);
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

namespace BitBoards
{
	void init();
	void print(BitBoard);
}
extern const BitBoard SquareBB[SquareNum];

#endif
