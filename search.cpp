#include <iostream>
#include <string>

using namespace std;

#include "position.h"
#include "movegen.h"
#include "search.h"

using namespace std;


bool think(Position &pos,Move *ml)
{
    ml = generate_moves(pos,ml);
    int n = 0;
    Move m = ml[0];
    if(n == 0){
        //合法手がなければ投了
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //指し手を実行して、その手を将棋所に送信
        cout << "bestmove " << "bookMove" << endl;
        do_move(pos,m);
    }
    return true;
}


