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
        //���@�肪�Ȃ���Γ���
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //�w��������s���āA���̎���������ɑ��M
        cout << "bestmove " << "bookMove" << endl;
        do_move(pos,m);
    }
    return true;
}


