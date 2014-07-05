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
    short *mf = next_modify[ply].next_dirty = next_modify[ply].last_dirty;
    int u = next_move[ply].last_move - ml;
    /*
    �ǖʍX�V�̌��^
    */
    for(Move *m = ml;ml != next_move[ply].last_move;m++){
        next_modify[ply].last_dirty = do_move(pos,*m,mf);
        print_board(pos);
        undo_move(pos,ply);
    }

    /**/
    int r = rand();
    int n = (u*r)/(RAND_MAX+1);
    Move m = *(ml+n);
    /**/
    if(u == 0){
        //���@�肪�Ȃ���Γ���
        print_board(pos);
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //�w��������s���āA���̎���������ɑ��M
        do_move(pos,m,mf);
        print_board(pos);
        cout << "bestmove " << string_from_move(m) << endl;
    }
    return true;
}


