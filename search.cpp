#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

#include "gtest\gtest.h"
#include "types.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "move.h"
#include "misc.h"
#include "evaluate.h"
#include "usi.h"

search_t sech;
status_t stats;

int search_max(Position &pos,int ply);
int search_min(Position &pos,int ply);

bool think(Position &pos)
{
    ptime_t aPT;
    int ply = 0;
    
    //nps計測
    stats.search_node = 0;
    ptime_init(&aPT);
    Move m = search_root(pos,ply);
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

Move search_root(Position &pos,int ply)
{
    Move m;
    backup_info_t bfo;

    short *mf = next_modify[ply].next_dirty = next_modify[ply].last_dirty;
    //全ての手を生成
    Move *ml = next_move[ply].next_move = next_move[ply].last_move;
    next_move[ply+1].last_move = next_move[ply].last_move = generate_moves(pos,ml);
    int max_score = -2147483648;
    //Move構造体をDoMove関数に渡して局面更新、もし合法手が0なら（王手を避ける手がないなど）このforループをパスする
    for(;ml != next_move[ply].last_move;ml++){
        bfo.material = sech.material;
        next_modify[ply+1].last_dirty=next_modify[ply].last_dirty = DoMove(pos.turn,pos,*ml,mf);
        int score = search_min(pos,ply+1);
        undo_move(pos,ply);
        sech.material = bfo.material;
        if(max_score < score){
            max_score = score;
            m = *ml;
        }
    }
    return m;
}

int search_max(Position &pos,int ply)
{
    backup_info_t bfo;

    stats.search_node++;
    if(ply > PLY_MAX){
        return evaluate(pos);
    }
    short *mf = next_modify[ply].next_dirty = next_modify[ply].last_dirty;
    //全ての手を生成
    Move *ml = next_move[ply].next_move = next_move[ply].last_move;
    next_move[ply+1].last_move = next_move[ply].last_move = generate_moves(pos,ml);
    int max_score = -2147483648;
    //Move構造体をDoMove関数に渡して局面更新、もし合法手が0なら（王手を避ける手がないなど）このforループをパスする
    for(;ml != next_move[ply].last_move;ml++){
        bfo.material = sech.material;
        next_modify[ply+1].last_dirty=next_modify[ply].last_dirty = DoMove(pos.turn,pos,*ml,mf);
        int score = search_min(pos,ply+1);
        undo_move(pos,ply);
        sech.material = bfo.material;
        if(max_score < score){
            max_score = score;
        }
    }
    return max_score;
}

int search_min(Position &pos,int ply)
{
    backup_info_t bfo;

    stats.search_node++;
    if(ply > PLY_MAX){
        return evaluate(pos);
    }
    short *mf = next_modify[ply].next_dirty = next_modify[ply].last_dirty;
    //全ての手を生成
    Move *ml = next_move[ply].next_move = next_move[ply].last_move;
    next_move[ply+1].last_move = next_move[ply].last_move = generate_moves(pos,ml);
    int min_score = 2147483647;
    //Move構造体をDoMove関数に渡して局面更新、もし合法手が0なら（王手を避ける手がないなど）このforループをパスする
    for(;ml != next_move[ply].last_move;ml++){
        bfo.material = sech.material;
        next_modify[ply+1].last_dirty=next_modify[ply].last_dirty = DoMove(pos.turn,pos,*ml,mf);
        int score = search_max(pos,ply+1);
        undo_move(pos,ply);
        sech.material = bfo.material;
        if(min_score > score){
            min_score = score;
        }
    }
    return min_score;
}

TEST(serach,search_root)
{
    //Q1の局面でテスト
    string expect ="lR1B3nl/2gp5/ngk1+BspPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b P5psr 1";
    from_sfen(expect);
    game_init(root_position);
    think(root_position);
}
