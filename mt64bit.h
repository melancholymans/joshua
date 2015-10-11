#if !defined(MT64BIT_H_INCLUDE)
#define MT64BIT_H_INCLUDE

#include <random>

using std::mt19937_64;

//64bit�̃����_���Ȓl��Ԃ����߂̃N���X,apery�̃\�[�X�ێʂ�
class MT64bit :public std::mt19937_64{
public:
	//�V�[�h��^�����A��ɓ����������K�v�ȏꍇ������̃R���X�g���N�^���Ă�
	MT64bit() :std::mt19937_64(){}
	//�V�[�h��^���A�Ⴄ�������ق����ꍇ������̃R���X�g���N�^�ŌĂ�
	explicit MT64bit(const unsigned int seed) : std::mt19937_64(seed){}
	//64bit�̗�����Ԃ�
	uint64_t random()
	{
		return (*this)();
	}
};

#endif