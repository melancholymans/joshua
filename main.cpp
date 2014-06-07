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

int main(int argc,char *argv[])
{
    //標準入出力とリンクさせて、将棋所と通信ができる
    setvbuf(stdin,NULL,_IONBF,0);
    setvbuf(stdout,NULL,_IONBF,0);

    main_test(argc,argv);

    init_usi_options();
    usi_main_loop();
    return 0;
}

int main_test(int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    int result = RUN_ALL_TESTS();
    return result;
}
