#if !defined(TT_H_INCLUDE)
#define TT_H_INCLUDE

#include "types.h"

//TranspositionTable�̂P�v�f�̍\����
struct TTEntry{
	void save(uint32_t k, int v, Bound b, int d,Move m, int g, int ev)
	{
		key32 = k;
		move16 = (uint16_t)m;	//Move�͑S�f�[�^��32bit�ϐ��Ɋi�[���Ă���(�L���f�[�^��24bit�juint16_t�ɂ���邱�Ƃɂ��ړ��������,��������̏�񂪎�����
		bound8 = (uint8_t)b;	//bound��2bit�����K�v���Ȃ��̂ŏ�񌇗��͂����Ȃ�
		generation8 = (uint8_t)g;
		value16 = (int16_t)v;
		depth16 = (int16_t)d;
		evalue = (int16_t)ev;
	}
	void set_generation(uint8_t g)
	{
		generation8 = (uint8_t)g;
	}
	uint32_t key() const
	{
		return key32;
	}
	int depth() const
	{
		return (int)depth16;
	}
	Move move() const
	{
		return (Move)move16;
	}
	int value() const
	{
		return (int)value16;
	}
	Bound bound() const
	{
		return (Bound)bound8;
	}
	int generation() const
	{
		return (int)generation8;
	}
	int eval_value() const
	{
		return (int)evalue;
	}
private:
	uint32_t key32;
	uint16_t move16;
	uint8_t bound8;
	uint8_t generation8;
	int16_t value16;
	int16_t depth16;
	int16_t evalue;
};
class TranspositionTable {
	static const unsigned ClusterSize = 4;
public:
	~TranspositionTable()
	{
		free(mem);
	}
	//����𑝂₵�Ă���
	void new_search()
	{
		++generation;
	}
	const TTEntry* probe(const Key_t key) const;
	TTEntry* first_entry(const Key_t key) const;
	void refresh(const TTEntry* tte) const;
	void set_size(size_t mb_size);
	void clear();
	void store(const Key_t key, int v, Bound type, int d, Move m, int statv);
#ifdef _DEBUG
	int get_size()
	{
		return hash_mask;
	}
#endif
private:
	uint32_t hash_mask;
	TTEntry* table;
	void* mem;
	uint8_t generation;
};

//�󂯎����key(uint64_t�̉���32bit���g���čŏ��̃G���g���[�ւ̃A�h���X���擾���ĕԂ��Ă���
inline TTEntry* TranspositionTable::first_entry(const Key_t key) const
{
	return table + ((uint32_t)key & hash_mask);
}
//TranspositionTable�̐����ݒ�ɂ���
inline void TranspositionTable::refresh(const TTEntry* tte) const
{
	const_cast<TTEntry*>(tte)->set_generation(generation);
}
#endif