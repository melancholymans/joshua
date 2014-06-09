#include <Windows.h>

#include "gtest\gtest.h"
#include "misc.h"

//CMAGAZINE 2005/2���
void ptime_init(ptime_t *iPT)
{
    *iPT = clock();
}

double ptime_now(const ptime_t *iPT)
{
    //�o�ߎ��Ԃ�sec�ŕԂ�
    return (double)(clock() - *iPT)/(double)(CLOCKS_PER_SEC);
}
TEST(misc,ptime_init_ptime_now)
{
    ptime_t aPT;

    /*�v���J�n*/
    ptime_init(&aPT);
    
    //�v���Ώ�
    Sleep(1234);    //�P�ʂ�msec
    //�v���I��

    printf("%f msec \n",ptime_now(&aPT));
}