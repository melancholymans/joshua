#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

#include "position.h"
#include "movegen.h"
#include "usi.h"
#include "search.h"

using namespace std;


bool think(Position &pos,Move *ml)
{
    ml = generate_moves(pos,ml);
    int u = ml - next_move[0].last_move;
    double r = rand();
    int n = u*(r/RAND_MAX);
    Move m = *(next_move[0].last_move+n);
    if(u == 0){
        //合法手がなければ投了
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //指し手を実行して、その手を将棋所に送信
        cout << "bestmove " << string_from_move(m) << endl;
        do_move(pos,m);
    }
    return true;
}


