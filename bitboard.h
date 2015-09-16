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
		_mm_store_si128(&m_, rhs.m_);
		return *this;
	}
	//�R�s�[�R���X�g���N�^
	BitBoard(const BitBoard& bb)
	{
		_mm_store_si128(&m_, bb.m_);
	}
	//��R���X�g���N�^
	BitBoard() {}
	//64bit�ϐ��Őݒ肷��
	//�g�p��
	//BitBoard bb(0,0);
	BitBoard(const uint64_t v0, const uint64_t v1)
	{
		p_[0] = v0;
		p_[1] = v1;
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
		_mm_store_si128(&this->m_, _mm_and_si128(m_, rhs.m_));
		return *this;
	}
	//����rhs�Ƃ�OR���Z
	BitBoard operator |= (const BitBoard& rhs)
	{
		_mm_store_si128(&m_, _mm_or_si128(m_, rhs.m_));
		return *this;
	}
	//����rhs�Ƃ�XOR���Z
	BitBoard operator ^= (const BitBoard& rhs)
	{
		_mm_store_si128(&this->m_, _mm_xor_si128(m_, rhs.m_));
		return *this;
	}
	//bitboard������i�������V�t�g����
	BitBoard operator <<= (const int i)
	{
		_mm_store_si128(&m_, _mm_slli_epi64(m_, i));
		return *this;
	}
	//bitboard������i�����E�V�t�g����
	BitBoard operator >>= (const int i)
	{
		_mm_store_si128(&m_, _mm_srli_epi64(m_, i));
		return *this;
	}
	//bitboard������rhs�Ɣ�r���ē��������true��Ԃ�
	bool operator == (const BitBoard& rhs) const
	{
		return _mm_testc_si128(_mm_cmpeq_epi8(m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xFFu))) ? true : false;
	}
	//bitboard������rhs�Ɣ�r���ē������Ȃ����true��Ԃ�
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
	//_mm_testz_si128�͈������m��bit AND���Z���Č��ʂ��[���ɂȂ��1��Ԃ�
	//����is_not_zero�֐���BitBoard�N���X��bitboard�����ׂă[���ɂȂ�����FALSE��Ԃ�
	bool is_not_zero() const
	{
		return !(_mm_testz_si128(m_, _mm_set1_epi8(static_cast<char>(0xFFu))));
	}
	//�ǖ�bitboard�ƈ�����bitboard��AND���Z��bit�������Ă����true��Ԃ�
	bool is_biton_inmask(const BitBoard& mask) const
	{
		return !(_mm_testz_si128(m_, mask.m_));
	}
	int pop_count() const
	{
		return int(_mm_popcnt_u64(p_[0]) + _mm_popcnt_u64(p_[1]));
	}
	//�w�肵�����W��bit��on
	void set_bit(const Square sq)
	{
		*this |= SquareBB[sq];
	}
	//�w�肵�����W�ɋ���邩���肷��
	bool is_bit_on(const Square sq) const
	{
		return !(_mm_testz_si128(m_, SquareBB[sq].m_));
	}
	//mask��on�ɂȂ��Ă���Ƃ�����N���A����
	BitBoard clear_bits(const BitBoard& mask)
	{
		_mm_store_si128(&m_, _mm_andnot_si128(mask.m_, m_));
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
		_mm_store_si128(&m_, _mm_xor_si128(m_, _mm_or_si128(SquareBB[sq1].m_, SquareBB[sq2].m_)));
	}
	//bitboard�����S�Ƀ[���łȂ����Ƃ�O��Ƃ��Ă���
	//bitboard��p_[0]��LSB������X�L�������čŏ���bit on��index��Ԃ��Bindex�̓[������n�܂�
	//index��bit��off�ɂ���
	Square first_one_right()
	{
		unsigned long index;

		_BitScanForward64(&index, p_[0]);
		p_[0] &= p_[0] - 1;
		return Square(index);
	}
	//bitboard�����S�Ƀ[���łȂ����Ƃ�O��Ƃ��Ă���
	//bitboard��p_[1]��LSB������X�L�������čŏ���bit on��index��Ԃ��Bindex�̓[������n�܂�
	//index��bit��off�ɂ���
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
	//on�ɂȂ��Ă���bit���P���������肵�A�����Ȃ�true��Ԃ�
	bool is_one_bit() const
	{
		return (pop_count() == 1);
	}
	uint64_t p(const int index) const
	{
		return p_[index];
	}
	//_p[0],_p[1]���}�[�W����
	uint64_t merge() const
	{
		return this->p_[0] | this->p_[1];
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

namespace BitBoardns
{
	//�w�肵��file�̃}�X�N��Ԃ�
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
	//�w�肵��rank�̃}�X�N��Ԃ�
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
	//�w�肵��rank���O���i�J���[�ɂ���ĕς��j��bit��on�ɂȂ�bitboard��Ԃ��A�w�肵��rank��bit�͊܂܂�Ȃ�
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
