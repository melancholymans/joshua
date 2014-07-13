/*
Shogidokoro shogi playing engin
*/
#include <stdio.h>
#include <cstdlib>
#include <iostream>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "types.h"
#include "usi.h"
#include "usioption.h"
#include "evaluate.h"
#include "misc.h"
#include "search.h"

/*
https://github.com/sunfish-shogi/sunfish
https://github.com/sunfish-shogi/sunfish3
*/
void profile(void);
int main_test(int argc,char *argv[]);
void init(void);

int main(int argc,char *argv[])
{
    init();
    //標準入出力とリンクさせて、将棋所と通信ができる
    setvbuf(stdin,NULL,_IONBF,0);
    setvbuf(stdout,NULL,_IONBF,0);
    init_usi_options();
#if !defined(NDEBUG)
    profile();
    main_test(argc,argv); 
#endif
    usi_main_loop();
    return 0;
}

//アプリケーション全体の初期化
void init(void)
{
    //駒割配列評価値設定
    memset(piece_value,0,sizeof(int)*16);
    piece_value[PAWN] = DPawn;
    piece_value[LANCE] = DLance;
    piece_value[KNIGHT] = DKnight;
    piece_value[SILVER] = DSilver;
}

//テスト起動
int main_test(int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    int result = RUN_ALL_TESTS();
    return result;
}
