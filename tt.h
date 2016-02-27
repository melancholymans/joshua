#if !defined(TT_H_INCLUDE)
#define TT_H_INCLUDE

class TranspositionTable {
public:
	void set_size(size_t mb_size);
	void clear();
};

extern TranspositionTable TT;
#endif