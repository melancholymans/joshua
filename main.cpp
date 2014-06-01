/*
Shogidokoro shogi playing engin
*/
#include <stdio.h>
#include <cstdlib>
#include <iostream>

#include "uci.h"

using namespace std;

int main(int argc,char argv[])
{
    //標準入出力とリンクさせて、将棋所と通信ができる
    setvbuf(stdin,NULL,_IONBF,0);
    setvbuf(stdout,NULL,_IONBF,0);

    uci_main_loop();
    return 0;
}

