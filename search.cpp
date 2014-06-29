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
    Move *ml = next_move[0].last_move = mlist;
    ml = generate_moves(pos,ml);
    int u = ml - next_move[0].last_move;
    int r = rand();
    int n = (u*r)/(RAND_MAX+1);
    Move m = *(next_move[0].last_move+n);
    if(u == 0){
        //���@�肪�Ȃ���Γ���
        print_board(pos);
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //�w��������s���āA���̎���������ɑ��M
        do_move(pos,m);
        print_board(pos);
        cout << "bestmove " << string_from_move(m) << endl;
    }
    return true;
}


