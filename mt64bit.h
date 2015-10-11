#if !defined(MT64BIT_H_INCLUDE)
#define MT64BIT_H_INCLUDE

#include <random>

using std::mt19937_64;

//64bitのランダムな値を返すためのクラス,aperyのソース丸写し
class MT64bit :public std::mt19937_64{
public:
	//シードを与えず、常に同じ乱数が必要な場合こちらのコンストラクタを呼ぶ
	MT64bit() :std::mt19937_64(){}
	//シードを与え、違う乱数がほしい場合こちらのコンストラクタで呼ぶ
	explicit MT64bit(const unsigned int seed) : std::mt19937_64(seed){}
	//64bitの乱数を返す
	uint64_t random()
	{
		return (*this)();
	}
};

#endif