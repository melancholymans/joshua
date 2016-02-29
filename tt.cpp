#include "types.h"
#include "tt.h"

//Global TranspositionTable
TranspositionTable TT;

//TranspositionTableに必要なメモリサイズを計算し、メモリーを確保し、tableポインタに接続する
//この置換表はリハッシュが４回に固定されている（ClusterSize=4）
//したがってデータを保存するところは１エントリーと数えそれが４つありそれを１クラスターと呼ぶ
//１エントリーは16byte、１クラスターは16*4=64byte、１つのクラスターは１局面のKey値に対応している
//さてここでの計算はユーザーから指定した概算メモリー量に応じてどれくらいの正確なメモリー量を割り当てればよいか
//計算している。たとえばユーザーが32と指定した場合（32MByte)
//size = (32 << 20)/sizeof(TTEntorySize]で524,288クラスター生成できると算定
//つづいてsize = ClusterSize << indexでいくらエントリーが必要か計算している
//4 << 19 = 2,097,152エントリー（2097152/524288=4)
//最後にエントリあたりのbyteをかけて必要なメモリー量を計算している
//mem = calloc(size * sizeof(TTEntry) + 64 - 1, 1);　（+ 64 - 1の計算はなんのためにしているのかは不明->アライメントのためでは ）
//mem = 2,097,152*16 + 64 - 1 = 33.554495MByte
//ユーザーは概算で32MByte要求し実際は33.554495MByte必要であることをわかった
void TranspositionTable::set_size(size_t mb_size)
{
	unsigned long index;

	//このsizeは指定されたメモリサイズで確保できるTTEntryのエントリー数
	uint32_t size = (mb_size << 20) / sizeof(TTEntry[ClusterSize]);
	_BitScanReverse(&index, size);
	size = ClusterSize << index;
	//要求されたサイズが現在のサイズとおなじなら何もせず返す
	if (hash_mask == size - ClusterSize){
		return;
	}
	hash_mask = size - ClusterSize;
	free(mem);
	mem = calloc(size * sizeof(TTEntry) + 64 - 1, 1);
	//メモリーが確保されなかったらメッセージを出して強制終了
	if (!mem){
		cerr << "Failed to allocate " << mb_size << "MB for TranspositionTable." << endl;
	}
	//tableの開始点をmemアドレスより64 - 1だけずらしている
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