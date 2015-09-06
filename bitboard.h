#if !defined(BITBOARD_H_INCLUDE)
#define BITBOARD_H_INCLUDE

#include "types.h"

class BitBoard;		//��s�錾
extern const BitBoard SquareBB[SquareNum];

class BitBoard{
public:
	//BitBoard��"="���Z�q�ő������
	BitBoard& operator = (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, rhs.m_);
		return *this;
	}
	//�R�s�[�R���X�g���N�^
	BitBoard(const BitBoard& bb)
	{
		_mm_store_si128(&this->m_, bb.m_);
	}
	//��R���X�g���N�^
	BitBoard() {}
	//64bit�ϐ��Őݒ肷��
	//�g�p��
	//BitBoard bb(0,0);
	BitBoard(const uint64_t v0, const uint64_t v1)
	{
		this->p_[0] = v0;
		this->p_[1] = v1;
	}
	//�P�����Z�q�A�ێ����Ă���bitboard�𔽓]����
	BitBoard operator ~() const
	{
		BitBoard tmp;
		_mm_store_si128(&tmp.m_, _mm_andnot_si128(this->m_, _mm_set1_epi8(static_cast<char>(0xFFu))));
		return tmp;
	}
	//����rhs�Ƃ�AND���Z
	BitBoard operator &= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_and_si128(this->m_, rhs.m_));
		return *this;
	}
	//����rhs�Ƃ�OR���Z
	BitBoard operator |= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_or_si128(this->m_, rhs.m_));
		return *this;
	}
	//����rhs�Ƃ�XOR���Z
	BitBoard operator ^= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_xor_si128(this->m_, rhs.m_));
		return *this;
	}
	//bitboard������i�������V�t�g����
	BitBoard operator <<= (const int i)
	{
		_mm_store_si128(&this->m_, _mm_slli_epi64(this->m_, i));
		return *this;
	}
	//bitboard������i�����E�V�t�g����
	BitBoard operator >>= (const int i)
	{
		_mm_store_si128(&this->m_, _mm_srli_epi64(this->m_, i));
		return *this;
	}
	//bitboard������rhs�Ɣ�r���ē��������true��Ԃ�
	bool operator == (const BitBoard& rhs) const
	{
		return _mm_testc_si128(_mm_cmpeq_epi8(this->m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xFFu))) ? true:false;
	}
	//bitboard������rhs�Ɣ�r���ē������Ȃ����true��Ԃ�
	bool operator != (const BitBoard& rhs) const
	{
		return _mm_testc_si128(_mm_cmpeq_epi8(this->m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xFFu))) ? false : true;
	}

	//_mm_testz_si128�͈������m��bit AND���Z���Č��ʂ��[���ɂȂ��1��Ԃ�
	//����is_not_zero�֐���BitBoard�N���X��bitboard�����ׂă[���ɂȂ�����FALSE��Ԃ�
	bool is_not_zero()
	{
		return !(_mm_testz_si128(this->m_, _mm_set1_epi8(static_cast<char>(0xFFu))));
	}
	//�ǖ�bitboard�ƈ�����bitboard��AND���Z��bit�������Ă����true��Ԃ�
	bool is_biton_inmask(const BitBoard& mask)
	{
		return !(_mm_testz_si128(this->m_, mask.m_));
	}
	int pop_count()
	{
		return int(_mm_popcnt_u64(p_[0]) + _mm_popcnt_u64(p_[1]));
	}
	//�w�肵�����W��bit��on
	void set_bit(const Square sq)
	{
		*this |= SquareBB[sq];
	}
	//�w�肵�����W�ɋ���邩���肷��
	bool is_bit_on(const Square sq)
	{
		return !(_mm_testz_si128(this->m_, SquareBB[sq].m_));
	}
	//mask��on�ɂȂ��Ă���Ƃ�����N���A����
	BitBoard clear_bits(const BitBoard& mask)
	{
		_mm_store_si128(&this->m_, _mm_andnot_si128(mask.m_, this->m_));
		return *this;
	}
	//�w�肵�����W��bit��off�ɂ���
	void clear_square(const Square sq)
	{
		clear_bits(SquareBB[sq]);
	}
	//�w�肵�����W��bit on�Ȃ�bit off��off�Ȃ�on�ɂ���
	void xor_bit(const Square sq)
	{
		*this ^= SquareBB[sq];
	}
	//�w�肵�����Wsq1,sq2��xor���Z�A��̈ړ��\��
	void xor_bit(const Square sq1, const Square sq2)
	{
		_mm_store_si128(&this->m_,_mm_xor_si128(this->m_, _mm_or_si128(SquareBB[sq1].m_, SquareBB[sq2].m_)));
	}
	//private:
	union{
		//�����Ղ̍��W0�`62�܂ł�p_[0]���\��,���W63����80�܂ł�p_[1]���\��
		//���W���Ⴂ�قǉ���bit,p_[0]�ł͍��W0���ŉ���bit,���W63���ŏ��bit
		//p_[1]�ł͍��W63���ŉ���bit�ō��W80���ŏ��bit
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
