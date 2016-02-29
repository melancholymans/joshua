#if !defined(TT_H_INCLUDE)
#define TT_H_INCLUDE

#include "types.h"

//TranspositionTableの１要素の構造体
struct TTEntry{
	void save(uint32_t k, int v, Bound b, int d,Move m, int g, int ev)
	{
		key32 = k;
		move16 = (uint16_t)m;	//Moveは全データを32bit変数に格納している(有効データは24bit）uint16_tにいれることにより移動した駒種,取った駒種の情報が失われる
		bound8 = (uint8_t)b;	//boundは2bitしか必要がないので情報欠落はおきない
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
	//世代を増やしている
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

//受け取ったkey(uint64_tの下位32bitを使って最初のエントリーへのアドレスを取得して返している
inline TTEntry* TranspositionTable::first_entry(const Key_t key) const
{
	return table + ((uint32_t)key & hash_mask);
}
//TranspositionTableの世代を設定にする
inline void TranspositionTable::refresh(const TTEntry* tte) const
{
	const_cast<TTEntry*>(tte)->set_generation(generation);
}
#endif