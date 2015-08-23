#if !defined(BITBOARD_H_INCLUDE)
#define BITBOARD_H_INCLUDE

#include "types.h"

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
	BitBoard(const uint64_t v0, const uint64_t v1)
	{
		this->p_[0] = v0;
		this->p_[1] = v1;
	}
	//_mm_testz_si128�͈������m��bit AND���Z���Č��ʂ��[���ɂȂ��1��Ԃ�
	//����is_not_zero�֐��͂���BitBoard�N���X��bitboard�����ׂă[���ɂȂ�����FALSE��Ԃ�
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
	void print(BitBoard);
}
extern const BitBoard SquareBB[SquareNum];

#endif
