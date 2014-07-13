#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

#include "types.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "move.h"
#include "misc.h"

search_t sech;
status_t stats;

bool think(Position &pos)
{
    ptime_t aPT;
    int ply = 0;
    
    //nps計測
    stats.search_node = 0;
    ptime_init(&aPT);
    Move m = search(pos,ply);
    if(m == 0){
        //合法手がなければ投了
        print_board(pos);
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //指し手を実行して、その手を将棋所に送信
        short dummy[8];
        DoMove(pos.turn,pos,m,dummy);
        print_board(pos);
        cout << "bestmove " << string_from_move(m) << endl;
    }
    //局面データを表示
    double eapsed_time = ptime_now(&aPT);  //sec
    printf("eapsed_time=%.2f(sec) \n",eapsed_time);
    printf("search node=%dk \n",stats.search_node/1000);
    printf("nps=%.2fk \n",double(stats.search_node/(eapsed_time*1000)));
    print_board(pos);
    return true;
}

Move search(Position &pos,int ply)
{
    stats.search_node++;
    if(ply > PLY_MAX){
        return 0;
    }
    short *mf = next_modify[ply].next_dirty = next_modify[ply].last_dirty;

    Move *ml = next_move[ply].next_move = next_move[ply].last_move;
    next_move[ply+1].last_move = next_move[ply].last_move = generate_moves(pos,ml);
    for(;ml != next_move[ply].last_move;ml++){
        next_modify[ply+1].last_dirty=next_modify[ply].last_dirty = DoMove(pos.turn,pos,*ml,mf);
        search(pos,ply+1);
        undo_move(pos,ply);
    }
    //ルートでランダムに手を選ぶ（評価関数がまだないので）
    int u = next_move[ply].last_move - next_move[ply].next_move;
    if(u == 0){
        return 0;
    }
    int r = rand();
    int n = (u*r)/(RAND_MAX+1);
    Move m = *(next_move[ply].next_move+n);
    return m;
}

