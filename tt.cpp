#include "types.h"
#include "tt.h"

//Global TranspositionTable
TranspositionTable TT;

//TranspositionTable�ɕK�v�ȃ������T�C�Y���v�Z���A�������[���m�ۂ��Atable�|�C���^�ɐڑ�����
//���̒u���\�̓��n�b�V�����S��ɌŒ肳��Ă���iClusterSize=4�j
//���������ăf�[�^��ۑ�����Ƃ���͂P�G���g���[�Ɛ������ꂪ�S���肻����P�N���X�^�[�ƌĂ�
//�P�G���g���[��16byte�A�P�N���X�^�[��16*4=64byte�A�P�̃N���X�^�[�͂P�ǖʂ�Key�l�ɑΉ����Ă���
//���Ă����ł̌v�Z�̓��[�U�[����w�肵���T�Z�������[�ʂɉ����Ăǂꂭ�炢�̐��m�ȃ������[�ʂ����蓖�Ă�΂悢��
//�v�Z���Ă���B���Ƃ��΃��[�U�[��32�Ǝw�肵���ꍇ�i32MByte)
//size = (32 << 20)/sizeof(TTEntorySize]��524,288�N���X�^�[�����ł���ƎZ��
//�Â���size = ClusterSize << index�ł�����G���g���[���K�v���v�Z���Ă���
//4 << 19 = 2,097,152�G���g���[�i2097152/524288=4)
//�Ō�ɃG���g���������byte�������ĕK�v�ȃ������[�ʂ��v�Z���Ă���
//mem = calloc(size * sizeof(TTEntry) + 64 - 1, 1);�@�i+ 64 - 1�̌v�Z�͂Ȃ�̂��߂ɂ��Ă���̂��͕s��->�A���C�����g�̂��߂ł� �j
//mem = 2,097,152*16 + 64 - 1 = 33.554495MByte
//���[�U�[�͊T�Z��32MByte�v�������ۂ�33.554495MByte�K�v�ł��邱�Ƃ��킩����
void TranspositionTable::set_size(size_t mb_size)
{
	unsigned long index;

	//����size�͎w�肳�ꂽ�������T�C�Y�Ŋm�ۂł���TTEntry�̃G���g���[��
	uint32_t size = (mb_size << 20) / sizeof(TTEntry[ClusterSize]);
	_BitScanReverse(&index, size);
	size = ClusterSize << index;
	//�v�����ꂽ�T�C�Y�����݂̃T�C�Y�Ƃ��Ȃ��Ȃ牽�������Ԃ�
	if (hash_mask == size - ClusterSize){
		return;
	}
	hash_mask = size - ClusterSize;
	free(mem);
	mem = calloc(size * sizeof(TTEntry) + 64 - 1, 1);
	//�������[���m�ۂ���Ȃ������烁�b�Z�[�W���o���ċ����I��
	if (!mem){
		cerr << "Failed to allocate " << mb_size << "MB for TranspositionTable." << endl;
	}
	//table�̊J�n�_��mem�A�h���X���64 - 1�������炵�Ă���
	table = (TTEntry*)((uintptr_t(mem) + 64 - 1) & ~(64 - 1));
}
void TranspositionTable::clear()
{
	memset(table, 0, (hash_mask + ClusterSize) * sizeof(TTEntry));
}
const TTEntry* TranspositionTable::probe(const Key_t key) const
{
	const TTEntry* tte = first_entry(key);
	uint32_t key32 = key >> 32;

	for (unsigned int i = 0; i < ClusterSize; ++i, ++tte){
		if (tte->key() == key32){
			return tte;
		}
	}
	return nullptr;
}
void TranspositionTable::store(const Key_t key, int v, Bound b, int d, Move m, int statv)
{
	int c1, c2, c3;
	TTEntry *tte;
	TTEntry *replace;
	uint32_t key32 = key >> 32;

	tte = replace = first_entry(key);
	for (unsigned int i = 0; i < ClusterSize; ++i, ++tte){
		if (!tte->key() || tte->key() == key32){
			if (!m){
				m = tte->move();
			}
			replace = tte;
			break;
		}
		c1 = (replace->generation() == generation ? 2 : 0);
		c2 = (tte->generation() == generation || tte->bound() == BoundExact ? -2 : 0);
		c3 = (tte->depth() < replace->depth() ? 1 : 0);
		if (c1 + c2 + c3 > 0){
			replace = tte;
		}
	}
	replace->save(key32, v, b, d, m, generation, statv);
}