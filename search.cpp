#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

#include "position.h"
#include "movegen.h"
#include "search.h"
#include "move.h"

bool think(Position &pos)
{
    int ply = 0;

    Move *ml = next_move[ply].next_move = next_move[ply].last_move;
    next_move[ply].last_move = generate_moves(pos,ml);
    int u = next_move[ply].last_move - ml;
    int r = rand();
    int n = (u*r)/(RAND_MAX+1);
    Move m = *(ml+n);
    if(u == 0){
        //合法手がなければ投了
        print_board(pos);
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //指し手を実行して、その手を将棋所に送信
        do_move(pos,m);
        print_board(pos);
        cout << "bestmove " << string_from_move(m) << endl;
    }
    return true;
}


