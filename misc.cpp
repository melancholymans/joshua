#include <Windows.h>

#include "gtest\gtest.h"
#include "misc.h"

//CMAGAZINE 2005/2より
void ptime_init(ptime_t *iPT)
{
    *iPT = clock();
}

double ptime_now(const ptime_t *iPT)
{
    //経過時間をsecで返す
    return (double)(clock() - *iPT)/(double)(CLOCKS_PER_SEC);
}

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