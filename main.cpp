/*
Shogidokoro shogi playing engin
*/
#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include "gtest\gtest.h"
#include "types.h"
#include "usi.h"
#include "usioption.h"

using namespace std;

int main_test(int argc,char *argv[]);
void init(void);

int main(int argc,char *argv[])
{
    //標準入出力とリンクさせて、将棋所と通信ができる
    setvbuf(stdin,NULL,_IONBF,0);
    setvbuf(stdout,NULL,_IONBF,0);
    init_usi_options();
#if !defined(NDEBUG)
    main_test(argc,argv); 
#endif
    usi_main_loop();
    return 0;
}

//アプリケーション全体の初期化
void init(void)
{
}

//テスト起動
int main_test(int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    int result = RUN_ALL_TESTS();
    return result;
}
