#include <Windows.h>
#include <mutex>
#ifdef _DEBUG
	#include "gtest\gtest.h"
#endif
#include "misc.h"

//CMAGAZINE 2005/2より
void ptime_init(ptime_t *iPT)
{
    *iPT = clock();
}

double ptime_now(const ptime_t *iPT)
{
    //経過時間をmsecで返す
	return (double)(clock() - *iPT);
}

std::ostream& operator<<(std::ostream& os, SyncCout sc)
{
	static std::mutex m;

	if (sc == io_lock){
		m.lock();
	}
	if (sc == io_unlock){
		m.unlock();
	}
	return os;
}

#ifdef _DEBUG
TEST(misc,ptime_init_ptime_now)
{
    ptime_t aPT;

    /*計測開始*/
    ptime_init(&aPT);
    
    //計測対象
    Sleep(1234);    //単位はmsec

    //計測終了
    printf("%f msec \n",ptime_now(&aPT));
}
#endif